// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/FormattedDataHandler.hpp"

#include <sndfile.hh>
#include <spdlog/spdlog.h>

#include <functional>
#include <vector>

namespace jar {

namespace {

int
toSndFormat(const DataFormat /*format*/)
{
    return SF_FORMAT_WAV;
}

} // namespace

class FormattedDataHandler::Impl {
public:
    using OnBegin = void(DataFormat format, int sampleRate, int sampleWidth, int channels);
    using OnData = void(const void* ptr, size_t size);
    using OnEnd = void(const SynthesisResult&);

    explicit Impl(const DataFormat format, const size_t defaultSize = 16 * 1024)
        : _format{format}
        , _buffer(defaultSize)
    {
    }

    void
    onFormattedBegin(std::function<OnBegin> callback)
    {
        _beginCallback = std::move(callback);
    }

    void
    onFormattedData(std::function<OnData> callback)
    {
        _dataCallback = std::move(callback);
    }

    void
    onFormattedEnd(std::function<OnEnd> callback)
    {
        _endCallback = std::move(callback);
    }

    void
    open(const int sampleRate, int sampleWidth, const int channels)
    {
        const int kFormat{toSndFormat(_format) | SF_FORMAT_PCM_16};

        if (sampleWidth != 2 /* 2 bytes per sample */) {
            SPDLOG_ERROR("Samples with <{}> width is not supported", sampleWidth);
        }

        SF_VIRTUAL_IO io = {
            .get_filelen = &onSndGetLen,
            .seek = &onSndSeek,
            .read = nullptr,
            .write = &onSndWrite,
            .tell = &onSndTell,
        };
        _handle = SndfileHandle{io, this, SFM_WRITE, kFormat, channels, sampleRate};
        if (not _handle) {
            SPDLOG_ERROR("Unable to create handle: {}", _handle.strError());
            throw std::runtime_error{"Unable to create handle"};
        }

        notifyBegin(sampleRate, sampleWidth, channels);
    }

    void
    write(const void* ptr, const unsigned int count)
    {
        _handle.write(static_cast<const short*>(ptr), count);
    }

    void
    finalize(const SynthesisResult& result)
    {
        notifyData(_buffer.data(), _buffer.size());

        _buffer.clear();
        _buffer.shrink_to_fit();

        if (_handle) {
            _handle = SndfileHandle();
        }

        notifyEnd(result);
    }

private:
    void
    notifyBegin(const int sampleRate, const int sampleWidth, const int channels) const
    {
        if (_beginCallback) {
            _beginCallback(_format, sampleRate, sampleWidth, channels);
        }
    }

    void
    notifyData(const void* ptr, const size_t size) const
    {
        if (_dataCallback) {
            _dataCallback(ptr, size);
        }
    }

    void
    notifyEnd(const SynthesisResult& result) const
    {
        if (_endCallback) {
            _endCallback(result);
        }
    }

    [[nodiscard]] sf_count_t
    seek(const sf_count_t offset, const int whence)
    {
        switch (whence) {
        case SF_SEEK_CUR:
            _offset += offset;
            break;
        case SF_SEEK_END:
            _offset = _length + offset;
            break;
        case SF_SEEK_SET:
            _offset = offset;
            break;
        default:
            break;
        }
        return _offset;
    }

    [[nodiscard]] sf_count_t
    length() const
    {
        return _length;
    }

    [[nodiscard]] sf_count_t
    offset() const
    {
        return _offset;
    }

    [[nodiscard]] sf_count_t
    processFormattedData(const void* ptr, const sf_count_t count)
    {
        if (_buffer.size() < _offset + count) {
            std::vector<uint8_t> buffer(_offset + count);
            memcpy(buffer.data(), _buffer.data(), _offset);
            _buffer.swap(buffer);
        }

        memcpy(_buffer.data() + _offset, ptr, count);
        _offset += count;

        if (_offset > _length) {
            _length = _offset;
        }

        return count;
    }

private:
    static sf_count_t
    onSndGetLen(void* data)
    {
        if (const auto* const self = static_cast<Impl*>(data); self) {
            return self->length();
        }
        return 0;
    }

    static sf_count_t
    onSndSeek(sf_count_t offset, int whence, void* data)
    {
        if (auto* const self = static_cast<Impl*>(data); self) {
            return self->seek(offset, whence);
        }
        return 0;
    }

    static sf_count_t
    onSndWrite(const void* ptr, sf_count_t count, void* data)
    {
        if (auto* const self = static_cast<Impl*>(data); self) {
            return self->processFormattedData(ptr, count);
        }
        return 0;
    }

    static sf_count_t
    onSndTell(void* data)
    {
        if (const auto* const self = static_cast<Impl*>(data); self) {
            return self->offset();
        }
        return 0;
    }

private:
    DataFormat _format{};
    sf_count_t _offset{};
    sf_count_t _length{};
    std::vector<uint8_t> _buffer;
    std::function<OnBegin> _beginCallback;
    std::function<OnData> _dataCallback;
    std::function<OnEnd> _endCallback;
    SndfileHandle _handle;
};

FormattedDataHandler::FormattedDataHandler(DataFormat format)
    : _impl(std::make_unique<Impl>(format))
{
    _impl->onFormattedBegin(std::bind_front(&FormattedDataHandler::onFormattedBegin, this));
    _impl->onFormattedData(std::bind_front(&FormattedDataHandler::onFormattedData, this));
    _impl->onFormattedEnd(std::bind_front(&FormattedDataHandler::onFormattedEnd, this));
}

FormattedDataHandler::~FormattedDataHandler()
{
    _impl->onFormattedBegin(nullptr);
    _impl->onFormattedData(nullptr);
    _impl->onFormattedEnd(nullptr);
}

void
FormattedDataHandler::onBegin(int sampleRate, int sampleWidth, int channels)
{
    _impl->open(sampleRate, sampleWidth, channels);
}

void
FormattedDataHandler::onData(const void* ptr, unsigned int count)
{
    _impl->write(ptr, count);
}

void
FormattedDataHandler::onEnd(const SynthesisResult& result)
{
    _impl->finalize(result);
}

} // namespace jar
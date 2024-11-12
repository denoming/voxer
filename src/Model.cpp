// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "Model.hpp"

#include "Types.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <utf8cpp/utf8.h>

#include <fstream>

using nlohmann::json;

static const char* kInstanceName{"voxer"};

namespace jar {

// Get the first UTF-8 codepoint of a string
static Phoneme
getCodepoint(std::string s)
{
    utf8::iterator character_iter(s.begin(), s.begin(), s.end());
    return *character_iter;
}

// True if the string is a single UTF-8 codepoint
static bool
isSingleCodepoint(std::string s)
{
    return utf8::distance(s.begin(), s.end()) == 1;
}

static void
parsePhonemizeConfig(json& configRoot, PhonemizeConfig& phonemizeConfig)
{
    if (configRoot.contains("espeak")) {
        auto espeakValue = configRoot["espeak"];
        if (espeakValue.contains("voice")) {
            phonemizeConfig.eSpeak.voice = espeakValue["voice"].get<std::string>();
        }
    }

    if (configRoot.contains("phoneme_type")) {
        auto phonemeTypeStr = configRoot["phoneme_type"].get<std::string>();
        if (phonemeTypeStr == "text") {
            phonemizeConfig.phonemeType = TextPhonemes;
        }
    }

    // phoneme to [id] map
    // Maps phonemes to one or more phoneme ids (required).
    if (configRoot.contains("phoneme_id_map")) {
        auto phonemeIdMapValue = configRoot["phoneme_id_map"];
        for (auto& fromPhonemeItem : phonemeIdMapValue.items()) {
            std::string fromPhoneme = fromPhonemeItem.key();
            if (not isSingleCodepoint(fromPhoneme)) {
                std::stringstream idsStr;
                for (auto& toIdValue : fromPhonemeItem.value()) {
                    PhonemeId toId = toIdValue.get<PhonemeId>();
                    idsStr << toId << ",";
                }

                spdlog::error(
                    "\"{}\" is not a single codepoint (ids={})", fromPhoneme, idsStr.str());
                throw std::runtime_error("Phonemes must be one codepoint (phoneme id map)");
            }

            auto fromCodepoint = getCodepoint(fromPhoneme);
            for (auto& toIdValue : fromPhonemeItem.value()) {
                PhonemeId toId = toIdValue.get<PhonemeId>();
                phonemizeConfig.phonemeIdsMap[fromCodepoint].push_back(toId);
            }
        }
    }

    // phoneme to [phoneme] map
    // Maps phonemes to one or more other phonemes (not normally used).
    if (configRoot.contains("phoneme_map")) {
        if (!phonemizeConfig.phonemesMap) {
            phonemizeConfig.phonemesMap.emplace();
        }

        auto phonemeMapValue = configRoot["phoneme_map"];
        for (auto& fromPhonemeItem : phonemeMapValue.items()) {
            std::string fromPhoneme = fromPhonemeItem.key();
            if (not isSingleCodepoint(fromPhoneme)) {
                spdlog::error("\"{}\" is not a single codepoint", fromPhoneme);
                throw std::runtime_error("Phonemes must be one codepoint (phoneme map)");
            }

            auto fromCodepoint = getCodepoint(fromPhoneme);
            for (auto& toPhonemeValue : fromPhonemeItem.value()) {
                std::string toPhoneme = toPhonemeValue.get<std::string>();
                if (not isSingleCodepoint(toPhoneme)) {
                    throw std::runtime_error("Phonemes must be one codepoint (phoneme map)");
                }

                auto toCodepoint = getCodepoint(toPhoneme);
                (*phonemizeConfig.phonemesMap)[fromCodepoint].push_back(toCodepoint);
            }
        }
    }
}

static void
parseSynthesisConfig(json& configRoot, SynthesisConfig& synthesisConfig)
{
    if (configRoot.contains("audio")) {
        auto audioValue = configRoot["audio"];
        if (audioValue.contains("sample_rate")) {
            // Default sample rate is 22050 Hz
            synthesisConfig.sampleRate = audioValue.value("sample_rate", 22050);
        }
    }

    if (configRoot.contains("inference")) {
        // Overrides default inference settings
        auto inferenceValue = configRoot["inference"];
        if (inferenceValue.contains("noise_scale")) {
            synthesisConfig.noiseScale = inferenceValue.value("noise_scale", 0.667f);
        }

        if (inferenceValue.contains("length_scale")) {
            synthesisConfig.lengthScale = inferenceValue.value("length_scale", 1.0f);
        }

        if (inferenceValue.contains("noise_w")) {
            synthesisConfig.noiseW = inferenceValue.value("noise_w", 0.8f);
        }

        if (inferenceValue.contains("phoneme_silence")) {
            // phoneme -> seconds of silence to add after
            synthesisConfig.phonemeSilenceSeconds.emplace();
            auto phonemeSilenceValue = inferenceValue["phoneme_silence"];
            for (auto& phonemeItem : phonemeSilenceValue.items()) {
                std::string phonemeStr = phonemeItem.key();
                if (!isSingleCodepoint(phonemeStr)) {
                    spdlog::error("\"{}\" is not a single codepoint", phonemeStr);
                    throw std::runtime_error("Phonemes must be one codepoint (phoneme silence)");
                }

                auto phoneme = getCodepoint(phonemeStr);
                (*synthesisConfig.phonemeSilenceSeconds)[phoneme]
                    = phonemeItem.value().get<float>();
            }
        }
    }
}

static void
parseModelConfig(json& configRoot, ModelConfig& modelConfig)
{
    modelConfig.numSpeakers = configRoot["num_speakers"].get<std::underlying_type_t<SpeakerId>>();

    if (configRoot.contains("speaker_id_map")) {
        if (!modelConfig.speakerIdMap) {
            modelConfig.speakerIdMap.emplace();
        }

        auto speakerIdMapValue = configRoot["speaker_id_map"];
        for (auto& speakerItem : speakerIdMapValue.items()) {
            std::string speakerName = speakerItem.key();
            (*modelConfig.speakerIdMap)[speakerName] = speakerItem.value().get<SpeakerId>();
        }
    }
}

} // namespace jar

namespace jar {

Model::Model(const std::string& modelPath, const bool useCuda, const SpeakerId speakerId)
    : Model{modelPath, modelPath + ".json", useCuda, speakerId}
{
}

Model::Model(const std::string& modelPath,
             const std::string& configPath,
             const bool useCuda,
             const SpeakerId speakerId)
{
    loadModel(modelPath, useCuda);
    loadConfig(configPath, speakerId);
}

const PhonemizeConfig&
Model::phonemizeConfig() const
{
    return _phonemizeConfig;
}

const SynthesisConfig&
Model::synthesisConfig() const
{
    return _synthesisConfig;
}

bool
Model::isMultiSpeaker() const
{
    return (_modelConfig.numSpeakers > 1);
}

SpeakerId
Model::speakerId() const
{
    return _synthesisConfig.speakerId.value_or(SpeakerId::Default);
}

void
Model::loadModel(const std::string& modelPath, const bool useCuda)
{
    SPDLOG_DEBUG("Loading onnx <{}> model", modelPath);
    _session.env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, kInstanceName);
    _session.env.DisableTelemetryEvents();

    if (useCuda) {
        SPDLOG_DEBUG("Enable using CUDA provider");
        OrtCUDAProviderOptions cudaOptions{};
        cudaOptions.cudnn_conv_algo_search = OrtCudnnConvAlgoSearchHeuristic;
        _session.options.AppendExecutionProvider_CUDA(cudaOptions);
    }

    _session.options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);
    _session.options.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
    _session.options.DisableCpuMemArena();
    _session.options.DisableMemPattern();
    _session.options.DisableProfiling();

    _session.onnx = Ort::Session(_session.env, modelPath.data(), _session.options);
    SPDLOG_DEBUG("Loaded onnx model");
}

void
Model::loadConfig(const std::string& configPath, SpeakerId speakerId)
{
    SPDLOG_DEBUG("Parsing <{}> config file with <{}> speaker",
                 configPath,
                 (speakerId == SpeakerId::Default) ? "default" : fmt::to_string(int(speakerId)));

    std::ifstream modelConfigFile{configPath};
    auto configRoot = json::parse(modelConfigFile);
    parsePhonemizeConfig(configRoot, _phonemizeConfig);
    parseSynthesisConfig(configRoot, _synthesisConfig);
    parseModelConfig(configRoot, _modelConfig);

    if (isMultiSpeaker()) {
        if (speakerId == SpeakerId::Default) {
            /* Use default speaker */
            _synthesisConfig.speakerId = SpeakerId{0};
        } else {
            /* Use particular speaker */
            _synthesisConfig.speakerId = speakerId;
        }
    }

    SPDLOG_DEBUG("Model contains <{}> speaker(s)", _modelConfig.numSpeakers);
}

std::vector<Ort::Value>
Model::inference(const Ort::RunOptions& options,
                 const char* const* inNames,
                 const Ort::Value* inValues,
                 const size_t inCount,
                 const char* const* outNames,
                 const size_t outCount)
{
    return _session.onnx.Run(options, inNames, inValues, inCount, outNames, outCount);
}

} // namespace jar

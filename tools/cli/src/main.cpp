// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/Options.hpp"
#include "voxer/Voxer.hpp"
#include "voxer/FormattedDataSaver.hpp"
#include "FormattedDataOutputStream.hpp"
#include "DataOutputStream.hpp"

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <fmt/chrono.h>

#include <iostream>
#include <optional>
#include <filesystem>
#include <chrono>
#include <voxer/DataSaver.hpp>

using namespace jar;
using namespace nlohmann;

namespace fs = std::filesystem;
namespace krn = std::chrono;

enum class OutputType { ToStdOut, ToFile, ToDir };

enum class OutputFormat { Raw, Wav };

static struct {
    std::optional<fs::path> model;
    std::optional<fs::path> files;
    fs::path outputPath;
    OutputType outputType{OutputType::ToFile};
    OutputFormat outputFormat{OutputFormat::Raw};
    bool cuda{false};
    bool json{false};
    bool showInfo{false};
    bool showVersion{false};
    SpeakerId speaker{SpeakerId::Default};
} config;

[[nodiscard]] static fs::path
getFileName()
{
    const auto now = krn::system_clock::now();
    const auto timestamp = krn::duration_cast<krn::nanoseconds>(now.time_since_epoch());
    return fmt::format(fmt::runtime("{:%Q}.wav"), timestamp);
}

[[nodiscard]] std::unique_ptr<DataHandler>
getDataHandler(std::ostream& os)
{
#ifdef VOXER_ENABLE_FORMATTED
    if (config.outputFormat == OutputFormat::Wav) {
        return std::make_unique<FormattedDataOutputStream>(DataFormat::Wav, os);
    }
#endif
    return std::make_unique<DataOutputStream>(os);
}

[[nodiscard]] std::unique_ptr<DataHandler>
getDataHandler(const fs::path& outputFile)
{
#ifdef VOXER_ENABLE_FORMATTED
    if (config.outputFormat == OutputFormat::Wav) {
        return std::make_unique<FormattedDataSaver>(DataFormat::Wav, outputFile);
    }
#endif
    return std::make_unique<DataSaver>(outputFile);
}

[[nodiscard]] static bool
parseArgs(const cxxopts::ParseResult& result)
{
    if (result.count("model")) {
        config.model = fs::absolute(result["model"].as<std::string>());
        if (not fs::exists(*config.model)) {
            return false;
        }
    } else {
        // Rely on VOXER_MODEL_FILE environment variable
    }

    if (result.count("files")) {
        config.files = fs::absolute(result["files"].as<std::string>());
        if (not fs::exists(*config.files)) {
            return false;
        }
    } else {
        // Rely on VOXER_ESPEAK_DIR environment variable
    }

    if (result.count("output-file")) {
        const std::string outputFile = result["output-file"].as<std::string>();
        if (outputFile == "-") {
            config.outputType = OutputType::ToStdOut;
        } else {
            config.outputPath = fs::absolute(result["output-file"].as<std::string>());
            config.outputType = OutputType::ToFile;
        }
    }

    if (result.count("output-dir")) {
        config.outputPath = fs::canonical(result["output-dir"].as<std::string>());
        config.outputType = OutputType::ToDir;
    }

    if (result.count("cuda")) {
        config.cuda = result["cuda"].as<bool>();
    }

    if (result.count("json")) {
        config.json = result["json"].as<bool>();
    }

    if (result.count("raw")) {
        config.outputFormat = OutputFormat::Raw;
    }

#ifdef VOXER_ENABLE_FORMATTED
    if (result.count("wav")) {
        config.outputFormat = OutputFormat::Wav;
    }
#endif

    if (result.count("speaker")) {
        config.speaker = static_cast<SpeakerId>(result["speaker"].as<int>());
    }

    if (result.count("version")) {
        config.showVersion = true;
    }

    if (result.count("info")) {
        config.showInfo = true;
    }

    return true;
}

[[nodiscard]] static bool
handleArgs(std::istream& is, std::ostream& os)
{
    if (config.showVersion) {
        os << Voxer::version() << std::endl;
        return true;
    }

    Voxer voxer;
    if (config.model and config.files) {
        if (not voxer.configure(*config.model, *config.files, config.cuda, config.speaker)) {
            return false;
        }
    } else if (config.model) {
        if (not voxer.configure(*config.model, config.cuda, config.speaker)) {
            return false;
        }
    } else {
        if (not voxer.configure(config.cuda, config.speaker)) {
            return false;
        }
    }

    if (config.showInfo) {
        os << voxer.info() << std::endl;
        return false;
    }

    std::string input;
    while (std::getline(is, input)) {
        if (config.json) {
            json root = json::parse(input);
            input = root["text"].get<std::string>();
            if (root.contains("output-file")) {
                config.outputPath = fs::absolute(root["output-file"].get<std::string>());
                config.outputType = OutputType::ToFile;
            }
            if (root.contains("speaker")) {
                config.speaker = static_cast<SpeakerId>(root["speaker"].get<int>());
            }
        }
        if (config.outputType == OutputType::ToStdOut) {
            auto handler = getDataHandler(os);
            voxer.textToAudio(input, *handler);
            continue;
        }
        if (config.outputType == OutputType::ToDir) {
            fs::path filePath = config.outputPath / getFileName();
            auto handler = getDataHandler(filePath);
            voxer.textToAudio(input, *handler);
            continue;
        }
        if (config.outputType == OutputType::ToFile) {
            if (not config.json) {
                std::stringstream ss;
                ss << input;
                while (std::getline(is, input)) {
                    ss << " " << input;
                }
                input = ss.str();
            }
            auto handler = getDataHandler(config.outputPath);
            voxer.textToAudio(input, *handler);
            continue;
        }
        throw std::runtime_error("Unhandled output type");
    }

    return true;
}

int
main(const int argc, char* argv[])
{
    cxxopts::Options options{"Voxer CLI"};

    // clang-format off
    options.add_options()
        ("m,model", "Path to ONNX model file", cxxopts::value<std::string>())
        ("f,files", "Path to eSpeak data files", cxxopts::value<std::string>())
        ("of,output-file", "Path to output file ('-' output to STDOUT)", cxxopts::value<std::string>()->default_value("-"))
        ("od,output-dir", "Path to output dir (default: current working dir)", cxxopts::value<std::string>())
        ("j,json", "Handle input in JSON format")
        ("r,raw", "Print output in RAW format (default)", cxxopts::value<bool>())
#ifdef VOXER_ENABLE_FORMATTED
        ("w,wav", "Print output in WAV format", cxxopts::value<bool>())
#endif
        ("c,cuda", "Use CUDA")
        ("s,speaker", "Speaker id (default: 0)", cxxopts::value<int>()->default_value("0"))
        ("v,version", "Print version")
        ("i,info", "Print information")
        ("q,quiet", "Disable logging")
        ("h,help", "Print usage");
    // clang-format on

    try {
        const auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return EXIT_SUCCESS;
        }
        if (not parseArgs(result)) {
            return EXIT_FAILURE;
        }
        if (not handleArgs(std::cin, std::cout)) {
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

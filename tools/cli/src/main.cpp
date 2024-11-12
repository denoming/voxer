// SPDX-License-Identifier: MIT
//
// (c) Copyright 2024
// Denys Asauliak, denoming@gmail.com

#include "voxer/Voxer.hpp"
#include "voxer/FormattedDataSaver.hpp"

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <fmt/chrono.h>

#include <iostream>
#include <optional>
#include <filesystem>
#include <chrono>

using namespace jar;
using namespace nlohmann;

namespace fs = std::filesystem;
namespace krn = std::chrono;

enum class OutputType { ToFile, ToDir };

static struct {
    std::optional<fs::path> model;
    std::optional<fs::path> files;
    fs::path outputPath;
    OutputType outputType{OutputType::ToFile};
    bool cuda{false};
    bool json{false};
    SpeakerId speaker{jar::SpeakerId::Default};
} config;

[[nodiscard]] static bool
parseArgs(const cxxopts::ParseResult& result)
{
    if (result.count("model")) {
        config.model = fs::absolute(result["model"].as<std::string>());
        if (not fs::exists(*config.model)) {
            return false;
        }
    } else {
        // Rely on VOXER_MODEL_DIR environment variable
    }

    if (result.count("files")) {
        config.files = fs::absolute(result["files"].as<std::string>());
        if (not fs::exists(*config.files)) {
            return false;
        }
    } else {
        // Rely on VOXER_FILES_DIR environment variable
    }

    if (result.count("output-file")) {
        config.outputPath = fs::absolute(result["output-file"].as<std::string>());
        config.outputType = OutputType::ToFile;
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

    if (result.count("speaker")) {
        config.speaker = static_cast<SpeakerId>(result["speaker"].as<int>());
    }

    return true;
}

[[nodiscard]] static fs::path
generateFileName()
{
    const auto now = krn::system_clock::now();
    const auto timestamp = krn::duration_cast<krn::nanoseconds>(now.time_since_epoch());
    return fmt::format(fmt::runtime("{:%Q}.wav"), timestamp);
}

[[nodiscard]] static bool
handleInput(std::istream& is)
{
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
        if (config.outputType == OutputType::ToDir) {
            const fs::path filePath = config.outputPath / generateFileName();
            FormattedDataSaver handler{DataFormat::Wav, filePath};
            voxer.textToAudio(input, handler);
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
            FormattedDataSaver handler{DataFormat::Wav, config.outputPath};
            voxer.textToAudio(input, handler);
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
        ("of,output-file", "Path to output file", cxxopts::value<std::string>()->default_value("output.wav"))
        ("od,output-dir", "Path to output dir (default: current working dir)", cxxopts::value<std::string>())
        ("j,json", "Interpret input in JSON format")
        ("c,cuda", "Use CUDA")
        ("s,speaker", "Speaker id (default: 0)", cxxopts::value<int>()->default_value("0"))
        ("v,version", "Print version")
        ("q,quiet", "Disable logging")
        ("h,help", "Print usage")
    ;
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
        if (not handleInput(std::cin)) {
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

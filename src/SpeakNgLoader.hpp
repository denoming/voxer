#pragma once

#include <string>

namespace jar {

class SpeakNgLoader {
public:
    explicit SpeakNgLoader(const std::string& filesPath);

    ~SpeakNgLoader();
};

} // namespace jar
#pragma once

#include <string_view>

std::string_view
getVersion();

unsigned
getVersionMajor();

unsigned
getVersionMinor();

unsigned
getVersionPatch();

unsigned
getVersionTweak();

std::string_view
getVersionRevision();
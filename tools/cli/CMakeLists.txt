# Copyright 2025 Denys Asauliak
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(GNUInstallDirs)

add_executable(voxer-cli "")
add_executable(Voxer::Voxer::Cli ALIAS voxer-cli)

target_sources(voxer-cli
    PRIVATE
        src/main.cpp
        src/DataOutputStream.cpp
)

target_link_libraries(voxer-cli
    PRIVATE cxxopts::cxxopts
            fmt::fmt
            Voxer::Voxer
)

if(VOXER_ENABLE_FORMATTED)
    target_sources(voxer-cli PRIVATE src/FormattedDataOutputStream.cpp)
endif()

target_compile_features(voxer-cli PRIVATE cxx_std_23)

install(
    TARGETS voxer-cli
    COMPONENT Voxer_Bin
)

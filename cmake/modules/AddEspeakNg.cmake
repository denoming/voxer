# SPDX-License-Identifier: MIT
#
# (c) Copyright 2024
# Denys Asauliak, denoming@gmail.com

find_package(PkgConfig)

pkg_check_modules(eSpeakNg REQUIRED IMPORTED_TARGET espeak-ng)
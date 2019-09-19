#!/usr/bin/env bash

cmake .. -DwxWidgets_CONFIG_EXECUTABLE="/opt/local/bin/wx-config -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-mmacosx-version-min=10.5 -arch x86_64 -weak_reference_mismatches non-weak -headerpad_max_install_names -v -Wl"
#!/bin/bash
# Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
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

# check if build sdk into lib

build_trustzone=$1
build_mini_sys=$2
build_bin_type=$3
product_path=$4
flash_size=$5

BUILD_PIECE="false"
flash_config=""
rel_flash_config=""
MAKEFILE="./bsp/platform/main/Makefile"
if [ ! -f $MAKEFILE ]; then
    echo -e "${MAKEFILE} not exist!"
    BUILD_PIECE="true"
    rel_flash_config=" CP_BIN_NAME=cmcp_${flash_size}  "
fi


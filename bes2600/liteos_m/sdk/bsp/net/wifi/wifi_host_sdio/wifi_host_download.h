/***************************************************************************
 *
 * Copyright 2015-2023 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/
#ifndef __WIFI_HOST_DOWNLOAD_H__
#define __WIFI_HOST_DOWNLOAD_H__

/****frame header code****/
enum frame_header_data_type {
    FRAME_HEADER_REPLY             =  0xB0,
    FRAME_HEADER_DOWNLOAD_INFO     =  0xB1,
    FRAME_HEADER_DOWNLOAD_DATA     =  0xB2,
    FRAME_HEADER_DOWNLOAD_END      =  0xB3,
    FRAME_HEADER_RUN_CODE          =  0xB4,
    FRAME_HEADER_SYS_REBOOT        =  0xB5,
    FRAME_HEADER_SYS_SHUTDOWN      =  0xB6,
    FRAME_HEADER_SYS_FLASH_BOOT    =  0xB7,
    FRAME_HEADER_SYS_SET_BOOT_MODE =  0xB8,
    FRAME_HEADER_SYS_CLR_BOOT_MODE =  0xB9,
    FRAME_HEADER_SYS_GET_BOOT_MODE =  0xBA,
    TYPE_CONN_GET_FLASH_FW_VERSION =  0xBB,
};

struct FRAME_STRUCTURE_T {
    uint8_t type;
    uint8_t frame_num;
    uint16_t len;
    uint32_t payload;
};

struct sdio_msg_hdr_t {
    uint8_t type;
    uint8_t seq;
    uint16_t len;
};

struct sdio_msg_replay_t {
    uint32_t replay;
};

struct sdio_firmware_info_t {
    uint32_t len;
    uint32_t addr;
};

struct sdio_download_firmware_t {
    uint32_t addr;
    uint8_t data[0];
};

struct sdio_firmware_crc_t {
    uint32_t crc32;
};

struct sdio_run_firmware_t {
    uint32_t addr;
};

struct fw_version_t {
	uint32_t migc_num;
	uint32_t crc32;
};

struct sdio_boot_mode_t {
    uint32_t boot_mode;
};

void wifi_sdio_firmware_download(void);

#endif

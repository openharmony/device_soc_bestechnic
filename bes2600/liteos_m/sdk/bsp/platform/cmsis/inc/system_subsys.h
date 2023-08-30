/***************************************************************************
 *
 * Copyright 2015-2021 BES.
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
#ifndef __SYSTEM_SUBSYS_H__
#define __SYSTEM_SUBSYS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

#define SEG_NAME_LEN                        16

struct SUBSYS_SEGMENT {
    char seg_name[SEG_NAME_LEN];
    uint32_t laddr_offset;
    uint32_t laddr_end;
    uint32_t vaddr_start;
    uint32_t vaddr_end;
    uint32_t flags;
};

#define SUBSYS_SEGMENT_LOC                  __attribute__((section(".subsys_segment")))

struct SUBSYS_IMAGE_DESC_T {
    uint16_t version;
    uint8_t  type;
    uint8_t  compress;
    uint32_t image_size;
    uint32_t code_start_offset;
    union {
        uint32_t exec_addr;
        uint32_t seg_map_size;
    } u;
};

struct CODE_SEG_MAP_ITEM_T {
    uint32_t exec_addr;
    uint32_t load_offset;
    uint32_t size;
};

typedef uint32_t (*SUBSYS_EXEC_ADDR_CONV_CB)(uint32_t addr);

//---------------------------------------------------------------------
// Subsys APIs
//---------------------------------------------------------------------
void system_subsys_reset_handler(void);

void system_subsys_init(void);

void system_subsys_term(void);

//---------------------------------------------------------------------
// AP APIs
//---------------------------------------------------------------------
int subsys_loader_load_image(uint32_t addr, uint32_t len, int check_magic,
                             SUBSYS_EXEC_ADDR_CONV_CB conv_cb, uint32_t *code_start_addr);

struct SUBSYS_SEGMENT *subsys_loader_get_segment(uint32_t image_addr, const char *seg_name);

int subsys_loader_load_image_fs(const char *file_name,
                                SUBSYS_EXEC_ADDR_CONV_CB conv_cb, uint32_t *code_start_addr);

int subsys_loader_get_segment_fs(const char *file_name, const char *seg_name, struct SUBSYS_SEGMENT *seg_out);

#ifdef __cplusplus
}
#endif

#endif

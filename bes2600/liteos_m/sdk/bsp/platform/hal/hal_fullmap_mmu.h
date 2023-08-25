/***************************************************************************
 *
 * Copyright 2015-2022 BES.
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
#ifndef __HAL_FULLMAP_MMU_H__
#define __HAL_FULLMAP_MMU_H__

#include "plat_addr_map.h"

#ifdef __cplusplus
extern "C" {
#endif

enum HAL_FULLMAP_MMU_ID_T {
#ifdef FULLMAP_MMU0_BASE
    HAL_FULLMAP_MMU_ID_0,
#endif
#ifdef FULLMAP_MMU1_BASE
    HAL_FULLMAP_MMU_ID_1,
#endif

    HAL_FULLMAP_MMU_ID_QTY
};

enum HAL_FULLMAP_MMU_LUT_SEL {
    HAL_FULLMAP_MMU_LUT_SEL_LOW_32M = 0,
    HAL_FULLMAP_MMU_LUT_SEL_HIGH_32M
};

struct HAL_FULLMAP_MMU_CONFIG_T {
    bool over_provison_mode;
    enum HAL_FULLMAP_MMU_LUT_SEL lut_sel;
    bool trans_error_en;
    bool virtual_write_en;
    bool virtual_read_en;
    bool low_power_mode;
};

void hal_fullmap_mmu_open(enum HAL_FULLMAP_MMU_ID_T id, struct HAL_FULLMAP_MMU_CONFIG_T *cfg);
void hal_fullmap_mmu_close(enum HAL_FULLMAP_MMU_ID_T id);

uint32_t hal_fullmap_mmu_get_irq_info1_write(enum HAL_FULLMAP_MMU_ID_T id);
uint32_t hal_fullmap_mmu_get_irq_info2_write(enum HAL_FULLMAP_MMU_ID_T id);
uint32_t hal_fullmap_mmu_get_irq_info1_read(enum HAL_FULLMAP_MMU_ID_T id);
uint32_t hal_fullmap_mmu_get_irq_info2_read(enum HAL_FULLMAP_MMU_ID_T id);
uint32_t hal_fullmap_mmu_get_trans_cnt_write(enum HAL_FULLMAP_MMU_ID_T id);
uint32_t hal_fullmap_mmu_get_trans_cnt_read(enum HAL_FULLMAP_MMU_ID_T id);
void hal_fullmap_mmu_read_access_bit_clear(enum HAL_FULLMAP_MMU_ID_T id);
uint32_t hal_fullmap_mmu_get_read_access_bit(enum HAL_FULLMAP_MMU_ID_T id, uint32_t page_id);//page_id:0~16383

#ifdef __cplusplus
}
#endif

#endif



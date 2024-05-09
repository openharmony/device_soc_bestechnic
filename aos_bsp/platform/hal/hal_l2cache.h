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
#ifndef __HAL_L2CACHE_H__
#define __HAL_L2CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_addr_map.h"
#include "plat_types.h"
#include "hal_cmu.h"

#ifdef L2CC0_BASE
#define L2CACHE_FUNC_DECLARE(d, r)    d
#else
#define L2CACHE_FUNC_DECLARE(d, r)    static inline d { r; }
#endif

enum HAL_L2CACHE_ID_T {
#ifdef L2CC0_BASE
    HAL_L2CACHE_ID_0,
#endif
#ifdef L2CC1_BASE
    HAL_L2CACHE_ID_1,
#endif

    HAL_L2CACHE_ID_QTY,
};

L2CACHE_FUNC_DECLARE(int hal_l2cache_enable(enum HAL_L2CACHE_ID_T id), return 0);
L2CACHE_FUNC_DECLARE(int hal_l2cache_disable(enum HAL_L2CACHE_ID_T id), return 0);
L2CACHE_FUNC_DECLARE(int hal_l2cache_enabled(enum HAL_L2CACHE_ID_T id), return 0);
L2CACHE_FUNC_DECLARE(int hal_l2cache_invalidate_all(enum HAL_L2CACHE_ID_T id), return 0);
L2CACHE_FUNC_DECLARE(int hal_l2cache_invalidate(enum HAL_L2CACHE_ID_T id, uint32_t start_address, uint32_t len), return 0);
L2CACHE_FUNC_DECLARE(int hal_l2cache_sync_all(enum HAL_L2CACHE_ID_T id), return 0);
L2CACHE_FUNC_DECLARE(int hal_l2cache_sync(enum HAL_L2CACHE_ID_T id, uint32_t start_address, uint32_t len), return 0);
L2CACHE_FUNC_DECLARE(void hal_l2cache_sleep(enum HAL_CMU_LPU_SLEEP_MODE_T mode), return);
L2CACHE_FUNC_DECLARE(void hal_l2cache_wakeup(enum HAL_CMU_LPU_SLEEP_MODE_T mode), return);

#ifdef __cplusplus
}
#endif

#endif

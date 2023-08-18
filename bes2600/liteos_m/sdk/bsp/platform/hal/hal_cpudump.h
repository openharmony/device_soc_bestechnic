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
#ifndef __HAL_CPUDUMP_H__
#define __HAL_CPUDUMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_addr_map.h"

#include "plat_types.h"

enum HAL_CPUDUMP_ID_T {
#ifdef CPUDUMP_BASE
    HAL_CPUDUMP_ID_0,
#endif
#ifdef BTC_CPUDUMP_BASE
    HAL_CPUDUMP_ID_BTC,
#endif
#ifdef WIFI_CPUDUMP_BASE
    HAL_CPUDUMP_ID_WF,
#endif

    HAL_CPUDUMP_ID_QTY
};

void hal_cpudump_enable(enum HAL_CPUDUMP_ID_T id);
void hal_cpudump_disable(enum HAL_CPUDUMP_ID_T id);
void hal_cpudump_dump_pc(enum HAL_CPUDUMP_ID_T id);
void hal_cpudump_dump_lr(enum HAL_CPUDUMP_ID_T id);

int hal_cpudump_pc_read(enum HAL_CPUDUMP_ID_T id, uint32_t *buff, uint16_t size);
int hal_cpudump_lr_read(enum HAL_CPUDUMP_ID_T id, uint32_t *buff, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif

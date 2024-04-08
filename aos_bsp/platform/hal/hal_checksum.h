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
#ifndef __HAL_CHECKSUM_H__
#define __HAL_CHECKSUM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

struct HAL_CHS_CFG_T {
    uint8_t type_tcp;
    void *data;
    uint16_t len;
    uint32_t src;
    uint32_t dst;
    uint8_t proto;
    bool out_bigd;
};

typedef void (*HAL_CHS_RUN_CB_T)(const uint16_t res, uint32_t error);

void hal_checksum_open(void);
void hal_checksum_close(void);

int hal_checksum_start(const struct HAL_CHS_CFG_T *cfg, uint16_t *res, HAL_CHS_RUN_CB_T cb);
int hal_checksum_tcp_start(uint8_t *addr, uint16_t *res, HAL_CHS_RUN_CB_T cb);
int hal_checksum_ip_start(uint8_t *addr, uint16_t *res, HAL_CHS_RUN_CB_T cb);

#endif

#ifdef __cplusplus
}
#endif

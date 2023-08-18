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
#ifndef __HAL_PCIE_H__
#define __HAL_PCIE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

void hal_pcie_open(void);

void hal_pcie_sel_wifi(void);

bool hal_pcie_get_ts1_status(void);

uint32_t hal_pcie_get_dig_status(void);

void hal_pcie_state_check(void);

void hal_pcie_register_irq(void);

#ifdef __cplusplus
}
#endif

#endif


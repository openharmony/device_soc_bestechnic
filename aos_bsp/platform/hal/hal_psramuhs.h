/***************************************************************************
 *
 * Copyright 2015-2019 BES.
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
#ifndef __HAL_PSRAMUHS_H__
#define __HAL_PSRAMUHS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"
#include "hal_cmu.h"

void hal_psramuhs_sleep(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
void hal_psramuhs_wakeup(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
void hal_psramuhs_init(void);
void hal_psramuhs_hold(void);
void hal_psramuhs_release(void);
void hal_psramuhs_refresh_enable(void);
void hal_psramuhs_refresh_disable(void);
uint32_t hal_psramuhs_ca_calib_result(void);

#ifdef __cplusplus
}
#endif

#endif


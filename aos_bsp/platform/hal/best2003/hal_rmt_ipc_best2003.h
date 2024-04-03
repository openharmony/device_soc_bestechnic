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
#ifndef __HAL_RMT_IPC_BEST2003_H__
#define __HAL_RMT_IPC_BEST2003_H__

#ifdef __cplusplus
extern "C" {
#endif

enum HAL_RMT_IPC_CORE_T {
    HAL_RMT_IPC_CORE_MCU_C0,
    HAL_RMT_IPC_CORE_MCU_C1,

    HAL_RMT_IPC_CORE_QTY,
};

#define HAL_RMT_IPC_CORE_MCU                HAL_RMT_IPC_CORE_MCU_C0

#if defined(CP_AS_SUBSYS) || defined(CHIP_ROLE_CP)
#define HAL_RMT_IPC_CORE_CORE0              HAL_RMT_IPC_CORE_MCU_C0
#define HAL_RMT_IPC_CORE_CORE1              HAL_RMT_IPC_CORE_MCU_C1
#endif

#ifdef __cplusplus
}
#endif

#endif


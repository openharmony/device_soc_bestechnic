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
#ifndef __HAL_TRACE_ID_H__
#define __HAL_TRACE_ID_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"
#include "stdio.h"
#include "stdarg.h"

// Binary frame format:
// <Magic> <FrameType> <FrameLenLowByte> <FrameLenHighByte> <FrameData> ...
// Magic    : 0xB7
// FrameType: e.g, 0xA5 for trace
// FrameLen : 16-bit frame length including 2 bytes of this frame len field
// FrameData: Content of frame

#define TRACE_BIN_MAGIC                     0xB7
#define TRACE_ID_MAGIC                      ((0xA5 << 8) | TRACE_BIN_MAGIC)

#define TRACE_ID_MAX_ARG_NUM                15

// TAG
#define TAG_VAL_TWS_LR_SHIFT                (0)
#define TAG_VAL_TWS_LR_MASK                 (0x03<<TAG_VAL_TWS_LR_SHIFT)   //2 bit ,L,R,N
#define TAG_VAL_TWS_LR(n)                   (((n) & 0x03) << 0)
#define TAG_VAL_TWS_TWS_N                   0
#define TAG_VAL_TWS_TWS_R                   1
#define TAG_VAL_TWS_TWS_L                   2

#define TAG_VAL_TWS_ROLE_SHIFT              (2)
#define TAG_VAL_TWS_ROLE_MASK               (0x03<<TAG_VAL_TWS_ROLE_SHIFT) //2 bit ,M,S,U,F
#define TAG_VAL_TWS_ROLE(n)                 (((n) & 0x03) << 2)
#define TAG_VAL_ROLE_UNKNOWN                0
#define TAG_VAL_ROLE_MASTER                 1
#define TAG_VAL_ROLE_SLAVE                  2
#define TAG_VAL_ROLE_FREEMAN                3

#define TAG_VAL_CPUID_SHIFT                 (4)
#define TAG_VAL_CPUID_MASK                  (0x07<<TAG_VAL_CPUID_SHIFT) //3 bit
#define TAG_VAL_CPUID(n)                    (((n) & 0x07) << 4)
#define TAG_VAL_CPU_DSPM                    0
#define TAG_VAL_CPU_DSPH                    1
#define TAG_VAL_CPU_DSPA0                   0
#define TAG_VAL_CPU_DSPA1                   1
#define TAG_VAL_CPU_SYS0                    2
#define TAG_VAL_CPU_SYS1                    3
#define TAG_VAL_CPU_MCU0                    2
#define TAG_VAL_CPU_MCU1                    3
#define TAG_VAL_CPU_BTH0                    4
#define TAG_VAL_CPU_BTH1                    5
#define TAG_VAL_CPU_SENS0                   6
#define TAG_VAL_CPU_SENS1                   7

#define TAG_VAL_SECURE_SHIFT                (7)
#define TAG_VAL_SECURE_MASK                 (1<<TAG_VAL_SECURE_SHIFT) //1 bit, NS/,SE/
#define TAG_VAL_SECURE(n)                   (((n) & 0x01) << 7)
#define TAG_VAL_SECURE_NS                   0
#define TAG_VAL_SECURE_SE                   1

#define ATTR_VAL_LEVEL_SHIFT                0
#define ATTR_VAL_LEVEL_MASK                 (0x7 << ATTR_VAL_LEVEL_SHIFT)
#define ATTR_VAL_LEVEL(n)                   BITFIELD_VAL(ATTR_VAL_LEVEL, n)
#define ATTR_VAL_MOD_SHIFT                  3
#define ATTR_VAL_MOD_MASK                   (0x7F << ATTR_VAL_MOD_SHIFT)
#define ATTR_VAL_MOD(n)                     BITFIELD_VAL(ATTR_VAL_MOD, n)
#define ATTR_VAL_NO_LF                      (1 << 10)
#define ATTR_VAL_NO_TS                      (1 << 11)

struct LOG_HDR_T {
    uint16_t magic;
    uint16_t len;

    uint32_t ver        :  4;
    uint32_t time       : 28;

    uint32_t tag        :  8;
    uint32_t attr       : 12;
    uint32_t seq        : 11;
    uint32_t is_irq     :  1;

    uint32_t task       :  8;
    uint32_t addr       : 24;
};

void hal_trace_set_tag(char *buf, unsigned int value);

unsigned int hal_trace_get_tag(char *buf);

#ifdef __cplusplus
}
#endif
#endif

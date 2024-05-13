/***************************************************************************
 *
 * Copyright 2015-2020 BES.
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
#ifndef __PLAT_ADDR_MAP_H__
#define __PLAT_ADDR_MAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HOST_TOOL_BUILD

#if 0
#elif defined(CHIP_FPGA1000)
#define CHIP_ID_LITERAL                         fpga1000
#elif defined(CHIP_BEST1000)
#define CHIP_ID_LITERAL                         best1000
#elif defined(CHIP_BEST1305)
#define CHIP_ID_LITERAL                         best1305
#elif defined(CHIP_BEST1306)
#define CHIP_ID_LITERAL                         best1306
#elif defined(CHIP_BEST1306P)
#define CHIP_ID_LITERAL                         best1306p
#elif defined(CHIP_BEST1307)
#define CHIP_ID_LITERAL                         best1307
#elif defined(CHIP_BEST1400) || defined(CHIP_BEST1402)
#define CHIP_ID_LITERAL                         best1400
#elif defined(CHIP_BEST1501)
#define CHIP_ID_LITERAL                         best1501
#elif defined(CHIP_BEST1501P)
#define CHIP_ID_LITERAL                         best1501p
#elif defined(CHIP_BEST1502P)
#define CHIP_ID_LITERAL                         best1502p
#elif defined(CHIP_BEST1502X)
#define CHIP_ID_LITERAL                         best1502x
#elif defined(CHIP_BEST1503)
#define CHIP_ID_LITERAL                         best1503
#elif defined(CHIP_BEST1600)
#define CHIP_ID_LITERAL                         best1600
#elif defined(CHIP_BEST1603)
#define CHIP_ID_LITERAL                         best1603
#elif defined(CHIP_BEST1605)
#define CHIP_ID_LITERAL                         best1605
#elif defined(CHIP_BEST1700)
#define CHIP_ID_LITERAL                         best1700
#elif defined(CHIP_BEST2000)
#define CHIP_ID_LITERAL                         best2000
#elif defined(CHIP_BEST2001)
#define CHIP_ID_LITERAL                         best2001
#elif defined(CHIP_BEST2002)
#define CHIP_ID_LITERAL                         best2002
#elif defined(CHIP_BEST2003)
#define CHIP_ID_LITERAL                         best2003
#elif defined(CHIP_BEST2005)
#define CHIP_ID_LITERAL                         best2005
#elif defined(CHIP_BEST2007)
#define CHIP_ID_LITERAL                         best2007
#elif defined(CHIP_BEST2007P)
#define CHIP_ID_LITERAL                         best2007p
#elif defined(CHIP_BEST2007H)
#define CHIP_ID_LITERAL                         best2007h
#elif defined(CHIP_BEST2009)
#define CHIP_ID_LITERAL                         best2009
#elif defined(CHIP_BEST2300)
#define CHIP_ID_LITERAL                         best2300
#elif defined(CHIP_BEST2300A)
#define CHIP_ID_LITERAL                         best2300a
#elif defined(CHIP_BEST2300P)
#define CHIP_ID_LITERAL                         best2300p
#elif defined(CHIP_BEST3001) || defined(CHIP_BEST3005)
#define CHIP_ID_LITERAL                         best3001
#elif defined(CHIP_BEST3003)
#define CHIP_ID_LITERAL                         best3003
#elif defined(CHIP_BEST3601)
#define CHIP_ID_LITERAL                         best3601
#elif defined(CHIP_BEST1501SIMU)
#define CHIP_ID_LITERAL                         best1501simu
#elif defined(CHIP_BEST1600SIMU)
#define CHIP_ID_LITERAL                         best1600simu
#else
#error "Unknown chip ID"
#endif

#endif /* !HOST_TOOL_BUILD */

#define _TO_STRING_A(s)                         # s
#define _TO_STRING(s)                           _TO_STRING_A(s)

#define CONCAT_NAME_A(a, b)                     a ## b
#define CONCAT_NAME(a, b)                       CONCAT_NAME_A(a, b)
#define CONCAT_SUFFIX(a, b)                     a.b

#ifndef HOST_TOOL_BUILD

#define CONSTRUCT_HDR_NAME_A(d, n)              d/n ## _ ## d
#define CONSTRUCT_HDR_NAME(d, n)                CONSTRUCT_HDR_NAME_A(d, n)
#define CHIP_SPECIFIC_HDR(n)                    _TO_STRING(CONSTRUCT_HDR_NAME(CHIP_ID_LITERAL, n).h)

#include CHIP_SPECIFIC_HDR(plat_addr_map)

#ifndef ROMX_BASE
#define ROMX_BASE                               ROM_BASE
#endif

#ifndef ROM_EXT_SIZE
#define ROM_EXT_SIZE                            0
#endif

#define ROM_TOTAL_SIZE                          (ROM_SIZE + ROM_EXT_SIZE)

#ifndef RAMX_SIZE
#define RAMX_SIZE                               RAM_SIZE
#endif

#ifndef RAMXRET_SIZE
#define RAMXRET_SIZE                            RAMRET_SIZE
#endif

#ifndef RAMX_NS_SIZE
#define RAMX_NS_SIZE                            RAM_NS_SIZE
#endif

#ifdef MAIN_RAM_USE_TCM
#define RAM_TO_RAMX(a)                          (a)
#define RAMX_TO_RAM(a)                          (a)
#else
#define RAM_TO_RAMX(a)                          ((a) - RAM_BASE + RAMX_BASE)
#define RAMX_TO_RAM(a)                          ((a) - RAMX_BASE + RAM_BASE)
#endif

#ifdef RAM_SECURITY_FLAG
#define RAM_NS_TO_S(a)                          ((a) | RAM_SECURITY_FLAG)
#define RAM_S_TO_NS(a)                          ((a) & ~RAM_SECURITY_FLAG)
#else
#define RAM_NS_TO_S(a)                          (a)
#define RAM_S_TO_NS(a)                          (a)
#endif

#ifdef RAM_REGION_OFFSET
#define RAM_REGION_BASE                         (RAM_BASE + RAM_REGION_OFFSET)
#else
#define RAM_REGION_BASE                         RAM_BASE
#endif
#ifndef RAM_REGION_SIZE
#define RAM_REGION_SIZE                         (RAM_BASE + RAM_SIZE - RAM_REGION_BASE)
#endif

#ifdef RAMX_REGION_OFFSET
#define RAMX_REGION_BASE                        (RAMX_BASE + RAMX_REGION_OFFSET)
#else
#define RAMX_REGION_BASE                        RAMX_BASE
#endif
#ifndef RAMX_REGION_SIZE
#define RAMX_REGION_SIZE                        (RAMX_BASE + RAMX_SIZE - RAMX_REGION_BASE)
#endif

#define FLASH_TO_FLASHX(a)                      ((a) - FLASH_BASE + FLASHX_BASE)
#define FLASHX_TO_FLASH(a)                      ((a) - FLASHX_BASE + FLASH_BASE)

#define PSRAM_TO_PSRAMX(a)                      ((a) - PSRAM_BASE + PSRAMX_BASE)
#define PSRAMX_TO_PSRAM(a)                      ((a) - PSRAMX_BASE + PSRAM_BASE)

#define PSRAMUHSX_TO_PSRAMUHS(a)                ((a) - PSRAMUHSX_BASE + PSRAMUHS_BASE)
#define PSRAMUHS_TO_PSRAMUHSX(a)                ((a) - PSRAMUHS_BASE + PSRAMUHSX_BASE)

#define FLASH_C_TO_NC(a)                        ((a) - FLASH_BASE + FLASH_NC_BASE)
#define FLASH_NC_TO_C(a)                        ((a) - FLASH_NC_BASE + FLASH_BASE)

#define FLASH1_C_TO_NC(a)                       ((a) - FLASH1_BASE + FLASH1_NC_BASE)
#define FLASH1_NC_TO_C(a)                       ((a) - FLASH1_NC_BASE + FLASH1_BASE)

#define FLASH2_C_TO_NC(a)                       ((a) - FLASH2_BASE + FLASH2_NC_BASE)
#define FLASH2_NC_TO_C(a)                       ((a) - FLASH2_NC_BASE + FLASH2_BASE)

#define PSRAM_C_TO_NC(a)                        ((a) - PSRAM_BASE + PSRAM_NC_BASE)
#define PSRAM_NC_TO_C(a)                        ((a) - PSRAM_NC_BASE + PSRAM_BASE)

#define PSRAMUHS_C_TO_NC(a)                     ((a) - PSRAMUHS_BASE + PSRAMUHS_NC_BASE)
#define PSRAMUHS_NC_TO_C(a)                     ((a) - PSRAMUHS_NC_BASE + PSRAMUHS_BASE)

#ifndef HAL_NORFLASH_ADDR_MASK
/* 64M Bytes */
#define HAL_NORFLASH_ADDR_MASK                  0x03FFFFFF
#endif
#if defined(HAL_NORFLASH0_ADDR_MASK) || defined(HAL_NORFLASH1_ADDR_MASK) || defined(HAL_NORFLASH2_ADDR_MASK)
#define HAL_NORFLASH_ADDR_MASK_BY_CTRL_ID
#endif
#if defined(FLASH_CTRL_BASE) && !defined(HAL_NORFLASH0_ADDR_MASK)
#define HAL_NORFLASH0_ADDR_MASK                 HAL_NORFLASH_ADDR_MASK
#endif
#if defined(FLASH1_CTRL_BASE) && !defined(HAL_NORFLASH1_ADDR_MASK)
#define HAL_NORFLASH1_ADDR_MASK                 HAL_NORFLASH_ADDR_MASK
#endif
#if defined(FLASH2_CTRL_BASE) && !defined(HAL_NORFLASH2_ADDR_MASK)
#define HAL_NORFLASH2_ADDR_MASK                 HAL_NORFLASH_ADDR_MASK
#endif

#endif /* !HOST_TOOL_BUILD */

#define BUILD_INFO_MAGIC                        0xBE57341D

#define TRACE_ID_STR_OFFSET                     0xFF000000

#ifdef LINKER_SCRIPT

/* Common RAM config checks (only check in lds file, the first compiled file during build) */

#if (RAM_BASE & (0x400 - 1))
#error "RAM_BASE should be aligned on 0x400 boundary"
#endif
#if defined(RAMCP_BASE) && (RAMCP_BASE & (0x400 - 1))
#error "RAMCP_BASE should be aligned on 0x400 boundary"
#endif

#ifdef RAM_REGION_OFFSET
#if (RAM_SIZE <= RAM_REGION_OFFSET)
#error "Bad RAM_REGION_OFFSET"
#endif
#if (RAM_SIZE < RAM_REGION_OFFSET + RAM_REGION_SIZE)
#error "Bad RAM_REGION_SIZE"
#endif
#endif

#ifdef RAMX_REGION_OFFSET
#if (RAMX_SIZE <= RAMX_REGION_OFFSET)
#error "Bad RAMX_REGION_OFFSET"
#endif
#if (RAMX_SIZE < RAMX_REGION_OFFSET + RAMX_REGION_SIZE)
#error "Bad RAMX_REGION_SIZE"
#endif
#endif

#if defined(SEPARATE_RAM_RAMX) && !defined(MAIN_RAM_USE_TCM)
#if !defined(RAM_REGION_OFFSET) || !defined(RAMX_REGION_OFFSET)
#error "RAM_REGION_OFFSET and RAMX_REGION_OFFSET should be defined with SEPARATE_RAM_RAMX"
#endif
#if !((RAM_REGION_OFFSET + RAM_REGION_SIZE <= RAMX_REGION_OFFSET) || \
        (RAMX_REGION_OFFSET + RAMX_REGION_SIZE <= RAM_REGION_OFFSET))
#error "RAM_REGION_OFFSET and RAMX_REGION_OFFSET overlap"
#endif
#endif

#endif /* LINKER_SCRIPT */

#ifdef __cplusplus
}
#endif

#endif


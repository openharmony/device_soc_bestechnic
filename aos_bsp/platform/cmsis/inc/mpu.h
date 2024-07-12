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
#ifndef __MPU_H__
#define __MPU_H__

#include "plat_types.h"
#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The MPU region numbers
 */
#ifdef __MPU_REGION_NUM
#define MPU_ID_QTY                          __MPU_REGION_NUM
#else
#define MPU_ID_QTY                          (8)
#endif

#define MPU_INVALID_ID                      0xFF

enum MPU_ATTR_T {
    MPU_ATTR_READ_WRITE_EXEC = 0,
    MPU_ATTR_READ_EXEC,
    MPU_ATTR_EXEC,
    MPU_ATTR_READ_WRITE,
    MPU_ATTR_READ,
    MPU_ATTR_NO_ACCESS,

    MPU_ATTR_QTY,
};

enum MEM_ATTR_T {
    MEM_ATTR_NORMAL_WRITE_THROUGH,
    MEM_ATTR_NORMAL_WRITE_BACK,
    MEM_ATTR_NORMAL_NON_CACHEABLE,
    MEM_ATTR_DEVICE,
    MEM_ATTR_4,
    MEM_ATTR_5,
    MEM_ATTR_6,
    MEM_ATTR_7,

    MEM_ATTR_QTY,

    MEM_ATTR_FLASH          = MEM_ATTR_NORMAL_WRITE_THROUGH,
    MEM_ATTR_INT_SRAM       = MEM_ATTR_NORMAL_WRITE_BACK,
    MEM_ATTR_EXT_RAM        = MEM_ATTR_NORMAL_WRITE_BACK,
};

typedef struct
{
    uint32_t addr;
    uint32_t len;
    enum MPU_ATTR_T ap_attr;
#if defined(__ARM_ARCH_8M_MAIN__)
    enum MEM_ATTR_T mem_attr;
#endif
} mpu_regions_t;

/**
 * @brief Open the MPU.
 *
 * @return 0 on success, negative error code on failure.
 */
int mpu_open(void);

/**
 * @brief Close the MPU.
 *
 * @return 0 on success, negative error code on failure.
 */
int mpu_close(void);

/**
 * @brief Set MPU region.
 *
 * @param addr The base address of the MPU region. Must be aligned to 32 bytes for armv8m.
 * @param len The length of the MPU region. Valid lengths: 32, 64, 128, 256, 512, 1K, 2K, ..., 4G.
 * @param attr MPU attributes.
 * @param mem_attr Memory attributes.
 * @return 0 on success, negative error code on failure.
 */
int mpu_set(uint32_t addr, uint32_t len, enum MPU_ATTR_T attr, enum MEM_ATTR_T mem_attr);

/**
 * @brief Clear MPU region.
 *
 * @param addr The base address of the MPU region to clear.
 * @param len The length of the MPU region to clear.
 * @return 0 on success, negative error code on failure.
 */
int mpu_clear(uint32_t addr, uint32_t len);

/**
 * @brief Setup MPU regions during boot.
 *
 * @param mpu_table Pointer to the table of MPU regions.
 * @param region_num Number of regions in the table.
 * @return 0 on success, negative error code on failure.
 */
int mpu_boot_setup(const mpu_regions_t *mpu_table, uint32_t region_num);

/**
 * @brief Setup MPU regions.
 *
 * @param mpu_table Pointer to the table of MPU regions.
 * @param region_num Number of regions in the table.
 * @return 0 on success, negative error code on failure.
 * NOTE: this function must be called before other mpu operations, if
 *       there are no real mpu regions, just set the table to NULL.
 */
int mpu_setup(const mpu_regions_t *mpu_table, uint32_t region_num);

/**
 * @brief Allocate an MPU region.
 *
 * @return Region number on success, MPU_INVALID_ID on failure.
 */
uint8_t mpu_alloc_region(void);

/**
 * @brief Dump MPU region information.
 */
void mpu_region_dump(void);

/**
 * @brief Put the MPU to sleep.
 */
void mpu_sleep(void);

/**
 * @brief Wake the MPU up from sleep.
 */
void mpu_wakeup(void);

/**
 * @brief Disable the MPU.
 *
 * @return 0 on success, negative error code on failure.
 */
int mpu_disable(void);

/**
 * @brief Enable the MPU.
 *
 * @return 0 on success, negative error code on failure.
 */
int mpu_enable(void);

/**
 * @brief Set a no-access region in the MPU.
 *
 * @param addr The base address of the no-access region. Must be aligned to 32 bytes for armv8m.
 * @param len The length of the no-access region.
 * @return 0 on success, negative error code on failure.
 */
int mpu_no_access_set(uint32_t addr, uint32_t len);

/**
 * @brief Clear a no-access region in the MPU.
 *
 * @param no_access_id The ID of the no-access region to clear.
 * @return 0 on success, negative error code on failure.
 */
int mpu_no_access_clear(int no_access_id);

#ifdef __cplusplus
}
#endif

#endif


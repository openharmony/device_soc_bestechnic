
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
 * **************************************************************************/

#ifndef __HAL_GAMU_H__
#define __HAL_GAMU_H__

#ifdef __cplusplus
extern "C" {
#endif

enum GAMU_FORMAT_E {
    GAMU_RGB32 = 0,
    GAMU_RGB16 = 2,
    GAMU_RGB24 = 3,
};

struct GAMU_LINE_CONFIG_T {
    uint16_t pixel_start;
    uint16_t pixel_end;
};

/**
 * @brief Enable GAMU clock and initialize settings.
 *
 * @param fmt Buffer color format, RGB32/24/16;
 *
 * @return 0 for success, other for failed.
 */
int hal_gamu_open(enum GAMU_FORMAT_E fmt);

/**
 * @brief Disable GAMU clock and clear settings.
 */
void hal_gamu_close(void);

/**
 * @brief Enter low power mode.
 */
void hal_gamu_sleep(void);

/**
 * @brief Exit low power mode.
 */
void hal_gamu_wakeup(void);

/**
 * @brief Configure GAMU buffer shape, line by line;
 *
 * @param cfg Buffer line configs to define visible area;
 * @param count Buffer line configs count;
 *
 * @return 0 for success, other for failed.
 */
int hal_gamu_config_shape(struct GAMU_LINE_CONFIG_T *cfg, int count);

/**
 * @brief Configure and enable GAMU buffer, call after hal_gamu_config_shape().
 *
 * @param id Buffer ID, 0~3;
 * @param paddr Buffer physical memory address;
 * @param psize Buffer Physical memory size;
 *
 * @return 0 for success, other for failed.
 */
int hal_gamu_config_buf(uint8_t id, uint32_t paddr, uint32_t psize);

/**
 * @brief Get the width of GAMU buffer;
 */
uint16_t hal_gamu_buf_width(void);

/**
 * @brief Get the height of GAMU buffer;
 */
uint16_t hal_gamu_buf_height(void);

/**
 * @brief Get the virtual address of pixel in GAMU buffer.
 *
 * @param id Buffer ID, 0~3;
 * @param x  pixel offset x;
 * @param y  pixel offset y;
 *
 * @return 0 for invalid address;
 */
uint32_t hal_gamu_buf_vaddr(uint8_t id, uint16_t x, uint16_t y);

/**
 * @brief Set the fake read data.
 */
void hal_gamu_set_fake_rdata(uint32_t val);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_GAMU_H__ */

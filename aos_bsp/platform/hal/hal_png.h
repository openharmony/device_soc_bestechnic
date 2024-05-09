/***************************************************************************
 *
 * Copyright 2022-2023 BES.
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
#ifndef __HAL_PNG_H__
#define __HAL_PNG_H__
#include "plat_types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum HAL_PNG_RET_T {
    HAL_PNG_OK = 0,
    HAL_PNG_ERR = 1,
    HAL_PNG_NOT_SUPPORT = 2,
    HAL_PNG_DMA_WR_ERR = 3,
    HAL_PNG_COMP_MODE_ERR = 4,
    HAL_PNG_DECODE_ERR = 5,
};

enum HAL_PNG_COLOR_TYPE_T {
    HAL_PNG_COLOR_GRAY = 0,
    HAL_PNG_COLOR_RGB888 = 2,
    HAL_PNG_COLOR_INDEX = 3,
    HAL_PNG_COLOR_GRAYALPHA = 4,
    HAL_PNG_COLOR_RGBA8888 = 6,
};

enum HAL_PNG_PIX_FORMAT_T {
    HAL_PNG_PIX_RGBA8888 = 0,
    HAL_PNG_PIX_ARGB8888,
    HAL_PNG_PIX_RGB888,
    HAL_PNG_PIX_RGB565,
    HAL_PNG_PIX_RGBA565,
    HAL_PNG_PIX_ARGB565,
};

struct HAL_PNG_CONFIG_T {
    /* Set by user */
    uint32_t png_size;
    void *png_data;
    enum HAL_PNG_PIX_FORMAT_T pix_fmt;
    void *pix_buf;
    void (*done_cb)(void);
    uint8_t global_alpha;

    /* Set by decoder */
    enum HAL_PNG_RET_T result;
    uint16_t img_width;
    uint16_t img_height;
    uint8_t img_color_type;
    uint8_t img_color_depth;
    uint8_t img_comp_mode;
    uint8_t img_comp_level;
};

/**
 * @brief Enable PNG decoder.
 */
void hal_png_open(void);

/**
 * @brief Disable PNG decoder.
 */
void hal_png_close(void);

/**
 * @brief Start PNG decoding.
 *
 * @param cfg PNG decoding configuration.
 */
int hal_png_start(struct HAL_PNG_CONFIG_T *cfg);

/**
 * @brief Finish PNG decoding.
 */
void hal_png_stop(void);

/**
 * @brief Check PNG Codec working status.
 */
bool hal_png_busy(void);

#ifdef __cplusplus
}
#endif

#endif


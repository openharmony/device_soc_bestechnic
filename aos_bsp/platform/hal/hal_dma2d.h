/***************************************************************************
 *
 * Copyright 2015-2024 BES.
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

#ifndef __HAL_DMA2D_H__
#define __HAL_DMA2D_H__

enum DMA2D_RET_T {
    DMA2D_OK = 0,
    DMA2D_FAILED,
    DMA2D_ERROR,
    DMA2D_TIMEOUT,
    DMA2D_UNALIGNMENT,
};

enum DMA2D_BLEND_MODE_T {
    DMA2D_BLEND_NONE = 0,
    DMA2D_BLEND_CONST_ALPHA,
    DMA2D_BLEND_PIXEL_ALPHA,
};

enum DMA2D_COLOR_FORMAT_T {
    DMA2D_RGBA8888 = 0x00,
    DMA2D_BGRA8888 = 0x80,
    DMA2D_ARGB8888 = 0x01,
    DMA2D_ABGR8888 = 0x81,
    DMA2D_RGB888   = 0x02,
    DMA2D_BGR888   = 0x82,
    DMA2D_RGB565   = 0x03,
    DMA2D_BGR565   = 0x83,
};

struct DMA2D_BUFFER_T {
    int width;
    int height;
    enum DMA2D_COLOR_FORMAT_T format;
    uint8_t *data;
};

struct DMA2D_AREA_T {
    int x;
    int y;
    int w;
    int h;
};

/**
 * @brief Enable DMA2D clocks and register callback.
 *
 * @param done_cb callback function pointer.
 */
int hal_dma2d_open(void (*done_cb)(int));

/**
 * @brief Disable DMA2D clocks.
 */
int hal_dma2d_close(void);

/**
 * @brief Enter low power mode.
 */
void hal_dma2d_sleep(void);

/**
 * @brief Exit low power mode.
 */
void hal_dma2d_wakeup(void);

/**
 * @brief Fill color to a buffer.
 *
 * @param buf   target buffer.
 * @param area  fill area, NULL for the whole buffer.
 * @param color color value.
 */
int hal_dma2d_clear(struct DMA2D_BUFFER_T *buf, struct DMA2D_AREA_T *area, uint32_t color);

/**
 * @brief Copy part/whole buffer to target buffer.
 *
 * @param src_buf  source buffer.
 * @param src_area source area, NULL for the whole buffer.
 * @param dst_buf  target buffer.
 * @param dst_area target area, NULL for the whole buffer.
 * @param scale_x  width scaling, in percent, > 1;
 * @param scale_y  height scaling, in percent, > 1;
 */
int hal_dma2d_blit(struct DMA2D_BUFFER_T *src_buf, struct DMA2D_AREA_T *src_area,
                   struct DMA2D_BUFFER_T *dst_buf, struct DMA2D_AREA_T *dst_area,
                   int scale_x, int scale_y);

/**
 * @brief Blend one buffer to target buffer.
 *
 * @param src_buf  source buffer.
 * @param src_area source area, NULL for the whole buffer.
 * @param dst_buf  target buffer.
 * @param dst_area target area, NULL for the whole buffer.
 * @param blend    blend mode.
 * @param alpha    alpha value, only valid for CONST_ALPHA mode.
 * @param scale_x  width scaling, in percent, > 1;
 * @param scale_y  height scaling, in percent, > 1;
 */
int hal_dma2d_blend(struct DMA2D_BUFFER_T *src_buf, struct DMA2D_AREA_T *src_area,
                   struct DMA2D_BUFFER_T *dst_buf, struct DMA2D_AREA_T *dst_area,
                   enum DMA2D_BLEND_MODE_T blend, uint8_t alpha,
                   int scale_x, int scale_y);

/**
 * @brief Blend two buffers to target buffer.
 *
 * @param src_buf1 source buffer1.
 * @param src_buf2 source buffer2.
 * @param dst_buf  target buffer.
 * @param dst_area target area, NULL for the whole buffer.
 * @param blend    blend mode.
 * @param alpha1   alpha for buffer1, only valid for CONST_ALPHA mode.
 * @param alpha2   alpha for buffer2, only valid for CONST_ALPHA mode.
 */
int hal_dma2d_blend2(struct DMA2D_BUFFER_T *src_buf1, struct DMA2D_BUFFER_T *src_buf2,
                   struct DMA2D_BUFFER_T *dst_buf, struct DMA2D_AREA_T *dst_area,
                   enum DMA2D_BLEND_MODE_T blend, uint8_t alpha1, uint8_t alpha2);

bool hal_dma2d_busy(void);

#endif /* __HAL_DMA2D_H__ */

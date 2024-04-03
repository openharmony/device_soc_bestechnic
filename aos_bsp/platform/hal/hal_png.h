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

enum PNG_PIX_FMT_T {
    PNG_PIX_FMT_RGB565      = 0,
    PNG_PIX_FMT_ARGB565     = 1,
    PNG_PIX_FMT_RGBA565     = 1,
    PNG_PIX_FMT_RGB888      = 1,
    PNG_PIX_FMT_ARGB888     = 2,
    PNG_PIX_FMT_RGBA888     = 3,
};

enum PNG_COLOR_TYPE_T {
    PNG_COLOR_GRAY        = 0,
    PNG_COLOR_UNKNOWN0    = 1,
    PNG_COLOR_RGB888      = 2,
    PNG_COLOR_IRGB888     = 3,
    PNG_COLOR_GRAYA       = 4,
    PNG_COLOR_UNKNOWN1    = 5,
    PNG_COLOR_RGBA888     = 6,
};

struct png_decoder {
    uint32_t base_addr;
};

struct png_imginfo {
  enum PNG_COLOR_TYPE_T type;
  uint16_t width;
  uint16_t height;
  uint8_t  bpp;
  uint8_t  c_mode;
};

void hal_png_decoder_clk_enable(struct png_decoder * decoder);

int hal_png_decoder_input_burst_length(struct png_decoder * decoder,uint8_t len);

/**
 * hal_png_decoder_ena_output_globle_alpha - enable/disable output pix color globle alpha
 * @decoder  :decoder object
 * @ena
 */
void hal_png_decoder_ena_output_globle_alpha(struct png_decoder * decoder, bool  ena);

/**
 * hal_png_decoder_set_output_globle_alpha - set output pix color globle alpha
 * @decoder  :decoder object
 * @alpha
 */
void hal_png_decoder_set_output_globle_alpha(struct png_decoder * decoder, uint8_t  alpha);

/**
 * hal_png_decoder_output_pix_rgb565 - set output pix color format rgb565
 * @decoder  :decoder object
 * @ena
 */
void hal_png_decoder_output_pix_rgb565(struct png_decoder * decoder, bool ena);

/**
 * hal_png_decoder_output_pix_rgba565 - set output pix color format rgba565
 * @decoder  :decoder object
 * @ena
 */
void hal_png_decoder_output_pix_rgba565(struct png_decoder * decoder, bool ena);

/**
 * hal_png_decoder_output_pix_argb565 - set output pix color format argb565
 * @decoder  :decoder object
 * @ena
 */
void hal_png_decoder_output_pix_argb565(struct png_decoder * decoder, bool ena);

/**
 * hal_png_decoder_pixdma_burst_length - set pixdma burst length
 * @decoder  :decoder object
 * @len  : dma burst length 8:128bytes 4:64bytes
 */

int hal_png_decoder_pixdma_burst_length(struct png_decoder * decoder,uint8_t len);

/**
 * hal_png_decoder_png_file_start_address - set png file memory start address
 * @decoder  :decoder object
 * @addr  : memory buffer address
 */
int hal_png_decoder_png_file_start_address(struct png_decoder * decoder, uint32_t addr);

/**
 * hal_png_decoder_pixdma_start_address - set pixdma memory start address
 * @decoder  :decoder object
 * @addr  : memory buffer0 address
 */
int hal_png_decoder_pixdma_start_address(struct png_decoder * decoder, uint32_t addr);

/**
 * hal_png_decoder_output_pix_alpha_hpos - set pix color alpha pos
 * @decoder  :decoder object
 * @high       :alpha pos
 */
int hal_png_decoder_output_pix_alpha_hpos(struct png_decoder * decoder, bool high);

/**
 * hal_png_decoder_pix_color_format - set pix color format
 * @decoder  :decoder object
 * @fmt       :color format
 */
void hal_png_decoder_pix_color_format(struct png_decoder * decoder, enum  PNG_PIX_FMT_T  fmt);

/**
 * hal_png_decoder_pixdma_output_trigger_lines - set pixdma trigger lines
 * @decoder  :decoder object
 * @length  : trigger interrupt after lines
 */
int hal_png_decoder_pixdma_output_trigger_lines(struct png_decoder * decoder, uint32_t lines);

/**
 * hal_png_decoder_file_size - set input file size
 * @decoder  :decoder object
 * @fsize     : size in bytes
 */
void hal_png_decoder_file_size(struct png_decoder * decoder,uint32_t fsize);

/**
 * hal_png_decoder_image_pitch - set image pitch
 * @decoder  :decoder object
 * @pitch   : pitch in bytes
 */
void hal_png_decoder_image_pitch(struct png_decoder * decoder, uint16_t pitch);

/**
 * hal_png_decoder_pixdma_stop
 * @decoder     - decoder object
 */
void hal_png_decoder_pixdma_stop(struct png_decoder * decoder);

/**
 * hal_png_decoder_pixdma_start
 * @decoder     - decoder object
 */
 void hal_png_decoder_pixdma_start(struct png_decoder * decoder);

/**
 * hal_png_decoder_stop - stop pix data dma
 * @decoder     - decoder object
 */
void hal_png_decoder_stop(struct png_decoder * decoder);
/**
 * hal_png_decoder_start - start pix data dma
 * @decoder     - decoder object
 */
 void hal_png_decoder_start(struct png_decoder * decoder);

/**
 * hal_png_decoder_get_imginfo - start pix data dma
 * @decoder     - decoder object
 * @info        - image info object
 */
 void hal_png_decoder_get_imginfo(struct png_decoder * decoder, struct png_imginfo* info);

/**
 * hal_png_decoder_reset_interface - decoder reset bus interface
 * @decoder     - decoder object
 */
 void hal_png_decoder_reset_interface(struct png_decoder * decoder);

/**
 * hal_png_decoder_irqn - get  png decoder irq  number
 */
int hal_png_decoder_irqn(struct png_decoder * decoder);

/**
 * hal_png_decoder_irq_disable
 */
int hal_png_decoder_irq_disable(struct png_decoder * decoder);

/**
 * hal_png_decoder_irq_enable
 */
int hal_png_decoder_irq_enable(struct png_decoder * decoder);

/**
 * hal_png_decoder_irq_clear
 */
int hal_png_decoder_irq_clear(struct png_decoder * decoder);

/**
 * hal_png_decoder_irq_err  - err exception
 */
int hal_png_decoder_irq_err(struct png_decoder * decoder);

/**
 * hal_png_decoder_irqmask_clear
 */
int hal_png_decoder_irqmask_clear(struct png_decoder * decoder);

/**
 * hal_png_decoder_dma2d_start
 * @decoder     - decoder object
 */
 void hal_png_decoder_dma2d_start(struct png_decoder * decoder);

/**
 * hal_png_decoder_dma2d_stop
 * @decoder     - decoder object
 */
 void hal_png_decoder_dma2d_stop(struct png_decoder * decoder);

#ifdef __cplusplus
}
#endif

#endif


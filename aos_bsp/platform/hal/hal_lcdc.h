
/***************************************************************************
 * Copyright 2015-2021 BES.
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

#ifndef __HAL_LCDC_H
#define __HAL_LCDC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

enum hal_lcdc_result_e {
    LCDC_OK      = 0,
    LCDC_FAILED  = -1,
    LCDC_INVAL   = -2,
    LCDC_ERROR   = -3,
    LCDC_TIMEOUT = -4,
};

enum hal_lcdc_layer_e {
    LCDC_LAYER_GRA = 0,
    LCDC_LAYER_DMA,
    LCDC_LAYER_TVG,
    LCDC_LAYER_TVD,
    LCDC_LAYER_DEC,
    LCDC_LAYER_WB,
    LCDC_LAYER_SPU,

    /* compatible with old version codes. */
    LCDC_LAYER_LFORE = LCDC_LAYER_GRA,
    LCDC_LAYER_LBACK = LCDC_LAYER_DMA,

    LCDC_LAYER_NONE = -1,
};

enum hal_lcdc_color_format_e {
    LCDC_CFMT_PALETE_4      = 5,
    LCDC_CFMT_PALETE_8      = 6,
    LCDC_CFMT_RGB565        = 11,
    LCDC_CFMT_RGB888        = 12,
    LCDC_CFMT_RGBA8888      = 13,
    LCDC_CFMT_ARGB8888      = 21,
    LCDC_CFMT_YUV422_YUYV   = 45,
    LCDC_CFMT_YUV422_PLANAR = 60,
    LCDC_CFMT_YUV420_PLANAR = 51,
    LCDC_CFMT_BGR565        = 63,
    LCDC_CFMT_BGR888        = 64,
    LCDC_CFMT_BGRA8888      = 65,
    LCDC_CFMT_ABGR8888      = 66,
};

enum hal_lcdc_colorkey_mode_e {
    LCDC_CKEY_NONE      = 0x0,
    LCDC_CKEY_Y         = 0x1,
    LCDC_CKEY_U         = 0x2,
    LCDC_CKEY_V         = 0x4,
    LCDC_CKEY_YUV_ALL   = 0x5,
    LCDC_CKEY_G         = 0x6,
    LCDC_CKEY_B         = 0x7,
    LCDC_CKEY_RGB_ALL   = 0x3,
};

enum hal_lcdc_dsi_cfmt_e {
    DSI_RGB888 = 28,
    DSI_RGB666,
    DSI_RGB565,
    DSI_RGB101010,
};

enum hal_lcdc_dsi_dswap_e {
    DSI_BD_BGR = 0,
    DSI_BD_BRG = 1,
    DSI_BD_GBR = 2,
    DSI_BD_RBG = 3,
    DSI_BD_GRB = 4,
    DSI_BD_RGB = 5,
};

enum hal_lcdc_dec_tile_mode_e {
    LCDC_DEC_LINEAR = 0,
    LCDC_DEC_TILED,
};

enum hal_lcdc_dec_compress_mode_e {
    LCDC_DEC_DISABLE = 0,
    LCDC_DEC_NON_SAMPLE,
    LCDC_DEC_H_SAMPLE,
    LCDC_DEC_HV_SAMPLE,
};

enum hal_lcdc_dec_format_e {
    LCDC_DEC_ARGB = 0,
    LCDC_DEC_XRGB,
};

enum hal_lcdc_alpha_mode_e {
    LCDC_CONST_ALPHA = 0,
    LCDC_PIXEL_ALPHA = 1,
};

enum hal_lcdc_blend_mode_e {
    LCDC_BLEND_MODE0 = 0,  /* (1-a) * dma-pix + a * gra-pix */
    LCDC_BLEND_MODE1 = 1,  /* (1-a) * dma-pix + 1 * gra-pix */
    LCDC_BLEND_MODE2 = 2,  /* a * dma-pix + 1 * gra-pix */
};


/**
 * hal_lcdc_irqn - get lcdc irq  number
 */
int hal_lcdc_irqn(void);

/**
 * hal_lcdc_write_clut - write clut data into lcdc memory
 * @citem  : clut item
 * @n      : clut item address index
 */
int hal_lcdc_write_clut(uint32_t citem, int n);

/**
 * hal_lcdc_write_gamma - write gamma data into lcdc memory
 * @gitem   : gamma item
 * @n      :  item address index
 */
int hal_lcdc_write_gamma(uint32_t citem, int n);

/**
 * hal_lcdc_lcadjcurves_enable - enable/disalbe  gamma curves function
 */
int hal_lcdc_lcadjcurves_enable(int lid, bool enable);

/**
 * hal_lcdc_axifastmode_enable - enable/disalbe  axi bus fast mode function
 */
int hal_lcdc_axifastmode_enable(bool enable);

/**
 * hal_lcdc_lstartaddr - set lcdc layer dma memory start address
 * @lid    : Layer id
 * @addr   : memory address
 */
int hal_lcdc_lstartaddr(int lid, uint32_t addr);

/**
 * hal_lcdc_lstartaddr1 - set lcdc layer dma memory start address1
 * @lid    : Layer id
 * @addr   : memory address
 */
int hal_lcdc_lstartaddr1(int lid, uint32_t addr);

/**
 * hal_lcdc_getcuraddr - get lcdc layer dma memory start address
 * @lid    : Layer id
 */
uint32_t hal_lcdc_getcuraddr(int lid);

/**
 * hal_lcdc_getcuraddr1 - get lcdc layer dma memory start address1
 * @lid    : Layer id
 */
uint32_t hal_lcdc_getcuraddr1(int lid);

/**
 * hal_lcdc_lpitch - set lcdc vsync start/end pixes count
 * @lid    : Layer id
 * @pitch  : pitch in bytes
 */
int hal_lcdc_lpitch(int lid, uint16_t pitch);

/**
 * hal_lcdc_lvsepxcount - set lcdc vsync start/end pix count
 * @start   : stby length
 * @end     : start line
 */
int hal_lcdc_lvsepxcount(int start, int end);

/**
 * hal_lcdc_stby_vsync_calc - calculate the minimal value of vsync standby time.
 */
int hal_lcdc_stby_vsync_calc(int width, int pixbit, int pixmhz, int dsimhz, int dsilanes);

/**
 * hal_lcdc_stby_vsync_set - set stby vsync
 * @stby_len    : stby length
 * @start_line  : start line
 * @end_line    : end line
 */
int hal_lcdc_stby_vsync_set(int stby_len, int start_line, int end_line);

/**
 * hal_lcdc_porch_vsync_set - set vsync porch in pixes
 * @hf      : hfront porch
 * @hb      : hback proch
 * @vlines  : vlines
 */
int hal_lcdc_porch_vsync_set(int hfp, int hbp, int vlines);

/**
 * hal_lcdc_pix_vsync_set - set vsync position in pixes
 * @start_pix  : start pixels
 * @end_pix    : end pixels
 */
int hal_lcdc_pix_vsync_set(int start_pix, int end_pix);

/**
 * hal_lcdc_sponscreen_set - set  F/B Layer screen start position in pixes
 * @lid    : Layer id
 * @vln    : y in line
 * @hpxl   : x in pixels
 */
int hal_lcdc_sponscreen_set(int lid, uint16_t vln, uint16_t hpxl);

/**
 * hal_lcdc_bgl_vhtotal_set - set  F/B Layer PN_V/H_TOTAL in pix
 * @htotal    : height total
 * @vtotal    : width total
 */
int hal_lcdc_bgl_vhtotal_set(int htotal, int vtotal);

/**
 * hal_lcdc_bgl_vtotal_set - set  F/B Layer PN_V_TOTAL in pixes
 * @vtotal  : width total
 */
int hal_lcdc_bgl_vtotal_set(int vtotal);

/**
 * hal_lcdc_bgl_htotal_set - set  F/B Layer PN_H_TOTAL in pixes
 * @htotal    : height total
 */
int hal_lcdc_bgl_htotal_set(int htotal);

/**
 * hal_lcdc_bgl_hporch_set - set  F/B Layer HPORCH in pixes
 * @hfp     : hfront porch
 * @hbp     : hback porch
 */
int hal_lcdc_bgl_hporch_set(int hfp, int hbp);

/**
 * hal_lcdc_bgl_vporch_set - set  F/B Layer VPORCH in pixes
 *
 * Input Parameters:
 * @vfp  : vfront porch
 * @vbp  : vback porch
 */
int hal_lcdc_bgl_vporch_set(int vfp, int vbp);

/**
 * hal_lcdc_lsize - set  Layer image size in pixes
 * @lid   : LCDC_LAYER_SPU only
 * @w     : Layer width
 * @h     : Layer height
 */
int hal_lcdc_lsize(int lid, uint16_t w, uint16_t h);

/**
 * hal_lcdc_lzoom_set - set  F/B Layer zoom in pixes
 * @lid  : Layer id
 * @w    : window active area width
 * @h    : window active area height
 */
int hal_lcdc_lzoom_set(int lid, uint16_t w, uint16_t h);

/**
 * hal_lcdcl_enable - enable/disable lcdc Layer
 * @lid     : Layer index
 * @enable  : enable/disable
 */
int hal_lcdcl_enable(int lid, bool enable);

/**
 * hal_lcdc_lcolor - set  background Layer color
 * @lid    : LCDC_LAYER_SPU only
 * @rgb    : color in RGB888
 */
int hal_lcdc_lcolor(int lid, uint32_t rgb);

/**
 * hal_lcdc_dither_s4x8 - enable/disable lcdc  DITHER_4X8_PN
 * @enable   : enable/disable
 */
int hal_lcdc_dither_s4x8(bool enable);

/**
 * hal_lcdc_dither - lcdc  dither configuration
 * @enable   : enable/disable
 * @level    : 0: low  1: high
 * @red      : color width(4,5,6,5)
 * @green    : color width(4,6,6,5)
 * @blue     : color width(4,5,6,5)
 */
int hal_lcdc_dither(bool enable, uint8_t level, uint8_t red,
             uint8_t green, uint8_t blue);

/**
 * hal_lcdc_dither_read - read  lcdc  dither table memory
 * @index   : table memory index
 *
 * Returns: mem value of dither table
 */
uint32_t hal_lcdc_dither_read(uint8_t index);

/**
 * hal_lcdc_dither_write - write  lcdc  dither table memory
 * @index   : table memory index
 * @value   : write value to dither table
 */
int hal_lcdc_dither_write(uint8_t index, uint32_t value);

/**
 * hal_lcdc_lpfc_fmt - Configure layer color format
 * @lid  : Layer id
 * @fmt  : LCDC_CFMT_XXX
 */
int hal_lcdc_lpfc_fmt(int lid, uint8_t fmt);

/**
 * hal_lcdc_lchromakeyenable - Enable colorkey
 */
int hal_lcdc_lchromakeyenable(int lid, bool enable);

/**
 * hal_lcdc_lpfc_ltransp - Configure layer blend function
 * @lid     : Layer id
 * @mode    : Pixel alpha mode, LCDC_CONST_ALPHA or LCDC_PIXEL_ALPHA
 * @alpha   : Layer alpha
 */
int hal_lcdc_lpfc_ltransp(int lid, int mode, uint8_t alpha);

/**
 * hal_lcdc_lchromakey - foreground and background color key Configure
 * @lid   : Layer id
 * @rgb   : chroma key
 * @rrgb  : color to replace chroma key
 * @ra    :  new alpha for replaced pixes
 *
 * Set foreground and background color key Configure, real color will be
 * replaced , if it's value is equal to the chroma key
 */
int hal_lcdc_lchromakey(int lid, uint32_t rgb, uint32_t rrgb, uint8_t ra);

/**
 * hal_lcdc_lchromakey_newalpha - foreground and background color key Configure
 * @lid    : Layer id
 * @alpha  : new alpha for replaced pixes
 *
 * set foreground and background color key Configure, real color alpha will be
 * replaced , if it's color value is equal to the chroma key
 */
int hal_lcdc_lchromakey_newalpha(int lid, uint8_t newalpha);

/**
 * hal_lcdc_panelpath_dmatrans_trigger_enable - enable/disable lcdc panel dma trigger
 * @lid    : Layer id
 * @ena    : enable/disable
 */
int hal_lcdc_panelpath_dmatrans_trigger_enable(int lid, bool enable);

/**
 * hal_lcdc_disp_avsync_trigger_disable - enable/disable lcdc disp avsync trigger
 * @dis     : enable/disable
 */
int hal_lcdc_disp_avsync_trigger_disable(bool disable);

/**
 * hal_lcdc_disp_svsync_trigger_disable - enable/disable lcdc disp svsync trigger
 * @dis   : enable/disable
 */
int hal_lcdc_disp_svsync_trigger_disable(bool disable);

/**
 * hal_lcdc_disp_mem_enable - enable/disable lcdc disp memory
 * @dis  : enable/disable
 */
int hal_lcdc_disp_mem_enable(bool enable);

/**
 * hal_lcdc_disp_extra_delay - set display extra delays
 */
int hal_lcdc_disp_extra_delay(uint16_t delay);

/**
 * hal_lcdc_ol_dmaburst_length_set - set lcdc wb path dma burst length
 * @len     :  burst length
 */
int hal_lcdc_ol_dmaburst_length_set(uint8_t len);

/**
 * hal_lcdc_smpn_mode_set - set lcdc smpn mode
 * @mode : smpn mode (0x0 = 3 cycles per pixel;
 *                    0x1 = RGB666, 3 cycles per pixel
 *                    0x2 = RGB565, 2 cycles per pixel;
 *                    0x3 = 1 cycle per pixel;
 *                    0x4 = RGB666, 1 cycle per pixel;
 *                    0x5 = RGB565 1 cycle per pixel )
 */
int hal_lcdc_smpn_mode_set(int mode);

/**
 * hal_lcdc_irq_disable - mask all lcdc_irqs
 */
int hal_lcdc_irq_disable(void);

/**
 * hal_lcdc_get_irqstat - get irq state.
 */
uint32_t hal_lcdc_get_irqstat(void);

/**
 * hal_lcd_framedone_irq_enable - enable/disable framedone irq.
 * @enable    : true or false
 */
int hal_lcdc_framedone_irq_enable(bool enable);

/**
 * hal_lcd_vsync_irq_enable - enable/disable vsync irq.
 * @enable    : true or false
 */
int hal_lcdc_vsync_irq_enable(bool enable);

/**
 * hal_lcdc_framedone_irqstat_clr  - clear framedone irq status.
 */
int hal_lcdc_framedone_irqstat_clr(void);

/**
 * hal_lcdc_vsync_irqstat_clr - clear vsync irq status.
 */
int hal_lcdc_vsync_irqstat_clr(void);

/**
 * hal_lcdc_irq_is_framedone - check get framedone irq status or not.
 * @regval   : lcdc ISR value
 */
bool hal_lcdc_irq_is_framedone(uint32_t regval);

/**
 * hal_lcdc_irq_is_hwc_framedone - check get hwc framedone irq status or not.
 * @regval   : lcdc ISR value
 */
bool hal_lcdc_irq_is_hwc_framedone(uint32_t regval);

/**
 * hal_lcdc_irq_is_framedone_irq0 - check layer framedone irq0.
 * @regval   : lcdc ISR value
 */
bool hal_lcdc_irq_is_framedone_irq0(uint32_t regval);

/**
 * hal_lcdc_irq_is_framedone_irq1 - check layer framedone irq1.
 * @regval   : lcdc ISR value
 */
bool hal_lcdc_irq_is_framedone_irq1(uint32_t regval);

/**
 * hal_lcdc_irq_is_framedone - check get vsync irq status or not.
 * @regval   : lcdc ISR value
 */
bool hal_lcdc_irq_is_vsync(uint32_t regval);

/**
 * hal_lcdc_irq_is_error - check error irq status.
 * @regval  : lcdc ISR value
 */
bool hal_lcdc_irq_is_error(uint32_t regval);

/**
 * hal_lcdc_error_irq_enable - enable all lcdc_error irqs
 */
int hal_lcdc_error_irq_enable(void);

/**
 * hal_lcdc_error_irqstat_clr - enable  all lcdc_error irqs
 */
int hal_lcdc_error_irqstat_clr(void);

/**
 * hal_cmu_disp_pn_reset   - reset display pannel
 */
int hal_lcdc_disp_pn_reset(void);

/**
 * hal_lcdc_reset - reset lcd configuration
 */
int hal_lcdc_reset(void);

/**
 * hal_lcdc_smpn_enable - lcdc smpn module enable.
 */
int hal_lcdc_smpn_enable(void);

/**
 * hal_lcdc_smpn_vsync_delaymode_set - set lcdc smpn module vsync delay.
 */
int hal_lcdc_smpn_vsync_delaymode_set(uint8_t delay);

/**
 * hal_lcdc_smpn_vsync_delaymode_get - get lcdc smpn module vsync delay.
 */
uint32_t hal_lcdc_smpn_vsync_delaymode_get(void);

/**
 * hal_lcdc_smpn_enable - lcdc smpn module enable/disable.
 */
int hal_lcdc_gen_frame_enable(bool enable);

/**
 * hal_lcdc_set_sync_line_pixes - set lcdc sync pixs.
 */
int hal_lcdc_set_sync_line_pixs(uint32_t pixs);

/**
 * hal_lcdc_set_sync_lines  - set lcdc sync lines.
 */
int hal_lcdc_set_sync_lines(uint32_t lines);

/**
 * hal_lcdc_get_sync_delay_pixs - set lcdc delay pixes.
 */
uint32_t hal_lcdc_get_sync_delay_pixs(void);

/**
 * hal_lcdc_get_sync_delay_lines - get lcdc delay lines.
 */
uint32_t hal_lcdc_get_sync_delay_lines(void);

/**
 * hal_lcdc_start_wb  - Starts the wblayer dma2d transfer process
 */
int hal_lcdc_start_wb(void);

/**
 * hal_lcdc_gen_start  - Starts the  dma2d transfer process by write register
 */
int hal_lcdc_gen_start(void);

/**
 * hal_lcdc_start_dsi - Starts the lcdc transfer process.
 */
int hal_lcdc_start_dsi(void);

/**
 * hal_lcdc_te_trigger - Enable TE trigger.
 */
int hal_lcdc_te_trigger(void);

/**
 * hal_lcdc_vsync_porch_set  - set vsync porch with default value
 */
int hal_lcdc_vsync_porch_set(void);

/**
 * hal_lcdc_wdma_1fpt_set - set write back dma in one frame per trigger mode
 */
int hal_lcdc_wdma_1fpt_set(void);

/**
 * hal_lcdc_wdma_toggle_frame - toggle signal to start a write back dma frame
 */
int hal_lcdc_wdma_toggle_frame(void);

/**
 * hal_lcdc_select_top - select f/b dma path  as top layer to blender
 */
int hal_lcdc_select_top(uint8_t lid);

/**
 * hal_lcdc_blend - set blend and alpha mode for pn layers.
 */
int hal_lcdc_blend(int blend_mode, int alpha_mode, uint8_t alpha);

/**
 * hal_lcdc_swap_path - select f/b dma path
 */
int hal_lcdc_swap_path(uint8_t lid);

/**
 * hal_lcdc_swap_path_enable  - enable selected f/b dma path
 */
int hal_lcdc_swap_path_enable(bool enable);

/**
 * hal_lcdc_swap_burst_length  - send burst length
 */
int hal_lcdc_swap_burst_length(uint8_t mode);

/**
 * hal_lcdc_set_vmirro_enable - enable/disable selected layer vertical mirro function
 * @lid : 0 - lcdc layer1, 1 - lcdc layer2
 */
int hal_lcdc_set_vmirro_enable(uint8_t lid, bool enable);

/**
 * hal_lcdc_disp_color_fmt - set color ouput format for display interface
 * @fmt  : ctrl bit:
 *         DSI_RGB888                (28)
 *         DSI_RGB666                (29)
 *         DSI_RGB565                (30)
 *         DSI_RGB101010             (31)
 */
int hal_lcdc_disp_color_fmt(enum hal_lcdc_dsi_cfmt_e fmt);

/**
 * hal_lcdc_disp_dsi_data_swap - set dsi output data byte-order
 * @order  : ctrl fmt
 *            DSI_BD_BGR : 0
 *            DSI_BD_BRG : 1
 *            DSI_BD_GBR : 2
 *            DSI_BD_RBG : 3
 *            DSI_BD_GRB : 4
 *            DSI_BD_RGB : 5
 */
int hal_lcdc_disp_dsi_data_swap(enum hal_lcdc_dsi_dswap_e order);

/**
 * hal_lcdc_sl_dmaburst_length_set - set lcdc source path dma read burst length
 */
int hal_lcdc_sl_dmaburst_length_set(uint8_t len);

/**
 * hal_lcdc_sleep - lcdc enter sleep mode.
 */
int hal_lcdc_sleep(void);

/**
 * hal_lcdc_wakeup - lcdc wakeup from sleep mode.
 */
int hal_lcdc_wakeup(void);

/**
 * hal_lcdc_video_mode_enable - enable lcdc setting for DSI video mode.
 * call it in front of other lcdc init functions.
 */
int hal_lcdc_video_mode_enable(bool enable);

/**
 * hal_lcdcl_dumb_enable - enable video frame transfer.
 */
int hal_lcdcl_dumb_enable(bool enable);

/**
 * hal_lcdc_dumb_vsync_mode - enable VSYNC sigal on dumb panel.
 */
int hal_lcdc_dumb_vsync_mode(uint8_t mode);

/**
 * hal_lcdc_source_switch - switch the dumb panel as source
 */
int hal_lcdc_source_switch(bool enable);

/**
 * hal_lcdc_dec_set_color_format - set decnano color format
 */
int hal_lcdc_dec_set_color_format(enum hal_lcdc_dec_format_e cfmt);

/**
 * hal_lcdc_dec_set_tile_mode - set decnano tile mode
 */
int hal_lcdc_dec_set_tile_mode(enum hal_lcdc_dec_tile_mode_e mode);

/**
 * hal_lcdc_dec_set_compress_mode - set decnano tile mode
 */
int hal_lcdc_dec_set_compress_mode(enum hal_lcdc_dec_compress_mode_e comp);

/**
 * hal_lcdc_yuv2rgb_enable - enable YUV to RGB convert
 */
int hal_lcdc_yuv2rgb_enable(int lid, bool enable, bool swap_yu, bool swap_uv);

/**
 * hal_lcdc_dec_swap_rb - swap decnano red & blue
 */
int hal_lcdc_dec_swap_rb(bool enable);

/**
 * hal_lcdc_test_mode - enable test mode for gra layer.
 */
int hal_lcdc_test_mode(bool enable);

/**
 * hal_lcdc_dsi0_layer_select - select layers for DSI0
 */
int hal_lcdc_dsi0_layer_select(int layer_fg, int layer_bg);

/**
 * hal_lcdc_dsi0_blend - configure dsi0 blend and alpha mode.
 */
int hal_lcdc_dsi0_blend(int blend_mode, int alpha_mode, uint8_t alpha);

/**
 * hal_lcdc_dsi1_layer_select - select layers for DSI1
 */
int hal_lcdc_dsi1_layer_select(int layer_fg, int layer_bg);

/**
 * hal_lcdc_dsi1_blend - configure dsi1 blend and alpha mode.
 */
int hal_lcdc_dsi1_blend(int blend_mode, int alpha_mode, uint8_t alpha);

/**
 * hal_lcdc_rgbif_enable - enable RGB interface
 */
int hal_lcdc_rgbif_enable(bool enable, bool clk_inv, bool hsync_inv, bool vsync_inv);

/**
 * hal_lcdc_dump_regs - dump lcdc all registers.
 */
int hal_lcdc_dump_regs(void);

/**
 * hal_lcdc_ftoggle_enable - enable/disable lcdc ftoggle
 * @lid     : Layer index
 * @enable  : enable/disable
 */
int hal_lcdc_ftoggle_enable(int lid, bool enable);

/**
 * hal_lcdc_rotate_enable - enable/disable lcdc gra Layer rotate
 * @enable  : enable/disable
 * @w    : rotate  width
 * @h    : rotate  height
 */
int hal_lcdc_rotate_enable(bool enable, uint16_t w, uint16_t h);

#ifdef __cplusplus
}
#endif
#endif /* __HAL_LCDC_H */

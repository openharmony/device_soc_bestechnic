/***************************************************************************
 *
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
#ifndef __HAL_DSI_H__
#define __HAL_DSI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

#include "hal_lcdc.h"

typedef void (*HAL_DSI_TE_GPIO_IRQ_HANDLER)(void);
typedef void (*HAL_DSI_ULPS_CB_HANDLER)(uint32_t param);

enum HAL_DSI_ID_E {
    HAL_DSI_ID_0 = 0,
#ifdef DSI1_BASE
    HAL_DSI_ID_1 = 1,
#endif

    HAL_DSI_ID_QTY,
};

enum HAL_DSI_RESULT_E {
    DSI_OK = 0,
    DSI_FAILED = -1,
    DSI_TIMEOUT = -2,
    DSI_ACK_ERROR = -3,
    DSI_DATA_ERROR = -4,
};

enum HAL_DSI_MODE_E {
    DSI_COMMAND_MODE = 0,
    DSI_VIDEO_PULSE_MODE = 1,  /* Non burst sync pulse mode */
    DSI_VIDEO_EVENT_MODE = 2,  /* Non burst sync event mode */
    DSI_VIDEO_BURST_MODE = 3,  /* burst mode */

    DSI_VIDEO_MODE = DSI_VIDEO_PULSE_MODE,
};

enum HAL_DSI_LANE_E {
    DSI_LANE_1 = 1,
    DSI_LANE_2,
    DSI_LANE_3,
    DSI_LANE_4,
};

struct HAL_DSI_CFG_T {
    uint32_t active_width;
    uint32_t active_height;//screen size
    uint32_t h_front_porch;
    uint32_t h_back_porch;
    uint32_t v_front_porch;
    uint32_t v_back_porch;
    uint32_t total_width;
    uint32_t total_height;

    // pn video
    uint32_t y_mem_pitch;
    uint32_t uv_mem_pitch;
    uint32_t c_mem_pitch;
    uint32_t image_h_sa;
    uint32_t image_v_sa;
    uint32_t image_width;
    uint32_t image_height;
    uint32_t zm_image_width;
    uint32_t zm_image_height;

    // pn graphic
    uint32_t g_mem_pitch;
    uint32_t graphic_h_sa;
    uint32_t graphic_v_sa;
    uint32_t graphic_width;
    uint32_t graphic_height;
    uint32_t zm_graphic_width;
    uint32_t zm_graphic_height;

    // tv graphic
    uint32_t g_tv_mem_pitch;
    uint32_t tvg_h_sa;
    uint32_t tvg_v_sa;
    uint32_t tvg_width;
    uint32_t tvg_height;
    uint32_t zm_tvg_width;
    uint32_t zm_tvg_height;

    // pn cursor
    uint32_t cursor_h_sa;
    uint32_t cursor_v_sa;
    uint32_t cursor_width;
    uint32_t cursor_height;
    int hwc_color1;
    int hwc_color2;

    //background
    int blankcolor;

    // pn viedo setting
    int cos0;
    int sin0;
    int c_mult_s;
    int saturation;
    int brightness;
    int contrast;

    // pn keys
    uint8_t ra;
    uint32_t rgb;
    uint32_t rrgb;
};

/**
 * @brief hal_dsi_init - init dsi controller & phy
 *
 * @param width : panel horizontal resolution
 * @param mode : command mode or video mode
 * @param lane : 1/2 lane mode
 * @param color_fmt: RGB888/RGB565/RGB666...
 */
int hal_dsi_init(enum HAL_DSI_ID_E id, uint16_t width, enum HAL_DSI_MODE_E mode,
                  enum HAL_DSI_LANE_E lane, enum hal_lcdc_dsi_cfmt_e color_fmt);

/**
 * @brief hal_dsi_exit - disable dsi controller & phy
 */
void hal_dsi_exit(enum HAL_DSI_ID_E id);

/**
 * @brief hal_dsi_reset - re-init dsi controller & phy
 *
 * @param width : panel horizontal resolution
 */
void hal_dsi_reset(enum HAL_DSI_ID_E id, uint16_t width);

/**
 * @brief hal_dsi_send_cmd - send cmd by lp mode
 *
 * @param cmd
 */
int hal_dsi_send_cmd(enum HAL_DSI_ID_E id, uint8_t cmd);

/**
 * @brief hal_dsi_read_cmd - read data by lp mode
 *
 * @param cmd : cmd
 * @param data : read buffer
 * @param len : read len
 * @return int
 */
int hal_dsi_read_cmd(enum HAL_DSI_ID_E id, uint8_t cmd, uint8_t *data, int len);

/**
 * @brief hal_dsi_send_cmd_data - send cmd with data by lp mode
 *
 * @param cmd : cmd
 * @param len : send param num
 * @param p0 : param0
 * @param p1 : param1
 * @param p2 : param2
 * @param p3 : param3
 */
int hal_dsi_send_cmd_data(enum HAL_DSI_ID_E id, uint8_t cmd, uint32_t len, uint8_t p0,
                           uint8_t p1, uint8_t p2, uint8_t p3);
/**
 * @brief hal_dsi_send_long_array - send cmd with data with array
 *
 * @param len : array count
 * @param data : array data
 */
int hal_dsi_send_long_array(enum HAL_DSI_ID_E id, uint8_t len, uint8_t *data);

/**
 * @brief hal_dsi_send_cmd_list - send cmd with data list
 *
 * @param cmd : command
 * @param param_count : count of parameters
 * @param params : data of parameters
 */
int hal_dsi_send_cmd_list(enum HAL_DSI_ID_E id, uint8_t cmd, uint8_t param_count,
                           uint8_t *params);

/**
 * hal_dsi_irqn - get dsi irq number
 */
int hal_dsi_irqn(enum HAL_DSI_ID_E id);

/**
 * hal_dsi_get_irqstate - get dsi irq state
 */
uint32_t hal_dsi_get_irqstate(enum HAL_DSI_ID_E id);

uint32_t hal_dsi_get_unov_flags(enum HAL_DSI_ID_E id);

/**
 * hal_dsi_teirq_enable - enable dsi te irq
 */
void hal_dsi_teirq_enable(enum HAL_DSI_ID_E id);

/**
 * hal_dsi_teirq_disable - disable dsi te irq
 */
void hal_dsi_teirq_disable(enum HAL_DSI_ID_E id);

/**
 * hal_dsi_irq_disable - disable dsi all irqs
 */
void hal_dsi_irq_disable(enum HAL_DSI_ID_E id);

/**
 * hal_dsi_clear_teirq_flag - clear dsi te irq state
 */
void hal_dsi_clear_teirq_flag(enum HAL_DSI_ID_E id);

/**
 * hal_dsi_irq_is_te - irq is te
 * @regval : dsi ISR value
 */
bool hal_dsi_irq_is_te(uint32_t regval);

/**
 * hal_dsi_irq_is_err - dsi is irq err
 */
bool hal_dsi_irq_is_err(uint32_t regval);

/**
 * hal_dsi_irq_clear_err - clear dsi error flag.
 */
void hal_dsi_irq_clear_err(enum HAL_DSI_ID_E id, uint32_t status);

/**
 * hal_dsi_enter_ulps_mode - enter ulps mode
 */
void hal_dsi_enter_ulps_mode(enum HAL_DSI_ID_E id);

/**
 * hal_dsi_exit_ulps_mode - exit ulps mode
 */
void hal_dsi_exit_ulps_mode(enum HAL_DSI_ID_E id);

void hal_dsi_exit_ulps_with_cb(enum HAL_DSI_ID_E id, HAL_DSI_ULPS_CB_HANDLER hdlr,void *param);

void hal_dsi_start(enum HAL_DSI_ID_E id);

void hal_dsi_sleep(enum HAL_DSI_ID_E id);

void hal_dsi_wakeup(enum HAL_DSI_ID_E id);

void hal_dsi_te_gpio_irq_register(enum HAL_DSI_ID_E id, HAL_DSI_TE_GPIO_IRQ_HANDLER hdlr);

void hal_dsi_te_gpio_irq_enable(enum HAL_DSI_ID_E id);

void hal_dsi_te_gpio_irq_disable(enum HAL_DSI_ID_E id);

void hal_dsi_start_hs_clock(enum HAL_DSI_ID_E id);

void hal_dsi_stop_hs_clock(enum HAL_DSI_ID_E id);

#ifdef __cplusplus
}
#endif

#endif


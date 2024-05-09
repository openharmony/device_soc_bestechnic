
/***************************************************************************
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

#ifndef __HAL_LCDC_QSPI_H
#define __HAL_LCDC_QSPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

typedef void (*HAL_LCDC_QSPI_TE_GPIO_IRQ_HANDLER)(void);

/**
 * enum HAL_LCDC_QSPI_LINE_MODE_E
 */
enum HAL_LCDC_QSPI_LINE_MODE_E {
    LCDC_QSPI_1LINE = 1, /* 1 data line  */
    LCDC_QSPI_2LINE = 2, /* 2 data lines */
    LCDC_QSPI_4LINE = 4, /* 4 data lines */
};

/**
 * enum HAL_LCDC_QSPI_CFMT_E - lcdc qspi out pixel color format
 */

enum HAL_LCDC_QSPI_CFMT_E {
    LCDC_QSPI_RGB888,
    LCDC_QSPI_RGB666,
    LCDC_QSPI_RGB565,
};

/**
 * qspi hal api
 */

/**
 * @brief hal_lcdc_qspi_init - initialize qspi sub-module, after lcdc & qspi color enable.
 *
 * @param color_fmt: LCDC_QSPI_RGB888/LCDC_QSPI_RGB666/LCDC_QSPI_RGB565
 * @param line_mode: LCDC_QSPI_4LINE/LCDC_QSPI_1LINE/LCDC_QSPI_2LINE
 */
void hal_lcdc_qspi_initialize(enum HAL_LCDC_QSPI_CFMT_E color_fmt, enum HAL_LCDC_QSPI_LINE_MODE_E line_mode);

/**
 * @brief hal_lcdc_qspi_exit - exit qspi sub-module, diable qspi clocks
 */
void hal_lcdc_qspi_exit(void);

/**
 * @brief hal_lcdc_qspi_sleep - qspi enter low power mode
 */
void hal_lcdc_qspi_sleep(void);

/**
 * @brief hal_lcdc_qspi_wakeup - qspi exit low power mode
 */
void hal_lcdc_qspi_wakeup(void);

/**
 * @brief hal_lcdc_qspi_swaprb_enable - enable red & blue order swap
 */
void hal_lcdc_qspi_swaprb_enable(bool enable);

/**
 * @brief hal_lcdc_qspi_set_command_freq - set command transfer frequncy in Mhz
 */
void hal_lcdc_qspi_set_command_freq(uint32_t freqmhz);

/**
 * @brief hal_lcdc_qspi_send_command - send one command
 *
 * @param cmd
 */
int hal_lcdc_qspi_send_command(uint8_t cmd);

/**
 * @brief hal_lcdc_qspi_read_command - read data from qspi
 *
 * @param cmd : cmd
 * @param read_buf : read buffer
 * @param len : read len
 * @return int
 */
int hal_lcdc_qspi_read_command(uint8_t cmd, uint8_t *read_buf, uint8_t len);

/**
 * @brief hal_lcdc_qspi_send_command_data - send command with 4 parameters
 *
 * @param cmd : cmd
 * @param len : send param num
 * @param p0 : param0
 * @param p1 : param1
 * @param p2 : param2
 * @param p3 : param3
 */
int hal_lcdc_qspi_send_command_data(uint8_t cmd, uint32_t len, uint8_t p0,
                           uint8_t p1, uint8_t p2, uint8_t p3);

/**
 * @brief hal_lcdc_qspi_send_command_list - send command with parameter list
 *
 * @param cmd
 * @param param_count
 * @param param_list
 */
int hal_lcdc_qspi_send_command_list(uint8_t cmd, uint8_t param_count,
                           uint8_t *param_list);


/**
 * @brief hal_lcdc_qspi_te_gpio_irq_register - register TE irq handler
 */
void hal_lcdc_qspi_te_gpio_irq_register(HAL_LCDC_QSPI_TE_GPIO_IRQ_HANDLER hdlr);

/**
 * @brief hal_lcdc_qspi_te_gpio_irq_enable - enable TE irq
 */
void hal_lcdc_qspi_te_gpio_irq_enable(void);


/**
 * @brief hal_lcdc_qspi_te_gpio_irq_disable - disable TE irq
 */
void hal_lcdc_qspi_te_gpio_irq_disable(void);


/**
 * @brief hal_lcdc_qspi_gen_start - start qspi frame transfer
 */
int hal_lcdc_qspi_gen_start(void);


/**
 * @brief hal_lcdc_qspi_cs_sel - active or deactive spi cs pin
 * Only for best1502x.
 */
void hal_lcdc_qspi_cs_sel(bool active);


#ifdef __cplusplus
}
#endif
#endif /* __HAL_QSPI_H */

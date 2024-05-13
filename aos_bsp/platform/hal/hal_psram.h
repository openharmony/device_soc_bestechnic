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
#ifndef __HAL_PSRAM_H__
#define __HAL_PSRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"
#include "hal_cmu.h"
#include "plat_addr_map.h"

enum HAL_PSRAM_ID_T {
    HAL_PSRAM_ID_0 = 0,
    HAL_PSRAM_ID_NUM,
};

enum HAL_PSRAMPHY_ID_T {
    HAL_PSRAMPHY_ID_0 = 0,
    HAL_PSRAMPHY_ID_NUM,
};

enum HAL_PSRAM_PHY_CHANNEL_T {
    HAL_PSRAM_PHY_CHANNEL_0 = (1 << 0),
    HAL_PSRAM_PHY_CHANNEL_1 = (1 << 1),
    HAL_PSRAM_PHY_CHANNEL_ALL = HAL_PSRAM_PHY_CHANNEL_0 | HAL_PSRAM_PHY_CHANNEL_1
};

enum HAL_PSRAM_MC_MGR_CHANEL_T {
    HAL_PSRAM_MC_MGR_CHANEL_0 = (1 << 0),
    HAL_PSRAM_MC_MGR_CHANEL_1 = (1 << 1),
    HAL_PSRAM_MC_MGR_CHANEL_ALL = HAL_PSRAM_MC_MGR_CHANEL_0 | HAL_PSRAM_MC_MGR_CHANEL_1
};

enum PSRAM_CS_POL_T {
    PSRAM_CS_POL_L = 0,
    PSRAM_CS_POL_H = 1,
};

enum PSRAM_DATA_BUS_T {
    PSRAM_DATA_BUS_X8 = 0,
    PSRAM_DATA_BUS_2X8,
    PSRAM_DATA_BUS_X16,
    PSRAM_DATA_BUS_2X16,
};

enum PSRAM_TYPE_T {
    PSRAM_TYPE_OPI = 0,
    PSRAM_TYPE_XOPI,
    PSRAM_TYPE_HYPERRAM,
    PSRAM_TYPE_UHS,
    PSRAM_TYPE_ULS,
    PSRAM_TYPE_QUITY,
};

enum HAL_PSRAM_CALIB_ITEM_T {
    PSRAM_CALIB_RX_DQS,
    PSRAM_CALIB_TX_DQS,
    PSRAM_CALIB_TX_CLK,
    PSRAM_CALIB_TX_CEB,
};

enum PSRAM_PREFETCH_WIDTH_T {
    PSRAM_PREFETCH_WIDTH_NULL = 0,
    PSRAM_PREFETCH_WIDTD_16N,
    PSRAM_PREFETCH_WIDTD_32N,
};

enum HAL_PSRAM_DLL_DLY_T {
    HAL_PSRAM_DLL_DLY_TX_CEB = (1 << 0),
    HAL_PSRAM_DLL_DLY_TX_CLK = (1 << 1),
    HAL_PSRAM_DLL_DLY_TX_DQS = (1 << 2),
    HAL_PSRAM_DLL_DLY_TX_DM  = (1 << 3),
    HAL_PSRAM_DLL_DLY_TX_DQ  = (1 << 4),
    HAL_PSRAM_DLL_DLY_TX_CMD = (1 << 5),
    HAL_PSRAM_DLL_DLY_RX_DQS = (1 << 6),
    HAL_PSRAM_DLL_DLY_RX_CLK = (1 << 7),
    HAL_PSRAM_DLL_DLY_RX_ADQ = (1 << 8)
};

enum HAL_PSRAM_ANAPHY_LOCK_RESULT_T {
    HAL_PSRAM_ANAPHY_LOCK_SUCCESS = 0,
    HAL_PSRAM_ANAPHY_LOCK_ALL_ONE = 1,
    HAL_PSRAM_ANAPHY_LOCK_ALL_ZERO = 2,
    HAL_PSRAM_ANAPHY_LOCK_TIME_OUT = 3,
};

enum HAL_PSRAM_DYN_FREQ_ID_T {
    HAL_PSRAM_DYN_FREQ_ID_0 = 0,
    HAL_PSRAM_DYN_FREQ_ID_1 = 1,
    HAL_PSRAM_DYN_FREQ_ID_MIAN = HAL_PSRAM_DYN_FREQ_ID_0,
    HAL_PSRAM_DYN_FREQ_ID_SEC =  HAL_PSRAM_DYN_FREQ_ID_1,
};

struct HAL_PSRAM_DQS_BOUNDARY_T {
    uint8_t l;
    uint8_t r;
    bool is_legal;
};

struct HAL_PSRAM_WINDOW_1D_ANALYSIS_T {
    uint8_t range;
    uint8_t t_div_2;
    uint8_t *raw_data;
    enum HAL_PSRAM_CALIB_ITEM_T item;
    struct HAL_PSRAM_DQS_BOUNDARY_T *boundary;
};

struct HAL_PSRAM_RECTANGLE_T{
    uint8_t x_start;
    uint8_t y_start;
    uint8_t x_end;
    uint8_t y_end;
};

struct HLA_PSRAM_DLL_DLY_VAL_T {
    uint16_t tx_ceb;
    uint16_t tx_clk;
    uint16_t tx_dqs;
    uint16_t tx_dm;
    uint16_t rx_dqs;
    uint16_t rx_clk;
    uint16_t tx_dq;
    uint16_t tx_cmd;
    uint16_t rx_adq;
};

struct PSRAM_INFO_T {
    char* manufacturers_str;
    char* type_str;
    uint32_t capacity_mbyte;
};

typedef void (*HAL_PSRAM_WINDOW_2D_CB_T)(void * const, uint8_t, uint8_t, int, const struct HAL_PSRAM_RECTANGLE_T *);
typedef void (*HAL_PSRAM_WINDOW_1D_CB_T)(struct HAL_PSRAM_WINDOW_1D_ANALYSIS_T *);

#define PSRAM_WINDOW_SIZE    (0x20)

void hal_psram_sleep(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
void hal_psram_wakeup(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
void hal_psram_init(void);
void hal_psram_phy_sleep(void);
void hal_psram_snoop_enable(void);
void hal_psram_snoop_disable(void);
void hal_psram_write_buffer_invalidate(void);
void hal_psram_calib(uint32_t clk);
void hal_psram_window_1d_cb_register(HAL_PSRAM_WINDOW_1D_CB_T func);
void hal_psram_window_2d_cb_register(HAL_PSRAM_WINDOW_2D_CB_T func);
uint32_t hal_psram_ca_calib_result(void);
uint32_t hal_psram_phy_get_dly_in_val(void);
uint32_t hal_psram_phy_get_range_val(void);
void hal_psram_change_speed(uint32_t psram_speed);
void hal_psram_dump_mr(void);
void hal_psram_dyn_freq_select(enum HAL_PSRAM_DYN_FREQ_ID_T freq_id);

#ifdef __cplusplus
}
#endif
#endif

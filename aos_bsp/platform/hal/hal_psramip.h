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
 ****************************************************************************/
#ifndef __HAL_PSRAMIP_H_
#define __HAL_PSRAMIP_H_

#if (CHIP_PSRAM_CTRL_VER == 6)
#include "psram_cfg.h"
#endif

#define T_NS_TO_NCK(clk, ns)        ((uint32_t)(((clk) / 1000000 * (ns) + (1000 - 1)) / 1000))
#define T_US_TO_NCK(clk, us)        ((uint32_t)((clk) / 1000000 * (us) + 1))
#define T_MS_TO_NCK(clk, ms)        ((uint32_t)((clk) / 1000 * (ms) + 1))
#define T_NS_TO_NCK_FLOOR(clk, ns)  ((uint32_t)(((clk) / 1000000 * (ns)) / 1000))
#define T_NCK_TO_NS(clk, n)         ((uint32_t)(((n) * 1000) / ((clk) / 1000000)))
#define T_NS_TO_NCK_CEIL  T_NS_TO_NCK

#define PSRAM_BIT_CONFIG(con, bitmask) ((con) ? (bitmask) : 0)

void hal_psram_phy_dll_reset_set_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_reset_clear_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_clk_enable_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_clk_disable_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_clk_rdy_dr_clear(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_reset_dr_clear(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_init(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
enum HAL_PSRAM_ANAPHY_LOCK_RESULT_T hal_psram_phy_dll_set_range(uint32_t range, uint32_t time_out_ms);
uint32_t hal_psram_phy_dll_range_dyn_config(uint32_t clk);
void hal_psram_anaphy_dll_range_init(uint32_t clk_freq);
void hal_psram_anaphy_set_dly(enum HAL_PSRAMPHY_ID_T phy_id, enum HAL_PSRAM_PHY_CHANNEL_T c, enum HAL_PSRAM_DLL_DLY_T t, uint16_t dly);
uint32_t hal_psram_anaphy_get_dll_dly_in_val(enum HAL_PSRAMPHY_ID_T phy_id);
uint32_t hal_psram_mc_set_mgr_ch(enum HAL_PSRAM_ID_T id, enum HAL_PSRAM_MC_MGR_CHANEL_T c);
void hal_psram_mc_init_complete(enum HAL_PSRAM_ID_T id);
bool hal_psram_mc_is_reset(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_clock_gate_disable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_clock_gate_enable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_busy_wait(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_wb_busy_wait(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_nop(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_start_clock(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_stop_clock(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_rst(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_freq(enum HAL_PSRAM_ID_T id, uint32_t clk);
void hal_psram_mc_init(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_mc_auto_lowpower_enable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_default_param(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_mc_set_timing(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg, uint32_t freq_mhz, bool config_latency);
void hal_psram_mc_write_buffer_invalidate(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_priority_config(enum HAL_PSRAM_ID_T id);
uint32_t hal_psram_device_mr_read(enum HAL_PSRAM_ID_T id, uint32_t mr);
void hal_psram_device_mr_write(enum HAL_PSRAM_ID_T id, uint32_t mr, uint32_t val);

#endif /*__HAL_PSRAMIP_H_*/
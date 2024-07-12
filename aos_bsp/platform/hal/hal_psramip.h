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

#if (CHIP_PSRAM_CTRL_VER == 10) || (CHIP_PSRAM_CTRL_VER == 11)
#define PSRAM_CLK_24MHZ             (24*1000*1000)
#define PSRAM_CLK_48MHZ             (48*1000*1000)
#define PSRAM_CLK_100MHZ            (100*1000*1000)
#define PSRAM_CLK_133MHZ            (133*1000*1000)
#define PSRAM_CLK_200MHZ            (200*1000*1000)
#define PSRAM_CLK_225MHZ            (225*1000*1000)

enum PSRAM_PHY_BYPASS_CFG_T {
    PHY_ANA_BYPASS,
    PHY_P2S_BYPASS,
    PHY_NOT_BYPASS,
};

struct PSRAM_DEVICE_CMD_TABLE_T {
    uint8_t  cmd_array_rd;
    uint8_t  cmd_array_wr;
    uint8_t  cmd_reg_rd;
    uint8_t  cmd_reg_wr;
    uint8_t  cmd_auto_refr;
    uint8_t  cmd_self_refr;
    uint8_t  cmd_hslp_entry;
    uint8_t  cmd_glbrst;
    uint8_t  cmd_nop;
};

struct PSRAM_AC_TIMING_T {
    uint32_t freq_mhz;
    uint8_t  wl;
    uint8_t  rl_min;
    uint8_t  rl_max;
    uint8_t  t_rc;
    uint8_t  t_cphr;
    uint8_t  t_cphw;
    uint8_t  t_mrr;
    uint8_t  t_mrs;
    uint16_t t_cem;
    uint16_t t_rst;
    uint32_t t_hs;
    uint32_t t_xphs;
    uint32_t t_xhs;
    uint8_t t_dqsck;
    uint8_t t_wr_dmy;
    //useless in xccela
    uint8_t  t_cke;
    uint8_t  t_cscke;
    uint8_t  t_csckeh;
    // useless for low speeed psram
    uint8_t  t_srf;  /// ?useless
    uint8_t  t_xsr;  /// ?useless
    uint16_t t_wpst;
    uint16_t t_xckd;
    uint16_t t_eckd;
    uint16_t t_zacal;
    uint16_t t_zqcrst;
    uint16_t t_refi;
    uint8_t  t_rfc;
};

struct PSRAM_DEVICE_TIMING_T {
    uint8_t  wl_def;
    uint8_t  rl_max_def;
    uint8_t  rl_min_def;
    uint8_t  bl_mrs;
    uint32_t t_xphs_min_ns;
    float    t_dqsck_max_ns;     // ? t_dqsck impact the rising eadeg if CE (delay)?
    bool     is_mrs_wl_fixed;
    uint8_t  fixed_wl_mrs;
    bool     mrs_as_pd_en;
    uint8_t  mrs_as_pd_mr;
    uint8_t  mrs_as_pd_md;

    const struct PSRAM_AC_TIMING_T *ac_timing;
    uint32_t ac_timing_count;
};

struct PSRAM_DEVICE_CFG_T {
    uint8_t  device_ca_map;     // enum PSRAM_CHIP_CA_MAP_t;
    uint8_t  device_cmd_4bit;   // enum PSRAM_CHIP_CMD_BITS_T;
    uint8_t  burst_length;
    uint8_t  page_boundary;
    bool     device_wrap_en;
    bool     device_auto_precharge;
    uint8_t  wr_dummy_cyc;
    uint8_t  cmd_dummy_cyc;
    uint8_t  nop_dummy_cyc;
    bool     stop_clk_in_nop;
    bool     stop_clk_in_tcph;

    uint8_t  device_pattern;    // enum PSRAM_CA_PATTERN_T
    bool     hyperbus_sample_lat_edge_sel;
    uint8_t  hyperbus_cmd_dly_cnt;
    bool     clk_preample_en;
    bool     device_preample_en;
    uint8_t  hyperbus_tcss;
    bool     phy_dummy_cyc_en;
    bool     phy_dummy_cyc_stop_dqs;
    bool     dqs_dm_merge_en;
    bool     mrw_wo_dm;   // ? opi xopi uls uhs look like donot care dm  when mrw
    bool     cs_pol; // enum PSRAM_CA_POL_T
};

struct PSRAM_PHY_CFG_T {
    enum PSRAM_PHY_BYPASS_CFG_T phy_bypass_cf;
    bool     phy_pd_trans_mask; /// ? can keep 0?
    bool     reg_ca_dq_lat_sw_en;
    bool     man_tx_wr_push_en;
    bool     auto_pwr_tx_wr_push_en;
    bool     idle_dq_oen;
    bool     idle_dqs_oen;
};

struct PSRAM_MC_CFG_T {
    uint8_t  prefetch_width;    // enum PREFETCH_WIDTH_T; 16n 32n
    bool     wb_bypass;
};

struct PSRAM_CFG_T {
    uint16_t run_freq_mhz;
    uint16_t cfg_freq_mhz;
    uint8_t  psram_type;
    uint8_t  psram_data_bus;
    enum PSRAM_PHY_BYPASS_CFG_T phy_bypass;
    struct PSRAM_PHY_CFG_T *phy_cfg;
    struct PSRAM_MC_CFG_T  *mc_cfg;
    const struct PSRAM_DEVICE_CMD_TABLE_T *cmd;
    const struct PSRAM_DEVICE_TIMING_T *device_timing;
    const struct PSRAM_DEVICE_CFG_T *device_cfg;
    const struct PSRAM_DEVICE_FUNC_CFG_T *device_func_cfg;
};

typedef void (*PSRAM_SET_MR_CB)(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
typedef void (*PSRAM_GET_MR_CB)(struct PSRAM_CFG_T *psram_cfg, uint8_t *reg, uint8_t *val, uint32_t clk);
typedef void (*PSRAM_SHOW_MR_CB)(enum HAL_PSRAM_ID_T id);

struct PSRAM_DEVICE_FUNC_CFG_T {
    PSRAM_SET_MR_CB psram_set_mr;
    PSRAM_GET_MR_CB psram_get_mr;
    PSRAM_SHOW_MR_CB psram_show_mr;
};
#endif

void hal_psram_phy_dll_reset_set_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_reset_clear_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_clk_enable_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_clk_disable_dr(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_clk_rdy_dr_clear(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_dll_reset_dr_clear(enum HAL_PSRAM_ID_T id);
void hal_psram_phy_init(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
#if (CHIP_PSRAM_CTRL_VER == 10 || CHIP_PSRAM_CTRL_VER == 11)
void hal_psram_phy_wakeup(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
void hal_psram_phy_sleep(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
#endif
enum HAL_PSRAM_ANAPHY_LOCK_RESULT_T hal_psram_phy_dll_set_range(uint32_t range, uint32_t time_out_ms);
uint32_t hal_psram_phy_dll_range_dyn_config(uint32_t clk);
void hal_psram_anaphy_dll_range_init(uint32_t clk_freq);
void hal_psram_anaphy_set_dly(enum HAL_PSRAMPHY_ID_T phy_id, enum HAL_PSRAM_PHY_CHANNEL_T c, enum HAL_PSRAM_DLL_DLY_T t, uint16_t dly);
void hal_psram_anaphy_set_dly_1(enum HAL_PSRAMPHY_ID_T phy_id, enum HAL_PSRAM_PHY_CHANNEL_T c, enum HAL_PSRAM_DLL_DLY_T t, uint16_t dly);
uint32_t hal_psram_anaphy_get_dll_dly_in_val(enum HAL_PSRAMPHY_ID_T phy_id);
void hal_psram_anaphy_open_auto(void);
uint32_t hal_psram_mc_set_mgr_ch(enum HAL_PSRAM_ID_T id, enum HAL_PSRAM_MC_MGR_CHANEL_T c);
void hal_psram_mc_init_complete(enum HAL_PSRAM_ID_T id);
bool hal_psram_mc_is_reset(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_clock_gate_disable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_clock_gate_enable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_busy_wait(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_wb_busy_wait(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_nop(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_pd(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_start_clock(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_stop_clock(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_cmd_rst(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_freq(enum HAL_PSRAM_ID_T id, uint32_t clk);
void hal_psram_mc_init(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_mc_auto_lowpower_enable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_auto_lowpower_disable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_snoop_disable(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_set_default_param(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_mc_set_timing(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg, uint32_t freq_mhz, bool config_latency);
void hal_psram_mc_write_buffer_invalidate(enum HAL_PSRAM_ID_T id);
void hal_psram_mc_priority_config(enum HAL_PSRAM_ID_T id);
uint32_t hal_psram_device_mr_read(enum HAL_PSRAM_ID_T id, uint32_t mr);
void hal_psram_device_mr_write(enum HAL_PSRAM_ID_T id, uint32_t mr, uint32_t val);
void hal_psram_mc_enable_dyn_freq(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_mc_sleep(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
void hal_psram_mc_wakeup(enum HAL_CMU_LPU_SLEEP_MODE_T mode);
void hal_psram_anaphy_set_dll_dly(uint32_t range, uint32_t t_div_4);
void hal_psram_anaphy_init(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_anaphy_set_rxdqs_dly(uint32_t dly);
void hal_psram_mc_wl_get(enum HAL_PSRAM_ID_T id, uint32_t *val);
void hal_psram_mc_wl_set(enum HAL_PSRAM_ID_T id, uint32_t val);
void hal_psram_mc_var_lat_en(enum HAL_PSRAM_ID_T id, bool v);
void hal_psram_phy_dll_dly_init(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_phy_init(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
bool hal_psram_mc_force_sleep(enum HAL_PSRAM_ID_T id);
uint32_t hal_psram_mc_mrs_pre_opration(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg);
void hal_psram_mc_mrs_post_opration(enum HAL_PSRAM_ID_T id, struct PSRAM_CFG_T *psram_cfg, uint32_t wl);


#endif /*__HAL_PSRAMIP_H_*/


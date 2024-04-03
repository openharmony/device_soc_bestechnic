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
#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "cmsis.h"
#include "hal_cmu.h"

enum HAL_SPI_MOD_CLK_SEL_T {
    HAL_SPI_MOD_CLK_SEL_NONE,
    HAL_SPI_MOD_CLK_SEL_OSC,
    HAL_SPI_MOD_CLK_SEL_OSC_X2,
    HAL_SPI_MOD_CLK_SEL_PLL,
};

struct HAL_SPI_CTRL_T {
    uint32_t sspcr0_tx;
    uint32_t sspcr0_rx;
    uint16_t sspcr1;
    uint8_t sspcpsr;
    uint8_t sspdmacr;
    uint8_t ssprxcr_tx;
    uint8_t ssprxcr_rx;
    uint8_t max_samp_delay;
    uint8_t samp_delay;
    enum HAL_SPI_MOD_CLK_SEL_T clk_sel;
};

struct HAL_SPI_CFG_T {
    uint32_t rate;
    uint8_t clk_delay_half :1;
    uint8_t clk_polarity :1;
    uint8_t slave :1;
    uint8_t dma_rx :1;
    uint8_t dma_tx :1;
    // Rx on separate line for different cs
    uint8_t rx_sep_line :1;
    // Tx data is: DSS_BITS (from bit 1) + RW_BIT (bit 0)
    // When RW_BIT=0 (write), rxfifo will not be filled
    uint8_t ext_dss_rw :1;
    uint8_t skip_en :1;
    uint8_t rx_big_endian :1;
    uint8_t tx_big_endian :1;
    uint8_t cs;
    uint8_t tx_bits;
    uint8_t rx_bits;
    uint8_t rx_frame_bits;
    uint8_t samp_delay;
};

typedef void (*HAL_SPI_DMA_HANDLER_T)(int error);

//------------------------------------------------------------
// SPI common functions
//------------------------------------------------------------

int hal_spi_init_ctrl(const struct HAL_SPI_CFG_T *cfg, struct HAL_SPI_CTRL_T *ctrl);

void hal_spi_sleep(enum HAL_CMU_LPU_SLEEP_MODE_T mode);

void hal_spi_wakeup(enum HAL_CMU_LPU_SLEEP_MODE_T mode);

//------------------------------------------------------------
// SPI ROM functions
//------------------------------------------------------------

#ifdef NO_ISPI

__STATIC_FORCEINLINE int hal_ispi_rom_open(const struct HAL_SPI_CFG_T *cfg) { return 0; }

__STATIC_FORCEINLINE void hal_ispi_rom_activate_cs(uint32_t cs) {}

__STATIC_FORCEINLINE int hal_ispi_rom_enable_ctrl(const struct HAL_SPI_CTRL_T *ctrl) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_rom_restore_ctrl(void) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_rom_busy(void) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_rom_send(const void *data, uint32_t len) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_rom_recv(const void *cmd, void *data, uint32_t len)
{
    uint8_t *d = (uint8_t *)data;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = 0;
    }
    return 0;
}

__STATIC_FORCEINLINE int hal_ispi_rom_send_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *data, uint32_t len) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_rom_recv_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *cmd, void *data, uint32_t len)
{
    return hal_ispi_rom_recv(cmd, data, len);
}

__STATIC_FORCEINLINE int hal_spiphy_rom_open(const struct HAL_SPI_CFG_T *cfg) { return 0; }

__STATIC_FORCEINLINE void hal_spiphy_rom_close(void) {}

__STATIC_FORCEINLINE int hal_spiphy_rom_busy(void) { return 0; }

__STATIC_FORCEINLINE int hal_spiphy_rom_send(const void *data, uint32_t len) { return 0; }

__STATIC_FORCEINLINE int hal_spiphy_rom_recv(const void *cmd, void *data, uint32_t len) { return hal_ispi_rom_recv(cmd, data, len); }

__STATIC_FORCEINLINE void hal_spiphy_rom_activate_cs(uint32_t cs) {}

#else

int hal_ispi_rom_open(const struct HAL_SPI_CFG_T *cfg);

void hal_ispi_rom_activate_cs(uint32_t cs);

int hal_ispi_rom_enable_ctrl(const struct HAL_SPI_CTRL_T *ctrl);

int hal_ispi_rom_restore_ctrl(void);

int hal_ispi_rom_busy(void);

int hal_ispi_rom_send(const void *data, uint32_t len);

int hal_ispi_rom_recv(const void *cmd, void *data, uint32_t len);

int hal_ispi_rom_send_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *data, uint32_t len);

int hal_ispi_rom_recv_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *cmd, void *data, uint32_t len);

int hal_spiphy_rom_open(const struct HAL_SPI_CFG_T *cfg);

void hal_spiphy_rom_close(void);

int hal_spiphy_rom_busy(void);

int hal_spiphy_rom_send(const void *data, uint32_t len);

int hal_spiphy_rom_recv(const void *cmd, void *data, uint32_t len);

void hal_spiphy_rom_activate_cs(uint32_t cs);

#endif

#ifdef NO_ISPI

//------------------------------------------------------------
// ISPI functions
//------------------------------------------------------------

__STATIC_FORCEINLINE int hal_ispi_open(const struct HAL_SPI_CFG_T *cfg) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_close(uint32_t cs) { return 0; }

__STATIC_FORCEINLINE void hal_ispi_activate_cs(uint32_t cs) {}

__STATIC_FORCEINLINE int hal_ispi_enable_ctrl(const struct HAL_SPI_CTRL_T *ctrl) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_restore_ctrl(void) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_busy(void) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_send(const void *data, uint32_t len) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_recv(const void *cmd, void *data, uint32_t len) { return hal_ispi_rom_recv(cmd, data, len); }

__STATIC_FORCEINLINE int hal_ispi_send_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *data, uint32_t len) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_recv_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *cmd, void *data, uint32_t len)
{
    return hal_ispi_rom_recv(cmd, data, len);
}

__STATIC_FORCEINLINE int hal_ispi_dma_send(const void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler) { return 0; }

__STATIC_FORCEINLINE int hal_ispi_dma_recv(const void *cmd, void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler)
{
    hal_ispi_rom_recv(cmd, data, len);
    if (handler) {
        handler(0);
    }
    return 0;
}

__STATIC_FORCEINLINE void hal_ispi_stop_dma_send(void) {}

__STATIC_FORCEINLINE void hal_ispi_stop_dma_recv(void) {}

//------------------------------------------------------------
// SPI PHY functions
//------------------------------------------------------------

__STATIC_FORCEINLINE int hal_spiphy_open(const struct HAL_SPI_CFG_T *cfg) { return 0; }

__STATIC_FORCEINLINE int hal_spiphy_close(uint32_t cs) { return 0; }

__STATIC_FORCEINLINE void hal_spiphy_activate_cs(uint32_t cs) {}

__STATIC_FORCEINLINE int hal_spiphy_busy(void) { return 0; }

__STATIC_FORCEINLINE int hal_spiphy_send(const void *data, uint32_t len) { return 0; }

__STATIC_FORCEINLINE int hal_spiphy_recv(const void *cmd, void *data, uint32_t len) { return hal_ispi_rom_recv(cmd, data, len); }

#else

//------------------------------------------------------------
// ISPI functions
//------------------------------------------------------------

int hal_ispi_open(const struct HAL_SPI_CFG_T *cfg);

int hal_ispi_close(uint32_t cs);

void hal_ispi_activate_cs(uint32_t cs);

int hal_ispi_enable_ctrl(const struct HAL_SPI_CTRL_T *ctrl);

int hal_ispi_restore_ctrl(void);

int hal_ispi_busy(void);

int hal_ispi_send(const void *data, uint32_t len);

int hal_ispi_recv(const void *cmd, void *data, uint32_t len);

int hal_ispi_send_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *data, uint32_t len);

int hal_ispi_recv_ex(const struct HAL_SPI_CTRL_T *ctrl, const void *cmd, void *data, uint32_t len);

int hal_ispi_dma_send(const void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);

int hal_ispi_dma_recv(const void *cmd, void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);

void hal_ispi_stop_dma_send(void);

void hal_ispi_stop_dma_recv(void);

//------------------------------------------------------------
// SPI PHY functions
//------------------------------------------------------------

int hal_spiphy_open(const struct HAL_SPI_CFG_T *cfg);

int hal_spiphy_close(uint32_t cs);

void hal_spiphy_activate_cs(uint32_t cs);

int hal_spiphy_busy(void);

int hal_spiphy_send(const void *data, uint32_t len);

int hal_spiphy_recv(const void *cmd, void *data, uint32_t len);

#endif

//------------------------------------------------------------
// SPI peripheral functions
//------------------------------------------------------------

int hal_spi_open(const struct HAL_SPI_CFG_T *cfg);

int hal_spi_close(uint32_t cs);

int hal_spi_activate_cs(uint32_t cs);

int hal_spi_busy(void);

int hal_spi_send(const void *data, uint32_t len);

int hal_spi_recv(const void *cmd, void *data, uint32_t len);

int hal_spi_dma_send(const void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);

int hal_spi_dma_recv(const void *cmd, void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);

void hal_spi_stop_dma_send(void);

void hal_spi_stop_dma_recv(void);

int hal_spi_enable_and_send(const struct HAL_SPI_CTRL_T *ctrl, const void *data, uint32_t len);

int hal_spi_enable_and_recv(const struct HAL_SPI_CTRL_T *ctrl, const void *cmd, void *data, uint32_t len);

//------------------------------------------------------------
// SPI LCD functions
//------------------------------------------------------------

int hal_spilcd_open(const struct HAL_SPI_CFG_T *cfg);

int hal_spilcd_close(uint32_t cs);

int hal_spilcd_activate_cs(uint32_t cs);

int hal_spilcd_busy(void);

int hal_spilcd_send(const void *data, uint32_t len);

int hal_spilcd_recv(const void *cmd, void *data, uint32_t len);

int hal_spilcd_dma_send(const void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);

int hal_spilcd_dma_recv(const void *cmd, void *data, uint32_t len, HAL_SPI_DMA_HANDLER_T handler);

void hal_spilcd_stop_dma_send(void);

void hal_spilcd_stop_dma_recv(void);

int hal_spilcd_set_data_mode(void);

int hal_spilcd_set_cmd_mode(void);

int hal_spilcd_enable_and_send(const struct HAL_SPI_CTRL_T *ctrl, const void *data, uint32_t len);

int hal_spilcd_enable_and_recv(const struct HAL_SPI_CTRL_T *ctrl, const void *cmd, void *data, uint32_t len);

//------------------------------------------------------------
// SPI DPD functions
//------------------------------------------------------------

int hal_spidpd_open(const struct HAL_SPI_CFG_T *cfg);

int hal_spidpd_close(uint32_t cs);

int hal_spidpd_busy(void);

int hal_spidpd_send(const void *data, uint32_t len);

int hal_spidpd_recv(const void *cmd, void *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif


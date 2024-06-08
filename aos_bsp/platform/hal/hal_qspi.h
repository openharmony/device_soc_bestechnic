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
#ifndef __HAL_QSPI_H__
#define __HAL_QSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "cmsis.h"
#include "hal_cmu.h"

enum HAL_QSPI_FORMAT_T {
    HAL_QSPI_STANDARD_MODE,
    HAL_QSPI_DUAL_MODE,
    HAL_QSPI_QUAD_MODE,
    HAL_QSPI_OCTAL_MODE,

    HAL_QSPI_MODE_QTY
};

enum HAL_QSPI_FRAME_FORMAT_T {
    HAL_QSPI_FRAME_SPI,
    HAL_QSPI_FRAME_SSP,
    HAL_QSPI_FRAME_MICROWIRE,

    HAL_QSPI_FRAME_QTY
};

enum HAL_QSPI_CS_T {
    HAL_QSPI_CS_0,
    HAL_QSPI_CS_1,

    HAL_QSPI_CS_QTY
};

enum HAL_QSPI_TRANSFER_MODE_T {
    HAL_QSPI_TX_AND_RX,
    HAL_QSPI_TX_ONLY,
    HAL_QSPI_RX_ONLY,
    HAL_QSPI_EEPROM_READ,

    HAL_QSPI_TRANSFER_MODE_QTY
};

enum HAL_QSPI_INST_L_T {
    HAL_QSPI_INST_L0,
    HAL_QSPI_INST_L4,
    HAL_QSPI_INST_L8,
    HAL_QSPI_INST_L16,

    HAL_QSPI_INST_LEN_QTY
};

enum HAL_QSPI_ADDR_L_T {
    HAL_QSPI_ADDR_L0,
    HAL_QSPI_ADDR_L4,
    HAL_QSPI_ADDR_L8,
    HAL_QSPI_ADDR_L12,
    HAL_QSPI_ADDR_L16,
    HAL_QSPI_ADDR_L20,
    HAL_QSPI_ADDR_L24,
    HAL_QSPI_ADDR_L28,
    HAL_QSPI_ADDR_L32,
    HAL_QSPI_ADDR_L36,
    HAL_QSPI_ADDR_L40,
    HAL_QSPI_ADDR_L44,
    HAL_QSPI_ADDR_L48,
    HAL_QSPI_ADDR_L52,
    HAL_QSPI_ADDR_L56,
    HAL_QSPI_ADDR_L60,

    HAL_QSPI_ADDR_LEN_QTY
};

enum HAL_QSPI_TRANS_TYPE_T {
    HAL_QSPI_TRANS_TYPE_TT0,
    HAL_QSPI_TRANS_TYPE_TT1,
    HAL_QSPI_TRANS_TYPE_TT2,

    HAL_QSPI_TRANS_TYPE_QTY
};

enum HAL_QSPI_MOD_CLK_SEL_T {
    HAL_QSPI_MOD_CLK_SEL_NONE,
    HAL_QSPI_MOD_CLK_SEL_OSC,
    HAL_QSPI_MOD_CLK_SEL_OSC_X2,
    HAL_QSPI_MOD_CLK_SEL_PLL,

    HAL_QSPI_MOD_CLK_QTY
};

struct HAL_QSPI_CTRL_T {
    uint32_t ctrlr0;
    uint32_t ctrlr1;
    uint32_t mwcr;
    uint32_t ser;
    uint32_t baudr;
    uint32_t txftlr;
    uint32_t rxftlr;
    uint32_t dmacr;
    uint32_t dmatdlr;
    uint32_t dmardlr;
    uint32_t spi_ctrlr0;
    uint32_t clk_sel;
    uint32_t clk_delay;
};

struct HAL_QSPI_CFG_T {
    uint32_t rate;                                // work rate
    enum HAL_QSPI_FORMAT_T format;                // mode
    uint8_t clk_delay_half;                       // serial clock phase
    uint8_t clk_polarity;                         // serial clock polarity
    enum HAL_QSPI_CS_T cs;                        // cs number
    enum HAL_QSPI_TRANSFER_MODE_T  transfer_mode; // send/recv
    enum HAL_QSPI_INST_L_T  inst_len;             //instruction len
    enum HAL_QSPI_ADDR_L_T  addr_len;             //addr len
    enum HAL_QSPI_TRANS_TYPE_T  trans_type;       //instruction&addr mode
    bool     slave;                               // master/slave
    bool     dma_rx;                              // tx dma
    bool     dma_tx;                              // rx dma
    uint8_t  tx_bits;                             // tx bits(8/32)
    uint8_t samp_delay;                           // rx clk delay
    uint8_t mult_number;                          // cs number
};

typedef void (*HAL_QSPI_DMA_HANDLER_T)(int cnt);

int hal_qspi_open(const struct HAL_QSPI_CFG_T *cfg);

void hal_qspi_loop(void);

int hal_qspi_send(const void *data, uint32_t len);

int hal_qspi_dma_send(uint32_t instruction, uint32_t addr, const void *data, uint32_t len, HAL_QSPI_DMA_HANDLER_T handler);

int hal_qspi_dma_send_data(const void *data, uint32_t len, HAL_QSPI_DMA_HANDLER_T handler);

int hal_qspi_recv(const void *cmd, void *data, uint32_t len);

int hal_qspi_dma_recv_data(const void *cmd, void *data, uint32_t len, HAL_QSPI_DMA_HANDLER_T handler);

int hal_qspi_dma_recv(uint32_t instruction, uint32_t addr, void *data, uint32_t len, HAL_QSPI_DMA_HANDLER_T handler);

int hal_qspi_close(uint32_t cs);

#ifdef __cplusplus
}
#endif

#endif

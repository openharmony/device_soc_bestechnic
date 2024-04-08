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

enum HAL_QSPI_MODE_T {
    HAL_QSPI_STANDARD_MODE,
    HAL_QSPI_DUAL_MODE,
    HAL_QSPI_QUAD_MODE,
    HAL_QSPI_OCTAL_MODE,

    HAL_QSPI_MODE_QTY,
};

enum HAL_QSPI_CS_T {
    HAL_QSPI_CS_0,
    HAL_QSPI_CS_1,

    HAL_QSPI_CS_QTY,
};

enum HAL_QSPI_TRANSFER_MODE_T {
    HAL_QSPI_TX_AND_RX,
    HAL_QSPI_TX_ONLY,
    HAL_QSPI_RX_ONLY,
    HAL_QSPI_EEPROM_READ,

    HAL_QSPI_TRANSFER_MODE_QTY,
};

enum HAL_QSPI_INST_L_T {
    HAL_QSPI_INST_L0,
    HAL_QSPI_INST_L4,
    HAL_QSPI_INST_L8,
    HAL_QSPI_INST_L16,

    HAL_QSPI_INST_LEN_QTY,
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
    uint32_t spi_ctrlr0;
};

struct HAL_QSPI_CFG_T {
    uint32_t rate;
    enum HAL_QSPI_CS_T cs;
    enum HAL_QSPI_MODE_T mode;
    uint8_t  transfer_mode;
    bool     slave;
    bool     dma_rx;
    bool     dma_tx;
    uint8_t  tx_bits;
};

typedef void (*HAL_QSPI_DMA_HANDLER_T)(int error);

int hal_qspi_open(struct HAL_QSPI_CFG_T *cfg);

int hal_qspi_dma_send(const void *data, uint32_t len, HAL_QSPI_DMA_HANDLER_T handler);

int hal_qspi_send(const void *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif

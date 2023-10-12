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
#ifndef __HAL_DMACFG_BEST2003_H__
#define __HAL_DMACFG_BEST2003_H__

#define AUDMA_PERIPH_NUM                        16
#define GPDMA_PERIPH_NUM                        16

#define AUDMA_CHAN_NUM                          8
#define GPDMA_CHAN_NUM                          8

#define AUDMA_CHAN_START                        (0)
#define GPDMA_CHAN_START                        (0)

static const uint32_t bes2003_gpdma_fifo_addr[AUDMA_PERIPH_NUM] = {
    CODEC_BASE + 0x01C,     // CODEC RX
    CODEC_BASE + 0x01C,     // CODEC TX
#ifdef CODEC_DSD
    CODEC_BASE + 0x034,     // DSD RX
    CODEC_BASE + 0x034,     // DSD TX
#else
    UART0_BASE + 0x000,     // UART0 RX
    UART0_BASE + 0x000,     // UART0 TX
#endif
    CODEC_BASE + 0x058,     // CODEC TX2
    I2C1_BASE + 0x010,      // I2C1 RX
    I2C1_BASE + 0x010,      // I2C1 TX
    SPI_BASE + 0x008,       // SPI RX
    SPI_BASE + 0x008,       // SPI TX
    SDMMC0_BASE + 0x200,    // SDMMC
    BTPCM_BASE + 0x1C0,     // BTPCM RX
    BTPCM_BASE + 0x1C8,     // BTPCM TX
    UART2_BASE + 0x000,     // UART2 RX
    UART2_BASE + 0x000,     // UART2 TX
    UART1_BASE + 0x000,     // UART1 RX
    UART1_BASE + 0x000,     // UART1 TX
};

static const enum HAL_DMA_PERIPH_T bes2003_gpdma_fifo_periph[AUDMA_PERIPH_NUM] = {
    HAL_AUDMA_CODEC_RX,
    HAL_AUDMA_CODEC_TX,
#ifdef CODEC_DSD
    HAL_AUDMA_DSD_RX,
    HAL_AUDMA_DSD_TX,
#else
    HAL_GPDMA_UART0_RX,
    HAL_GPDMA_UART0_TX,
#endif
    HAL_DMA_PERIPH_NULL,
    HAL_GPDMA_I2C1_RX,
    HAL_GPDMA_I2C1_TX,
    HAL_GPDMA_SPI_RX,
    HAL_GPDMA_SPI_TX,
    HAL_GPDMA_SDMMC0,
    HAL_AUDMA_BTPCM_RX,
    HAL_AUDMA_BTPCM_TX,
    HAL_GPDMA_UART2_RX,
    HAL_GPDMA_UART2_TX,
    HAL_GPDMA_UART1_RX,
    HAL_GPDMA_UART1_TX,
};

static const uint32_t bes2003_audma_fifo_addr[GPDMA_PERIPH_NUM] = {
    CODEC_BASE + 0x01C,     // CODEC RX
    CODEC_BASE + 0x01C,     // CODEC TX
    IRDA_BASE + 0x000,      // IR RX
    IRDA_BASE + 0x004,      // IR TX
    UART0_BASE + 0x000,     // UART0 RX
    UART0_BASE + 0x000,     // UART0 TX
    UART2_BASE + 0x000,     // UART2 RX
    UART2_BASE + 0x000,     // UART2 TX
    BTDUMP_BASE + 0x34,     // BTDUMP
    SDMMC0_BASE + 0x200,    // SDMMC0
    BTPCM_BASE + 0x1C0,     // BTPCM RX
    BTPCM_BASE + 0x1C8,     // BTPCM TX
    UART1_BASE + 0x000,     // UART1 RX
    UART1_BASE + 0x000,     // UART1 TX
    FLASH_CTRL_BASE + 0x008,// FLASH0
#ifdef FLASH1_CTRL_BASE
    FLASH1_CTRL_BASE + 0x008,// FLASH0
#else
    0,
#endif
};

static const enum HAL_DMA_PERIPH_T bes2003_audma_fifo_periph[GPDMA_PERIPH_NUM] = {
    HAL_AUDMA_CODEC_RX,
    HAL_AUDMA_CODEC_TX,
    HAL_GPDMA_IR_RX,
    HAL_GPDMA_IR_TX,
    HAL_GPDMA_UART0_RX,
    HAL_GPDMA_UART0_TX,
    HAL_GPDMA_UART2_RX,
    HAL_GPDMA_UART2_TX,
    HAL_AUDMA_BTDUMP,
    HAL_GPDMA_SDMMC0,
    HAL_AUDMA_BTPCM_RX,
    HAL_AUDMA_BTPCM_TX,
    HAL_GPDMA_UART1_RX,
    HAL_GPDMA_UART1_TX,
    HAL_GPDMA_FLASH0,
#ifdef FLASH1_CTRL_BASE
    HAL_GPDMA_FLASH1,
#else
    HAL_DMA_PERIPH_NULL,
#endif
};

#ifndef DSP_USE_GPDMA
#if !defined(CHIP_ROLE_CP)
#if defined(CHIP_DMA_CFG_IDX) && (CHIP_DMA_CFG_IDX == 1)
#define audma_fifo_addr bes2003_gpdma_fifo_addr
#define audma_fifo_periph bes2003_gpdma_fifo_periph
#else
#define audma_fifo_addr bes2003_audma_fifo_addr
#define audma_fifo_periph bes2003_audma_fifo_periph
#define gpdma_fifo_addr bes2003_gpdma_fifo_addr
#define gpdma_fifo_periph bes2003_gpdma_fifo_periph
#endif /* CHIP_DMA_CFG_IDX */
#else
#define audma_fifo_addr bes2003_audma_fifo_addr
#define audma_fifo_periph bes2003_audma_fifo_periph
#endif /* CHIP_ROLE_CP */
#else
#ifdef CHIP_BEST2003_DSP
//A7 use GPDMA
#define audma_fifo_addr bes2003_gpdma_fifo_addr
#define audma_fifo_periph bes2003_gpdma_fifo_periph
#else
//MCU use AUDMA
#define audma_fifo_addr bes2003_audma_fifo_addr
#define audma_fifo_periph bes2003_audma_fifo_periph
#endif
#endif

#endif


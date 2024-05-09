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
#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "hal_cmu.h"

#define HAL_DMA_CHAN_NONE               0xFF

#define HAL_DMA_MAX_DESC_XFER_SIZE      0xFFF

enum HAL_DMA_RET_T {
    HAL_DMA_OK              = 0,
    HAL_DMA_ERR             = 1,
    HAL_DMA_ERR_TSIZE       = 2,
    HAL_DMA_ERR_TYPE        = 3,
    HAL_DMA_ERR_NOT_EN      = 4,
    HAL_DMA_ERR_BUSY        = 5,
    HAL_DMA_ERR_SRC_XMOD    = 6,
    HAL_DMA_ERR_SRC_XCNT    = 7,
    HAL_DMA_ERR_SRC_YMOD    = 8,
    HAL_DMA_ERR_SRC_YCNT    = 9,
    HAL_DMA_ERR_DST_XMOD    = 10,
    HAL_DMA_ERR_DST_XCNT    = 11,
    HAL_DMA_ERR_DST_YMOD    = 12,
    HAL_DMA_ERR_DST_YCNT    = 13,
    HAL_DMA_ERR_NO_IRQ      = 14,
    HAL_DMA_ERR_SRC_ADDR    = 15,
    HAL_DMA_ERR_DST_ADDR    = 16,
};

enum HAL_DMA_GET_CHAN_T {
    HAL_DMA_HIGH_PRIO,
    HAL_DMA_LOW_PRIO,
    HAL_DMA_LOW_PRIO_ONLY
};

// DMA Type of DMA controller
enum HAL_DMA_FLOW_CONTROL_T {
    HAL_DMA_FLOW_M2M_DMA            = 0,    /* Memory to memory - DMA control */
    HAL_DMA_FLOW_M2P_DMA            = 1,    /* Memory to peripheral - DMA control */
    HAL_DMA_FLOW_P2M_DMA            = 2,    /* Peripheral to memory - DMA control */
    HAL_DMA_FLOW_P2P_DMA            = 3,    /* Source peripheral to destination peripheral - DMA control */
    HAL_DMA_FLOW_P2P_DSTPERIPH      = 4,    /* Source peripheral to destination peripheral - destination peripheral control */
    HAL_DMA_FLOW_M2P_PERIPH         = 5,    /* Memory to peripheral - peripheral control */
    HAL_DMA_FLOW_P2M_PERIPH         = 6,    /* Peripheral to memory - peripheral control */
    HAL_DMA_FLOW_P2P_SRCPERIPH      = 7,    /* Source peripheral to destination peripheral - source peripheral control */

    HAL_DMA_FLOW_FLAG_SI            = 0x10,
    HAL_DMA_FLOW_FLAG_DI            = 0x20,
    HAL_DMA_FLOW_FLAG_NO_SI         = 0x40,
    HAL_DMA_FLOW_FLAG_NO_DI         = 0x80,
    HAL_DMA_FLOW_FLAG_MASK          = (HAL_DMA_FLOW_FLAG_SI | HAL_DMA_FLOW_FLAG_DI | HAL_DMA_FLOW_FLAG_NO_SI | HAL_DMA_FLOW_FLAG_NO_DI),

    HAL_DMA_FLOW_M2P_DI_DMA         = HAL_DMA_FLOW_M2P_DMA | HAL_DMA_FLOW_FLAG_DI,
    HAL_DMA_FLOW_P2M_SI_DMA         = HAL_DMA_FLOW_P2M_DMA | HAL_DMA_FLOW_FLAG_SI,
    HAL_DMA_FLOW_P2P_DI_DMA         = HAL_DMA_FLOW_P2P_DMA | HAL_DMA_FLOW_FLAG_DI,
    HAL_DMA_FLOW_P2P_SI_DMA         = HAL_DMA_FLOW_P2P_DMA | HAL_DMA_FLOW_FLAG_SI,
};

// DMA Burst size in Source and Destination definitions
enum HAL_DMA_BSIZE_T {
    HAL_DMA_BSIZE_1           = 0,    /* Burst size = 1 */
    HAL_DMA_BSIZE_4           = 1,    /* Burst size = 4 */
    HAL_DMA_BSIZE_8           = 2,    /* Burst size = 8 */
    HAL_DMA_BSIZE_16          = 3,    /* Burst size = 16 */
    HAL_DMA_BSIZE_32          = 4,    /* Burst size = 32 */
    HAL_DMA_BSIZE_64          = 5,    /* Burst size = 64 */
    HAL_DMA_BSIZE_128         = 6,    /* Burst size = 128 */
    HAL_DMA_BSIZE_256         = 7,    /* Burst size = 256 */
#ifdef DMA_BSIZE_HALF_FIFO
    HAL_DMA_BSIZE_HALF_FIFO   = 8,    /* Burst size = DMA_FIFO_SIZE / 2 */
#endif
};

// Width in Source transfer width and Destination transfer width definitions
enum HAL_DMA_WIDTH_T {
    HAL_DMA_WIDTH_BYTE        = 0,    /* Width = 1 byte */
    HAL_DMA_WIDTH_HALFWORD    = 1,    /* Width = 2 bytes */
    HAL_DMA_WIDTH_WORD        = 2,    /* Width = 4 bytes */
};

enum HAL_DMA_MASTER_T {
    HAL_DMA_MASTER_DEFAULT,
    HAL_DMA_MASTER_0,
    HAL_DMA_MASTER_1,
    HAL_DMA_MASTER_SRC1_DST0,     /* source select master1, destination select master 0 */
    HAL_DMA_MASTER_SRC0_DST1,     /* source select master0, destination select master 1 */
};

enum HAL_DMA_PERIPH_T {
    HAL_DMA_PERIPH_NULL         = 0,
    HAL_DMA0_MEM                = 1,
    HAL_DMA1_MEM                = 2,
    HAL_DMA2_MEM                = 3,
    HAL_DMA3_MEM                = 4,

    HAL_AUDMA_MEM               = HAL_DMA0_MEM,
    HAL_GPDMA_MEM               = HAL_DMA1_MEM,

    HAL_GPDMA_SDIO              = 10,
    HAL_GPDMA_SDMMC0            = 11,
    HAL_GPDMA_SDMMC1            = 12,
    HAL_GPDMA_I2C0_RX           = 13,
    HAL_GPDMA_I2C0_TX           = 14,
    HAL_GPDMA_SPI_RX            = 15,
    HAL_GPDMA_SPI_TX            = 16,
    HAL_GPDMA_SPILCD_RX         = 17,
    HAL_GPDMA_SPILCD_TX         = 18,
    HAL_GPDMA_ISPI_RX           = 19,
    HAL_GPDMA_ISPI_TX           = 20,
    HAL_GPDMA_UART0_RX          = 21,
    HAL_GPDMA_UART0_TX          = 22,
    HAL_GPDMA_UART1_RX          = 23,
    HAL_GPDMA_UART1_TX          = 24,
    HAL_GPDMA_UART2_RX          = 25,
    HAL_GPDMA_UART2_TX          = 26,
    HAL_GPDMA_FLASH0            = 27,
    HAL_GPDMA_FLASH1            = 28,
    HAL_GPDMA_FLASH2            = 29,
    HAL_GPDMA_I2C1_RX           = 30,
    HAL_GPDMA_I2C1_TX           = 31,
    HAL_GPDMA_I2C2_RX           = 32,
    HAL_GPDMA_I2C2_TX           = 33,
    HAL_GPDMA_I2C3_RX           = 34,
    HAL_GPDMA_I2C3_TX           = 35,
    HAL_GPDMA_UART3_RX          = 36,
    HAL_GPDMA_UART3_TX          = 37,
    HAL_GPDMA_IR_RX             = 38,
    HAL_GPDMA_IR_TX             = 39,
    HAL_GPDMA_I2C4_RX           = 40,
    HAL_GPDMA_I2C4_TX           = 41,
    HAL_GPDMA_I2C5_RX           = 42,
    HAL_GPDMA_I2C5_TX           = 43,
    HAL_GPDMA_QSPI_RX           = 44,
    HAL_GPDMA_QSPI_TX           = 45,
    HAL_GPDMA_JPEG_PIX          = 46,
    HAL_GPDMA_JPEG_ECS          = 47,
    HAL_GPDMA_I2C6_RX           = 48,
    HAL_GPDMA_I2C6_TX           = 49,
    HAL_GPDMA_I2C7_RX           = 50,
    HAL_GPDMA_I2C7_TX           = 51,
    HAL_GPDMA_UART4_RX          = 52,
    HAL_GPDMA_UART4_TX          = 53,
    HAL_GPDMA_UART5_RX          = 54,
    HAL_GPDMA_UART5_TX          = 55,
    HAL_GPDMA_JPEG1_PIX         = 56,
    HAL_GPDMA_JPEG1_ECS         = 57,

    HAL_AUDMA_CODEC_RX          = 70,
    HAL_AUDMA_CODEC_TX          = 71,
    HAL_AUDMA_BTPCM_RX          = 72,
    HAL_AUDMA_BTPCM_TX          = 73,
    HAL_AUDMA_I2S0_RX           = 74,
    HAL_AUDMA_I2S0_TX           = 75,
    HAL_AUDMA_DPD_RX            = 76,
    HAL_AUDMA_DPD_TX            = 77,
    HAL_AUDMA_SPDIF0_RX         = 78,
    HAL_AUDMA_SPDIF0_TX         = 79,
    HAL_AUDMA_SPDIF1_RX         = 80,
    HAL_AUDMA_SPDIF1_TX         = 81,
    HAL_AUDMA_DSD_RX            = 82,
    HAL_AUDMA_DSD_TX            = 83,
    HAL_AUDMA_MC_RX             = 84,
    HAL_AUDMA_FFT_RX            = 85,
    HAL_AUDMA_FFT_TX            = 86,
    HAL_AUDMA_FIR_RX            = 87,
    HAL_AUDMA_FIR_TX            = 88,
    HAL_AUDMA_IIR_RX            = 89,
    HAL_AUDMA_IIR_TX            = 90,
    HAL_AUDMA_BTDUMP            = 91,
    HAL_AUDMA_I2S1_RX           = 92,
    HAL_AUDMA_I2S1_TX           = 93,
    HAL_AUDMA_FM_RX             = 94,
    HAL_AUDMA_FMDUMP0           = 95,
    HAL_AUDMA_FMDUMP1           = 96,
    HAL_AUDMA_CODEC_RX2         = 97,
    HAL_AUDMA_CODEC_TX2         = 98,
    HAL_AUDMA_CODEC_RX3         = 99,
    HAL_AUDMA_CODEC_TX3         = 100,
    HAL_AUDMA_CAP_SENS          = 101,
    HAL_AUDMA_I2S2_RX           = 102,
    HAL_AUDMA_I2S2_TX           = 103,
    HAL_AUDMA_I2S3_RX           = 104,
    HAL_AUDMA_I2S3_TX           = 105,

    HAL_DMA_PERIPH_QTY,
};

struct HAL_DMA_DESC_T;

typedef void (*HAL_DMA_IRQ_HANDLER_T)(uint8_t chan, uint32_t remain_tsize, uint32_t error, struct HAL_DMA_DESC_T *lli);

typedef void (*HAL_DMA_START_CALLBACK_T)(uint8_t chan);

typedef void (*HAL_DMA_DELAY_FUNC)(uint32_t ms);

// DMA structure using for DMA configuration
struct HAL_DMA_CH_CFG_T {
    uint8_t ch;                         /* DMA channel number */
    enum HAL_DMA_MASTER_T master : 4;
    uint8_t try_burst : 4;
    uint16_t src_tsize;                 /* Length/Size of transfer */
    enum HAL_DMA_WIDTH_T src_width;
    enum HAL_DMA_WIDTH_T dst_width;
    enum HAL_DMA_BSIZE_T src_bsize;
    enum HAL_DMA_BSIZE_T dst_bsize;
    enum HAL_DMA_FLOW_CONTROL_T type;   /* Transfer Type */
    enum HAL_DMA_PERIPH_T src_periph;
    enum HAL_DMA_PERIPH_T dst_periph;
    uint32_t src;                       /* Physical Source Address */
    uint32_t dst;                       /* Physical Destination Address */
    HAL_DMA_IRQ_HANDLER_T handler;
    HAL_DMA_START_CALLBACK_T start_cb;
};

// Transfer Descriptor structure typedef
struct HAL_DMA_DESC_T {
    uint32_t src;       /* Source address */
    uint32_t dst;       /* Destination address */
    uint32_t lli;       /* Pointer to next descriptor structure */
    uint32_t ctrl;      /* Control word that has transfer size, type etc. */
};

// DMA 2D configuration structure
struct HAL_DMA_2D_CFG_T {
    int32_t xmodify;
    uint32_t xcount;
    int32_t ymodify;
    uint32_t ycount;
};

struct HAL_DMA_BURST_ADDR_INC_T {
    bool src_inc_en;
    bool dst_inc_en;
    int16_t src_inc_val;
    int16_t dst_inc_val;
};

//=============================================================

void hal_dma_open(void);

void hal_dma_close(void);

void hal_dma_sleep(enum HAL_CMU_LPU_SLEEP_MODE_T mode);

void hal_dma_wakeup(enum HAL_CMU_LPU_SLEEP_MODE_T mode);

bool hal_dma_chan_busy(uint8_t ch);

uint8_t hal_dma_get_chan(enum HAL_DMA_PERIPH_T periph, enum HAL_DMA_GET_CHAN_T policy);

uint32_t hal_dma_get_base_addr(uint8_t ch);

void hal_dma_free_chan(uint8_t ch);

void hal_dma_cancel(uint8_t ch);

void hal_dma_stop(uint8_t ch);

enum HAL_DMA_RET_T hal_dma_init_desc(struct HAL_DMA_DESC_T *desc,
                                     const struct HAL_DMA_CH_CFG_T *cfg,
                                     const struct HAL_DMA_DESC_T *next,
                                     int tc_irq);

enum HAL_DMA_RET_T hal_dma_sg_start(const struct HAL_DMA_DESC_T *desc,
                                    const struct HAL_DMA_CH_CFG_T *cfg);

enum HAL_DMA_RET_T hal_dma_sg_2d_start(const struct HAL_DMA_DESC_T *desc,
                                       const struct HAL_DMA_CH_CFG_T *cfg,
                                       const struct HAL_DMA_2D_CFG_T *src_2d,
                                       const struct HAL_DMA_2D_CFG_T *dst_2d);

enum HAL_DMA_RET_T hal_dma_start(const struct HAL_DMA_CH_CFG_T *cfg);

uint32_t hal_dma_get_cur_src_addr(uint8_t ch);

uint32_t hal_dma_get_cur_dst_addr(uint8_t ch);

void hal_dma_get_cur_src_remain_and_addr(uint8_t ch, uint32_t *remain, uint32_t *src);

uint32_t hal_dma_get_sg_remain_size(uint8_t ch);

void hal_dma_get_irq_list(const int16_t **irq, uint32_t *cnt);

enum HAL_DMA_RET_T hal_dma_irq_run_chan(uint8_t ch);

bool hal_dma_busy(void);

HAL_DMA_DELAY_FUNC hal_dma_set_delay_func(HAL_DMA_DELAY_FUNC new_func);

void hal_dma_remap_periph(enum HAL_DMA_PERIPH_T periph, int enable);

void hal_dma_tc_irq_enable(uint8_t ch);

void hal_dma_tc_irq_disable(uint8_t ch);

void hal_dma_set_burst_addr_inc(uint8_t ch, const struct HAL_DMA_BURST_ADDR_INC_T *inc);

void hal_dma_clear_burst_addr_inc(uint8_t ch);

void hal_dma_set_desc_burst_addr_inc(struct HAL_DMA_DESC_T *desc, const struct HAL_DMA_BURST_ADDR_INC_T *inc);

void hal_dma_clear_desc_burst_addr_inc(struct HAL_DMA_DESC_T *desc);

void hal_dma_sync_for_cpu(const void *buf, uint32_t len);

void hal_dma_sync_for_device(const void *buf, uint32_t len);

void hal_dma_record_busy_chan(void);

void hal_dma_print_busy_chan(void);

void hal_dma_cust_set_dmaconfig(uint32_t dma_base, uint32_t clr_val, uint32_t set_val);

//=============================================================

#define hal_audma_open                  hal_dma_open
#define hal_audma_close                 hal_dma_close
#define hal_audma_chan_busy             hal_dma_chan_busy
#define hal_audma_get_chan              hal_dma_get_chan
#define hal_audma_get_base_addr         hal_dma_get_base_addr
#define hal_audma_free_chan             hal_dma_free_chan
#define hal_audma_cancel                hal_dma_cancel
#define hal_audma_stop                  hal_dma_stop
#define hal_audma_init_desc             hal_dma_init_desc
#define hal_audma_sg_start              hal_dma_sg_start
#define hal_audma_start                 hal_dma_start
#define hal_audma_get_cur_src_addr      hal_dma_get_cur_src_addr
#define hal_audma_get_cur_dst_addr      hal_dma_get_cur_dst_addr
#define hal_audma_get_sg_remain_size    hal_dma_get_sg_remain_size
#define hal_audma_irq_run_chan          hal_dma_irq_run_chan

#define hal_gpdma_open                  hal_dma_open
#define hal_gpdma_close                 hal_dma_close
#define hal_gpdma_chan_busy             hal_dma_chan_busy
#define hal_gpdma_get_chan              hal_dma_get_chan
#define hal_gpdma_get_base_addr         hal_dma_get_base_addr
#define hal_gpdma_free_chan             hal_dma_free_chan
#define hal_gpdma_cancel                hal_dma_cancel
#define hal_gpdma_stop                  hal_dma_stop
#define hal_gpdma_init_desc             hal_dma_init_desc
#define hal_gpdma_sg_start              hal_dma_sg_start
#define hal_gpdma_start                 hal_dma_start
#define hal_gpdma_get_cur_src_addr      hal_dma_get_cur_src_addr
#define hal_gpdma_get_cur_dst_addr      hal_dma_get_cur_dst_addr
#define hal_gpdma_get_sg_remain_size    hal_dma_get_sg_remain_size
#define hal_gpdma_irq_run_chan          hal_dma_irq_run_chan

//=============================================================

#ifdef __cplusplus
}
#endif

#endif

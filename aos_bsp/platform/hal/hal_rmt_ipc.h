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
#ifndef __HAL_RMT_IPC_H__
#define __HAL_RMT_IPC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_addr_map.h"
#ifdef RMT_IPC_API_ENABLE
#include CHIP_SPECIFIC_HDR(hal_rmt_ipc)
#else
// Just for compile
enum HAL_RMT_IPC_CORE_T {
    HAL_RMT_IPC_CORE_QTY,
};
#endif

enum HAL_RMT_IPC_CHAN_T {
    HAL_RMT_IPC_CHAN_0,
    HAL_RMT_IPC_CHAN_1,
    HAL_RMT_IPC_CHAN_2,
    HAL_RMT_IPC_CHAN_3,

    HAL_RMT_IPC_CHAN_QTY
};

#define HAL_RMT_IPC_ALL_SEND_SEQ                (~0UL)

typedef void *HAL_RMT_IPC_EP_T;

typedef unsigned int (*HAL_RMT_IPC_RX_IRQ_HANDLER)(const void *data, unsigned int len);
typedef void (*HAL_RMT_IPC_TX_IRQ_HANDLER)(const void *data, unsigned int len);

typedef void (*HAL_RMT_IPC_SEND_CALLBACK)(void);

int hal_rmt_ipc_open(enum HAL_RMT_IPC_CORE_T core, enum HAL_RMT_IPC_CHAN_T chan,
                     HAL_RMT_IPC_RX_IRQ_HANDLER rxhandler, HAL_RMT_IPC_TX_IRQ_HANDLER txhandler,
                     int manual_rx_done, HAL_RMT_IPC_EP_T *ep_out);

int hal_rmt_ipc_close(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_start_recv(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_stop_recv(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_send_seq(HAL_RMT_IPC_EP_T ep, const void *data, unsigned int len, unsigned int *seq);

int hal_rmt_ipc_send(HAL_RMT_IPC_EP_T ep, const void *data, unsigned int len);

int hal_rmt_ipc_rx_done(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_tx_irq_run(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_tx_active(HAL_RMT_IPC_EP_T ep, unsigned int seq);

int hal_rmt_ipc_opened(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_peer_opened(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_rx_irq_pending(HAL_RMT_IPC_EP_T ep);

int hal_rmt_ipc_get_ep(enum HAL_RMT_IPC_CORE_T core, enum HAL_RMT_IPC_CHAN_T chan, HAL_RMT_IPC_EP_T *ep_out);

int hal_rmt_ipc_update_cfg_loc(enum HAL_RMT_IPC_CORE_T core, void *peer_loc, void *local_loc);

int hal_rmt_ipc_register_send_callback(HAL_RMT_IPC_EP_T ep, HAL_RMT_IPC_SEND_CALLBACK cb);

int hal_rmt_ipc_get_rsc_table(HAL_RMT_IPC_EP_T ep, uint32_t *rsc_addr, uint32_t *rsc_sz);

#ifdef __cplusplus
}
#endif

#endif


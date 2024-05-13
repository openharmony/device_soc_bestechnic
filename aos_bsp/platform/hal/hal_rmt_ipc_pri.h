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
#ifndef __HAL_RMT_IPC_PRI_H__
#define __HAL_RMT_IPC_PRI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_addr_map.h"
#include "plat_types.h"
#include "cmsis.h"
#include "hal_rmt_ipc.h"
#include "hal_sleep.h"

#ifdef CPU_TO_DEV_ADDR_REMAP
#define RMT_IPC_OPEN_CFG_QUAL
#else
#define RMT_IPC_OPEN_CFG_QUAL                   const
#endif

struct HAL_RMT_IPC_MSG_T;
struct HAL_RMT_IPC_CH_CFG_T;

typedef void (*HAL_RMT_IPC_IRQ_INIT)(uint32_t chan);
typedef void (*HAL_RMT_IPC_PEER_TX_IRQ_SET)(uint32_t chan);
typedef void (*HAL_RMT_IPC_LOCAL_TX_IRQ_CLEAR)(uint32_t chan);
typedef void (*HAL_RMT_IPC_RX_IRQ_SUSPEND)(uint32_t chan);
typedef void (*HAL_RMT_IPC_RX_IRQ_RESUME)(uint32_t chan);
typedef void (*HAL_RMT_IPC_RX_DONE)(uint32_t chan);
typedef int (*HAL_RMT_IPC_IRQ_ACTIVE)(uint32_t chan, uint32_t type);
typedef void (*HAL_RMT_IPC_IRQ_ENTRY)(void);
typedef int (*HAL_RMT_IPC_PM_NOTIF_HANDLER)(enum HAL_PM_STATE_T state);
typedef int (*HAL_RMT_IPC_SEND_DATA_RELAY)(const struct HAL_RMT_IPC_CH_CFG_T *chan, struct HAL_RMT_IPC_MSG_T *msg);

enum HAL_RMT_IPC_IRQ_TYPE_T {
    HAL_RMT_IPC_IRQ_DATA_IND,
    HAL_RMT_IPC_IRQ_DATA_DONE,

    HAL_RMT_IPC_IRQ_TYPE_QTY
};

struct HAL_RMT_IPC_MSG_T {
    struct HAL_RMT_IPC_MSG_T *next;         // pointer to next element in the list
    unsigned int len;                       // message data length in bytes
    const void *data;                       // pointer to the message data
#ifdef RMT_IPC_SEND_DATA_RELAY
    const void *org_data;                   // pointer to the original message data
#endif
};

struct HAL_RMT_IPC_SEND_RECORD_T {
    struct HAL_RMT_IPC_MSG_T msg;
    bool in_use;
};

struct HAL_RMT_IPC_CH_CFG_T {
    const struct HAL_RMT_IPC_CFG_T * cfg;
    struct HAL_RMT_IPC_MSG_T * send_msg_list_p;
    struct HAL_RMT_IPC_MSG_T * send_pending_list_p;
    struct HAL_RMT_IPC_SEND_RECORD_T * send_record;

    const struct HAL_RMT_IPC_MSG_T ** recv_msg_list_pp;
    struct HAL_RMT_IPC_MSG_T recv_pending_head;

    HAL_RMT_IPC_RX_IRQ_HANDLER rx_irq_handler;
    HAL_RMT_IPC_TX_IRQ_HANDLER tx_irq_handler;
#ifdef RMT_IPC_SEND_CALLBACK
    HAL_RMT_IPC_SEND_CALLBACK send_cb;
#endif

    uint16_t send_rec_num;
    uint8_t wake_lock_idx;
    uint8_t chan;

    bool chan_manual_rx_done;
    bool chan_in_rx_hdlr;
    bool chan_rx_busy;
    bool chan_opened;
};

#ifdef OPENAMP_ENABLE
extern uint32_t __resource_table_start__[];
extern uint32_t __resource_table_end__[];
#endif

struct HAL_RMT_IPC_CFG_T {
    const char * name;
    const struct HAL_RMT_IPC_CFG_T ** peer_cfg_pp;
    const struct HAL_RMT_IPC_CFG_T ** local_cfg_pp;

    HAL_RMT_IPC_IRQ_INIT irq_init;
    HAL_RMT_IPC_PEER_TX_IRQ_SET peer_tx_irq_set;
    HAL_RMT_IPC_LOCAL_TX_IRQ_CLEAR local_tx_irq_clear;
    HAL_RMT_IPC_RX_IRQ_SUSPEND rx_irq_suspend;
    HAL_RMT_IPC_RX_IRQ_RESUME rx_irq_resume;
    HAL_RMT_IPC_RX_DONE rx_done;
    HAL_RMT_IPC_IRQ_ACTIVE irq_active;
    HAL_RMT_IPC_IRQ_ENTRY rx_irq_entry;
    HAL_RMT_IPC_IRQ_ENTRY tx_irq_entry;
    HAL_RMT_IPC_PM_NOTIF_HANDLER pm_notif_handler;
#ifdef RMT_IPC_SEND_DATA_RELAY
    HAL_RMT_IPC_SEND_DATA_RELAY send_data_relay;
#endif

    uint8_t chan_num;
    uint16_t rec_num_per_chan;
    struct HAL_RMT_IPC_CH_CFG_T * chan_cfg;
    struct HAL_RMT_IPC_SEND_RECORD_T * send_record;
    const IRQn_Type * rx_irq_id;
    const IRQn_Type * tx_irq_id;
    bool wake_lock_en;
#ifdef OPENAMP_ENABLE
    uint32_t openamp_rsc_start;
    uint32_t openamp_rsc_end;
#endif
};

const struct HAL_RMT_IPC_CFG_T *hal_rmt_ipc_get_cfg(enum HAL_RMT_IPC_CORE_T core);

void hal_rmt_ipc_rx_irq_handler(const struct HAL_RMT_IPC_CFG_T *cfg);

void hal_rmt_ipc_tx_irq_handler(const struct HAL_RMT_IPC_CFG_T *cfg);

int hal_rmt_ipc_pm_notif_handler(const struct HAL_RMT_IPC_CFG_T *cfg, enum HAL_PM_STATE_T state);

#ifdef __cplusplus
}
#endif

#endif

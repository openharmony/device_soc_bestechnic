/***************************************************************************
 *
 * Copyright 2015-2022 BES.
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
#ifndef __BT_A2DP_SERVICE_H__
#define __BT_A2DP_SERVICE_H__
#include "adapter_service.h"
#include "a2dp_common_define.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * standard a2dp (sink role) interface
 *
 */

typedef struct {
    uint8_t error_code;
    uint8_t codec_type;
    uint8_t codec_info_len;
    uint8_t *codec_info;
    uint8_t *cp_info;
    uint8_t cp_info_len;
    uint8_t cp_type;
} bt_a2dp_opened_param_t;

typedef struct {
    uint8_t error_code;
} bt_a2dp_closed_param_t;

typedef struct {
    uint8_t error_code;
    uint8_t codec_type;
    uint8_t codec_info_len;
    uint8_t *codec_info;
    uint8_t *cp_info;
    uint8_t cp_info_len;
    uint16_t cp_type;
} bt_a2dp_stream_start_param_t;

typedef struct {
    uint8_t error_code;
    uint8_t codec_type;
    uint8_t codec_info_len;
    uint8_t *codec_info;
    uint8_t *cp_info;
    uint8_t cp_info_len;
    uint16_t cp_type;
} bt_a2dp_stream_reconfig_param_t;

typedef struct {
    uint8_t error_code;
} bt_a2dp_stream_suspend_param_t;

typedef struct {
    uint8_t error_code;
} bt_a2dp_stream_close_param_t;

typedef union {
    bt_a2dp_opened_param_t *opened;
    bt_a2dp_closed_param_t *closed;
    bt_a2dp_stream_start_param_t *stream_start;
    bt_a2dp_stream_reconfig_param_t *stream_reconfig;
    bt_a2dp_stream_suspend_param_t *stream_suspend;
    bt_a2dp_stream_close_param_t *stream_close;
} bt_a2dp_callback_param_t;

typedef enum {
    BT_A2DP_EVENT_OPENED = BT_EVENT_A2DP_OPENED,
    BT_A2DP_EVENT_CLOSED,
    BT_A2DP_EVENT_STREAM_START,
    BT_A2DP_EVENT_STREAM_RECONFIG,
    BT_A2DP_EVENT_STREAM_SUSPEND,
    BT_A2DP_EVENT_STREAM_CLOSE,
    BT_A2DP_EVENT_END,
} bt_a2dp_event_t;

#if BT_A2DP_EVENT_END != BT_EVENT_A2DP_END
#error "bt_a2dp_event_t error define"
#endif

typedef int (*bt_a2dp_callback_t)(const bt_bdaddr_t *bd_addr, bt_a2dp_event_t event, bt_a2dp_callback_param_t param);

bt_status_t bt_a2dp_init(bt_a2dp_callback_t callback);
bt_status_t bt_a2dp_cleanup(void);
bt_status_t bt_a2dp_connect(const bt_bdaddr_t *bd_addr);
bt_status_t bt_a2dp_disconnect(const bt_bdaddr_t *bd_addr);

/**
 * standard a2dp (source role) interface
 *
 */

#ifdef BT_SOURCE

typedef enum {
    BT_A2DP_SOURCE_EVENT_OPENED = BT_EVENT_A2DP_SOURCE_OPENED,
    BT_A2DP_SOURCE_EVENT_CLOSED,
    BT_A2DP_SOURCE_EVENT_STREAM_START,
    BT_A2DP_SOURCE_EVENT_STREAM_RECONFIG,
    BT_A2DP_SOURCE_EVENT_STREAM_SUSPEND,
    BT_A2DP_SOURCE_EVENT_STREAM_CLOSE,
    BT_A2DP_SOURCE_EVENT_END,
} bt_a2dp_source_event_t;

#if BT_A2DP_SOURCE_EVENT_END != BT_EVENT_A2DP_SOURCE_END
#error "bt_a2dp_source_callback_t error define"
#endif

typedef int (*bt_a2dp_source_callback_t)(const bt_bdaddr_t *bd_addr, bt_a2dp_source_event_t event, bt_a2dp_callback_param_t param);

bt_status_t bt_a2dp_source_init(bt_a2dp_source_callback_t callback);
bt_status_t bt_a2dp_source_cleanup(void);
bt_status_t bt_a2dp_source_connect(const bt_bdaddr_t *bd_addr);
bt_status_t bt_a2dp_source_disconnect(const bt_bdaddr_t *bd_addr);
bt_status_t bt_a2dp_source_start_stream(const bt_bdaddr_t *bd_addr);
bt_status_t bt_a2dp_source_suspend_stream(const bt_bdaddr_t *bd_addr);

#endif /* BT_SOURCE */
#ifdef __cplusplus
}
#endif
#endif /* __BT_A2DP_SERVICE_H__ */


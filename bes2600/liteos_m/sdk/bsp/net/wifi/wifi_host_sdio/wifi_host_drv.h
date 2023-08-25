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

#include <string.h>
#include <stdio.h>
#include "bwifi_event.h"
#include "hal_sdio_host.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Different message type has different bit masks;
 * bit15 indicates its type.
 */
#define MSG_KIND_BIT              (1 << 15) /* 1 for MGMT and 0 for DATA */

/*When bit15 is 0*/
#define MSG_OFFSET_IN_REGADDR_DATA (12)
/* support 4 different messages at most */
#define MSG_MASK_IN_REGADDR_DATA   (0x3000)
#define OFT_OFFSET_IN_REGADDR_DATA (14)
#define OFT_MASK_IN_REGADDR_DATA   (0x4000)
#define LEN_OFFSET_IN_REGADDR_DATA (0)
#define LEN_MASK_IN_REGADDR_DATA   (0xFFF)

/*When bit15 is 1*/
#define MSG_OFFSET_IN_REGADDR_MGMT  (8)
/* support 128 different messages at most */
#define MSG_MASK_IN_REGADDR_MGMT    (0x7F00)
#define LEN_OFFSET_IN_REGADDR_MGMT  (0)
#define LEN_MASK_IN_REGADDR_MGMT    (0xFF)


/* The last SDIO_LOCAL_RETU_LEN bytes is assumed to hold the
 * return value of the remote function's.
 */
#define RSP_PADDING_LEN         (64)
#define SDIO_LOCAL_RETU_LEN     (4)
#define SDIO_LOCAL_RESP_LEN     (4)

#define MAX_RSP_BUF_LEN         (4096+4096)
#define HOST_MGMT_CNT           (2)
#define HOST_MGMT_SIZE          (MAX_RSP_BUF_LEN)
#define HOST_DATA_CNT           (4)
#define HOST_DATA_SIZE          (4096)
#define RSP_PADDING_LEN         (64)
#define IF_RECV_TMO             (10*1000)
#define IF_SEND_TMO             (30*1000)
#define IF_CHECK_INTERVAL       (1*1000)

#define SIGNAL_BUS_RX_INT       (1 << 0)

#ifdef __SNIFFER_MODE__
#define SNIFFER_SINGAL_OFFSET   10
#endif

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(char *)(&((type *)0)->member)))

enum wifi_host_bus_sm {
    HOST_BUS_IDLE,
    HOST_BUS_INIT,
    HOST_BUS_ERROR,
    HOST_BUS_RUNNING,
    HOST_BUS_TESTMODE,
};

enum if_sdio_msg_type {
    /* transceive application data */
    IF_SDIO_DATA_XMIT_DEV0 = 0,
    IF_SDIO_DATA_XMIT_DEV1,
    IF_SDIO_DATA_RECV_DEV0,
    IF_SDIO_DATA_RECV_DEV1,
    /* last DATA msg */
    IF_SDIO_DATA_LAST,

    /* wifi hal functions to be called */
    IF_SDIO_MGMT_FIRST = 9,
    IF_SDIO_MGMT_DEVICE_READY, //10
    IF_SDIO_MGMT_ADD_NETWORK,
    IF_SDIO_MGMT_MODIFY_NETWORK,
    IF_SDIO_MGMT_CNT_NETWORK,
    IF_SDIO_MGMT_GET_NETWORK,
    IF_SDIO_MGMT_DEL_NETWORK,
    IF_SDIO_MGMT_EN_NETWORK,
    IF_SDIO_MGMT_DIS_NETWORK,
    IF_SDIO_MGMT_SCAN,
    IF_SDIO_MGMT_CFG_SCAN,
    IF_SDIO_MGMT_GET_SCAN_RES, //20
    IF_SDIO_MGMT_FLUSH_SCAN_RES,
    IF_SDIO_MGMT_CONN,
    IF_SDIO_MGMT_WPS_WILD_OK,
    IF_SDIO_MGMT_WPS_PBC,
    IF_SDIO_MGMT_DISC,
    IF_SDIO_MGMT_GET_SSID,
    IF_SDIO_MGMT_GET_BSSID,
    IF_SDIO_MGMT_GET_MAC,
    IF_SDIO_MGMT_GET_CURR_RATE,
    IF_SDIO_MGMT_GET_LINKINFO,//30
    IF_SDIO_MGMT_GET_RSSI,
    IF_SDIO_MGMT_IFA_ADDR,
    IF_SDIO_MGMT_WORK_CHANNEL,
    IF_SDIO_MGMT_STAT_EN,
    IF_SDIO_MGMT_STAT_GET,
    IF_SDIO_MGMT_HAL_INIT,
    IF_SDIO_MGMT_PS,
    IF_SDIO_MGMT_AP_UP,
    IF_SDIO_MGMT_AP_DOWN,
    IF_SDIO_MGMT_AP_CFG,//40
    IF_SDIO_MGMT_AP_SET_STA_NUM,
    IF_SDIO_MGMT_AP_SET_COUNTRY,
    IF_SDIO_MGMT_AP_ADD_VENDOR_IE,
    IF_SDIO_MGMT_AP_GET_STA_INFO,
    IF_SDIO_MGMT_SW_CH,
    IF_SDIO_MGMT_SET_COUNTRY,
    IF_SDIO_MGMT_GET_COUNTRY,
    IF_SDIO_MGMT_SEND_MLME,
    IF_SDIO_MGMT_SET_FIX_RATE,
    IF_SDIO_MGMT_EPTA_PARAM,//50
    IF_SDIO_MGMT_EPTA_STATE,
    IF_SDIO_MGMT_WIFI_RESET,
    IF_SDIO_MGMT_STR_CMD, //53
    IF_SDIO_MGMT_DAT_CMD, //54
    IF_SDIO_MGMT_IP_ALIVE, //55

    IF_SDIO_MGMT_EVENT_UPLOAD,
    IF_SDIO_MGMT_RECV_SNIFFER,
    IF_SDIO_MGMT_DEV_REBOOT,
    /* last management msg */
    IF_SDIO_MGMT_LAST, //58

    /* last message */
    IF_SDIO_MSG_LAST,
};

enum wifi_host_buf_type {
    BUF_TYPE_MGMT,
    BUF_TYPE_DATA,
    BUF_TYPE_MAX,
};

/**
 * struct wifi_bus_drv_ops - Driver interface API definition
 */
struct wifi_bus_drv_ops {
    /** Name of the driver interface */
    const char *name;
    /** One line description of the driver interface */
    const char *desc;
    int (*open)(struct HAL_SDIO_HOST_CB_T *callback);
    int (*read_reg)(uint32_t reg, uint8_t *rsp);
    int (*write_reg)(uint32_t reg, uint8_t val);
    int (*read)(uint8_t *data_buff, uint32_t data_size);
    int (*write)(uint32_t addr, uint8_t *data_buff, uint32_t data_size);
    int (*close)(void);
    int (*reopen)(void);
    int (*get_rxbuf)(uint16_t *tx_size);
#ifdef SDIO_SLAVE_PERON_IO
    /**  sdio host control the deivce power on */
    int (*power_en)(void);
    int (*redownload)(void);
#endif
};

extern struct wifi_bus_drv_ops wbops;
#define WIFI_HOST_DRV_API_CALL(func,...)    (wbops.func ? (int)wbops.func(__VA_ARGS__) : -1)

int wifi_host_send_data(uint8_t devnum, uint8_t *buf, uint32_t len);
int wifi_host_send_mgmt(enum if_sdio_msg_type msg,
                              void *req,
                              int req_len,
                              void *rsp,
                              int *rsp_len,
                              void *retu,
                              int *retu_len);
int wifi_host_recv_data_hook(eth_input_handler cb);
int wifi_host_recv_evt_hook(evt_recv_handler cb);
#ifdef __SNIFFER_MODE__
int wifi_host_recv_sniffer_hook(sniffer_input_handler cb);
#endif
int wifi_host_main(void);
void wifi_sdio_firmware_download(void);

#ifdef __cplusplus
}
#endif


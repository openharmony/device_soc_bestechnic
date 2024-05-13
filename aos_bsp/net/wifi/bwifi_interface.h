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

#ifndef BWIFI_INTERFACE_H
#define BWIFI_INTERFACE_H

#include "plat_types.h"
#ifdef RTOS
#include "cmsis_os.h"
#endif
#include "wifi_def.h"
#include "bwifi_hal.h"
#include "bwifi_event.h"
#if LWIP_ETHERNETIF
#include "lwip/netif.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BWIFI_STATUS_IDLE            = 0,
    BWIFI_STATUS_DISCONNECTING   = 1,
    BWIFI_STATUS_SCANNING        = 2,
    BWIFI_STATUS_CONNECTING      = 3,
    BWIFI_STATUS_WPS_CONNECTING  = 4,
    BWIFI_STATUS_CONNECTED       = 5,
    BWIFI_STATUS_DHCPING         = 6,
    BWIFI_STATUS_GOT_IP          = 7,
    BWIFI_STATUS_ONLINE_SCANNING = 8,
    BWIFI_STATUS_END_DEF,  /* must be the last one */
} BWIFI_STATUS_T;

enum {
    BWIFI_R_OK                  = 0,
    BWIFI_R_COMMON_FAIL         = -1,
    BWIFI_R_INVALID_ARG         = -2,  //invalid argument
    BWIFI_R_INVALID_PASSWORD    = -3,  //invalid password
    BWIFI_R_MEMORY_ERROR        = -4,  //no memory to allocate resource
    BWIFI_R_INIT_FAIL           = -5,  //init wifi fail
    BWIFI_R_NOT_INITED          = -6,  //wifi is not initialized
    BWIFI_R_STATUS_ERROR        = -7,  //request in error STATUS
    BWIFI_R_SCAN_REQ_FAIL       = -8,  //scan fail to start
    BWIFI_R_SCAN_NO_AP_FOUND    = -9,  //scan result is NULL (didn't find any SSID)
    BWIFI_R_NO_SUITABLE_NETWORK = -10, //no suitable network to connect
    BWIFI_R_CONN_REQ_FAIL       = -11, //connect fail to start
    BWIFI_R_CONN_FAIL           = -12, //connect procedure result in fail
    BWIFI_R_CONN_NO_SSID_CONFIG = -13, //no saved SSID config to connect
    BWIFI_R_DISC_FAIL           = -14, //disconnect procedure result in fail
    BWIFI_R_WPS_NOT_FOUND       = -15, //couldn't find WPS AP
    BWIFI_R_WPS_REQ_FAIL        = -16, //WPS fail to start
};

enum {
    WIFI_MFP_DISABLE = 0,   /**< disable mfp capability */
    WIFI_MFP_ENABLE,        /**< have mfp capability, but not require peer to support mfp */
    WIFI_MFP_REQUIRED,      /**< have mfp capability, and require peer to support mfp */
    WIFI_MFP_DEFAULT,       /**< mfp is not specified, setting it based on our capability and network type */
};

typedef enum bwifi_softap_status {
    BWIFI_SOFTAP_STATUS_OFF   = 0,
    BWIFI_SOFTAP_STATUS_ON,
} BWIFI_SOFTAP_STATUS_T;

typedef enum {
    WIFI_USER_EVT_CONN_INTER_STATE,
    WIFI_USER_EVT_CONNECTED,
    WIFI_USER_EVT_GOT_IP,
    WIFI_USER_EVT_DISCONNECTED,
    WIFI_USER_EVT_ROAM_COMPLETE,
    WIFI_USER_EVT_AP_ENABLED,
    WIFI_USER_EVT_AP_DISABLED,
    WIFI_USER_EVT_AP_STA_CONNECTED,
    WIFI_USER_EVT_AP_STA_DISCONNECTED,
    WIFI_USER_EVT_FATAL_ERROR,
    /* mutually exclusive events */
    /* If user layer wants to take over the initiative of wifi reset,
     * register this event for the timing to trigger reset.
     */
    WIFI_USER_EVT_MAC_RESET_REQUEST,
    /* If user layer is only interested in the process of wifi reset,
     * register these events to get notify when reset start/end.
     */
    WIFI_USER_EVT_MAC_RESET_START,
    WIFI_USER_EVT_MAC_RESET_DONE,
    WIFI_USER_EVT_COEX_MODE_CHANGE,
    WIFI_USER_EVT_LOW_POWER_CHANGE,
    WIFI_USER_EVT_P2PMODE_EVENT,
    WIFI_USER_EVT_MAX
} WIFI_USER_EVT_ID;

typedef enum {
    FATAL_ERROR  = 1,
    UMAC_CRASH   = 2,
    USER_REQUEST = 3
} BWIFI_MAC_RESET_REASON;

struct bwifi_quick_connect_config {
    struct bwifi_network_config config;
    uint8_t channel;
    uint32_t ip[3];//struct ip_info ip;
};

struct bwifi_low_power_config {
    uint8_t arp_offload;    /* 0 - disable, 1 - enable, 0xFF - unset */
    uint8_t bcast_filter;   /* 0 - disable, 1 - enable, 0xFF - unset */
};

typedef void (*user_evt_handler_t)(WIFI_USER_EVT_ID evt_id, void *arg);

void bwifi_reg_user_evt_handler(WIFI_USER_EVT_ID evt_id, user_evt_handler_t cb);
void bwifi_unreg_user_evt_handler(WIFI_USER_EVT_ID evt_id);
void bwifi_reg_eth_input_handler(eth_input_handler cb);
void bwifi_unreg_eth_input_handler();
#ifdef __SNIFFER_MODE__
void bwifi_reg_sniffer_recv_handler(sniffer_recv_handler cb);
void bwifi_unreg_sniffer_recv_handler();
int bwifi_sniffer_cmd(int cmd, void *param);
#endif
#if WIFI_NET_TOOL_SUPPORT
/*
 * Add network test tool tcpdump element to wpa_supplicant
 */
typedef void (*bwifi_tcpdump_callback)(unsigned char *buf, int len);
void bwifi_reg_tcpdump_handler(bwifi_tcpdump_callback cb);
void bwifi_unreg_tcpdump_handler(void);
/*
 * Add network test tool wifi status query pta element to wpa_supplicant
 * return the pta of current wifi status.
 */
float bwifi_get_epta_ratio(void);
#endif
/*
 * Add network config to wifi stack
 * return the allocated network id, or negative on failure.
 */
int bwifi_add_network(struct bwifi_network_config *config);
/*
 * Modify network config item
 * return the result of current operation
 */
int bwifi_modify_network(struct bwifi_config_item *item);
/*
 * Get the number of saved network configs from wifi stack.
 */
int bwifi_count_configured_networks(void);
/*
 * Get network configs saved in wifi stack
 * return the number of networks
 */
int bwifi_get_configured_networks(struct bwifi_network_info *network, int count);
/*
 * Delete network config saved in wifi stack
 * It will check ssid, passwd, hidden, web_keyid, bssid (if not zero),
 * if config is NULL or config->ssid is zero length, delete all.
 */
int bwifi_del_network(struct bwifi_network_config *config);

#if 0
/*
 * Save network config to flash (max 8)
 */
int bwifi_save_config(struct bwifi_network_config *config);
/*
 * Get network configs saved in flash
 * return the number of networks
 */
int bwifi_get_saved_config(struct bwifi_network_config *config, int count);
/*
 * Find network config saved in flash who matches with the specified ssid
 */
int bwifi_find_saved_config_by_ssid(const char *ssid, struct bwifi_network_config *config);
/*
 * Delete network config saved in flash
 * It will check ssid, passwd, hidden, web_keyid, bssid (if not zero),
 * if config is NULL or config->ssid is zero length, delete all.
 */
int bwifi_del_config(struct bwifi_network_config *config);
/*
 * wifi record interface
 */
int bwifi_find_record(const char *type, void *record_info);
int bwifi_add_record(void *record_info);
int bwifi_del_record(char *type);
#endif

/*
 * Scan for wildcard ssid and saved hidden ssids
 * you can get bss list from bwifi_get_scan_result
 * return bss number or error code
 */
int bwifi_scan(void);
/*
 * Scan for specified ssid (if not NULL) on the specified channels (if no 0)
 * you can get bss list from bwifi_get_scan_result
 * return bss number or error code
 */
int bwifi_config_scan(struct bwifi_scan_config *scan_config);
/*
 * Get scan result
 */
int bwifi_get_scan_result(struct bwifi_bss_info *result, int count);
/*
 * Clear saved scan list which is not in use
 */
void bwifi_flush_scan_result(void);
/*
 * Auto connect to an AP saved in wpa_supplicant
 */
int bwifi_auto_connect(void);
/*
 * Connect to an AP assigned by user config
 *
 * This function will add the network config to wpa_supplicant and trigger new connection.
 * If user wants to connect to another AP after connected:
 *   1. bwifi_disconnect()
 *   2. bwifi_connect(config)
 */
int bwifi_connect(struct bwifi_network_config *config);
/*
 * Connect to an AP via WPS PBC
 */
int bwifi_connect_wps_pbc(void);
/*
 * Connect to specified ssid, passwd can be NULL for unencrypted AP.
 */
int bwifi_connect_to_ssid(const char *ssid, const char *passwd, int8_t wep_keyid, uint8_t hidden, uint8_t *bssid);
/*
 * Connect to specified ssid and mfp, passwd can't be NULL
 */
int bwifi_connect_to_ssid_with_mfp(const char *ssid, const char *passwd, int8_t wep_keyid, uint8_t hidden, uint8_t *bssid, uint8_t mfp);
/*
 * Connect to specified bssid, passwd can be NULL for unencrypted AP.
 */
int bwifi_connect_to_bssid(uint8_t *bssid, const char *passwd);
/*
 * Connect to specified network by network id allocated in wpa_supplicant.
 */
int bwifi_connect_to_nid(int nid);
/*
 * Disconnect to current connected AP or stop connecting to AP
 */
int bwifi_disconnect(void);

/*
 * Quick connect functions
 */
int bwifi_get_quick_connect_config(struct bwifi_quick_connect_config *quick_config);
int bwifi_set_quick_connect_config(struct bwifi_quick_connect_config *quick_config);
int bwifi_del_quick_connect_config(void);
int bwifi_quick_connect(struct bwifi_quick_connect_config *quick_config);

/*
 * Set timeout value for wifi connection progress
 * @timeout: msec for timeout value
 */
void bwifi_set_connect_timeout(uint32_t timeout);
/*
 * Set the retry count for auto reconnection after disconnection
 */
void bwifi_set_reconnect_retries(uint8_t retry_cnt);
/*
 * Get current retry count of auto reconnection
 */
uint8_t bwifi_get_reconnect_retries(void);
/*
 * Get current connected AP info
 */
int  bwifi_get_current_ssid(char *ssid);
int  bwifi_get_current_bssid(uint8_t *bssid);
int  bwifi_get_dev_mac(BWIFI_INTF_TYPE_T type, uint8_t *addr);
uint8_t   bwifi_get_current_channel(void);
int8_t bwifi_get_current_rssi(void);
int  bwifi_get_current_rate(void);

/**
 * Get station linkinfo statistics which can be used to evaluate network traffic.
 * @info: pointer to a bwifi_station_linkinfo struct to linkinfo statistics
 * Returns: void
 */
void bwifi_get_station_linkinfo(bwifi_station_linkinfo *info);

#if !LWIP_SUPPORT
/*
 * Set netif ip addr to wifi mac layer for ARP filter feature
 *
 * This function should only be called when SDK inside LWIP is turned off
 * and DHCP procedure is also taken over by user's LWIP stack,
 * we need user to tell us the assigned local ip addr.
 */
int bwifi_set_ip_addr(BWIFI_INTF_TYPE_T type, struct ip_info *ip);
#endif

/*
 * Set powersave mode for legacy Wi-Fi.
 * @ps: 0 = disable, 1 = enable
 * Returns: 0 on success or negtive on failure
 */
int bwifi_set_ps_mode(uint8_t ps);

/**
 * Change current wifi status
 *
 * @new_status: The new wifi status
 * Returns: void
 */
void bwifi_change_current_status(BWIFI_STATUS_T new_status);

/**
 * Test whether wifi module is initialized
 *
 * Returns: true or false
 */
bool bwifi_init_done(void);

/**
 * Enable or disable the statistics of the frames sent out and received
 * in the last interval_sec time.
 * @en: 0 = disable, 1 = enable
 * @interval_sec: time of the statistics in seconds
 * Returns: void
 *
 * @note    If enabled, the statistics information will be output via uart log.
 */
void bwifi_trans_stat_en(uint8_t en, uint8_t interval_sec);
/**
 * Get trans statistics which can be used to evaluate network traffic.
 * @stat: pointer to a bwifi_trans_stat struct to retrieve trans statistics
 * @clear: 1 = clear, 0 = keep
 * Returns: void
 *
 * @note    Elements in bwifi_trans_stat will be cleared every time this function
 *          is called if clear is set to 1.
 */
void bwifi_trans_stat_get(bwifi_trans_stat *stat, int8_t clear);

/*
 * Initialize wifi hardware and interface
 */
int bwifi_init(void);

/*
 * Deinitialize wifi hardware and interface
 */
int bwifi_deinit(void);

#ifdef WIFI_ON_CP
/*
 * reset wifi stack on cp
 */
int bwifi_reinit(void);
#endif

/*
 * Set country code
 * @country_code: Two octets string to represent a country code
 * If not set, we use the default country code CN.
 */
int bwifi_set_country_code(char *country);
/*
 * Get current country code
 */
int bwifi_get_country_code(char *country);

/**
 * set_probe_req_filter - set driver to recive probe request frame or not
 * @enable: filter switch, 0 - close filter to rx probe request frame, other - opposite
 * Returns: 0 - success, other - failure
 */
int bwifi_set_probe_req_filter(uint8_t enable);

/*
 * Set wpa log level
 */
int bwifi_set_wpa_log_level(char *level);

/*
 * Switch channel for specific interface mode
 * @type: Type of interface mode
 * @prim20_channel: primary channel
 * @bandwidth: bandwidth
 * @csa_count: Number of Beacons including CSA element until channel switch
 * Returns: 0 on success or negtive on failure
 */
int bwifi_switch_channel(BWIFI_INTF_TYPE_T type, uint8_t prim20_channel, uint8_t bandwidth, uint8_t csa_count);

#ifdef __AP_MODE__
/*
 * Start softap with previous configuration
 */
int bwifi_softap_start(void);

/*
 * softap mode disconnect a station
 */
int bwifi_softap_disconnect_sta(uint8_t *ucMac);

/*
 * Stop softap
 */
void bwifi_softap_stop(void);

/**
 * Set softap basic configuration
 *
 * @ssid: AP's SSID
 * @channel: Primary channel for AP to start on
 * @bandwidth:
 *     0 = 20M;
 *     1 = 40M;
 *     2 = 80M
 * @hidden: Whether to ignore broadcast SSID
 *     0 = broadcast ssid;
 *     1 = hide ssid with zero length;
 *     2 = hide ssid with zero contents
 * @security: Enum of security type
 * @password: User passphrase
 *
 * Returns: 0 on success, nagtive if invalid arguments
 */
int bwifi_set_softap_config(char *ssid,
                            uint8_t channel, uint8_t bandwidth,
                            uint8_t hidden,
                            BWIFI_SEC_TYPE_T security,
                            char *passwd);

/*
 * Set softap's maximum client number
 * @sta_num: Maximum number of STAs
 * Returns: void
 */
void bwifi_set_softap_max_sta_num(int sta_num);
/*
 * Add vendor elements in softap's Beacon/ProbeRsp
 * @ie: Pointer to raw data of vendor elements
 * @ie_len: Length of vendor elements
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_add_softap_vendor_elements(const uint8_t *ie, size_t ie_len);
/*
 * Get softap's client list
 * @mac_list: Pointer to buffer to store the mac address list
 * @count: STA count to retrieve, returns the actual connected STA count
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_softap_get_station_list(struct bwifi_mac_addr *mac_list, int *count);
/*
 * bwifi_softap_get_sta_linkinfo - get linkinfo of the ap's affiliated sta
 * @info: see struct bwifi_station_linkinfo
 * Returns: 0 on success, nagtive on failure
*/
int bwifi_softap_get_sta_linkinfo(bwifi_station_linkinfo *info);
#endif

/*
 * Get current Wi-Fi status
 */
BWIFI_STATUS_T bwifi_get_current_status(void);
/*
 * Get current softap status
 */
BWIFI_SOFTAP_STATUS_T bwifi_get_softap_status(void);

/*
 * This function is used for userspace to send an encapsulated mgmt frame.
 * @type:  Type of interface to send the frame
 * @data:  Pointer to the frame data
 * @len:   Length of the frame data
 * @noack: Whether need to wait for ack
 * @chan:  Channel on which to send, or 0 for not specified (interface working channel)
 */
int bwifi_send_mgmt_frame(BWIFI_INTF_TYPE_T type, const uint8_t *data, size_t len, int noack, uint8_t chan);

/*
 * Enable or disable Wi-Fi recovery mechanism on fatal error.
 * which is turned off by default for debug purpose.
 */
void bwifi_recovery_enable(bool en);

/**
 * Set fix data rate for Wi-Fi transmission
 * @type: Type of interface mode
 * @fix_rate_idx
 *   - 0 ~ 3:   DSSS/CCK rates: 1, 2, 5.5, 11;
 *   - 4 ~ 5:   reserved;
 *   - 6 ~ 13:  legacy OFDM rates: 6, 9, 12, 18, 24, 36, 48, 54;
 *   - 14 ~ 21: 1*1 MCS rates: MCS0 ~ MCS7;
 *   - 0xff:    default value to disable the fix rate function.
 *   - value >= WIFI_FIX_RATE_MIN_VALUE (0x1<<16) is mask as a fix rate table.
 *           bit[0-7] rate idx 0
 *               bit[0-3] is MCS, bit[4-5] is BW, bit[6] is S-GI, bit[7] is HE DCM
 *           bit[8-15] rate idx 1
 *               bit[8-11] is MCS, bit[12-13] is BW, bit[14] is S-GI, bit[15] is HE DCM
 *           bit[16-23] rate idx 2
 *               bit[16-19] is MCS, bit[20-21] is BW, bit[22] is S-GI, bit[23] is HE DCM
 *           bit[24-31] rate idx 3
 *               bit[24-27] is MCS, bit[28-29] is BW, bit[30] is S-GI, bit[31] is HE DCM
 */
void bwifi_set_fix_rate(BWIFI_INTF_TYPE_T type, uint32_t fix_rate_idx);

/**
 * Send TWT setup request to AP
 *
 * @mac_addr: MAC address of the peer STA
 * @setup_type: Setup request type (0 - request, 1 - suggest, 2 - demand)
 * @config: TWT config parameters
 *
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_twt_setup(const uint8_t *addr, uint8_t setup_type,
                    struct bwifi_twt_config *config);

/**
 * Send TWT teardown request
 * @mac_addr: MAC address of the peer STA
 * @flow_id: TWT flow id
 *
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_twt_teardown(const uint8_t *addr, uint8_t flow_id);

#ifdef ENABLE_FW_KEEP_ALIVE
/*
 * Set ip keep alive feature's parameters.
 * @paras: ip alive parameters
 * Returns: The idx of tcp stream when adding one or the delete result when deleting one
 */
int bwifi_set_ip_alive(struct ip_alive_param *paras);

/*
 * Enable/disable ip keep alive function.
 * @period: interval to send tcp/udp heartbeat in seconds, set to 0 will stop sending out any
 *
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_enable_ip_alive(uint16_t period);
#endif

#ifdef __SET_MULTICAST_FILTER__
/*
 * Set a multicast address filter.
 * @mode: 0 - disable filter
 *        1 - enable filter
 *        2 - clean multicast address table
 *        3 - filter all multicast frames
 *        4 - add filter
 *        5 - del filter
 * @addr: mac address to filter
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_set_multicast_filter(uint8_t mode, uint8_t *addr);
#endif

#ifdef __SET_BROADCAST_FILTER__
/*
 * Enable/disable broadcast filter.
 * @enable: 0 - disable, 1 - enable
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_set_broadcast_filter(uint8_t enable);
#endif

#ifdef __SET_ARP_OFFLOAD__
/*
 * Enable/disable arp offload.
 * @enable: 0 - disable, 1 - enable
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_set_arp_offload(uint8_t enable);
#endif

/*
 * Do wifi reset
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_reset(void);

/*
 * wifi power on/off
 * onoff: 1 - wifi power on, 0 - wifi power off
 * Returns: 0 on success, nagtive on failure
 */
int bwifi_power_on(uint8_t onoff);

/*
 * set epta parameters of wifi/bt coex
 * wifi_dur: wifi duration of active window
 * bt_dur: bt duration of active window
 * mode: epta mode
        0 - periodic mode in which the wifi/bt active time is specified by wifi_dur/bt_dur, only support 100ms period
        1 - arbitrate mode by hw itself
        2 - wifi active only mode
*/
void bwifi_set_epta_param(int wifi_dur, int bt_dur, int mode);

/*
0 - bt not active, allow to alloc max duration for wlan
1 - bt active, allow to alloc medium duration for wlan
2 - bt a2dp active
*/
void bwifi_bt_state_notify(int state);

/**
 * bwifi_coex_test_cmd - execute the coex command
 * @value: pointer to the array of command arguments
 * @len: number of the arguments
 * Returns: BWIFI_R_OK on success, others on failure
 */
int bwifi_coex_test_cmd(uint32_t *value, uint8_t len);

/*
 * get free buf to send
 * Returns: address of buf pointer
 */
static inline uint8_t ** bwifi_get_data_buf(void)
{
    return (uint8_t **)BWIFI_HAL_API_CALL(get_send_buf);
}

/*
 * send data buf
 * @devnum: dev interface number
 * @tx_buf: address of send buffer pointer
 * @tx_len: send data length
 * Returns: 0 on success, nagtive on failure
 */
static inline int bwifi_send_data_buf(uint8_t devnum, uint8_t **tx_buf, uint16_t tx_len)
{
    return BWIFI_HAL_API_CALL(data_send, devnum, tx_buf, tx_len);
}

/*
 * bwifi_str_cmd - send string command
 * @type: command type, 0 - rf test, other - reserved
 * @cmd_buf: command string end with '\0'
 * @rsp_buf: pointer to the buffer that contains the response string
 * @rsp_size: size of the buffer
 * Returns: 0 - success, other - failure
 */
int bwifi_str_cmd(uint8_t type, char *cmd_buf, char *rsp_buf, size_t rsp_size);

/**
 * bwifi_dat_cmd - send data command to wifi subsystem
 * @type: command type @ref enum bwifi_dat_cmd_type
 * @tx_buf: pointer of the data buffer to be sent.
 * @tx_len: length of the data to be sent.
 * Returns: 0 - success, other - failure
 */
int bwifi_dat_cmd(uint8_t type, uint8_t *tx_buf, size_t tx_len);

/*
 * bwifi_check_capability - Check whether the macros in struct bwifi_hal_ops are aligned
 * Instruction: First, at the begining of bwifi_init(), the state of macros in bwifi_hal_ops
 * are saved with varible g_wifi_hal_cap; Then we can check the state of macros in bwifi_hal_ops
 * after bwifi_init() through call this function bwifi_check_capability().
 * Returns: 0 - success, other - failure
 */
extern uint32_t g_wifi_hal_cap;
static inline int bwifi_check_capability(uint32_t *localCap)
{
    uint32_t local_cap = 0;

#ifdef __AP_MODE__
    local_cap |= (1 << 0);
#endif

#ifdef ENABLE_FW_KEEP_ALIVE
    local_cap |= (1 << 1);
#endif

#ifdef __SNIFFER_MODE__
    local_cap |= (1 << 2);
#endif

#ifdef CSI_REPORT
    local_cap |= (1 << 3);
#endif

#ifdef __P2P_MODE_SUPPORT__
    local_cap |= (1 << 4);
#endif

#ifdef __SET_MULTICAST_FILTER__
    local_cap |= (1 << 5);
#endif

#ifdef __SET_BROADCAST_FILTER__
    local_cap |= (1 << 6);
#endif

#ifdef __SET_ARP_OFFLOAD__
    local_cap |= (1 << 7);
#endif

    if (localCap)
        *localCap = local_cap;

    if (local_cap == g_wifi_hal_cap)
        return 0;
    else
        return -1;
}

/**
 * bwifi_ext_vendor_cmd - send ext vendor command to wifi subsystem
 * @hdr: include command id and length
 * Returns: 0 - success, other - failure
 */
int bwifi_ext_vendor_cmd(struct ext_vendor_cmd_info_hdr *hdr);

/*
 * Add custom mgmt frame defined ie info
 */
int bwifi_vendor_custom_ie_add(u8 *ie_info, u8 ie_length, u16 trans_type);

/*
 * Delete custom mgmt frame defined ie info
 */
int bwifi_vendor_custom_ie_del(int index);

/*
 * Set rx frame handler
 */
int bwifi_vendor_set_rx_frame_handler(int enable, wifi_frame_recv_handler cb, void *param);

#if (WIFI_STACK_VER == 2)
/**
 * bwifi_set_lmac_module_log_level - set lmac modules log level
 * @module: module name string  match with rwnx_lmac_modules[]
 * @level: module log level
 * Returns: 0 - success, other - failure
 */
int bwifi_set_lmac_module_log_level(char *module, int level);
#endif

#ifdef DUMP_FW_LOG_TO_DRIVER
/**
 * bwifi_set_lmac_log_mode - set lmac log mode
 * @log_mode: see NX_PRINT values
 * Returns: 0 - success, other - failure
 */
int bwifi_set_lmac_log_mode(uint8_t log_mode);
#endif

#if (WIFI_STACK_VER == 2)
/**
 * bwifi_set_vif_info - set lmac vif info ext
 * @vif_idx: vif index
 * @offset: member's offset in struct vif_info_ext_tag of this vif
 * @value_len: length of the value to set
 * @value: pointer to the value set to this member
 * Returns: 0 - success, other - failure
 */
int bwifi_set_vif_info(int vif_idx, int offset, size_t value_len, void *value);
#endif

#ifdef __AP_MODE__
/**
 * bwifi_vendor_softap_add_entry - add an entry
 * @info: peer info, see struct bwifi_peer_info
 * Returns：0 - success, other - failure
*/
int bwifi_vendor_softap_add_entry(struct bwifi_peer_info *info);

/**
 * bwifi_vendor_softap_del_entry - delete an entry
 * @mac_addr: sta mac address to delete, NULL to del all sta
 * Returns：0 - success, other - failure
*/
int bwifi_vendor_softap_del_entry(uint8_t *mac_addr);

/**
 * bwifi_vendor_get_mac_capa - get mac capa like bandwidth, tx mcs...
 * @info: mac config, see struct bwifi_mac_config
 * Returns：0 - success, other - failure
*/
int bwifi_vendor_get_mac_capa(struct bwifi_mac_config *info);

/**
 * bwifi_vendor_change_mac_config - change config like max tx mcs
 * @info: info, see struct bwifi_mac_config
 * Returns：0 - success, other - failure
*/
int bwifi_vendor_change_mac_config(struct bwifi_mac_config *info);

/**
 * bwifi_vendor_switch_channel - to switch channle, but not csa
 * @channel: new channel
 * @bandwidth: new bandwidth
 * Returns：0 - success, other - failure
*/
int bwifi_vendor_switch_channel(uint8_t channel, uint8_t bandwidth);

/**
 * bwifi_vendor_softap_perform_ps - softap do psm, not general psm
 * @psm_flag: 0 - to wake up, other - to fall asleep
 * Returns：0 - success, other - failure
*/
int bwifi_vendor_softap_perform_ps(uint8_t psm_flag);
#endif

/**
 * bwifi_csi_cap_req - csi_cap
 * @bw: bit0 is 20M, bit1 is 40M, bit2 is 80M
 * @type:bit0 is ht, bit1 is vht, bit2 is he_su,bit3 is he_mu,bit4 is he_er,bit5 is he_tb,
 * @stbc: Space-Time Block Coding, 1 is need to capture
 * @mac_match: mac address match, 1 is need to match our mac
 * @lsig_len_min:
 * @lsig_len_max:
 * @csi_sniff: rxvect2 of pkt set csi_sniff flag
 * @mode: status done indication way: 0 is intrrupt, 1 is loop;
 * Returns: 0 - success, other - failure
*/
#ifdef CSI_REPORT
int bwifi_csi_cap_req(uint8_t bw, uint8_t type, uint8_t stbc, uint8_t mac_match, uint16_t lsig_len_min, uint16_t lsig_len_max, uint8_t csi_sniff, uint8_t mode);
#endif

#ifdef ADVANCED_CSA_SUPPORT
/**
 * bwifi_set_csa_mode - Set CSA mode
 * @csa_mode: mode to set,
 *            0 - CSA in Beacon (legacy mode)
 *            1 - Additional CSA in Action frame to gurantee the reception
 *            2 - CSA in Action frame for switch immediately
 *            3 - CSA in Action frame for switch in schedule
 * @sched_time: Time in us after which the channel switch occurs,
 *              used in mode 3，otherwise set 0.
 * Returns：0 - success, other - failure
*/
int bwifi_set_csa_mode(uint8_t csa_mode, uint32_t sched_time);
#endif

#ifdef WIFI_NARROW_BW_SUPPORT
/**
 * bwifi_vendor_set_narrow_bw - Set narrow bandwidth mode
 *
 * @bw: bandwidth: 0 - 20M(default), 1 - 10M, 2 - 5M
 *
 * Returns: 0 on success, others on failure
 */
int bwifi_vendor_set_narrow_bw(uint8_t bw);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BWIFI_INTERFACE_H */


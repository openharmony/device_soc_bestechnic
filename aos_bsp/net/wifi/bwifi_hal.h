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
#ifndef BWIFI_HAL_H
#define BWIFI_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bwifi_event.h"
#include "wifi_def.h"

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#define BWIFI_MAX_COUNTRY_CODE_SIZE     2
#define BWIFI_CONFIG_VAL_LEN            100
#define WIFI_SCAN_DUMP
#define WIFI_COEX_MODE_FDD_BIT          (1<<0)
#define WIFI_COEX_MODE_FDD_HYBRID_BIT   (1<<1)
#define COEX_METRICS_PREFIX             "EPTA:"
#define COEX_NOTIFY_STAT_OFFSET         (0)
#define COEX_NOTIFY_TYPE_OFFSET         (1)
#define COEX_NOTIFY_MAX_LEN             (2)

#define WIFI_FIX_RATE_MIN_VALUE         (1<<16)

#ifdef __P2P_MODE_SUPPORT__
#define BWIFI_P2P_INIT "P2P_INIT"
#define BWIFI_P2P_DEINIT "P2P_DEINIT"
#endif

typedef enum bwifi_security_type {
    BWIFI_SECURITY_NONE,              /*!< open access point */
    BWIFI_SECURITY_WEP40,             /*!< phrase conforms to WEP */
    BWIFI_SECURITY_WEP104,            /*!< phrase conforms to WEP EXTENDED */
    BWIFI_SECURITY_WPA,               /*!< WPA-PSK */
    BWIFI_SECURITY_WPA2,              /*!< WPA2-PSK */
    BWIFI_SECURITY_WPA_WPA2,          /*!< WPA WPA2 mixed mode */
    BWIFI_SECURITY_WPA3_SAE,          /*!< WPA3-SAE */
    BWIFI_SECURITY_WPA3_SAE_WPA2,     /*!< WPA3-SAE WPA2 mixed mode*/
    BWIFI_SECURITY_ERROR,             /*!< error mode */
} BWIFI_SEC_TYPE_T;

struct wifi_init_param {
    char      band;
    char      country[3];
    uint16_t *tx_power;
    uint16_t *tx_power_5G;
    uint8_t  *wifi_address;
    void     *fac_wifi;
    uint32_t  coex_mode;              /* bit0: 0 - TDD, 1 - FDD; bit1: HYBRID */
    uint32_t  sleep_mode;             /* 0 - normal mode, 1 - no sleep mode */
    uint8_t  *sw_matrix_desc;
    int       sw_matrix_desc_len;
    uint32_t *new_run_flag;           /* match with HOOK_CFG_S.new_run_flag in wifi_drv.c */
    uint32_t  mac_sleep_lock_bit;     /* match LMAC_SLEPP in wiif_drv.h */
#ifdef BES_SOFT_AGC_ANTI_NOISE
    uint32_t *agc_mode;               /* 0 - hw_agc, 1 - soft agc, 2 - invalid */
#endif
};

struct bwifi_mac_addr {
    uint8_t mac[ETH_ALEN];
};

struct bwifi_bss_info {
    uint8_t bssid[ETH_ALEN];
    char ssid[32 + 1];
    uint8_t channel;
    int8_t rssi;
    BWIFI_SEC_TYPE_T security_type;
    /* user program couldn't free(ie),
     * and not available for Wi-Fi stack running on another CPU */
    uint8_t *ie;
    uint32_t ie_length;
};

struct bwifi_ap_info {
    char ssid[32 + 1];
    uint8_t channel;
    uint8_t bandwidth;
    uint8_t hidden;
    BWIFI_SEC_TYPE_T security;
    int wpa_mode;
    char passwd[64];
};

struct bwifi_ssid {
    struct bwifi_ssid *next;
    uint8_t ssid[32 + 1]; //null-terminated string
};

struct bwifi_scan_config {
    /* List of specified SSIDs */
    struct bwifi_ssid *ssids;
    /* Array of channels (zero-terminated) to scan or NULL for all channels */
    uint8_t *channels;
    /* Bandmask for scan if no specified channels
     * 0 - AUTO, 1 - 2G4, 2 - 5G */
    uint8_t band;
    /* 0 - auto, active scan on radiation allowed channel
     * 1 - force to passive scan (no probe request is sent during scan) */
    uint8_t passive;
    /* Max number of probe requests to send out on each channel, @ref MAX_PROBE_REQ_NUM
     * 0 - not defined, use default value in lmac */
    uint8_t max_probes;
    /* Time between TX of successive probes
     * 0 - not defined, if max_probes > 1，must set a valid value */
    uint32_t probe_interval_ms;
    /* Time to wait on each channel for Beacon and probe response
     * 0 - not defined, use default value in lmac
     * Notice this value only takes effect on channels with active scan */
    uint32_t duration_ms;
};

struct bwifi_network_config {
    uint8_t ssid[32 + 1];
    uint8_t passwd[65];      /* Password for encrypted ssid, "all zero" for unencryped ssid */
    int8_t  wep_keyid;       /* WEP key index, [0-3] default val:0 */
    uint8_t hidden;          /* 0 - normal, 1- treat as hidden ssid */
    uint8_t bssid[ETH_ALEN]; /* bssid or "all zero" */
    uint8_t mfp;             /* @ref WIFI_MFP_XXX enum */
};

struct bwifi_network_info {
    uint8_t ssid[32 + 1];
    uint8_t bssid[ETH_ALEN]; /* bssid or "all zero" */
    int nid;
    /**
     * disabled - Whether this network is currently disabled
     *
     * 0 = this network can be used (default).
     * 1 = this network block is disabled (can be enabled through
     * ctrl_iface, e.g., with wpa_cli or wpa_gui).
     * 2 = this network block includes parameters for a persistent P2P
     * group (can be used with P2P ctrl_iface commands)
     */
    int disabled;
};

typedef enum {
    ALLINFO = 0,
    TXSUCC,
    TXFAIL,
    TXRETRY,
    RXSUCC,
    RXLEAK,
    TXBPS,
    RXBPS,
    RSSI,
    CHANNEL,
    TXRATE,
    RXRATE,
} LINKINFO_ID;

typedef struct bwifi_trans_stat_t {
    uint32_t tx_succ_cnt;
    uint32_t tx_fail_cnt;
    uint32_t tx_retry_cnt;
    uint32_t rx_cnt;
    uint32_t rx_leak;
    uint32_t txbps;          /* Tx throughput bit/s */
    uint32_t rxbps;          /* Rx throughput bit/s */
    uint64_t tx_bytes;
    uint64_t rx_bytes;
} bwifi_trans_stat;

typedef struct bwifi_station_linkinfo_t {
    uint8_t *mac;
    bwifi_trans_stat stat;
    int8_t rssi;
    uint8_t channel;
    /* BIT 31: MCS_FLAG = 1, BTI 30: ShortGI ,BIT[29-26]: BW(0 20M,1 40M,2 80M,3 160M), BIT[25-0]: MCS index
     * BIT 31: MCS_FALG = 0, BIT[0-25] is b/g tx rate
     * */
    /* used on wifi stack v1 */
    int tx_rate;
    /* used on wifi stack v2 */
    uint32_t tx_nego_max_rate;
    uint32_t tx_highest_throughput;
    uint32_t tx_highest_probability_rate;
    /* [hw rate]:
     * MCS rate value:     [14-21]={MCS0,MCS1,MCS2,MCS3,MCS4,MCS5,MCS6,MCS7}
     * 11g/11a rate value: [6-13]={6M, 9M,12M 18M,24M,36M,48M,54M}
     * 11b rate value:     [0-3]={1M,2M,5.5M,11M}
     */
    /* BIT 31: MCS_FLAG = 1, BIT 30: ShortGI, BIT[29-26]: BW(0 20M,1 40M,2 80M,3 160M), BIT[14-0] is [hw rate]
     * BIT 31: MCS_FALG = 0, BIT[29-26]: BW(0 20M,1 40M,2 80M,3 160M), BIT[14-0] is [hw rate]
     * */
    int rx_rate;
} bwifi_station_linkinfo;

enum wifi_config_item {
    WIFI_CONFIG_ITEM_WAPI_CERT_PRIVATE_KEY = 0, /**< WAPI own private key */
    WIFI_CONFIG_ITEM_WAPI_CERT_AS_CERT,         /**< WAPI AS certification */
    WIFI_CONFIG_ITEM_WAPI_CERT_ASUE_CERT,       /**< WAPI ASUE certification */
    WIFI_CONFIG_ITEM_MAX
};

struct bwifi_config_item {
    int nid;                            /**< network index */
    int item_index;                     /**< config item index */
    int val_len;                        /**< config item value length */
    uint8_t val[BWIFI_CONFIG_VAL_LEN];  /**< config item value */
};

struct bwifi_twt_config {
    uint8_t flow_type;             /**< flow type (0: announced, 1: unannounced) */
    uint8_t flow_id;               /**< flow_id = <0..7> */
    uint8_t wake_dur_unit;         /**< Unit of TWT minimum wake duration (0:256us, 1:tu) */
    uint8_t min_wake_dur;          /**< Nominal minimum TWT wake duration */
    uint8_t wake_int_exp;          /**< TWT wake interval exponent */
    uint8_t wake_int_mantissa;     /**< TWT wake interval mantissa */
};

enum bwifi_epta_state {
    EPTA_STATE_WIFI_DISCONNECTED = 0,
    EPTA_STATE_WIFI_SCANNING,
    EPTA_STATE_WIFI_CONNECTING,
    EPTA_STATE_WIFI_CONNECTED,
    EPTA_STATE_WIFI_GOT_IP,
    EPTA_STATE_WIFI_TTS_START,
    EPTA_STATE_WIFI_TTS_END,
    EPTA_STATE_BT_INACTIVE,
    EPTA_STATE_BT_ACTIVE,
    EPTA_STATE_BT_A2DP,
    EPTA_STATE_NUM
};

enum bwifi_sniffer_cmd {
    SNIFFER_CMD_START,
    SNIFFER_CMD_STOP,
    SNIFFER_CMD_SETFILTER,
    SNIFFER_CMD_SETCH
};

enum bwifi_low_power_state {
    WIFI_LOW_POWER_DISABLE,
    WIFI_LOW_POWER_ENABLE,
};

enum bwifi_dat_cmd_type {
    DATA_CMD_RF_POWER_CFG,
};

#ifdef ENABLE_FW_KEEP_ALIVE
/*
 * ip keep alive feature's parameters.
 * @proto 0 for udp and 1 for tcp;
 * @klv_vendor different number stands for different vendor's keepalive configuration;
 * @idx add(idx=15) a tcp/udp keep alive stream, or the idx number(idx= 0-7) when reconfig one;
 *      set payload_len to 0 and payload to NULL when deleting one stream.
 * @src_port local port;
 * @dst_port tcp server's listen port;
 * @src_ip local ip address;
 * @dst_ip tcp server's ip address;
 * @payload payload of the keep alive packet.
 * @payload_len length of the payload.
 */
struct ip_alive_param
{
    uint8_t klv_vendor;
    uint8_t idx;
    uint8_t period;
    uint8_t proto;
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t payload_len;
    uint8_t *payload;
    uint8_t *key;
    uint8_t *iv;
};
#endif

#ifdef __SET_MULTICAST_FILTER__
struct multicast_filter_param
{
    uint8_t mode;
    uint8_t addr[ETH_ALEN];
};
#endif

#ifdef __P2P_MODE_SUPPORT__
struct wifi_p2p_cmd {
    uint8_t  *cmd;
    int cmd_len;
    uint8_t  *rsp;
    int *rsp_len;
};
#endif

enum EXT_VENDOR_CMD {
    GET_NET_IF,
    SET_MAGIC_WOWLAN,
    SET_WIFI_EXT_CUSTOM_IE,
    DEL_WIFI_EXT_CUSTOM_IE,
    REG_WIFI_MAC_MGMT_FRAME_RX_CB,
    SET_TDD_STAION_P2P_DUR,
    SET_LMAC_LOG_LEVEL,
    SET_LMAC_LOG_MODE,
    SET_SCAN_PARAM_EXT,
    SET_VIF_INFO,
    SOFTAP_ADD_ENTRY,
    SOFTAP_DEL_ENTRY,
    GET_WIFI_MAC_CAPA,
    CHANGE_WIFI_MAC_CONFIG,
    SWITCH_CHANNEL_BYPASS_CSA,
    SOFTAP_PERFORM_PS,
    SET_CSA_MODE,
    SET_NARROW_BW,
};

struct ext_vendor_cmd_info_hdr {
    enum EXT_VENDOR_CMD cmd_id;
    size_t cmd_len;
};

struct set_magic_wowlan {
    struct ext_vendor_cmd_info_hdr hdr;
    uint8_t enable;
};

struct set_wifi_ext_custom_ie {
    struct ext_vendor_cmd_info_hdr hdr;
    uint8_t *ie_info;
    uint8_t ie_info_len;
    uint16_t trans_type;
};

struct del_wifi_ext_custom_ie {
    struct ext_vendor_cmd_info_hdr hdr;
    int index;
};

struct reg_wifi_mgmt_frame_rx_cb {
    struct ext_vendor_cmd_info_hdr hdr;
    void *rx_cb;
};

#if LWIP_ETHERNETIF
struct get_net_if {
    struct ext_vendor_cmd_info_hdr hdr;
    int type;
    struct netif *net_if;
};
#endif

struct set_station_p2p_dur {
    struct ext_vendor_cmd_info_hdr hdr;
    int sta_dur;
    int p2p_dur;
};

#if (WIFI_STACK_VER == 2)
#define LMAC_MAC_MODULE_LEN     7
struct lmac_log_level {
    struct ext_vendor_cmd_info_hdr hdr;
    char module[LMAC_MAC_MODULE_LEN + 1];
    int level;
};
#endif

struct lmac_log_mode {
    struct ext_vendor_cmd_info_hdr hdr;
    uint8_t log_mode;
};

struct set_scan_param_ext {
    struct ext_vendor_cmd_info_hdr hdr;
    uint8_t max_probes;
    uint32_t probe_interval_ms;
    uint32_t duration_ms;
};

struct set_lmac_vif_info {
    struct ext_vendor_cmd_info_hdr hdr;
    int vif_idx;
    int offset;
    size_t len;
    void *value;
};

struct bwifi_peer_info {
    uint8_t mac_addr[ETH_ALEN];
    uint8_t channel;
    uint8_t protocol_type;
    uint8_t band_width;
    uint8_t tx_mcs;
    uint8_t rx_mcs;
};

struct bwifi_mac_config {
    uint8_t protocol_type;
    uint8_t band_width;
    uint8_t tx_mcs;
    uint8_t rx_mcs;
};

struct ext_vendor_config {
    struct ext_vendor_cmd_info_hdr hdr;
    union {
        struct bwifi_peer_info peer_info;
        struct bwifi_mac_config mac_config;
        struct {
            uint8_t channel;
            uint8_t band_width;
        } switch_channel;
        uint8_t ps;
    };
};

enum bwifi_csa_mode {
    BEACON_CSA                      = 0,
    ACTION_CSA_FOR_PROTECTION,
    ACTION_CSA_FOR_SWITCH,
    ACTION_CSA_FOR_SWITCH_IN_SCHEDULE,
    CSA_MODE_MAX,
} ;

struct set_csa_mode {
    struct ext_vendor_cmd_info_hdr hdr;
    uint8_t csa_mode;
    uint32_t sched_time;             // us
};

struct narrow_bw_cfg {
    struct ext_vendor_cmd_info_hdr hdr;
    uint8_t bw;
};
/* End of ext vendor command struct defines */

#define WIFI_CUSTOM_IE_TRANS_BEACON         1
#define WIFI_CUSTOM_IE_TRANS_PROBE_REQ      2
#define WIFI_CUSTOM_IE_TRANS_PROBE_RSP      4

#define WIFI_CUSTOM_IE_INFO_MAX_LEN         255

struct bwifi_frame_info {
    uint8_t     frame_is_ok;
    int8_t      wifi_strength;
    uint16_t    rx_channel;
};

typedef enum {
    WIFI_IF_STATION = 0,
    WIFI_IF_SOFTAP,
    WIFI_IF_P2P,
    WIFI_IF_MAX,
} BWIFI_INTF_TYPE_T;

/**
 * wifi_csi_cap_req
 * @bw: bit0 is 20M, bit1 is 40M, bit2 is 80M
 * @type:bit0 is ht, bit1 is vht, bit2 is he_su,bit3 is he_mu,bit4 is he_er,bit5 is he_tb,
 * @stbc: Space-Time Block Coding, 1 is need to capture
 * @mac_match: mac address match, 1 is need to match our mac
 * @lsig_len_min;
 * @lsig_len_max;
 * @mode: status done indication way: 0 is intrrupt, 1 is loop;
 */
struct wifi_csi_cap_req {
    uint8_t bw;
    uint8_t type;
    uint8_t stbc;
    uint8_t mac_match;
    uint16_t lsig_len_min;
    uint16_t lsig_len_max;
    uint8_t csi_sniff;
    uint8_t mode;
};

enum WIFI_BT_COEX_CMD {
    COEX_EPTA_STAT_NOTIFY = 0,
    COEX_EPTA_CONFIG,
    COEX_TEST,
};

struct coex_cmd_param {
    uint8_t id;
    uint32_t *value;
    uint8_t len;
};

struct bwifi_hal_ops {
    /* wifi power status 0 - power on, 1 - power off */
    int power_status;

    /**
     * add_network - add network config to wifi stack
     * @config: bwifi_network_config
     *
     * Returns: the allocated network id, or negative on failure
     */
    int (*add_network)(struct bwifi_network_config *config);

    /**
     * modify_network - modify network config
     * @item: bwifi_config_item contains the network id and modify item
     *
     * Returns: 0 - success, other - failure
     */
    int (*modify_network)(struct bwifi_config_item *item);

    /**
     * count_networks - get the number of network configs saved in wifi stack
     *
     * Returns: the number of network configs
     */
    int (*count_networks)(void);

    /**
     * get_networks - get network configs from wifi stack
     * @network: bwifi_network_info array to store the fetched configs
     * @count: max items to fetch
     *
     * Returns: the number of fetched networks on success, negative on failure
     */
    int (*get_networks)(struct bwifi_network_info *network, int count);

    /**
     * del_network - delete a specific network config
     * @config: bwifi_network_config
     *
     * Returns: 0 - success, other - failure
     */
    int (*del_network)(struct bwifi_network_config *config);

    /**
     * enable_network - enable a specific network, or -1 for all networks
     * @nid: network id
     *
     * Returns: 0 - success, other - failure
     */
    int (*enable_network)(int nid);

    /**
     * disable_network - disable a specific network, or -1 for all networks
     * @nid: network id
     *
     * Returns: 0 - success, other - failure
     */
    int (*disable_network)(int nid);

    /**
     * scan - start to scan
     *
     * Returns: 0 - success, other - failure
     */
    int (*start_scan)(void);

    /**
     * config_scan - config specific scan
     * @scan_config: scan config
     *
     * Returns: 0 - success, other - failure
     */
    int (*config_scan)(struct bwifi_scan_config *scan_config);

    /**
     * abort_scan - cancel the ongoing scan
     * Returns: 0 - success, other - failure
     */
    int (*abort_scan)(void);

    /**
     * get_scan_result - get scan results
     * @result: bwifi_bss_info array to store the scan results
     * @count: max items to fetch
     *
     * Returns: the number of scanned bss
     */
    int (*get_scan_result)(struct bwifi_bss_info *result, int count);

    /**
     * flush_scan_result - flush all scan results
     *
     * Returns: 0 - success, other - failure
     */
    int (*flush_scan_result)(void);

    /**
     * connect - start to connect
     *
     * Returns: 0 - no suitable network found, 1 - connect success, other - connect fail
     */
    int (*start_connect)(void);

    /**
     * wps_bss_find - find wps enabled bss
     * @bss: bwifi_bss_info
     *
     * Returns: 0 - success, other - failure
     */
    int (*wps_bss_find)(struct bwifi_bss_info * bss);

    /**
     * wps_connect - start wps connection
     * @bssid: specific bssid to connect
     *
     * Returns: 0 - success, other - failure
     */
    int (*wps_connect)(uint8_t *bssid);

    /**
     * disconnect - disconnect current connection
     *
     * Returns: 0 - success, other - failure
     */
    int (*disconnect)(void);

    /**
     * get_current_ssid - get the ssid of current connection
     * @ssid: ssid buffer, 32 ascii chars with '0' end
     *
     * Returns: 0 - success, other - failure
     */
    int (*get_current_ssid)(char *ssid);

    /**
     * get_current_bssid - get the bssid of current connection
     * @bssid: bssid buffer
     *
     * Returns: 0 - success, other - failure
     */
    int (*get_current_bssid)(uint8_t *bssid);

    /**
     * get_dev_mac - get device address of specific interface
     * @iftype: Type of interface @ref BWIFI_INTF_TYPE_T
     * @addr: mac address buffer
     *
     * Returns: 0 - success, other - failure
     */
    int (*get_dev_mac)(uint8_t iftype, uint8_t *addr);

    /**
     * get_current_rate - get current tx rate
     *
     * Returns: rate index
     * bit 31: MCS rate flag
     * bit 30:ShortGI flag
     * bit 29~26: BW (0:20M, 1:40M, other:reserved)
     * bit 25-0: MCS index
     */
    int (*get_current_rate)(void);

    /**
     * get_station_linkinfo - get link info of current sta connection
     * @info: bwifi_station_linkinfo buffer
     *
     * Returns: 0 - success, other - failure
     */
    int (*get_station_linkinfo)(bwifi_station_linkinfo *info);

    /**
     * get_current_rssi - get current rssi
     *
     * Returns: rssi (dbm)
     */
    int8_t (*get_current_rssi)(void);

    /**
     * set_ifa_ip - set ip address of specific interface
     * @iftype: Type of interface @ref BWIFI_INTF_TYPE_T
     * @ip: Pointer to hex format ip address, netmask and gateway
     *
     * Returns: 0 - success, other - failure
     */
    int (*set_ifa_ip)(uint8_t iftype, uint32_t *ip);

    /**
     * work_channel_notify - notify current work channel
     * @ch: channel number
     *
     * Returns: 0 - success, other - failure
     * Need to notify work channel to wifi hal to use the most suitable configuration
     * e.g. dpd parameter, coexistance mode, etc.
     */
    int (*work_channel_notify)(uint8_t ch);

    /**
     * traffic_stat_en - enable wifi traffic statistic
     * @en: enable/disable statistic
     * @interval_sec: the interval of statistic (seconds)
     *
     * Returns: 0 - success, other - failure
     */
    int (*traffic_stat_en)(uint8_t en, uint8_t interval_sec);

    /**
     * traffic_stat_get - get the latest traffic statistic
     * @stat: statistic data buffer
     * @clear: indicate if need to clear statistic data
     *
     * Returns: 0 - success, other - failure
     */
    int (*traffic_stat_get)(bwifi_trans_stat *stat, uint8_t clear);

    /**
     * init - wifi subsystem initialize
     * @flag: 0 - init during sys reboot, 1 - init during wifi reset
     *
     * Returns: 0 - success, other - failure
     */
    int (*init)(uint8_t flag);

    /**
     * deinit - wifi subsystem deinitialize
     *
     * Returns: 0 - success, other - failure
     */
    int (*deinit)(void);

    /**
     * power_on - set power status of wifi subsystem
     * @onoff: 1 - power on, 0 - power off
     *
     * Returns: 0 - success, other - failure
     */
    int (*power_on)(uint8_t onoff);

    /**
     * ps_mode_set - set wifi power saving mode
     * @ps: 0 - disable, 1 - enable, 0xFF - default setting
     *
     * Returns: 0 - success, other - failure
     */
    int (*ps_mode_set)(uint8_t ps);

#ifdef __AP_MODE__
    /**
     * softap_start - start ap interface
     *
     * Returns: 0 - success, other - failure
     */
    int (*softap_start)(void);

    /** softap_stop - stop ap interface
     *
     * Returns: 0 - success, other - failure
     */
    int (*softap_stop)(void);

    /**
     * softap_config_set - set softap config parameters
     * @info: config parameters
     *
     * Returns: 0 - success, other - failure
     */
    int (*softap_config_set)(struct bwifi_ap_info *info);

    /**
     * softap_peer_num_set - set the maximum station number allowed to connect
     * @num: maximum station number
     *
     * Returns: 0 - success, other - failure
     */
    int (*softap_peer_num_set)(int num);

    /**
     * softap_vendor_elements_set - set vendor ie of softap
     * @ie: pointer to information element
     * @len: the length of information element
     *
     * Returns: 0 - success, other - failure
     */
    int (*softap_vendor_elements_set)(const uint8_t *ie, size_t ie_len);

    /**
     * softap_peer_query - query the mac address list of connected stations
     * @mac_list: buffer to store the fetched mac address(es)
     * @count: maximum peer count to fetch
     *
     * Returns: the number of connected station(s)
     */
    int (*softap_peer_query)(struct bwifi_mac_addr *mac_list, int count);

    /**
     * softap_disconnect_sta - disconnect a specified peer station
     * @mac_addr: mac address of the station to disconnect
     *
     * Returns: 0 - success, other - failure
     */
    int (*softap_disconnect_sta)(uint8_t *mac_addr);
#endif

    /**
     * channel_switch - switch channel for specific interface
     * @iftype: Type of interface @ref BWIFI_INTF_TYPE_T
     * @channel:  primary channel.
     * @bandwidth: bandwidth
     * @csa_count: Number of Beacons including CSA element until channel switch
     *
     * Returns: 0 - success, other - failure
     */
    int (*channel_switch)(uint8_t iftype, uint8_t channel, uint8_t bandwidth, uint8_t csa_count);

    /**
     * set_country_code - set country code of regulation domain
     * @country: country code represented by two ascii characters ended with '\0'
     *
     * Returns: 0 - success, other - failure
     */
    int (*set_country_code)(char *country);

    /**
     * get_country_code - get country code of regulation domain
     * @country: country code represented by two ascii characters ended with '\0'
     *
     * Returns: 0 - success, other - failure
     */
    int (*get_country_code)(char *country);

    /**
     * set_wpa_log_level - set wpa_supplicant log level
     * @level: log level
     *
     * Returns: 0 - success, other - failure
     */
    int (*set_wpa_log_level)(char *level);

    /**
     * set_probe_req_filter - set driver to recive probe request frame or not
     * @enable: filter switch, 0 - close filter to rx probe request frame, other - opposite
     *
     * Returns: 0 - success, other - failure
     */
    int (*set_probe_req_filter)(uint8_t enable);

    /**
     * send_mgmt_frame - send mgmt frame on specific channel
     * @iftype: Type of interface @ref BWIFI_INTF_TYPE_T
     * @data: frame buffer
     * @len: frame length
     * @noack: indicate whether need to wait for ack
     * @chan: channel number to send, or 0 for not specified (interface working channel)
     *
     * Returns: 0 - success, other - failure
     */
    int (*send_mgmt_frame)(uint8_t iftype, const uint8_t *data, size_t len,
                           int noack, uint8_t chan);

    /**
     * data_rate_set - set the rate to send data frame
     * @iftype: Type of interface @ref BWIFI_INTF_TYPE_T
     * @rate_idx: rate index
     *   0 ~ 3:   DSSS/CCK rates: 1, 2, 5.5, 11;
     *   4 ~ 5:   reserved;
     *   6 ~ 13:  legacy OFDM rates: 6, 9, 12, 18, 24, 36, 48, 54;
     *   14 ~ 21: 1*1 MCS rates: MCS0 ~ MCS7;
     *   0xff:    auto-adjusting mode (default)
     *
     * Returns: 0 - success, other - failure
     */
    int (*data_rate_set)(uint8_t iftype, uint32_t rate_idx);

    /**
     * twt_setup - send TWT setup request
     * @mac_addr: MAC address of the peer STA
     * @setup_type: Setup request type (0 - request, 1 - suggest, 2 - demand)
     * @config: TWT config parameters
     *
     * Returns: 0 - success, other - failure
     */
    int (*twt_setup)(const uint8_t *addr, uint8_t setup_type,
                     struct bwifi_twt_config *config);

    /**
     * twt_teardown - send TWT teardown request
     * @mac_addr: MAC address of the peer STA
     * @flow_id: TWT flow id
     *
     * Returns: 0 - success, other - failure
     */
    int (*twt_teardown)(const uint8_t *addr, uint8_t flow_id);

#ifdef ENABLE_FW_KEEP_ALIVE
    /**
     * Set ip keep alive feature's parameters.
     * @paras: ip alive parameters
     * Returns: The idx of tcp stream when adding one or the delete result when deleting one
     */
    int (*set_ip_alive)(struct ip_alive_param *paras);
#endif

#ifdef __SET_MULTICAST_FILTER__
    /**
     * set_multicast_filter - set a multicast address filter
     * @paras: Include filter mode and address
     *   mode: 0 - disable filter
     *         1 - enable filter
     *         2 - clean multicast address table
     *         3 - filter all multicast frames
     *         4 - add filter
     *         5 - del filter
     *   addr: MAC address to filter
     * Returns: 0 - success, other - failure
     */
    int (*set_multicast_filter)(struct multicast_filter_param *paras);
#endif

#ifdef __SET_BROADCAST_FILTER__
    /**
     * set_broadcast_filter - enable/disable broadcast filter
     * @enable: 1 - enable broadcast filter, 0 - disable broadcast filter
     * Returns: 0 - success, other - failure
     */
    int (*set_broadcast_filter)(uint8_t enable);
#endif

#ifdef __SET_ARP_OFFLOAD__
    /**
     * set_arp_offload - enable/disable arp offload
     * @ipv4_addr: ipv4 address for arp offload, or 0 to disable
     * Returns: 0 - success, other - failure
     */
    int (*set_arp_offload)(uint32_t ipv4_addr);
#endif

#ifdef __SET_ARP_KEEP_ALIVE__
    /**
     * set_arp_keepalive - enable/disable arp keepalive in lmac
     * @iftype: Type of interface @ref BWIFI_INTF_TYPE_T
     * @enable: 1 - start, 0 - stop
     * Returns: 0 - success, other - failure
     */
    int (*set_arp_keepalive)(uint8_t iftype, uint8_t enable);
#endif

    /**
     * coex_cmd - execute the coex command
     * @param: param for coex command
     * Returns: 0 - success, other - failure
     */
    int (*coex_cmd)(struct coex_cmd_param param);

    /**
     * reset - wifi subsystem reset
     * Returns: 0 - success, other - failure
     */
    int (*reset)(void);

    /**
     * data_recv_cb_set - set callback to handle ethernet frame
     * @hdl: call back func pointer
     * Returns: 0 - success, other - failure
     */
    int (*data_recv_cb_set)(eth_input_handler hdl);

#ifdef __SNIFFER_MODE__
    /**
     * sniffer_recv_cb_set - set callback to handle sniffer frame
     * @hdl: call back func pointer
     * Returns: 0 - success, other - failure
     */
    int (*sniffer_recv_cb_set)(sniffer_recv_handler hdl);

    /**
     * sniffer cmd - send sniffer cmd.
     * @cmd: cmd id.
     * @param: match with cmd id.
     * Returns: 0 - success, other - failure
     */
    int (*sniffer_cmd)(int cmd, void *param);
#endif

#ifdef CSI_REPORT
    /**
     * csi_recv_cb_set - set callback to handle csi info
     * @hdl: call back func pointer
     * Returns: 0 - success, other - failure
     */
    int (*csi_recv_cb_set)(csi_recv_handler hdl);
    int (*csi_cap_req)(void* param);
#endif

    /**
     * eth_cb_set - set callback to handle ethernet frame
     * @hdl: call back func pointer
     * Returns: 0 - success, other - failure
     */
    uint8_t **(*get_send_buf)(void);

    /**
     * eth_cb_set - set callback to handle ethernet frame
     * @hdl: call back func pointer
     * Returns: 0 - success, other - failure
     */
    int (*data_send)(uint8_t devnum, uint8_t **tx_buf, uint16_t tx_len);

    /**
     * event_cb_set - set callback to handle event
     * @hdl: call back func pointer
     * Returns: 0 - success, other - failure
     */
    int (*event_cb_set)(evt_recv_handler hdl);

    /**
     * str_cmd - send string command to wifi subsystem
     * @type: command type, 0 - wifi rf test, other - reserved
     * @cmd_buf: command string ended with '\0'
     * @rsp_buf: response string ended with '\0'
     * @rsp_size: size of buffer to hold the rsp_buf's content
     * Returns: 0 - success, other - failure
     */
    int (*str_cmd)(uint8_t type, char *cmd_buf, char *rsp_buf, size_t rsp_size);

    /**
     * dat_cmd - send data command to wifi subsystem
     * @type: command type @ref enum bwifi_dat_cmd_type
     * @tx_buf: pointer of the data buffer to be sent.
     * @tx_len: length of the data to be sent.
     * Returns: 0 - success, other - failure
     */
    int (*dat_cmd)(uint8_t type, uint8_t *tx_buf, size_t tx_len);

#ifdef __P2P_MODE_SUPPORT__
    /**
     * p2p_cmd - send string command to wifi p2p subsystem
     * @vif_idx: virtual wifi inteface index
     * @cmd_buf: command string ended with '\0'
     * @cmd_len: the sizeof(cmd_buf) '\0'
     * @rsp_buf: response string ended with '\0'
     * @rsp_size: size of buffer to hold the rsp_buf's content
     * Returns: 0 - success, other - failure
     */
    int (*p2p_cmd)(int vif_idx, uint8_t *cmd_buf, uint8_t cmd_len, char *rsp_buf, int *rsp_size);

    /**
     * p2p_cmd - send string command to wifi p2p subsystem
     * @vif_idx: virtual wifi inteface index
     * @cmd_buf: command string ended with '\0'
     * @cmd_len: the sizeof(cmd_buf) '\0'
     * @rsp_buf: response string ended with '\0'
     * @rsp_size: size of buffer to hold the rsp_buf's content
     * Returns: 0 - success, other - failure
     */
    int (*p2p_persistent_connect)(int vif_idx, uint8_t *ssid, uint8_t ssid_len, uint8_t *password, int pw_len);
#endif

    /**
     * ext_vendor_cmd - send ext vendor command to wifi subsystem
     * @hdr: include cmd id and cmd len
     * Returns: 0 - success, other - failure
     */
    int (*ext_vendor_cmd)(struct ext_vendor_cmd_info_hdr *hdr);

};

extern struct bwifi_hal_ops whops;
#define BWIFI_HAL_API_CALL(func,...)    ((whops.func && (!whops.power_status || ((void *)whops.func == (void *)whops.power_on))) ? (int)whops.func(__VA_ARGS__) : -1)

extern struct wifi_init_param wifi_param;

#ifdef __cplusplus
}
#endif

#endif

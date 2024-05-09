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

#ifndef BWIFI_P2P_INTERFACE_H
#define BWIFI_P2P_INTERFACE_H

#include "plat_types.h"
#ifdef RTOS
#include "cmsis_os.h"
#endif
#include "wifi_def.h"

#if LWIP_ETHERNETIF
#include "lwip/netif.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BWIFI_P2P_STATUS_IDLE                  = 0,
    BWIFI_P2P_STATUS_P2P_FIND              = 1,
    BWIFI_P2P_STATUS_P2P_LISTEN            = 2,
    BWIFI_P2P_STATUS_P2P_STOP_FIND         = 3,
    BWIFI_P2P_STATUS_P2P_CONNECTING        = 4,
    BWIFI_P2P_STATUS_P2P_DISCONNECTING     = 5,
    BWIFI_P2P_STATUS_CONNECTED             = 6,
    BWIFI_P2P_STATUS_DHCPING               = 7,
    BWIFI_P2P_STATUS_GOT_IP                = 8,
    BWIFI_P2P_STATUS_GO_START              = 9,
    BWIFI_P2P_STATUS_GO_DHCPD              = 10,
    BWIFI_P2P_STATUS_ONLINE_P2P_FIND       = 11,
    BWIFI_P2P_STATUS_ONLINE_P2P_LISTEN     = 12,
    BWIFI_P2P_STATUS_END_DEF,  /* must be the last one */
} BWIFI_P2P_STATUS_T;

/**
 * Change current wifi status
 *
 * Returns: @BWIFI_P2P_STATUS_T, p2p status
 */
BWIFI_P2P_STATUS_T bwifi_get_p2p_current_status(void);

/**
 * Get current wifi status
 *
 * @stat: The new wifi p2p status
 * Returns: void
 */
void bwifi_set_p2p_current_status(BWIFI_P2P_STATUS_T stat);

/**
 * Get p2p connected.
 *
 * Returns: @p2p_connected, p2p connected flag
 */
int bwifi_get_p2p_connected_flag(void);

/**
 * Change p2p connected
 *
 * @p2p_connected: The wifi p2p connected flag.
 */
void bwifi_set_p2p_connected_flag(int p2p_connected);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * @find_params: find params
 * Returns: BWIFI_R_OK is success, other is fail
 *
 * bwifi_p2p_find find params format
 * [timeout] [type=*] = find P2P Devices for up-to timeout seconds
 * eg: "10 type=social/progressive" or NULL
 */
int bwifi_p2p_find(int vif_idx, char *find_params);

/**
 * open p2p device
 *
 * Returns: BWIFI_R_OK is success, other is fail
 */
int bwifi_p2p_device_open(void);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * @time: p2p stat time.
 * Returns: BWIFI_R_OK is success, other is fail
 */
int bwifi_p2p_listen(int vif_idx, int time);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * @mac: The p2p peer macs
 * @buf: return The p2p peer infor.
 * @len: The size of p2p peer infor.
 * Returns: BWIFI_R_OK is success, other is fail
 */
int bwifi_p2p_peer(int vif_idx,  char *mac, char *buf, int len);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * @buf: return The p2p peers infor.
 * @len: The size of p2p peers infor.
 * Returns: BWIFI_R_OK is success, other is fail
 */
int bwifi_p2p_peers(int vif_idx, char *buf, int len);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * @cmd: the p2p cmd.
 * For example: p2p P2P_CONNECT 9e:07:73:d6:ec:08 pbc freq=2412...
 * Returns: BWIFI_R_OK is success, other is fail
 */
int bwifi_p2p_connect(int vif_idx,  char *cmd);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * @cmd: the p2p cmd.
 * Returns: BWIFI_R_OK is success, other is fail
 */
int bwifi_p2p_disconnect(int vif_idx);

#if (WIFI_STACK_VER == 1)
/* invoke it as receive EVENT_P2P_WPS_CONNECTED after wps done, only version_1 invoke. */
/**
 * reconnect command after wps done
 *
 * Returns: BWIFI_R_OK is success, other is fail
*/
int bwifi_p2p_reconnect(void);
#elif (WIFI_STACK_VER == 2)
/*
 * TDD timeslot repartition between station and p2p .
 *
 * @sta_dur: station duration.
 * @p2p_dur: p2p duration.
 */
int bwifi_p2p_sta_timeslot_repartition(int sta_dur, int p2p_dur);
#endif

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_cancel(int vif_idx);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_stop_find(int vif_idx);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_ext_listen(int vif_idx);

/**
 * set the command to wpa_supplicant
 *
 * @vif_idx: The p2p vif_idx index.
 * @p2p_dev_addr, wps peer mac addrs,
 * if p2p_dev_addr is null, any peer can wps with us.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_wps_pbc(int vif_idx, char *p2p_dev_addr);

/**
 * close p2p device
 *
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_device_close(void);

/**
 * p2p event handle, owner called by bes.
 *
 * @evt_id WIFI_USER_EVT_ID
 * @arg point to the type BWIFI_EVENT_P2P_T
 * Returns: BWIFI_R_OK is success, other is fail.
 */
void bwifi_p2p_event_handle(int evt_id, void *arg);

/**
 * set the command to wpa_supplicant.
 *
 * @vif_idx: The p2p vif_idx index
 * @cmd, p2p_set command.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set(int vif_idx, char *cmd);


/**
 * Config p2p name.
 *
 * @vif_idx: The p2p vif_idx index
 * @p2p_name, p2p device name.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_name(int vif_idx, char *p2p_name);

/**
 * Config 2.4G ht40
 *
 * @vif_idx: The p2p vif_idx index
 * @ht40, 1 is p2p 2.4G ht40 support, 0 is p2p 2.4G ht40 not support.
 * default is 2.4G not support ht40.
 */
void bwifi_p2p_2g4_ht40(int vif_idx, int ht40);

/**
 * Config 5G ht40
 *
 * @vif_idx: The p2p vif_idx index
 * @ht40, 1 is p2p 5G ht40 support, 0 is p2p 5G ht40 not support.
 * default is 5G not support ht40.
 */
void bwifi_p2p_5g_ht40(int vif_idx, int ht40);

/**
 * Config wps methods
 *
 * @vif_idx: The p2p vif_idx index
 * @wps_methods, wps config motheds, same with wpa_suppllicant.
 * default WPS_PBC
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_config_methods(int vif_idx, int wps_methods);

/**
 * Get p2p peer config method
 *
 * @vif_idx: The p2p vif_idx index
 * Returns: p2p peer config methed.
 */
int bwifi_p2p_get_config_methods(int vif_idx);

/**
 * Set p2p operation channel
 *
 * @vif_idx: The p2p vif_idx index or The station vif_idx.
 * @op_ch, p2p operation channel.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_op_channel(int vif_idx, int op_ch);

/**
 * Get p2p operation channel
 *
 * @vif_idx: The p2p vif_idx index
 * Returns: p2p operation channel.
 */
int bwifi_p2p_get_op_channel(int vif_idx);

/**
 * Config p2p role(GO or GC)
 *
 * @vif_idx: The p2p vif_idx index
 * Returns: @WIFI_P2P_ROLE_ID GO or GC.
 */
int bwifi_p2p_get_p2p_role(int vif_idx);

/**
 * Config p2p role(GO or GC)
 *
 * @vif_idx: The p2p vif_idx index
 * @role: @WIFI_P2P_ROLE_ID GO or GC.
 */
void bwifi_p2p_set_p2p_role(int vif_idx, int role);

/**
 * Lists the configured networks, including stored information for
 * persistent groups.
 * @vif_idx:The p2p vif_idx index.
 * Returns: BWIFI_R_OK is success, other is fail.
 */

int bwifi_p2p_list_networks(int vif_idx);

/**
 * remove the configured networks.
 * @vif_idx:The p2p vif_idx index.
 * @cmd: NULL for all, else for id index.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_remove_network(int vif_idx, char *cmd);

/**
 * Terminate a P2P group. If a new virtual network interface was used for
 * the group, it will also be removed. The network interface name of the
 * group interface is used as a parameter for this command.
 *
 * @vif_idx: The p2p vif_idx index.
 * @cmd: group interface name. For example: wl1, wl3...
 * If the cmd is "*", it will remove all P2P devices connected.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_group_remove(int vif_idx, char *cmd);

/**
 * add a P2P group.
 *
 * @vif_idx: The p2p vif_idx index.
 * @cmd: p2p group param; for example:[a string info(freq=2412 vht=1 persistent he...)]
 * If the cmd is "", no p2p grroup param, use default value.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_group_add(int vif_idx, char *cmd);

/**
 * Set p2p persistent GO info, only use by bwifi_p2p_group_add
 *
 * @vif_idx: The p2p vif_idx index.
 * @ssid: p2p groud's SSID
 * @password: p2p groud's passphrase
 *
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_persistent_apinfo(int vif_idx, char *ssid, char *psk);

/**
 * clear p2p persistent GO info
 *
 * @vif_idx: The p2p vif_idx index.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_clear_persistent_apinfo(int vif_idx);

/**
 * connect to  p2p persistent GO info
 *
 * @vif_idx: The p2p must set WIFI_IF_P2P.
 * @ssid: P2P GO persistent ssid.
 * @ssid_len: the len of @ssid.
 * @pw: P2P GO persistent password.
 * @pw_len: the len of @pw
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_persistent_connect(int vif_idx, uint8_t *ssid, int ssid_len, uint8_t *pw, int pw_len);

/**
 * Get p2p wlan name.
 * defaule is wl2.
 *
 * @name: p2p wlan name.
 * @len: the strlen of the name.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
bool wpa_get_p2p_name(char *name, int len);

/**
 * Get p2p wlan name.
 *
 * @vif_idx: The p2p vif_idx index or The station vif_idx.
 * Returns: p2p_name string addr.
 */
char *bwifi_p2p_get_name(int vif_idx);

/**
 * pre init p2p. only bes call it.
 *
 * Returns: @WIFI_P2P_ROLE_ID GO or GC.
 */
int bwifi_pre_p2p_init(void);

/**
 * config p2p go intent.
 *
 * @vif_idx: The p2p vif_idx index or The station vif_idx.
 * @go_intent: The p2p go intent value.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_go_intent(int vif_idx, int go_intent);

/**
 * Get p2p go intent.
 *
 * @vif_idx: The p2p vif_idx index or The station vif_idx.
 * Returns: p2p go intent value.
 */
int bwifi_p2p_get_go_intent(int vif_idx);

/**
 * Config p2p peer mac.
 *
 * @vif_idx: The p2p vif_idx index.
 * @peer_mac: The p2p peer mac.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_peer_mac(int vif_idx, u8 *peer_mac);

/**
 * Get p2p peer mac.
 *
 * @vif_idx: The p2p vif_idx index.
 * Returns: p2p peer mac addr.
 */
u8 *bwifi_p2p_get_peer_mac(int vif_idx);

/**
 * Get p2p interface is open.
 *
 * @vif_idx: The p2p vif_idx index.
 * Returns:
 *         true: p2p interface is open,
 *         false: p2p interface is close.
 */
bool bwifi_p2p_is_open(int vif_idx);

/**
 * Config p2p go status. call by bwifi_p2p_interface
 *
 * @vif_idx: The p2p vif_idx index.
 * @on: true p2p go is start. false p2p go is stop.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_go_stat(int vif_idx, bool on);

/**
 * Get p2p go status.
 *
 * @vif_idx: The p2p vif_idx index
 * Returns: true p2p go is start. false p2p go is stop.
 */
bool bwifi_p2p_get_go_stat(int vif_idx);

/**
 * User Config p2p go status call by customer.
 *
 * @vif_idx: The p2p vif_idx index.
 * @on: true p2p go is start. false p2p go is stop.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_user_go_stat(int vif_idx, bool on);

/**
 * Get User config p2p go status.
 *
 * @vif_idx: The p2p vif_idx index
 * Returns: true p2p go is start. false p2p go is stop.
 */
bool bwifi_p2p_get_user_go_stat(int vif_idx);

/**
 * Get p2p freq from chan.
 *
 * @chan: The wifi chan
 * Returns: freq, 0 is fail
 */
int bwifi_p2p_channel_to_freq(u8 chan);

/**
 * flush p2p scan result.
 *
 * @vif_idx: The p2p vif_idx index.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_flush(int vif_idx);

#ifdef __P2P_WFD_MODE_SUPPORT__
/**
 * Set p2p wfd element.
 *
 * @vif_idx: The p2p vif_idx index.
 * @ie: The p2p ie, hex string buf.
 * @len: The len of the ie
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_set_wfd_subelem(int vif_idx, u8 *ie, int len);

/**
 * Set p2p wfd element.
 *
 * @vif_idx: The p2p vif_idx index.
 * @ie: The p2p ie, hex string buf.
 * @len: The len of the ie.
 * @ret_len: The valid len of the ie.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_get_wfd_subelem(int vif_idx, u8 *ie, int len, int *ret_len);
#endif

/**
 * Set p2p wildcard ssid.
 *
 * @ssid: The p2p wildcard ssid.
 * Returns: BWIFI_R_OK is success, other is fail.
 */
int bwifi_p2p_wildcard_ssid(char *ssid);

#ifdef __cplusplus
}
#endif

#endif /* BWIFI_INTERFACE_H */


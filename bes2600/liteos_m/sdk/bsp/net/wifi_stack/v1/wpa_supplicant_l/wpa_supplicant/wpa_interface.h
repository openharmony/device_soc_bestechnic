/*
 * wpa_supplicant - Exported control interfaces
 */

#ifndef WPA_INTERFACE_H
#define WPA_INTERFACE_H

#include "wifi_def.h"
#include "bwifi_hal.h"

#ifdef  __cplusplus
extern "C"
{
#endif

int  wpa_interface_add_network(struct bwifi_station_config *config);
int  wpa_interface_enable_network_id(int nid, int enable);
int  wpa_interface_enable_all_network(int enable);
int  wpa_interface_modify_network(struct bwifi_config_item *item);
int  wpa_interface_count_configured_ssid(void);
int  wpa_interface_get_current_config(struct bwifi_station_config *config, int count);
int  wpa_interface_nv_add_network(struct bwifi_station_config *config);
int  wpa_interface_nv_read_network(struct bwifi_station_config *config, int count);
int  wpa_interface_nv_find_network_by_ssid(const char *ssid, struct bwifi_station_config *config);
int  wpa_interface_del_config(u8 *ssid, u8 *passwd, int wep_keyid, u8 hidden, u8 *bssid);
int  wpa_interface_add_ext_mgmt_ie(u8 *ie_info, u8 ie_length, u16 trans_type);
int  wpa_interface_del_ext_mgmt_ie(int index);
#ifdef CONFIG_WPS
int  wpas_wps_wildcard_ok(u8 *ie, u32 ie_len);
int  wpa_interface_wps_pbc(u8 *bssid);
#endif
int  wpa_interface_auto_connect(void);
int  wpa_interface_disconnect(void);
int  wpa_interface_scan(void);
int  wpa_interface_config_scan(struct bwifi_scan_config *scan_config);
void wpa_interface_flush_scan_result(void);
int  wpa_interface_get_scan_result(struct bwifi_bss_info *result, int count);
int  wpa_interface_get_current_ssid(char *ssid);
int  wpa_interface_get_current_bssid(u8 *bssid);
int  wpa_interface_get_own_addr(u8 *addr);
u8   wpa_interface_get_current_channel(void);
int  wpa_interface_set_channel(u8 ch, s8 snd_offset);
s8   wpa_interface_get_current_rssi(void);
int  wpa_interface_set_ps_mode(int ps);
int  wpa_interface_send_mlme_on_chan(const u8 *data, size_t data_len, int noack, u8 chan);
#ifdef __P2P_MODE_SUPPORT__
/* bes p2p common operate interface start */
int wpa_interface_p2p_init(void);
int wpa_interface_p2p_deinit(void);
int wpa_interface_p2p_ctrl_peer(char *cmd_buf, char *buf, size_t buflen);
int wpa_interface_p2p_ctrl_peers(char *cmd_buf, char *buf, size_t buflen);
int wpa_interface_p2p_find(void);
int wpa_interface_p2p_stop_find(void);
int wpa_interface_p2p_listen(char *cmd_buf);
int wpa_interface_p2p_connect(char *cmd_buf);
int wpa_interface_p2p_reconnect(void);
int wpa_interface_p2p_disconnect(void);
int wpa_interface_p2p_cancel(void);
int wpa_interface_p2p_wps_pbc(char *cmd_buf);
/* bes p2p common operate interface end */
/* bes p2p common config interface start */
int wpa_interface_p2p_set_config(char *cmd_buf);
#ifdef __P2P_WFD_MODE_SUPPORT__
int wpa_interface_wfd_subelem_set(char *cmd_buf);
int wpa_interface_wfd_subelem_get(char *cmd_buf, char *buf, size_t buflen);
#endif
/* bes p2p common config interface end */
#endif

#ifdef  __cplusplus
}
#endif

#endif /* WPA_INTERFACE_H */

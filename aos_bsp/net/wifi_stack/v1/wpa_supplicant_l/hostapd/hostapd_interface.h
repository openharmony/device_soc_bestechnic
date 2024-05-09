/*
 * hostapd - Exported control interfaces
 */

#ifndef HOSTAPD_INTERFACE_H
#define HOSTAPD_INTERFACE_H

/**
 * Softap user configuration (extensible)
 */
struct softap_user_config {
	char *ssid;
	u8 channel;
	int sec_channel_offset;
	u8 hidden;
	int wpa_mode;
	char *password;
	int max_num_sta;
	char country[2];
	u8 *vendor_ie;
	size_t vendor_ie_len;
};

#ifdef  __cplusplus
extern "C"
{
#endif

/**
 * hostapd_ctrl_set_config - Set softap basic configuration
 *
 * @ssid: AP's SSID
 * @channel: Primary channel for AP to start on
 * @sec_channel_offset: Secondary channel offset for HT40
 *     0 = HT40 disabled;
 *     1 = HT40 enabled, secondary channel above primary;
 *    -1 = HT40 enabled, secondary channel below primary
 * @hidden: Whether to ignore broadcast SSID
 *     0 = broadcast ssid;
 *     1 = hide ssid with zero length;
 *     2 = hide ssid with zero contents
 * @wpa_mode: Security mode
 * 	   0 - open; 1 - WPA-PSK; 2 - WPA2-PSK; 3 - WPA/WPA2 mixed
 * @password: User passphrase
 *
 * Returns: 0 on success, nagtive if invalid arguments
 */
int hostapd_ctrl_set_config(char *ssid,
			    u8 channel,
			    int sec_channel_offset,
			    u8 hidden,
			    int wpa_mode,
			    char *password);

/**
 * hostapd_ctrl_set_max_sta_num - Set maximum client number
 * @sta_num: Maximum number of STAs
 * Returns: void
 */
void hostapd_ctrl_set_max_sta_num(int sta_num);

/**
 * hostapd_ctrl_set_country_code - Set country code
 * @country_code: Two octets string to replace a country code
 * Returns: void
 */
void hostapd_ctrl_set_country_code(char *country_code);

/**
 * hostapd_ctrl_add_vendor_elements - Add vendor elements in Beacon/ProbeRsp
 * @ie: Pointer to raw data of vendor elements
 * @ie_len: Length of vendor elements
 * Returns: 0 on success, nagtive on failure
 */
int hostapd_ctrl_add_vendor_elements(const u8 *ie, size_t ie_len);

/**
 * hostapd_ctrl_del_vendor_elements - Del vendor elements in Beacon/ProbeRsp
 * Returns: 0 on success, nagtive on failure
 */
int hostapd_ctrl_del_vendor_elements(void);

/**
 * hostapd_ctrl_get_station_list - Get mac list of connected stations
 * @mac_list: Pointer to buffer to store the mac address list
 * @size: STA count to retrieve, returns the actual connected STA count
 * Returns: void
 */
void hostapd_ctrl_get_station_list(u8 *mac_list, int *size);

int hostapd_startup(void);

int hostapd_disconnect_sta(uint8_t *ucMac);

void hostapd_stop(void);

/**
 * hostapd_interface_set_channel - Switch channel after softap start
 * @channel: channel number
 * @sec_ch_offset: secondary channel offset to center freq
 *     0 - no snd ch, 1 - upper 10M, -1 - lower 10M
 *
 * Returns: 0 on success, nagtive on failure
 */
int hostapd_interface_set_channel(u8 channel, int snd_offset);

/**
 * hostapd_interface_get_channel - Get current channel of softap
 * @channel[out]: Pointer to store the channel number (primary)
 * @snd_offset[out]: Pointer to store the secondary channel offset
 *     0 - no secondary channel;
 *     1 - upper 10M to center freq;
 *    -1 - lower 10M to center freq
 *
 * Returns: 0 on success, nagtive on failure
 */
int hostapd_interface_get_channel(u8 *channel, int *snd_offset);

/**
 * hostapd_interface_send_mlme - Send mgmt frame via AP interface
 * @data: Pointer to the frame data
 * @data_len: Length of the frame
 * @noack: Whether need to wait for ack
 *
 * Returns: 0 on success, nagtive on failure
 */
int hostapd_interface_send_mlme(const u8 *data, size_t data_len, int noack);


#ifdef  __cplusplus
}
#endif

#endif /* HOSTAPD_INTERFACE_H */

#ifndef __NET_MGMT_H__
#define __NET_MGMT_H__
#include "plat_types.h"
#ifdef ENABLE_FW_KEEP_ALIVE
#include "bwifi_hal.h"
#endif
#define NET_MGMT_PERIOD_MS     (1000) // 1s
typedef struct {
	osThreadId  thread_id;
	uint8_t     en;
	uint8_t     interval_sec;
} net_stat_cfg;

int net_device_set_ifa_addr(uint8_t dev_no, void *addr);
void net_device_start(uint8_t dev_no);
void net_device_stop(uint8_t dev_no);
void net_cfg80211_attach(uint8_t dev_no);

int detect_wifi_card(void (*cb)(uint8_t status));
void remove_wifi_card(void);

int net_init(bool first, void (*cb)(uint8_t status));
void net_deinit(void);

int netdev_sniffer_get_stat(void);
int netdev_sniffer_start(void);
int netdev_sniffer_stop(void);
int netdev_sniffer_set_channel(u8 channel);
int netdev_sniffer_set_filter(u32 filter);
#ifdef ENABLE_FW_KEEP_ALIVE
int net_device_add_ip_offload(struct ip_alive_paras *paras);
int net_device_del_ip_offload(int idx);
int net_device_en_ip_offload(int period);
#endif
#ifdef __SET_MULTICAST_FILTER__
int net_device_set_multicast_filter(u8 mode, u8 *addr);
#endif
#ifdef __SET_BROADCAST_FILTER__
int net_device_set_broadcast_filter(u8 enable);
#endif
#ifdef __SET_ARP_OFFLOAD__
int net_device_set_arp_offload(u32 enable);
#endif
int netdev_wifi_low_power(int lowpoer);
uint8_t netdev_get_wifi_low_power_stat(void);

#endif /* __NET_MGMT_H__ */

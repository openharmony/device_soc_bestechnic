/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "lwip/igmp.h"
#include "lwip/prot/tcp.h"
#include "lwip/prot/udp.h"
#include "lwip/prot/ip.h"
#include "lwip/prot/ethernet.h"
#include "lwip/sys.h"
#include "netif/etharp.h"
#include "hal_trace.h"
#include "bwifi_interface.h"
#if LWIP_IPV6
#include "lwip/ethip6.h"
#endif

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'
#define LWIP_ETH_HDR_LEN (SIZEOF_ETH_HDR - ETH_PAD_SIZE)
#ifdef LITEOS_LWIP
extern void driverif_input(struct netif *netif, struct pbuf *p);
extern err_t driverif_init(struct netif *netif);
#endif
struct netif          if_wifi;
struct netif          if_wifi_ap;

extern void   *mymemcpy(void *dst, const void *src, size_t num);
extern void   hw_checksum_init(void);
extern uint16_t hw_checksum(void *data, int16_t len, uint32_t src, uint32_t dest, uint8_t proto);

static sys_mutex_t hw_checksum_mutex;

#if WIFI_NET_TOOL_SUPPORT
bwifi_tcpdump_callback net_tool_tcpdump_hook = NULL;
void net_tool_tcpdump(unsigned char *buf, int len)
{
    struct eth_hdr *ethhdr = (struct eth_hdr *)buf;
    if (htons(ethhdr->type) == ETHTYPE_IP) {
        struct ip_hdr *ip_hdr = (struct ip_hdr *)(buf + SIZEOF_ETH_HDR);
        if (ip_hdr->_proto == IP_PROTO_TCP) {
            if (net_tool_tcpdump_hook) {
                (*net_tool_tcpdump_hook)(buf + ETH_PAD_SIZE, len);
            }
        }
    }
}
#endif
/* Forward declarations. */

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
    /* Set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
#ifdef LITEOS_LWIP
    netif->link_layer_type = WIFI_DRIVER_IF;
#endif

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP | NETIF_FLAG_IGMP;

#if LWIP_IPV6_MLD
    netif->flags |= NETIF_FLAG_MLD6;
#endif
    /* Do whatever else is needed to initialize interface. */
}

int
eth_hw_checksum_init(void)
{
    if (sys_mutex_new(&hw_checksum_mutex) != ERR_OK)
        return -1;
    hw_checksum_init();
    return 0;
}

uint16_t
eth_hw_checksum(void *data, int16_t len, uint32_t src, uint32_t dest, uint8_t proto)
{
    uint16_t sum = 0;
    sys_mutex_lock(&hw_checksum_mutex);
    sum = hw_checksum(data, len, src, dest, proto);
    sys_mutex_unlock(&hw_checksum_mutex);
    return sum;
}

void
lwip_netif_mac_addr_init(struct netif *netif, uint8_t *mac, int mac_len)
{
    memcpy(netif->hwaddr, mac, mac_len);
}

int
lwip_netif_is_link_up(struct netif *netif)
{
    return (((netif)->flags & NETIF_FLAG_LINK_UP) ? (u8_t)1 : (u8_t)0);
}

int
lwip_netif_ip_addr_isany(struct netif *netif)
{
#if LWIP_IPV4
    return (((&netif->ip_addr) == NULL) ? 1 : ip4_addr_isany(ip_2_ip4(&netif->ip_addr)));
#endif
#if LWIP_IPV6
    return (((&netif->ip_addr) == NULL) ? 1 : ((IP_IS_V6(&netif->ip_addr)) ? ip6_addr_isany(ip_2_ip6(&netif->ip_addr)) : ip4_addr_isany(ip_2_ip4(&netif->ip_addr))));
#endif
}

#ifdef CHECKSUM_BY_HARDWARE
#if LWIP_IPV6
/** Split an u32_t in two u16_ts and add them up */
#ifndef FOLD_U32T
#define FOLD_U32T(u)          ((u32_t)(((u) >> 16) + ((u) & 0x0000ffffUL)))
#endif
/*copy from ip6_chksum_pseudo_partial() which is included in inet_chksum.c*/
static void
ipv6_addr_adapt(ip6_addr_t *p_src, ip6_addr_t *p_dest, u32_t *src, u32_t *dst)
{
    u32_t addr_src =0;
    u32_t addr_dst = 0;
    u32_t tmp_src = 0;
    u32_t tmp_dst = 0;

    for (u8_t addr_part = 0; addr_part < 4; addr_part++) {
        addr_src = p_src->addr[addr_part];
        tmp_src = (u32_t)(tmp_src + (addr_src & 0xffffUL));
        tmp_src = (u32_t)(tmp_src + ((addr_src >> 16) & 0xffffUL));
        addr_dst = p_dest->addr[addr_part];
        tmp_dst = (u32_t)(tmp_dst + (addr_dst & 0xffffUL));
        tmp_dst = (u32_t)(tmp_dst + ((addr_dst >> 16) & 0xffffUL));
    }

    /* fold down to 16 bits */
    tmp_src = FOLD_U32T(tmp_src);
    tmp_src = FOLD_U32T(tmp_src);
    tmp_dst = FOLD_U32T(tmp_dst);
    tmp_dst = FOLD_U32T(tmp_dst);

    *src = tmp_src;
    *dst = tmp_dst;
}
#endif
#endif

int
mac_data_xmit(uint8_t devnum, struct pbuf *p)
{
    unsigned char **tx_buf;
    unsigned char *p802x_hdr;
    unsigned char *tmp;
    struct pbuf *q;
    int ret;

    tx_buf = bwifi_get_data_buf();
    if (tx_buf == NULL || (int)tx_buf == -1)
        return -1;
    p802x_hdr = *tx_buf;

    tmp = p802x_hdr;
    for (q = p; q != NULL; q = q->next) {
        memcpy(p802x_hdr, q->payload, q->len);
        p802x_hdr += q->len;
    }
    p802x_hdr = tmp;

#ifdef CHECKSUM_BY_HARDWARE
#if (!(CHECKSUM_GEN_TCP) || !(CHECKSUM_GEN_UDP))
    struct eth_hdr *ethhdr = (struct eth_hdr *)(p802x_hdr -  ETH_PAD_SIZE);

#if LWIP_IPV6
    if (htons(ethhdr->type) == ETHTYPE_IP || htons(ethhdr->type) == ETHTYPE_IPV6) {
#else
    if (htons(ethhdr->type) == ETHTYPE_IP) {
#endif
        uint32_t dst = 0;
        uint32_t src = 0;
        uint32_t proto = 0;
        uint32_t ip_hdr_size = 0;
        unsigned char *ip_hdr = NULL;

        if (htons(ethhdr->type) == ETHTYPE_IP) {
            struct ip_hdr *ip_hdr_tmp;
            ip_hdr_tmp = (struct ip_hdr *)(p802x_hdr + LWIP_ETH_HDR_LEN);

            memcpy(&dst, &ip_hdr_tmp->dest, 4);
            memcpy(&src, &ip_hdr_tmp->src, 4);

            proto = ip_hdr_tmp->_proto;
            ip_hdr_size = sizeof(struct ip_hdr);
            ip_hdr = (unsigned char *)ip_hdr_tmp;
        } else {
#if LWIP_IPV6
            ip6_addr_t dst_tmp, src_tmp;
            struct ip6_hdr *ip_hdr_tmp;

            ip_hdr_tmp = (struct ip6_hdr *)(p802x_hdr + LWIP_ETH_HDR_LEN);

            ip6_addr_copy_to_packed(dst_tmp, ip_hdr_tmp->dest);
            ip6_addr_copy_to_packed(src_tmp, ip_hdr_tmp->src);
            /*convert ipv6 address to adapt eth_hw_checksum function */
            ipv6_addr_adapt(&src_tmp, &dst_tmp, &src, &dst);

            proto = ip_hdr_tmp->_nexth;
            ip_hdr_size = sizeof(struct ip6_hdr);
            ip_hdr = (unsigned char *)ip_hdr_tmp;
#endif
        }

#if !(CHECKSUM_GEN_TCP)
        if (proto == IP_PROTO_TCP) {
            struct tcp_hdr *tcphdr = (struct tcp_hdr *)((unsigned char *)ip_hdr + ip_hdr_size);
            tcphdr->chksum = 0;
            tcphdr->chksum = eth_hw_checksum((void *)tcphdr, p->tot_len - ip_hdr_size - LWIP_ETH_HDR_LEN, src, dst, IP_PROTO_TCP);
        }
#endif

#if !(CHECKSUM_GEN_UDP)
        if (proto == IP_PROTO_UDP) {
            struct udp_hdr *udphdr = (struct udp_hdr *)((unsigned char *)ip_hdr + ip_hdr_size);
            udphdr->chksum = 0;
            udphdr->chksum = eth_hw_checksum((void *)udphdr, p->tot_len - ip_hdr_size - LWIP_ETH_HDR_LEN, src, dst, IP_PROTO_UDP);
        }
#endif
     }

#endif /* !(CHECKSUM_GEN_TCP) || !(CHECKSUM_GEN_UDP) */
#endif /* CHECKSUM_BY_HARDWARE */
#if WIFI_NET_TOOL_SUPPORT
    net_tool_tcpdump(p802x_hdr - ETH_PAD_SIZE, p->tot_len);
#endif
    ret = bwifi_send_data_buf(devnum, tx_buf, p->tot_len);
    if (ret < 0)
        return -1;

    return 0;
}

#define LWIP_ETH_HDR_LEN	(SIZEOF_ETH_HDR - ETH_PAD_SIZE)
#if IPSEC_SUPPORT
extern err_t ipsecdev_input_ip6_lwip_wifi(struct pbuf *p, struct netif *inp);
extern err_t ipsecdev_output_ip6_lwip_wifi(struct netif *, struct pbuf *,struct pbuf **send_p);
#endif

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    uint8_t RetryCnt = 20, devnum;
    int xmit;
    err_t ret;

#if IPSEC_SUPPORT
    struct eth_hdr *ethhdr;
    struct pbuf *send_p = NULL;
    ethhdr = p->payload;
    switch (htons(ethhdr->type)) {
    case ETHTYPE_IPV6:
        if (ipsecdev_output_ip6_lwip_wifi(netif, p, &send_p) == ERR_OK && send_p != NULL) {
            p = send_p;
        }
        break;
    default:
        break;
    }
#endif

#if ETH_PAD_SIZE
    /* Note:hmos tx should not drop the paddiing word */
#ifndef LITEOS_LWIP
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
#endif
    (netif == &if_wifi) ? (devnum = 0) : (devnum = 1);

    while ((xmit = mac_data_xmit(devnum, p)) < 0 && (RetryCnt--)) {
        osDelay(1);
    }

#if ETH_PAD_SIZE
    /* Note: hmos tx should not drop the paddiing word */
#ifndef LITEOS_LWIP
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
#endif

#if IPSEC_SUPPORT
    if(send_p != NULL)
        pbuf_free(send_p);
#endif

    (xmit == 0) ? (ret = ERR_OK) : (ret = ERR_MEM);
    return ret;
}


/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
struct pbuf *
low_level_input(struct netif *netif, void *p_buf, int size)
{
    struct pbuf *p, *q;
    uint16_t len;
    int rem_len;
    static int drop_cnt = 0;

    /* Obtain the size of the packet and put it into the "len"
     * variable. */
    len = size;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL/*PBUF_RAM*/);  //rx should use pbuf_pool. to avoid alloc failed in tcp server(rx mostly)

    if (p != NULL && (p->tot_len >= len)) {

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf. */
        rem_len = p->tot_len;
        for (q = p; q != NULL; q = q->next) {
            /* Read enough bytes to fill this pbuf in the chain. The
             * available data in the pbuf is given by the q->len
             * variable.
             * This does not necessarily have to be a memcpy, you can also preallocate
             * pbufs for a DMA-enabled MAC and after receiving truncate it to the
             * actually received size. In this case, ensure the tot_len member of the
             * pbuf is the sum of the chained pbuf len members.
             */
            if (rem_len > 0) {
                //printk(KERN_DEBUG, "low_level_input: 0x%x, 0x%x.\n", (uint32_t)q->payload, (uint32_t)((char*)p_buf + (p->tot_len - rem_len)));
                mymemcpy(q->payload, (char *)p_buf + (p->tot_len - rem_len), q->len);
                rem_len -= q->len;
            } else
                LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input memcpy err\n"));
        }

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
        LINK_STATS_INC(link.recv);
    } else {
        ++drop_cnt;
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input alloc failed, drop frame count=%d!\n", drop_cnt));
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
    }
    return p;
}

#ifdef LITEOS_LWIP

static void
liteos_low_level_output(struct netif *netif, struct pbuf *p)
{
    low_level_output(netif, p);
}
#endif

extern int netdev_wifi_low_power(int lowpoer);
/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void
ethernetif_input(uint8_t devnum, void *p_buf, int size)
{
    struct eth_hdr *ethhdr;
    struct netif *netif;
    struct pbuf *p;

    (devnum == 0) ? (netif = &if_wifi) : (netif = &if_wifi_ap);
    /* move received packet into a new pbuf */
#if(SKB_SRAM == 2)
    p = (struct pbuf *)p_buf;
#else
    p = low_level_input(netif, p_buf, size);
#endif
#ifdef LITEOS_LWIP
    driverif_input(netif, p);
    return;
#endif
    /* no packet could be read, silently ignore this */
    if (p == NULL) return;
    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch (htons(ethhdr->type)) {
    case ETHTYPE_LLDP:
    /* IP or ARP packet? */
    /* fall through */
    case ETHTYPE_IP:
#if LWIP_IPV6
    /* IPv6 packet? */
    /* fall through */
    case ETHTYPE_IPV6:
#endif /* IPv6 packet */
#if defined(CHECKSUM_BY_HARDWARE) && (!(CHECKSUM_CHECK_TCP) || !(CHECKSUM_CHECK_UDP))
    {
        char *trpkt;
        uint16_t trpkt_len;
        uint16_t chksum_cal = 0;
        uint32_t ip_hdr_size = 0;
        uint32_t proto = 0;
        uint32_t dst = 0, src = 0;
        unsigned char *ip_hdr = NULL;

        if (htons(ethhdr->type) == ETHTYPE_IP) {
            struct ip_hdr *ip_hdr_tmp;

            ip_hdr_tmp = (struct ip_hdr *)((char *)ethhdr + SIZEOF_ETH_HDR);

            memcpy(&dst, &ip_hdr_tmp->dest, 4);
            memcpy(&src, &ip_hdr_tmp->src, 4);

            proto = ip_hdr_tmp->_proto;
            ip_hdr_size = sizeof(struct ip_hdr);
            ip_hdr = (unsigned char *)ip_hdr_tmp;
        } else {
#if LWIP_IPV6
            ip6_addr_t dst_tmp, src_tmp;
            struct ip6_hdr *ip_hdr_tmp;

            ip_hdr_tmp = (struct ip6_hdr *)((char *)ethhdr + SIZEOF_ETH_HDR);

            ip6_addr_copy_to_packed(dst_tmp, ip_hdr_tmp->dest);
            ip6_addr_copy_to_packed(src_tmp, ip_hdr_tmp->src);
            /*convert ipv6 address to adapt eth_hw_checksum function */
            ipv6_addr_adapt(&src_tmp, &dst_tmp, &src, &dst);

            proto = ip_hdr_tmp->_nexth;
            ip_hdr_size = sizeof(struct ip6_hdr);
            ip_hdr = (unsigned char *)ip_hdr_tmp;
#endif
	}

        LWIP_ASSERT("eth input invalid size", (size >= (ip_hdr_size + LWIP_ETH_HDR_LEN)));
#if(SKB_SRAM == 2)
        trpkt = (struct pbuf *)p_buf->payload;
#else
        trpkt = (char *)p_buf;
#endif
        trpkt += (SIZEOF_ETH_HDR - ETH_PAD_SIZE) + ip_hdr_size;
        trpkt_len = size - ip_hdr_size - (SIZEOF_ETH_HDR - ETH_PAD_SIZE);

#if !(CHECKSUM_CHECK_TCP)
        if (proto == IP_PROTO_TCP) {
            struct tcp_hdr *tcphdr = (struct tcp_hdr *)((char *)ip_hdr + ip_hdr_size);
            if (tcphdr->chksum)
                chksum_cal = eth_hw_checksum((void *)trpkt, trpkt_len, src, dst, IP_PROTO_TCP);
            if (chksum_cal != 0) {
                //printk(KERN_DEBUG, "hw tcp checksum error\n");
            }
        }
#endif

#if !(CHECKSUM_CHECK_UDP)
        if (proto == IP_PROTO_UDP) {
            struct udp_hdr *udphdr = (struct udp_hdr *)((char *)ip_hdr + ip_hdr_size);
            if (udphdr->chksum) {
                chksum_cal = eth_hw_checksum((void *)trpkt, trpkt_len, src, dst, IP_PROTO_UDP);
                if (chksum_cal != 0) {
                    //printk(KERN_DEBUG, "hw udp chksum error src=%x dst=%x pkt=%x len=%x\n", src, dst, trpkt, trpkt_len);
                }
            }
        }
#endif
    }
#endif

#if LWIP_IPV6_SUPPORT
#if IPSEC_SUPPORT
    if(htons(ethhdr->type) == ETHTYPE_IPV6){
        if (ipsecdev_input_ip6_lwip_wifi(p, netif) != ERR_OK) {
            pbuf_free(p);
            p = NULL;
        }
        /*don't need to remove header,ipsecdev_input_ip6 have done*/
    }
#endif
#endif
    /* fall through */
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
#ifdef __NET_WIFI_SET_TCP_WAKEUP_PORT__
        /* filter tcp port */
        if (htons(ethhdr->type) == ETHTYPE_IP) {
            u16 *port;
            u16 port_dst, port_src;
            port = (u16 *)((char *)ethhdr + SIZEOF_ETH_HDR + sizeof(struct ip_hdr));

            port_dst = ntohs(*port);
            port_src = ntohs(*(port+1));
            //printf("%s %d %d\n\r", __func__, port_dst, port_src);
            if (port_src == NET_WIFI_SET_TCP_WAKEUP_PORT_NUM || port_dst == NET_WIFI_SET_TCP_WAKEUP_PORT_NUM) {
               netdev_wifi_low_power(WIFI_LOW_POWER_DISABLE);
            }
        }
#if LWIP_IPV6_SUPPORT
        else if(htons(ethhdr->type) == ETHTYPE_IPV6) {
            u16 *port;
            u16 port_dst, port_src;
            port = (u16 *)((char *)ethhdr + SIZEOF_ETH_HDR + sizeof(struct ip6_hdr));

            port_dst = ntohs(*port);
            port_src = ntohs(*(port+1));
            if (port_src == NET_WIFI_SET_TCP_WAKEUP_PORT_NUM || port_dst == NET_WIFI_SET_TCP_WAKEUP_PORT_NUM) {
               netdev_wifi_low_power(WIFI_LOW_POWER_DISABLE);
            }
        }
#endif
#endif
        /* full packet send to tcpip_thread to process */
        if (netif->input(p, netif) != ERR_OK) {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
            p = NULL;
#if WIFI_NET_TOOL_SUPPORT
        } else {
            unsigned char *p802x_hdrs = (unsigned char *)p->payload;
            int p802x_len = p->tot_len - ETH_PAD_SIZE;
            net_tool_tcpdump(p802x_hdrs, p802x_len);
#endif
        }
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
    uint32_t localCap = 0;
    if (0 != bwifi_check_capability(&localCap)) {
        printf("capability error, localCap:0x%02x\n", localCap);
        assert(0 == bwifi_check_capability(&localCap));
    }

    LWIP_ASSERT("netif != NULL", (netif != NULL));

#ifdef CHECKSUM_BY_HARDWARE
    eth_hw_checksum_init();
#endif

#ifdef LITEOS_LWIP
#if LWIP_IPV4
    netif->output = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput = low_level_output;

    netif->drv_send = liteos_low_level_output;
    netif->hwaddr_len = NETIF_MAX_HWADDR_LEN;
    low_level_init(netif);
    driverif_init(netif);
    return ERR_OK;
#else
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    //NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state = NULL;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /*
     * We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...)
     */
#if LWIP_IPV4
    netif->output = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);
    return ERR_OK;
#endif
}


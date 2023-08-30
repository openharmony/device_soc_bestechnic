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
#ifndef __DEBUG_CFG_H__
#define __DEBUG_CFG_H__

#define NULL_MODULE         0
#define HCI_MODULE          1
#define L2CAP_MODULE        2
#define GAP_MODULE          3
#define GATT_MODULE         4
#define SDP_MODULE          5
#define SMP_MODULE          6
#define BTM_MODULE          7
#define A2DP_MODULE         8
#define AVRCP_MODULE        9
#define BNEP_MODULE         10
#define HFP_MODULE          11
#define HID_MODULE          12
#define MAP_MODULE          13
#define OBEX_MODULE         14
#define OPP_MODULE          15
#define PAN_MODULE          16
#define PBAP_MODULE         17
#define RFCOMM_MODULE       18
#define SPP_MODULE          19
#define BTIF_MODULE         20
#define DIP_MODULE          21
#define BESAUD_MODULE       22
#define BTGATT_MODULE       23
#define A2DP_API_MODULE     24
#define AVRCP_API_MODULE    25
#define HFP_API_MODULE      26
#define HID_API_MODULE      27
#define MAP_API_MODULE      28
#define ME_API_MODULE       29
#define OPP_API_MODULE      30
#define PAN_API_MODULE      31
#define PBAP_API_MODULE     32
#define BAP_MODULE          33
#define ISO_MODULE          34
#define BLE_MODULE          35
#define TEST_MODULE         36
#define COBT_MODULE         37
#define LOG_MODULE_MAX      38

#define BT_DBG_EX_CONCAT(a, b) a##b
#define BT_DBG_CONCAT(a, b) BT_DBG_EX_CONCAT(a, b)
#define BT_DBG_EX_NAME(a) #a
#define BT_DBG_GET_NAME(a) BT_DBG_EX_NAME(a)

#define DEBUG_PRINT TRACE

#define DBG_ASSERT_LEVEL    0
#define DBG_ERROR_LEVEL     1
#define DBG_WARNING_LEVEL   2
#define DBG_INFO_LEVEL      3
#define DBG_VERBOSE_LEVEL   4
#define DBG_MAIN_FLOW_LEVEL 5

#ifdef BT_STACK_LOG_DISABLE
#define HCI_LEVEL           DBG_ERROR_LEVEL
#define L2CAP_LEVEL         DBG_ERROR_LEVEL
#define GAP_LEVEL           DBG_ERROR_LEVEL
#define GATT_LEVEL          DBG_ERROR_LEVEL
#define SDP_LEVEL           DBG_ERROR_LEVEL
#define SMP_LEVEL           DBG_ERROR_LEVEL
#define BTM_LEVEL           DBG_ERROR_LEVEL
#define A2DP_LEVEL          DBG_ERROR_LEVEL
#define AVRCP_LEVEL         DBG_ERROR_LEVEL
#define BNEP_LEVEL          DBG_ERROR_LEVEL
#define HFP_LEVEL           DBG_ERROR_LEVEL
#define HID_LEVEL           DBG_ERROR_LEVEL
#define MAP_LEVEL           DBG_ERROR_LEVEL
#define OBEX_LEVEL          DBG_ERROR_LEVEL
#define OPP_LEVEL           DBG_ERROR_LEVEL
#define PAN_LEVEL           DBG_ERROR_LEVEL
#define PBAP_LEVEL          DBG_ERROR_LEVEL
#define RFCOMM_LEVEL        DBG_INFO_LEVEL
#define SPP_LEVEL           DBG_ERROR_LEVEL
#define BTIF_LEVEL          DBG_ERROR_LEVEL
#define DIP_LEVEL           DBG_ERROR_LEVEL
#define BESAUD_LEVEL        DBG_ERROR_LEVEL
#define BTGATT_LEVEL        DBG_ERROR_LEVEL
#define A2DP_API_LEVEL      DBG_ERROR_LEVEL
#define AVRCP_API_LEVEL     DBG_ERROR_LEVEL
#define HFP_API_LEVEL       DBG_ERROR_LEVEL
#define HID_API_LEVEL       DBG_ERROR_LEVEL
#define MAP_API_LEVEL       DBG_ERROR_LEVEL
#define ME_API_LEVEL        DBG_ERROR_LEVEL
#define OPP_API_LEVEL       DBG_ERROR_LEVEL
#define PAN_API_LEVEL       DBG_ERROR_LEVEL
#define PBAP_API_LEVEL      DBG_ERROR_LEVEL
#define BAP_LEVEL           DBG_ERROR_LEVEL
#define ISO_LEVEL           DBG_ERROR_LEVEL
#define BLE_LEVEL           DBG_ERROR_LEVEL
#define TEST_LEVEL          DBG_VERBOSE_LEVEL
#else
#define HCI_LEVEL           DBG_VERBOSE_LEVEL
#define L2CAP_LEVEL         DBG_VERBOSE_LEVEL
#define GAP_LEVEL           DBG_VERBOSE_LEVEL
#define GATT_LEVEL          DBG_INFO_LEVEL
#define SDP_LEVEL           DBG_INFO_LEVEL
#define SMP_LEVEL           DBG_VERBOSE_LEVEL
#define BTM_LEVEL           DBG_VERBOSE_LEVEL // DBG_INFO_LEVEL
#define A2DP_LEVEL          DBG_ERROR_LEVEL
#define AVRCP_LEVEL         DBG_ERROR_LEVEL
#define BNEP_LEVEL          DBG_ERROR_LEVEL
#define HFP_LEVEL           DBG_ERROR_LEVEL
#define HID_LEVEL           DBG_ERROR_LEVEL
#define MAP_LEVEL           DBG_ERROR_LEVEL
#define OBEX_LEVEL          DBG_ERROR_LEVEL
#define OPP_LEVEL           DBG_ERROR_LEVEL
#define PAN_LEVEL           DBG_ERROR_LEVEL
#define PBAP_LEVEL          DBG_ERROR_LEVEL
#define RFCOMM_LEVEL        DBG_INFO_LEVEL
#define SPP_LEVEL           DBG_WARNING_LEVEL
#define BTIF_LEVEL          DBG_ERROR_LEVEL
#define DIP_LEVEL           DBG_ERROR_LEVEL
#define BESAUD_LEVEL        DBG_ERROR_LEVEL
#define BTGATT_LEVEL        DBG_ERROR_LEVEL
#define A2DP_API_LEVEL      DBG_ERROR_LEVEL
#define AVRCP_API_LEVEL     DBG_ERROR_LEVEL
#define HFP_API_LEVEL       DBG_ERROR_LEVEL
#define HID_API_LEVEL       DBG_ERROR_LEVEL
#define MAP_API_LEVEL       DBG_ERROR_LEVEL
#define ME_API_LEVEL        DBG_ERROR_LEVEL
#define OPP_API_LEVEL       DBG_ERROR_LEVEL
#define PAN_API_LEVEL       DBG_ERROR_LEVEL
#define PBAP_API_LEVEL      DBG_ERROR_LEVEL
#define BAP_LEVEL           DBG_VERBOSE_LEVEL
#define ISO_LEVEL           DBG_VERBOSE_LEVEL
#define BLE_LEVEL           DBG_VERBOSE_LEVEL
#define TEST_LEVEL          DBG_VERBOSE_LEVEL
#endif

#define COBT_LEVEL          DBG_ERROR_LEVEL
#define bap_uc_svc_LEVEL    DBG_ERROR_LEVEL
#define bap_bc_deleg_LEVEL  DBG_ERROR_LEVEL

#endif /* __DEBUG_CFG_H__ */

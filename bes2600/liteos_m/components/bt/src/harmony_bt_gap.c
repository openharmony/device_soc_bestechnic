/**
 * @copyright Copyright (c) 2015-2021 BES Technic.
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
 */
/*****************************header include********************************/

#include <stdlib.h>
#include <string.h>
#include "plat_types.h"
#include "hal_trace.h"
#include "factory_section.h"
#include "adapter_service.h"
#include "bes_me_api.h"
#include "gap_service.h"

#include "ohos_bt_debug.h"
#include "ohos_bt_gap.h"
#include "ohos_bt_utils.h"

/****************************Macro defination***************************/

/****************************Type defination****************************/
enum BT_SCAN_STIMER_STATE
{
    SCAN_TIMER_IDLE = 0,
    SCAN_TIMER_RUNNING,
};

typedef struct
{
    uint8_t bt_state;
    uint8_t ble_state;
    uint8_t *bt_addr;
    uint8_t *ble_addr;
    int     scan_mode;
    osTimerId scan_mode_timer_id;
    uint8_t   timer_run_state;
    BtGapCallBacks *event_callback;
} BtGapEnv_t;

/****************************Function declaration***********************/
static void ohos_scan_mode_timer_handler(void const *param);

/****************************Variable defination************************/
osTimerDef(scan_mode_timer, ohos_scan_mode_timer_handler);
static BtGapEnv_t BtGapEnv =
{
    .bt_state = OHOS_GAP_STATE_TURN_ON,
    .ble_state = OHOS_GAP_STATE_TURN_ON,
    .event_callback = NULL,
};

static void ohos_bt_conn_state_change_callback(uint8_t type,bt_bdaddr_t* addr, uint32_t para)
{
    switch (type)
    {
        case BT_ACL_CONNECT_REQ:
        {
            if (BtGapEnv.event_callback && BtGapEnv.event_callback->pairRequestedCallback)
            {
                BtGapEnv.event_callback->pairRequestedCallback((BdAddr *)addr, OHOS_BT_TRANSPORT_BR_EDR);
            }
        } break;
        case BT_ACL_CONNECT:
        {
            BtGapEnv.event_callback->aclStateChangedCallbak((BdAddr *)addr, OHOS_GAP_ACL_STATE_CONNECTED, 0);
        } break;
        case BT_ACL_DISCONNECT:
        {
            BtGapEnv.event_callback->aclStateChangedCallbak((BdAddr *)addr, OHOS_GAP_ACL_STATE_DISCONNECTED, para);
        } break;
        default:
            break;
    }
}

static void ohos_confirmation_req_callback(struct bdaddr_t *bdaddr, uint32 numeric_value)
{
    if (BtGapEnv.event_callback && BtGapEnv.event_callback->pairConfiremedCallback)
    {
        BtGapEnv.event_callback->pairConfiremedCallback((BdAddr *)bdaddr, OHOS_BT_TRANSPORT_BR_EDR, 0, (int)numeric_value);
    }
}

static uint8_t ohos_scan_mode_switch_to_bes(uint8_t scanMode)
{
    uint8_t bes_mode;
    switch(scanMode)
    {
        case OHOS_GAP_SCAN_MODE_NONE:
            bes_mode = BTIF_BAM_NOT_ACCESSIBLE;
        break;
        case OHOS_GAP_SCAN_MODE_CONNECTABLE:
            bes_mode = BTIF_BAM_CONNECTABLE_ONLY;
        break;
        case OHOS_GAP_SCAN_MODE_GENERAL_DISCOVERABLE:
            bes_mode = BTIF_BAM_DISCOVERABLE_ONLY;
        break;
        case OHOS_GAP_SCAN_MODE_LIMITED_DISCOVERABLE:
            bes_mode = BTIF_BAM_DISCOVERABLE_ONLY;
        break;
        case OHOS_GAP_SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE:
            bes_mode = BTIF_BAM_GENERAL_ACCESSIBLE;
        break;
        case OHOS_GAP_SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE:
            bes_mode = BTIF_BAM_GENERAL_ACCESSIBLE;
        break;
        default:
            bes_mode = BTIF_BAM_NOT_ACCESSIBLE;
            break;
    }
    return bes_mode;
}

static void ohos_scan_mode_timer_handler(void const *param)
{
    bes_bt_me_set_accessible_mode(BTIF_BAM_NOT_ACCESSIBLE,NULL);
    BtGapEnv.timer_run_state = SCAN_TIMER_IDLE;
}

static void ohos_set_bt_scan_limited_mode(int mode, int duration)
{
    static uint8_t timer_init_flag = 0;

    if (!timer_init_flag) {
        timer_init_flag = 1;
        BtGapEnv.scan_mode_timer_id = osTimerCreate(osTimer(scan_mode_timer), osTimerOnce, NULL);
    }
    switch(mode)
    {
        case OHOS_GAP_SCAN_MODE_LIMITED_DISCOVERABLE:
        case OHOS_GAP_SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE:
            if (BtGapEnv.scan_mode_timer_id != NULL) {
                if (BtGapEnv.timer_run_state == SCAN_TIMER_IDLE){
                    osTimerStart(BtGapEnv.scan_mode_timer_id, duration);
                    BtGapEnv.timer_run_state = SCAN_TIMER_RUNNING;
                }
                else{
                    osTimerStop(BtGapEnv.scan_mode_timer_id);
                    osTimerStart(BtGapEnv.scan_mode_timer_id, duration);
                }
            }
        break;
        default:
            if (BtGapEnv.timer_run_state == SCAN_TIMER_RUNNING){
                osTimerStop(BtGapEnv.scan_mode_timer_id);
                BtGapEnv.timer_run_state = SCAN_TIMER_IDLE;
            }
        break;
    }
}

/****************************Api function implementation****************************/

int GapRegisterCallbacks(BtGapCallBacks *func)
{
    BtGapEnv.event_callback = func;
    bes_bt_me_acl_conn_state_callback_register(ohos_bt_conn_state_change_callback);
    bes_bt_me_confirmation_register_callback(ohos_confirmation_req_callback);

    return OHOS_BT_STATUS_SUCCESS;
}

bool EnableBle(void)
{
    if (BtGapEnv.ble_state == OHOS_GAP_STATE_TURN_ON)
    {
        return false;
    }

    if(BtGapEnv.event_callback && BtGapEnv.event_callback->stateChangeCallback){
        BtGapEnv.ble_state = OHOS_GAP_STATE_TURN_ON;
        BtGapEnv.event_callback->stateChangeCallback(OHOS_BT_TRANSPORT_LE,OHOS_GAP_STATE_TURN_ON);
    }
    return true;
}

bool DisableBle(void)
{
    if (BtGapEnv.ble_state == OHOS_GAP_STATE_TURN_OFF)
    {
        return false;
    }

    if(BtGapEnv.event_callback && BtGapEnv.event_callback->stateChangeCallback){
        BtGapEnv.ble_state = OHOS_GAP_STATE_TURN_OFF;
        BtGapEnv.event_callback->stateChangeCallback(OHOS_BT_TRANSPORT_LE,OHOS_GAP_STATE_TURN_OFF);
    }
    return true;
}

bool EnableBt(void)
{
    if (BtGapEnv.bt_state == OHOS_GAP_STATE_TURN_ON)
    {
        return false;
    }

    if(BtGapEnv.event_callback && BtGapEnv.event_callback->stateChangeCallback){
        BtGapEnv.bt_state = OHOS_GAP_STATE_TURN_ON;
        BtGapEnv.event_callback->stateChangeCallback(OHOS_BT_TRANSPORT_BR_EDR,OHOS_GAP_STATE_TURN_ON);
    }

    return true;
}

bool DisableBt(void)
{
    if (BtGapEnv.bt_state == OHOS_GAP_STATE_TURN_OFF)
    {
        return false;
    }

    bes_bt_me_set_accessible_mode(BTIF_BAM_NOT_ACCESSIBLE,NULL);
    if(BtGapEnv.event_callback && BtGapEnv.event_callback->stateChangeCallback){
        BtGapEnv.bt_state = OHOS_GAP_STATE_TURN_OFF;
        BtGapEnv.event_callback->stateChangeCallback(OHOS_BT_TRANSPORT_BR_EDR,OHOS_GAP_STATE_TURN_OFF);
    }

    return true;
}

int GetBtState()
{
    return BtGapEnv.bt_state;
}

bool IsBleEnabled()
{
    return (BtGapEnv.ble_state == OHOS_GAP_STATE_TURN_ON)? true:false;
}

bool GetLocalAddr(unsigned char *mac, unsigned int len)
{
    if ((!mac ) || (len != 6))
    {
        return BT_STS_FAILED;
    }

    if (!BtGapEnv.bt_addr)
    {
        BtGapEnv.bt_addr = factory_section_get_bt_address();
    }
    if (!BtGapEnv.ble_addr)
    {
        BtGapEnv.ble_addr = factory_section_get_ble_address();
    }

    memcpy(mac, BtGapEnv.ble_addr, 6);

    return true;
}

bool SetLocalName(unsigned char *localName, unsigned char length)
{
    if (localName == NULL){
        return false;
    }

    bes_bt_me_set_bt_name((char *)localName, length);
    bes_bt_me_set_ble_name((char *)localName, length);

    factory_section_set_bt_name((char*)localName, length);
    factory_section_set_ble_name((char*)localName);
    if(BtGapEnv.event_callback && BtGapEnv.event_callback->deviceNameChangedCallback){
        BtGapEnv.event_callback->deviceNameChangedCallback(localName, length);
    }

    return true;
}

bool SetBtScanMode(int mode, int duration)
{
    const btif_access_mode_info_t info = {0x800, 0x12, 0x800, 0x12};
    uint8_t scanMode;

    if (BtGapEnv.scan_mode != mode){
        BtGapEnv.scan_mode = mode;
        if(BtGapEnv.event_callback && BtGapEnv.event_callback->scanModeChangedCallback){
            BtGapEnv.event_callback->scanModeChangedCallback(BtGapEnv.scan_mode);
        }
    }
    
    scanMode = ohos_scan_mode_switch_to_bes(mode);
    ohos_set_bt_scan_limited_mode(mode, duration);
    bes_bt_me_set_accessible_mode(scanMode,&info);

    return true;
}

bool PairRequestReply(const BdAddr *bdAddr, int transport, bool accept)
{
    bes_bt_me_response_acl_conn_req((bt_bdaddr_t *)bdAddr, accept);
    return true;
}

bool SetDevicePairingConfirmation(const BdAddr *bdAddr, int transport, bool accept)
{
    bes_bt_me_confirmation_resp((struct bdaddr_t *)bdAddr, accept);
    return true;
}

bool SetFastScan(bool isEnable)
{
    return false;
}

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

#include "adapter_service.h"
#include "bes_me_api.h"

#include "ohos_bt_gatt.h"
#include "ohos_bt_def.h"
//#include "harmony_utils.h"

#include "gap_service.h"
#include "bt_adaptor_dbg.h"

/****************************Macro defination***************************/
#define ADV_ID_INVALID  0XFFFFFFFF
#define ADV_MAX_NUM 9

/****************************Type defination****************************/
typedef struct
{
    gap_adv_event_t event;
    int (*BleAdvCallback)(uintptr_t adv, gap_adv_callback_param_t param);
} OhosAdvStackEventList_T;

typedef struct
{
    osMutexDef_t mutex;
    osMutexId    mutex_id;

    // ohos info
    int32_t ScannerId;
    uint32_t FilterSize;
    BleScanNativeFilter FilterInfo;
    BleScanCallbacks    *ScanFuncCb;
    BtGattCallbacks     *AdvFuncCb;

    // host stack info
    bool AdvEnabled;
    uint8_t AdvEventNum;
    uint8_t SetData;
} BleGattEnv_t;

/****************************Function declaration***********************/

/****************************Variable defination************************/
static BleGattEnv_t BleGattEnv = {0};

/****************************Private function implementation************************/

static inline void BleGapScanPrepareReportParam(const gap_adv_report_t* param, BtScanResultData *repot)
{
    if (param->adv.legacy_pdu)
    {
        repot->eventType |= 1<<4;
    }
    if (param->adv.connectable)
    {
        repot->eventType |= 1<<0;
    }
    if (param->adv.scannable)
    {
        repot->eventType |= 1<<1;
    }
    if (param->adv.directed)
    {
        repot->eventType |= 1<<2;
    }
    if (param->adv.scan_rsp)
    {
        repot->eventType |= 1<<3;
    }

    if (param->adv.cmpl_adv_data)
    {
        repot->dataStatus = OHOS_BLE_DATA_COMPLETE;
    }
    else if (param->adv.incmpl_adv_data)
    {
        repot->dataStatus = OHOS_BLE_DATA_INCOMPLETE_MORE_TO_COME;
    }
    else if (param->adv.has_more_data)
    {
        repot->dataStatus = OHOS_BLE_DATA_INCOMPLETE_TRUNCATED;
    }

    repot->addrType            = param->peer_type;
    repot->primaryPhy          = param->adv.primary_phy;
    repot->secondaryPhy        = param->adv.secondary_phy;
    repot->advSid              = param->adv_set_id;
    repot->txPower             = param->tx_power;
    repot->rssi                = param->rssi;
    repot->periodicAdvInterval = param->pa_interval;
    repot->directAddrType      = param->direct_addr_type;
    repot->advLen              = param->data_length;
    repot->advData             = (uint8_t *)param->data;
    memcpy(repot->addr.addr, param->peer_addr.address, OHOS_BD_ADDR_LEN);
    memcpy(repot->directAddr.addr, param->direct_addr.address, OHOS_BD_ADDR_LEN);
}

static int BleGapScanFilter(const gap_adv_report_t * param, BleScanNativeFilter *FilterInfo, int filter_num)
{
    bool ReportFlag = true;
    uint8_t FilterNameLen;

    for(int i=0; i<filter_num; ++i)
    {
        if (FilterInfo[i].address)
        {
            if (memcmp(param->peer_addr.address, FilterInfo[i].address, 6))
            {
                ReportFlag = false;
            }
            else
            {
                ReportFlag = true;
            }
        }

        if (ReportFlag && FilterInfo->deviceName)
        {
            FilterNameLen = strlen(FilterInfo->deviceName);
            FilterNameLen = (FilterNameLen > param->parsed.device_name_len)? (param->parsed.device_name_len) : FilterNameLen;
            if (memcmp(FilterInfo->deviceName, param->parsed.device_name, FilterNameLen))
            {
                ReportFlag = false;
            }
            else
            {
                ReportFlag = true;
            }
        }

        if (ReportFlag && FilterInfo->manufactureId)
        {
            if (FilterInfo->manufactureId != param->parsed.manufacturer_company_id)
            {
                ReportFlag = false;
            }
            else
            {
                ReportFlag = true;
            }
        }

        if (ReportFlag)
        {
            break;
        }
    }

    return ReportFlag;
}

static int BleGapScanCallback(uintptr_t scan, gap_scan_event_t event, gap_scan_callback_param_t param)
{
    osMutexWait(BleGattEnv.mutex_id, osWaitForever);
    if (!BleGattEnv.ScanFuncCb)
    {
        osMutexRelease(BleGattEnv.mutex_id);
        return 0;
    }
    osMutexRelease(BleGattEnv.mutex_id);

    switch (event)
    {
        case GAP_SCAN_EVENT_STARTED:
        {
            BleGattEnv.ScanFuncCb->scanStateChangeCb(param.scan_started->error_code, 1);
        } break;
        case GAP_SCAN_EVENT_STOPPED:
        {
            BleGattEnv.ScanFuncCb->scanStateChangeCb(param.scan_stopped->error_code, 0);
            osMutexWait(BleGattEnv.mutex_id, osWaitForever);
            BleGattEnv.FilterSize = 0;
            BleGattEnv.ScanFuncCb = NULL;
            memset(&BleGattEnv.FilterInfo, 0, sizeof(BleGattEnv.FilterInfo));
            osMutexRelease(BleGattEnv.mutex_id);
        } break;
        case GAP_SCAN_EVENT_ADV_REPORT:
        {
            bool ReportFlag = true;
            BtScanResultData report_param = {0};

            if (BleGattEnv.FilterSize)
            {
                ReportFlag = BleGapScanFilter(param.adv_report, &BleGattEnv.FilterInfo, BleGattEnv.FilterSize);
            }

            if (!ReportFlag)
            {
                return 0;
            }

            BleGapScanPrepareReportParam(param.adv_report, &report_param);
            BleGattEnv.ScanFuncCb->scanResultCb(&report_param);
        } break;
        default:
        {
            TRACE(0, "unkonw scan report event=%d", event);
        }
    }

    return 0;
}

static int BleAdvStartCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    int State;
    int AdvId;

    osMutexWait(BleGattEnv.mutex_id, osWaitForever);
    AdvId = param.adv_started->adv_handle;
    BleGattEnv.AdvEnabled = true;
    osMutexRelease(BleGattEnv.mutex_id);
    if (ADV_ID_INVALID != AdvId)
    {
        State = (param.adv_started->error_code)? OHOS_BT_STATUS_FAIL:OHOS_BT_STATUS_SUCCESS;
        BleGattEnv.AdvFuncCb->advEnableCb(AdvId, State);
    }

    return BT_STS_SUCCESS;
}

static int BleAdvStopCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    int State;
    int AdvId;

    osMutexWait(BleGattEnv.mutex_id, osWaitForever);
    AdvId = param.adv_started->adv_handle;
    BleGattEnv.AdvEnabled = false;
    osMutexRelease(BleGattEnv.mutex_id);
    if (ADV_ID_INVALID != AdvId)
    {
        State = (param.adv_stopped->error_code)? OHOS_BT_STATUS_FAIL:OHOS_BT_STATUS_SUCCESS;
        BleGattEnv.AdvFuncCb->advDisableCb(AdvId, State);
    }

    return BT_STS_SUCCESS;
}

static int BleAdvSetDataCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    int State;
    int AdvId;

    osMutexWait(BleGattEnv.mutex_id, osWaitForever);
    if (!BleGattEnv.SetData)
    {
        osMutexRelease(BleGattEnv.mutex_id);
        return BT_STS_FAILED;
    }
    else
    {
        BleGattEnv.SetData &= ~(1<<param.adv_set_data->data_type);
    }
    osMutexRelease(BleGattEnv.mutex_id);

    if (!BleGattEnv.SetData)
    {
        AdvId = param.adv_started->adv_handle;
        if ((ADV_ID_INVALID != AdvId) && (param.adv_set_data->data_type == GAP_ADV_RSP_DATA))
        {
            State = (param.adv_set_data->error_code)? OHOS_BT_STATUS_FAIL:OHOS_BT_STATUS_SUCCESS;
            BleGattEnv.AdvFuncCb->advDataCb(AdvId, State);
        }
    }

    return BT_STS_SUCCESS;
}

static int BleAdvScanReqCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: hdl=%d, addr_type=%d", __FUNCTION__, __LINE__,
        param.scan_req->adv_handle, param.scan_req->peer_type);
    DUMP8(" %02x", param.scan_req->peer_addr.address, 6);

    return BT_STS_SUCCESS;
}

static int BleAdvPaEnableCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: hdl=%d, error_code=%d", __FUNCTION__, __LINE__,
        param.pa_enabled->adv_handle, param.pa_enabled->error_code);

    return BT_STS_SUCCESS;
}

static int BleAdvPaDisabledCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: hdl=%d, error_code=%d", __FUNCTION__, __LINE__,
        param.pa_disable->adv_handle, param.pa_disable->error_code);

    return BT_STS_SUCCESS;
}

static int BleAdvConnOpenedCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d, error_code=%d", __FUNCTION__, __LINE__,
        param.conn_opened->con_idx, param.conn_opened->connhdl, param.conn_opened->error_code);
    DUMP8(" %02x", param.conn_opened->peer_addr.address, 6);

    return BT_STS_SUCCESS;
}

static int BleAdvConnClosedCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d, error_code=%d", __FUNCTION__, __LINE__,
        param.conn_closed->con_idx, param.conn_closed->connhdl, param.conn_closed->error_code);
    DUMP8(" %02x", param.conn_closed->peer_addr.address, 6);

    return BT_STS_SUCCESS;
}

static int BleAdvConnFailedCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: role=%d, error_code=%d", __FUNCTION__, __LINE__,
        param.conn_failed->role, param.conn_failed->error_code);
    DUMP8(" %02x", param.conn_failed->peer_addr.address, 6);

    return BT_STS_SUCCESS;
}

static int BleAdvConnMtuExchangedCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d, mtu=%d", __FUNCTION__, __LINE__,
        param.mtu_exchanged->con_idx, param.mtu_exchanged->connhdl, param.mtu_exchanged->mtu);

    return BT_STS_SUCCESS;
}

static int BleAdvUserConfirmCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.user_confirm->con_idx, param.user_confirm->connhdl);

    return BT_STS_SUCCESS;
}

static int BleAdvConnEncryptedCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.conn_encrypted->con_idx, param.conn_encrypted->connhdl);

    return BT_STS_SUCCESS;
}

static int BleAdvRecvKeyDistCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.recv_key_dist->con_idx, param.recv_key_dist->connhdl);

    return BT_STS_SUCCESS;
}

static int BleAdvConnUpdateCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.conn_update_req->con_idx, param.conn_update_req->connhdl);

    return BT_STS_SUCCESS;
}

static int BleAdvConnParamsUpdateCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.conn_params_update->con_idx, param.conn_params_update->connhdl);

    return BT_STS_SUCCESS;
}

static int BleAdvConnSubrateChangeCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.conn_subrate_change->con_idx, param.conn_subrate_change->connhdl);

    return BT_STS_SUCCESS;
}

static int BleAdvConnPhyUpdateCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.conn_phy_update->con_idx, param.conn_phy_update->connhdl);

    return BT_STS_SUCCESS;
}

static int BleAdvTxPowerReportCb(uintptr_t adv, gap_adv_callback_param_t param)
{
    LOG_I("[%s][%d]: id=%d, hdl=%d", __FUNCTION__, __LINE__,
        param.tx_power->con_idx, param.tx_power->connhdl);

    return BT_STS_SUCCESS;
}

static const OhosAdvStackEventList_T StackEventHandleList[] =
{
    {GAP_ADV_EVENT_STARTED, BleAdvStartCb},
    {GAP_ADV_EVENT_STOPPED, BleAdvStopCb},
    {GAP_ADV_EVENT_SET_DATA, BleAdvSetDataCb},
    {GAP_ADV_EVENT_SCAN_REQ, BleAdvScanReqCb},
    {GAP_ADV_EVENT_PA_ENABLED, BleAdvPaEnableCb},
    {GAP_ADV_EVENT_PA_DISABLED, BleAdvPaDisabledCb},
    {GAP_ADV_EVENT_CONN_OPENED, BleAdvConnOpenedCb},
    {GAP_ADV_EVENT_CONN_CLOSED, BleAdvConnClosedCb},
    {GAP_ADV_EVENT_CONN_FAILED, BleAdvConnFailedCb},
    {GAP_ADV_EVENT_CONN_MTU_EXCHANGED, BleAdvConnMtuExchangedCb},
    {GAP_ADV_EVENT_USER_CONFIRM, BleAdvUserConfirmCb},
    {GAP_ADV_EVENT_CONN_ENCRYPTED, BleAdvConnEncryptedCb},
    {GAP_ADV_EVENT_RECV_KEY_DIST, BleAdvRecvKeyDistCb},
    {GAP_ADV_EVENT_CONN_UPDATE_REQ, BleAdvConnUpdateCb},
    {GAP_ADV_EVENT_CONN_PARAMS_UPDATE, BleAdvConnParamsUpdateCb},
    {GAP_ADV_EVENT_CONN_SUBRATE_CHANGE, BleAdvConnSubrateChangeCb},
    {GAP_ADV_EVENT_CONN_PHY_UPDATE, BleAdvConnPhyUpdateCb},
    {GAP_ADV_EVENT_TX_POWER_REPORT, BleAdvTxPowerReportCb},
};

static int BleGapAdvCallback(uintptr_t adv, gap_adv_event_t event, gap_adv_callback_param_t param)
{
    int ret = BT_STS_SUCCESS;

    for (int i=0; i < BleGattEnv.AdvEventNum; ++i)
    {
        if (event == StackEventHandleList[i].event)
        {
            ret = StackEventHandleList[i].BleAdvCallback(adv, param);
            break;
        }
    }

    return ret;
}

/****************************Api function implementation****************************/
int BleRegisterScanCallbacks(BleScanCallbacks *func, int32_t *ScannerId)
{
    if (!BleGattEnv.mutex_id)
    {
        BleGattEnv.mutex_id = osMutexCreate(&BleGattEnv.mutex);
    }

    osMutexWait(BleGattEnv.mutex_id, osWaitForever);
    BleGattEnv.ScanFuncCb = func;
    osMutexRelease(BleGattEnv.mutex_id);

    *ScannerId = 0;

    return OHOS_BT_STATUS_SUCCESS;
}

int BleDeregisterScanCallbacks(int32_t ScannerId)
{
    if (BleGattEnv.ScanFuncCb)
    {
        osMutexWait(BleGattEnv.mutex_id, osWaitForever);
        BleGattEnv.ScanFuncCb = NULL;
        osMutexRelease(BleGattEnv.mutex_id);
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattRegisterCallbacks(BtGattCallbacks *func)
{
    if (!BleGattEnv.mutex_id)
    {
        BleGattEnv.mutex_id = osMutexCreate(&BleGattEnv.mutex);
    }

    osMutexWait(BleGattEnv.mutex_id, osWaitForever);
    BleGattEnv.AdvFuncCb = func;
    osMutexRelease(BleGattEnv.mutex_id);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleStartScanEx(int32_t ScannerId, const BleScanConfigs *configs, const BleScanNativeFilter *filter,
    uint32_t FilterSize)
{
    int ret;
    gap_scan_param_t param = {0};

    // save user param
    osMutexWait(BleGattEnv.mutex_id, osWaitForever);
    BleGattEnv.ScannerId = ScannerId;
    BleGattEnv.FilterSize = FilterSize;
    memcpy(&BleGattEnv.FilterInfo, filter, sizeof(BleScanNativeFilter));
    osMutexRelease(BleGattEnv.mutex_id);

    // call stack api
    param.own_addr_type     = BT_ADDR_TYPE_PUBLIC;
    param.continue_scanning = 1;
    param.filter_duplicated = 0;
    param.duration_ms       = 0;
    param.legacy            = configs->legacy;
    param.phys              = configs->phy;
    if (OHOS_BLE_SCAN_MODE_LOW_POWER == configs->scanMode)
    {
        param.slow_scan = 1;
    }
    ret = gap_start_background_scanning(&param, BleGapScanCallback);
    if (BT_STS_SUCCESS != ret)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int BleStopScan(int32_t ScannerId)
{
    int ret;

    if (BleGattEnv.ScannerId != ScannerId)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    ret = gap_disable_scanning();
    if (BT_STS_SUCCESS != ret)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int BleSetAdvData(int advId, const StartAdvRawData data)
{
    int ret;

    if (data.advData && data.advDataLen)
    {
        ret = gap_set_ext_adv_data((uint8_t)advId,
              GAP_ADV_DATA_COMPLETE, data.advData, data.advDataLen);
        if (BT_STS_SUCCESS != ret)
        {
            TRACE(0, "[%s][%d]: set adv data fail=0x%x", __FUNCTION__, __LINE__, ret);
            return OHOS_BT_STATUS_FAIL;
        }
        osMutexWait(BleGattEnv.mutex_id, osWaitForever);
        BleGattEnv.SetData |= (1<<GAP_ADV_DATA);
        osMutexRelease(BleGattEnv.mutex_id);
    }

    if (data.rspData && data.rspDataLen)
    {
        ret = gap_set_ext_scan_rsp_data((uint8_t)advId,
              GAP_ADV_DATA_COMPLETE, data.rspData, data.rspDataLen);
        if (BT_STS_SUCCESS != ret)
        {
            TRACE(0, "[%s][%d]: set adv rsp data fail=0x%x", __FUNCTION__, __LINE__, ret);
            return OHOS_BT_STATUS_FAIL;
        }
        osMutexWait(BleGattEnv.mutex_id, osWaitForever);
        BleGattEnv.SetData |= (1<<GAP_ADV_RSP_DATA);
        osMutexRelease(BleGattEnv.mutex_id);
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int BleStartAdvEx(int *advId, const StartAdvRawData rawData, BleAdvParams advParam)
{
    int ret = BT_STS_SUCCESS;
    uint8_t adv_handle;
    gap_adv_param_t param = {0};

    param.fast_advertising = 1;
    param.adv_timing.min_adv_fast_interval_ms = advParam.minInterval *5/8;
    param.adv_timing.max_adv_fast_interval_ms = advParam.maxInterval *5/8;

    switch(advParam.advType)
    {
        case OHOS_BLE_ADV_IND:
            param.connectable = 1;
            param.scannable = 1;
        break;
        case OHOS_BLE_ADV_DIRECT_IND_HIGH:
            param.connectable = 1;
            param.directed_adv = 1;
            param.high_duty_directed_adv = 1;
        break;
        case OHOS_BLE_ADV_SCAN_IND:
            param.scannable = 1;
        break;
        case OHOS_BLE_ADV_NONCONN_IND:
            param.scannable = 1;
        break;
        case OHOS_BLE_ADV_DIRECT_IND_LOW:
            param.connectable = 1;
            param.directed_adv = 1;
        break;
        default:
            break;
    }

    param.own_addr_type = advParam.ownAddrType;
    param.peer_type = advParam.peerAddrType;
    memcpy(param.peer_addr.address, advParam.peerAddr.addr, 6);
    param.policy = advParam.advFilterPolicy;
    param.adv_channel_map = advParam.channelMap;
    param.adv_tx_power = advParam.txPower;
    param.duration_ms = advParam.duration;

    adv_handle = gap_set_adv_parameters(&param, BleGapAdvCallback);
    if (GAP_INVALID_ADV_HANDLE == adv_handle)
    {
        LOG_E("[%s][%d]: set adv param fail hdl==0x%x", __FUNCTION__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }

    if (rawData.advData && rawData.advDataLen)
    {
        ret = gap_set_ext_adv_data(adv_handle, GAP_ADV_DATA_COMPLETE, rawData.advData, rawData.advDataLen);
        if (BT_STS_SUCCESS != ret)
        {
            LOG_E("[%s][%d]: set adv data fail=0x%x", __FUNCTION__, __LINE__, ret);
            return OHOS_BT_STATUS_FAIL;
        }
    }

    if (rawData.rspData && rawData.rspDataLen)
    {
        ret = gap_set_ext_scan_rsp_data(adv_handle, GAP_ADV_DATA_COMPLETE, rawData.rspData, rawData.rspDataLen);
        if (BT_STS_SUCCESS != ret)
        {
            LOG_E("[%s][%d]: set rsp adv data fail=0x%x", __FUNCTION__, __LINE__, ret);
            return OHOS_BT_STATUS_FAIL;
        }
    }

    ret = gap_set_ext_adv_enable(adv_handle, advParam.duration, 0);
    if (BT_STS_SUCCESS != ret)
    {
        LOG_E("[%s][%d]: set enable fail=0x%x", __FUNCTION__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }

    if (!BleGattEnv.AdvEventNum)
    {
        BleGattEnv.AdvEventNum = ARRAY_SIZE(StackEventHandleList);
    }
    *advId = (int)adv_handle;
    return OHOS_BT_STATUS_SUCCESS;
}

int BleStopAdv(int advId)
{
    int ret;

    if (!gap_is_advertising_enabled((uint8_t)advId))
    {
        return OHOS_BT_STATUS_SUCCESS;
    }

    ret = gap_disable_advertising((uint8_t)advId);
    if (BT_STS_SUCCESS != ret)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int GetAdvHandle(int advId, int *advHandle)
{
    *advHandle = advId;
    return OHOS_BT_STATUS_SUCCESS;
}

int InitBtStack(void)
{
    //bes_bt_thread_init();
    return 0;
}

int EnableBtStack(void)
{
    return BT_STS_SUCCESS;
}

int DisableBtStack(void)
{
    return BT_STS_SUCCESS;
}

// No supported Lp feature
bool IsLpDeviceAvailable(void)
{
    return 0;
}

int SetLpDeviceParam(const BtLpDeviceParam *lpDeviceParam)
{
    return OHOS_BT_STATUS_UNSUPPORTED;
}

int SetLpDeviceAdvParam(int duration, int maxExtAdvEvents, int window, int interval, int advHandle)
{
    return OHOS_BT_STATUS_UNSUPPORTED;
}

int EnableSyncDataToLpDevice(void)
{
    return OHOS_BT_STATUS_UNSUPPORTED;
}

int DisableSyncDataToLpDevice(void)
{
    return OHOS_BT_STATUS_UNSUPPORTED;
}

int SetScanReportChannelToLpDevice(int32_t ScannerId, bool enable)
{
    return OHOS_BT_STATUS_UNSUPPORTED;
}


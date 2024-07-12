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
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "plat_types.h"
#include "hal_trace.h"
#include "eshell.h"
#include "factory_section.h"
#include "adapter_service.h"
#include "bes_me_api.h"

#include "ohos_bt_utils.h"
#include "ohos_bt_debug.h"
#include "ohos_bt_gap.h"
#include "ohos_bt_gatt.h"
#include "ohos_bt_gatt_server.h"
#include "ohos_bt_gatt_client.h"
#include "softbus_adapter_ble_gatt_server.h"

/****************************Macro defination***************************/
static const int MAX_SERVICE_CHAR_NUM = 8;

/****************************Type defination****************************/
typedef struct
{
    int adv_id;
    int scanner_id;
    // client env
    int client_id;
    int client_conn_state;

    // service env
    int service_id;
    int service_conn_id;
    BdAddr bdAddr;
    int srvcHandle;
    int char_hdl1;
    int desc_hdl1;
    int char_hdl2;
    int desc_hdl2;
} harmony_test_env_t;

/****************************Function declaration***********************/
#define SCAN_FILTER_DEV_NAME1 "fjr_ble_test1"
#define SCAN_FILTER_DEV_NAME1_1 "fjr_ble_test1_1"
#define SCAN_FILTER_DEV_NAME2 "fjr_ble_test2"
#define SCAN_FILTER_DEV_NAME2_2 "fjr_ble_test2"

/****************************Variable defination************************/
static char scan_filter_addr1[6] = {0x91,0x33,0x33,0x23,0x22,0x11};
static char scan_filter_addr2[6] = {0x92,0x33,0x33,0x23,0x22,0x11};

#define SOFTBUS_SERVICE_UUID "11C8B310-80E4-4276-AFC0-F81590B2177F"
#define SOFTBUS_CHARA_BLENET_UUID "00002B00-0000-1000-8000-00805F9B34FB"
#define SOFTBUS_CHARA_BLECONN_UUID "00002B01-0000-1000-8000-00805F9B34FB"
#define SOFTBUS_DESCRIPTOR_CONFIGURE_UUID "00002902-0000-1000-8000-00805F9B34FB"

static harmony_test_env_t ohos_test_env = {0};

/****************************Private function implementation************************/
static void OhosScanResultCb(BtScanResultData *scanResultdata)
{
    LOG_I("[%s][%d]: scan dev addr_type=%d", __func__, __LINE__, scanResultdata->addrType);
    DUMP8("%02x ", scanResultdata->addr.addr, 6);
}

static void OhosScanStateChangeCb(int32_t resultCode, bool isStartScan)
{
    LOG_I("[%s][%d]: result=%ld, en=%d", __func__, __LINE__, resultCode, isStartScan);
}

static BleScanCallbacks ohos_scan_callback =
{
    .scanResultCb = OhosScanResultCb,
    .scanStateChangeCb = OhosScanStateChangeCb,
};

static void OhosAdvEnableCb(int advId, int status)
{
    LOG_I("[%s][%d]: advId=%d, state=%d", __func__, __LINE__, advId, status);
}

static void OhosAdvDisableCb(int advId, int status)
{
    LOG_I("[%s][%d]: advId=%d, state=%d", __func__, __LINE__, advId, status);
}

static void OhosadvDataCb(int advId, int status)
{
    LOG_I("[%s][%d]: advId=%d, state=%d", __func__, __LINE__, advId, status);
}

static BtGattCallbacks ohos_adv_callback =
{
    .advEnableCb = OhosAdvEnableCb,
    .advDisableCb = OhosAdvDisableCb,
    .advDataCb = OhosadvDataCb,
};

static void OhosConnectionStateCb(int clientId, int connectionState, int status)
{
    LOG_I("[%s][%d]: conn_state=%d, state=%d", __func__, __LINE__, connectionState, status);

    if (OHOS_STATE_CONNECTED == connectionState)
    {
        ohos_test_env.client_conn_state = connectionState;
    } else if (OHOS_STATE_DISCONNECTED == connectionState)
    {
        ohos_test_env.client_conn_state = connectionState;
    }
}

static void OhosConnectParaUpdateCb(int clientId, int interval, int latency, int timeout, int status)
{
    LOG_I("[%s][%d]: itv=%d,let=%d,to=%d,state=%d", __func__, __LINE__,
        interval, latency, timeout, status);
}

static void OhosSearchServiceCompleteCb(int clientId, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosReadCharacteristicCb(int clientId, BtGattReadData *readData, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosWriteCharacteristicCb(int clientId, BtGattCharacteristic *characteristic, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosReadDescriptorCb(int clientId, BtGattReadData *readData, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosWriteDescriptorCb(int clientId, BtGattDescriptor *descriptor, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosConfigureMtuSizeCb(int clientId, int mtuSize, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosRegisterNotificationCb(int clientId, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosNotificationCb(int clientId, BtGattReadData *notifyData, int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
    LOG_I("[%s][%d]: srv=%s", __func__, __LINE__, notifyData->attribute.characteristic.serviceUuid.uuid);
    LOG_I("[%s][%d]: char=%s", __func__, __LINE__, notifyData->attribute.characteristic.characteristicUuid.uuid);

    DUMP8("%02x ", notifyData->data, notifyData->dataLen);

}

static BtGattClientCallbacks ohos_client_callback =
{
    .ConnectionStateCb       = OhosConnectionStateCb,
    .connectParaUpdateCb     = OhosConnectParaUpdateCb,
    .searchServiceCompleteCb = OhosSearchServiceCompleteCb,
    .readCharacteristicCb    = OhosReadCharacteristicCb,
    .writeCharacteristicCb   = OhosWriteCharacteristicCb,
    .readDescriptorCb        = OhosReadDescriptorCb,
    .writeDescriptorCb       = OhosWriteDescriptorCb,
    .configureMtuSizeCb      = OhosConfigureMtuSizeCb,
    .registerNotificationCb  = OhosRegisterNotificationCb,
    .notificationCb          = OhosNotificationCb,
};

static void OhosRegisterServerCb(int status, int serverId, BtUuid *appUuid)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosConnectServerCb(int connId, int serverId, const BdAddr *bdAddr)
{
    LOG_I("[%s][%d]: connid=%d", __func__, __LINE__, connId);
    DUMP8("%02x ", bdAddr->addr, 6);

    ohos_test_env.service_conn_id = connId;
    memcpy(ohos_test_env.bdAddr.addr, bdAddr->addr, 6);
}

static void OhosDisconnectServerCb(int connId, int serverId, const BdAddr *bdAddr)
{
    LOG_I("[%s][%d]: connid=%d", __func__, __LINE__, connId);
    DUMP8("%02x ", bdAddr->addr, 6);

    ohos_test_env.service_conn_id = 0xFF;
    memset(ohos_test_env.bdAddr.addr, 0, 6);
}

static void OhosServiceAddCb(int status, int serverId, BtUuid *uuid, int srvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d", __func__, __LINE__, status, srvcHandle);
    ohos_test_env.srvcHandle = srvcHandle;
}

static void OhosIncludeServiceAddCb(int status, int serverId, int srvcHandle, int includeSrvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d,inc_hdl=%d", __func__, __LINE__, status, srvcHandle, includeSrvcHandle);
}

static void OhosCharacteristicAddCb(int status, int serverId, BtUuid *uuid,
                                          int srvcHandle, int characteristicHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d,char_hdl=%d", __func__, __LINE__, status, srvcHandle, characteristicHandle);
    if (!strcmp(SOFTBUS_CHARA_BLENET_UUID, uuid->uuid))
    {
        ohos_test_env.char_hdl1 = characteristicHandle;
    }
    else if(!strcmp(SOFTBUS_CHARA_BLECONN_UUID, uuid->uuid))
    {
        ohos_test_env.char_hdl2 = characteristicHandle;
    }
}

static void  OhosDescriptorAddCb(int status, int serverId, BtUuid *uuid,
                                    int srvcHandle, int descriptorHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d,desc_hdl=%d", __func__, __LINE__, status, srvcHandle, descriptorHandle);
    if (!strcmp(SOFTBUS_DESCRIPTOR_CONFIGURE_UUID, uuid->uuid))
    {
        if (!ohos_test_env.desc_hdl1)
        {
            ohos_test_env.desc_hdl1 = descriptorHandle;
        }
        else
        {
            ohos_test_env.desc_hdl2 = descriptorHandle;
        }
    }
}

static void OhosServiceStartCb(int status, int serverId, int srvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d", __func__, __LINE__, status, srvcHandle);
}

static void OhosServiceStopCb(int status, int serverId, int srvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d", __func__, __LINE__, status, srvcHandle);
}

static void OhosServiceDeleteCb(int status, int serverId, int srvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d", __func__, __LINE__, status, srvcHandle);
}

static void OhosRequestReadCb(BtReqReadCbPara readCbPara)
{
    LOG_I("[%s][%d]: conn=%d,attr_hdl=%d", __func__, __LINE__, readCbPara.connId, readCbPara.attrHandle);
    char rsp_data[] = {1,3,5,7,9};
    GattsSendRspParam rsp_param = {0};

    rsp_param.connectId = readCbPara.connId;
    rsp_param.status    = BT_STS_SUCCESS;
    rsp_param.attrHandle = readCbPara.attrHandle;
    rsp_param.valueLen = sizeof(rsp_data);
    rsp_param.value    = rsp_data;
    BleGattsSendResponse(ohos_test_env.service_id, &rsp_param);
}

static void OhosRequestWriteCb(BtReqWriteCbPara writeCbPara)
{
    LOG_I("[%s][%d]: conn=%d,attr_hdl=%d", __func__, __LINE__, writeCbPara.connId, writeCbPara.attrHandle);
    DUMP8("0x%x ", writeCbPara.value, writeCbPara.length);

    if (writeCbPara.needRsp)
    {
        char rsp_data[] = {1,3,5,7,9};
        GattsSendRspParam rsp_param = {0};

        rsp_param.connectId = writeCbPara.connId;
        rsp_param.status    = BT_STS_SUCCESS;
        rsp_param.attrHandle = writeCbPara.attrHandle;
        rsp_param.valueLen = sizeof(rsp_data);
        rsp_param.value    = rsp_data;
        BleGattsSendResponse(ohos_test_env.service_id, &rsp_param);
    }
}

static void OhosResponseConfirmationCb(int status, int handle)
{
    LOG_I("[%s][%d]: attr_hdl=%d", __func__, __LINE__, handle);
}

static void OhosIndicationSentCb(int connId, int status)
{
    LOG_I("[%s][%d]: status=%d", __func__, __LINE__, status);
}

static void OhosMtuChangeCb(int connId, int mtu)
{
    LOG_I("[%s][%d]: mtu=%d", __func__, __LINE__, mtu);
}

static BtGattServerCallbacks ohos_service_callback =
{
    .registerServerCb = OhosRegisterServerCb,
    .connectServerCb = OhosConnectServerCb,
    .disconnectServerCb = OhosDisconnectServerCb,
    .serviceAddCb = OhosServiceAddCb,
    .includeServiceAddCb = OhosIncludeServiceAddCb,
    .characteristicAddCb = OhosCharacteristicAddCb,
    .descriptorAddCb = OhosDescriptorAddCb,
    .serviceStartCb = OhosServiceStartCb,
    .serviceStopCb  = OhosServiceStopCb,
    .serviceDeleteCb = OhosServiceDeleteCb,
    .requestReadCb = OhosRequestReadCb,
    .requestWriteCb = OhosRequestWriteCb,
    .responseConfirmationCb = OhosResponseConfirmationCb,
    .indicationSentCb = OhosIndicationSentCb,
    .mtuChangeCb  = OhosMtuChangeCb,
};

static void OhosGapStateChangedCb(const int transport, const int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static  void OhosGapAclStateChangedCb(const BdAddr *bdAddr, GapAclState state, unsigned int reason)
{
    LOG_I("[%s][%d]: state=%d, reason=%d", __func__, __LINE__, state, reason);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapDiscoveryStateChangedCb(int status)
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__, status);
}

static void OhosGapDiscoveryResultCb(const BdAddr *bdAddr)
{
    LOG_I("[%s][%d]:", __func__, __LINE__);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapPairRequestedCb(const BdAddr *bdAddr, int transport)
{
    LOG_I("[%s][%d]: transport=%d", __func__, __LINE__ , transport);
    DUMP8("%02x ", bdAddr, 6);
    PairRequestReply(bdAddr, transport, true);
}

static void OhosGapPairConfirmedCb(const BdAddr *bdAddr, int transport, int reqType, int number)
{
    LOG_I("[%s][%d]: %d,%d,%x", __func__, __LINE__ , transport, reqType, number);
    DUMP8("%02x ", bdAddr, 6);
    SetDevicePairingConfirmation(bdAddr, transport, true);
}

static void OhosGapScanModeChangedCb(int mode)
{
    LOG_I("[%s][%d]: mode=%d", __func__, __LINE__ , mode);
}

static void OhosGapLocalDeviceNameChangedCb(const unsigned char *deviceName, unsigned char length)
{
    LOG_I("[%s][%d]: len=%d, %s", __func__, __LINE__, length, deviceName);
}

static void OhosGapLocalDeviceAddrChangedCb(const BdAddr *bdAddr)
{
    LOG_I("[%s][%d]: ", __func__, __LINE__);
    DUMP8("%02x ", bdAddr, 6);
}


static void OhosGapPairStatusChangedCb(const BdAddr *bdAddr, int status)
{
    LOG_I("[%s][%d]: status=%d", __func__, __LINE__, status);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapRemoteUuidChangedCb(const BdAddr *bdAddr, BtUuid uuid)
{
    LOG_I("[%s][%d]:", __func__, __LINE__);
    DUMP8("%02x ", uuid.uuid, uuid.uuidLen);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapRemoteNameChangedCb(const BdAddr *bdAddr,
    const unsigned char *deviceName, unsigned char length)
{
    LOG_I("[%s][%d]:length=%d, %s", __func__, __LINE__, length, deviceName);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapRemoteAliasChangedCb(const BdAddr *bdAddr, const unsigned char *alias, unsigned char length)
{
    LOG_I("[%s][%d]:", __func__, __LINE__);
    DUMP8("%02x ", bdAddr, 6);
    DUMP8("%02x ", alias, length);
}

static void OhosGapRemoteCodChangedCb(const BdAddr *bdAddr, int cod)
{
    LOG_I("[%s][%d]:cod=0x%x", __func__, __LINE__, cod);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapRemoteBatteryLevelChangedCb(const BdAddr *bdAddr, int batteryLevel)
{
    LOG_I("[%s][%d]:batteryLevel=0x%x", __func__, __LINE__, batteryLevel);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapReadRemoteRssiEventCb(const BdAddr *bdAddr, int rssi, int status)
{
    LOG_I("[%s][%d]:rssi=%d, status=%d", __func__, __LINE__, rssi, status);
    DUMP8("%02x ", bdAddr, 6);
}

static void OhosGapIsAcceptConnOnSafeModeCb(const BdAddr *bdAddr, bool *res)
{
    LOG_I("[%s][%d]:res=%d", __func__, __LINE__, *res);
    DUMP8("%02x ", bdAddr, 6);
}

static BtGapCallBacks ohos_gap_callback =
{
    .stateChangeCallback               = OhosGapStateChangedCb,
    .aclStateChangedCallbak            = OhosGapAclStateChangedCb,
    .scanModeChangedCallback           = OhosGapScanModeChangedCb,
    .pairStatusChangedCallback         = OhosGapPairStatusChangedCb,
    .deviceNameChangedCallback         = OhosGapLocalDeviceNameChangedCb,
    .deviceAddrChangedCallback         = OhosGapLocalDeviceAddrChangedCb,
    .pairRequestedCallback             = OhosGapPairRequestedCb,
    .pairConfiremedCallback            = OhosGapPairConfirmedCb,
    .discoveryStateChangedCallback     = OhosGapDiscoveryStateChangedCb,
    .discoveryResultCallback           = OhosGapDiscoveryResultCb,
    .remoteUuidChangedCallback         = OhosGapRemoteUuidChangedCb,
    .remoteNameChangedCallback         = OhosGapRemoteNameChangedCb,
    .remoteAliasChangedCallback        = OhosGapRemoteAliasChangedCb,
    .remoteCodChangedCallback          = OhosGapRemoteCodChangedCb,
    .remoteBatteryLevelChangedCallback = OhosGapRemoteBatteryLevelChangedCb,
    .readRemoteRssiEventCallback       = OhosGapReadRemoteRssiEventCb,
    .isAcceptConnOnSafeModeCallback    = OhosGapIsAcceptConnOnSafeModeCb,
};

static inline void bt_cmd_param_parse(int argc, char *argv[], int *data)
{
    int param_num = argc-1;
    LOG_I("[test] param_num=%d\r\n", param_num);
    for (int i=0; i<param_num; ++i)
    {
        sscanf(argv[i+1], "%d", data+i);
    }
}

static void bt_init_stack(int argc, char *argv[])
{
    LOG_I("[test] bt stack init");
    InitBtStack();

    GapRegisterCallbacks(&ohos_gap_callback);
    BleGattRegisterCallbacks(&ohos_adv_callback);
    BleRegisterScanCallbacks(&ohos_scan_callback, &ohos_test_env.scanner_id);

    // gatts callback
    BtUuid appUuid;
    char uuid[2] = {0x01, 0x02};
    appUuid.uuidLen = 2;
    appUuid.uuid = uuid;
    BleGattsRegisterCallbacks(&ohos_service_callback);
    ohos_test_env.service_id = BleGattsRegister(appUuid);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_stack_init", "none", bt_init_stack);

static void bt_set_address(int argc, char *argv[])
{
    int param[6] = {0};
    uint8_t bt_addr[6] = {0};

    bt_cmd_param_parse(argc, argv, param);
    bt_addr[0] = param[0];
    bt_addr[1] = param[1];
    bt_addr[2] = param[2];
    bt_addr[3] = param[3];
    bt_addr[4] = param[4];
    bt_addr[5] = param[5];
    LOG_I("[test] set_address, addr=%x:%x:%x:%x:%x:%x\r\n",
        bt_addr[0], bt_addr[1], bt_addr[2], bt_addr[3],bt_addr[4], bt_addr[5]);

     factory_section_set_bt_address(bt_addr);
     factory_section_set_ble_address(bt_addr);

    bes_bt_me_set_bt_address(bt_addr);
    bes_bt_me_set_ble_address(bt_addr);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_set_address", "none", bt_set_address);

static void bt_ble_adv_start(int argc, char *argv[])
{
    int ret = OHOS_BT_STATUS_SUCCESS;
    int data_len;
    unsigned char advData[20] = {0};
    //unsigned char advRspData[20] = {0};
    StartAdvRawData rawData = {0};
    BleAdvParams advParam   = {0};

    advData[0] = strlen(SCAN_FILTER_DEV_NAME1)+1;
    advData[1] = 0X08;
    memcpy(&advData[2], SCAN_FILTER_DEV_NAME1, advData[0]-1);
    data_len = advData[0] + 1;

    // adv data
    rawData.advData = advData;
    rawData.advDataLen = data_len;
    //rawData.rspData = advRspData;
    //rawData.rspDataLen = sizeof(advRspData);
    rawData.rspData = NULL;
    rawData.rspDataLen = 0;

    // adv param
    advParam.minInterval= 0x20;
    advParam.minInterval= 0x20;
    advParam.advType = OHOS_BLE_ADV_IND;
    advParam.ownAddrType = 0;
    advParam.channelMap = 0x07;
    advParam.advFilterPolicy = OHOS_BLE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
    advParam.txPower = -20;
    advParam.duration = 0;

    ret = BleStartAdvEx(&ohos_test_env.adv_id, rawData, advParam);
    LOG_I("[%s][%d]: advId=%d, state=%d", __func__, __LINE__, 0, ret);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_adv_start", "none", bt_ble_adv_start);

static void bt_ble_adv_stop(int argc, char *argv[])
{
    BleStopAdv(ohos_test_env.adv_id);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_adv_stop", "none", bt_ble_adv_stop);

static void bt_ble_adv_set_data(int argc, char *argv[])
{
    int data_len;
    unsigned char advData[20] = {0};
    //unsigned char advRspData[20] = {0};
    StartAdvRawData rawData = {0};

    advData[0] = strlen(SCAN_FILTER_DEV_NAME1_1)+1;
    advData[1] = 0X08;
    memcpy(&advData[2], SCAN_FILTER_DEV_NAME1_1, advData[0]-1);
    data_len = advData[0] + 1;

    rawData.advData =advData;
    rawData.advDataLen =data_len;
    //rawData.rspData = advRspData;
    //rawData.rspDataLen = sizeof(advRspData);
    rawData.rspData = NULL;
    rawData.rspDataLen = 0;

    BleSetAdvData(ohos_test_env.adv_id, rawData);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_adv_set_data", "none", bt_ble_adv_set_data);

static void bt_ble_scan_start(int argc, char *argv[])
{
    BleScanConfigs configs = {0};
    BleScanNativeFilter filter[2] = {0};

    configs.reportDelayMillis = 0;
    configs.scanMode = OHOS_BLE_SCAN_MODE_LOW_POWER;
    configs.legacy = 0;
    configs.phy = 0x01;

    filter[0].address = scan_filter_addr1;
    filter[0].deviceName = SCAN_FILTER_DEV_NAME1;
    filter[1].address = scan_filter_addr2;
    filter[1].deviceName = SCAN_FILTER_DEV_NAME2;

    BleStartScanEx(ohos_test_env.scanner_id, &configs, filter, sizeof(filter)/sizeof(filter[0]));
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_scan_start", "none", bt_ble_scan_start);

static void bt_ble_scan_stop(int argc, char *argv[])
{
    BleStopScan(ohos_test_env.adv_id);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_scan_stop", "none", bt_ble_scan_stop);

static void bt_ble_cli_creat_conn(int argc, char *argv[])
{
    int ret;

    // gattc callback
    BtUuid appUuid;
    char uuid[2] = {0x01, 0x02};
    appUuid.uuidLen = 2;
    appUuid.uuid = uuid;
    ohos_test_env.client_id = BleGattcRegister(appUuid);

    ret = BleGattcConnect(ohos_test_env.client_id, &ohos_client_callback, (BdAddr *)scan_filter_addr1,
        false, OHOS_BT_TRANSPORT_TYPE_LE);

    LOG_I("[%s][%d]: clientId=%d, state=%d", __func__, __LINE__, ohos_test_env.client_id, ret);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_creat_conn", "none", bt_ble_cli_creat_conn);

static void bt_ble_cli_set_priority(int argc, char *argv[])
{
    BleGattcSetPriority(ohos_test_env.client_id, (BdAddr *)scan_filter_addr1, OHOS_BT_GATT_PRIORITY_HIGH);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_set_priority", "none", bt_ble_cli_set_priority);

static void bt_ble_cli_creat_conn_fast(int argc, char *argv[])
{
    BleGattcSetFastestConn(ohos_test_env.client_id, true);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_creat_conn_fast", "none", bt_ble_cli_creat_conn_fast);

static void bt_ble_cli_cancel_conn(int argc, char *argv[])
{
    BleGattcDisconnect(ohos_test_env.client_id);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_cancel_conn", "none", bt_ble_cli_cancel_conn);

static void bt_ble_cli_disconn(int argc, char *argv[])
{
    BleGattcDisconnect(ohos_test_env.client_id);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_disconn", "none", bt_ble_cli_disconn);

static void bt_ble_cli_search_srv(int argc, char *argv[])
{
    BleGattcSearchServices(ohos_test_env.client_id);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_search_srv", "none", bt_ble_cli_search_srv);

static void bt_ble_cli_get_srv(int argc, char *argv[])
{
    BtUuid Uuid;

    Uuid.uuidLen = strlen(SOFTBUS_SERVICE_UUID);
    Uuid.uuid    = SOFTBUS_SERVICE_UUID;
    BleGattcGetService(ohos_test_env.client_id, Uuid);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_get_srv", "none", bt_ble_cli_get_srv);

static void bt_ble_cli_read(int argc, char *argv[])
{
/*
    int char_num = 0;
    BtUuid Uuid;
    sscanf((char *)BufPtr, "%d", &char_num);

    Uuid.uuidLen = strlen(SOFTBUS_SERVICE_UUID);
    Uuid.uuid    = SOFTBUS_SERVICE_UUID;
    BleGattcGetService(ohos_test_env.client_id, Uuid);
*/
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_read", "none", bt_ble_cli_read);

static void bt_ble_cli_wirte(int argc, char *argv[])
{
    int param[2] = {0};
    int char_num;
    int writeType;
    BtGattCharacteristic char_uuid = {0};
    const char value[] = {5,6,7,8,10};

    bt_cmd_param_parse(argc, argv, param);
    char_num = param[0];
    writeType = param[1];
    char_uuid.serviceUuid.uuidLen = strlen(SOFTBUS_SERVICE_UUID);
    char_uuid.serviceUuid.uuid    = SOFTBUS_SERVICE_UUID;
    if (char_num == 1)
    {
        char_uuid.characteristicUuid.uuidLen = strlen(SOFTBUS_CHARA_BLENET_UUID);
        char_uuid.characteristicUuid.uuid    = SOFTBUS_CHARA_BLENET_UUID;
        BleGattcWriteCharacteristic(ohos_test_env.client_id, char_uuid, writeType, sizeof(value), value);
    }
    else if (char_num == 2)
    {
        char_uuid.characteristicUuid.uuidLen = strlen(SOFTBUS_CHARA_BLECONN_UUID);
        char_uuid.characteristicUuid.uuid    = SOFTBUS_CHARA_BLECONN_UUID;
        BleGattcWriteCharacteristic(ohos_test_env.client_id, char_uuid, writeType, sizeof(value), value);
    }
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_wirte", "none", bt_ble_cli_wirte);

static void bt_ble_cli_reg_ntf(int argc, char *argv[])
{
    int ret;
    int char_num = 0;
    BtGattCharacteristic Uuid;

    bt_cmd_param_parse(argc, argv, &char_num);
    Uuid.serviceUuid.uuidLen = strlen(SOFTBUS_SERVICE_UUID);
    Uuid.serviceUuid.uuid    = SOFTBUS_SERVICE_UUID;

    if (char_num == 1)
    {
        Uuid.characteristicUuid.uuidLen = strlen(SOFTBUS_CHARA_BLENET_UUID);
        Uuid.characteristicUuid.uuid    = SOFTBUS_CHARA_BLENET_UUID;
    }
    else if (char_num == 2)
    {
        Uuid.characteristicUuid.uuidLen = strlen(SOFTBUS_CHARA_BLECONN_UUID);
        Uuid.characteristicUuid.uuid    = SOFTBUS_CHARA_BLECONN_UUID;
    }

    ret = BleGattcRegisterNotification(ohos_test_env.client_id, Uuid, true);
    LOG_I("[%s][%d]: clientId=%d, state=%d", __func__, __LINE__, ohos_test_env.client_id, ret);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_cli_reg_ntf", "none", bt_ble_cli_reg_ntf);

static void bt_ble_srv_add_srv(int argc, char *argv[])
{
    BtUuid Uuid;
    int properties = 0;
    int permissions = 0;

    // add service
    Uuid.uuidLen = strlen(SOFTBUS_SERVICE_UUID);
    Uuid.uuid    = SOFTBUS_SERVICE_UUID;
    BleGattsAddService(ohos_test_env.service_id, Uuid, true, MAX_SERVICE_CHAR_NUM);

    // add Characteristic
    Uuid.uuidLen = strlen(SOFTBUS_CHARA_BLENET_UUID);
    Uuid.uuid    = SOFTBUS_CHARA_BLENET_UUID;
    properties = SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_READ | SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP |
            SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_WRITE | SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY |
            SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_INDICATE;
    permissions = SOFTBUS_GATT_PERMISSION_READ | SOFTBUS_GATT_PERMISSION_WRITE;
    BleGattsAddCharacteristic(ohos_test_env.service_id, ohos_test_env.srvcHandle, Uuid, properties, permissions);

    // add Descriptor
    Uuid.uuidLen = strlen(SOFTBUS_DESCRIPTOR_CONFIGURE_UUID);
    Uuid.uuid    = SOFTBUS_DESCRIPTOR_CONFIGURE_UUID;
    permissions = SOFTBUS_GATT_PERMISSION_READ | SOFTBUS_GATT_PERMISSION_WRITE;
    BleGattsAddDescriptor(ohos_test_env.service_id, ohos_test_env.srvcHandle, Uuid, permissions);

    //add Characteristic
    Uuid.uuidLen = strlen(SOFTBUS_CHARA_BLECONN_UUID);
    Uuid.uuid    = SOFTBUS_CHARA_BLECONN_UUID;
    properties = SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_READ | SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP |
            SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_WRITE | SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY |
            SOFTBUS_GATT_CHARACTER_PROPERTY_BIT_INDICATE;
    permissions = SOFTBUS_GATT_PERMISSION_READ | SOFTBUS_GATT_PERMISSION_WRITE;
    BleGattsAddCharacteristic(ohos_test_env.service_id, ohos_test_env.srvcHandle, Uuid, properties, permissions);

    // add Descriptor
    Uuid.uuidLen = strlen(SOFTBUS_DESCRIPTOR_CONFIGURE_UUID);
    Uuid.uuid    = SOFTBUS_DESCRIPTOR_CONFIGURE_UUID;
    permissions = SOFTBUS_GATT_PERMISSION_READ | SOFTBUS_GATT_PERMISSION_WRITE;
    BleGattsAddDescriptor(ohos_test_env.service_id, ohos_test_env.srvcHandle, Uuid, permissions);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_srv_add_srv", "none", bt_ble_srv_add_srv);

static void bt_ble_srv_start(int argc, char *argv[])
{
    int ret;

    ret = BleGattsStartService(ohos_test_env.service_id, ohos_test_env.srvcHandle);
    LOG_I("[%s][%d]: clientId=%d, state=%d", __func__, __LINE__, ohos_test_env.client_id, ret);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_srv_start", "none", bt_ble_srv_start);

static void bt_ble_srv_stop(int argc, char *argv[])
{
    BleGattsStopService(ohos_test_env.service_id, ohos_test_env.srvcHandle);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_srv_stop", "none", bt_ble_srv_stop);

static void bt_ble_srv_send_ntf(int argc, char *argv[])
{
    int input_param[2] = {0};
    int char_num;
    int confirm;
    GattsSendIndParam param;
    char send_data[] = {1,2,3,4};

    bt_cmd_param_parse(argc, argv, input_param);

    char_num = input_param[0];
    confirm = input_param[1];
    param.connectId  = ohos_test_env.service_conn_id;
    param.confirm = confirm;
    if (char_num == 1)
    {
        param.attrHandle = ohos_test_env.char_hdl1;
    }
    else if (char_num == 2)
    {
        param.attrHandle = ohos_test_env.char_hdl2;
    }
    param.valueLen = sizeof(send_data);
    param.value    = send_data;

    BleGattsSendIndication(ohos_test_env.service_id, &param);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_srv_send_ntf", "none", bt_ble_srv_send_ntf);

static void bt_ble_srv_delete(int argc, char *argv[])
{
    BleGattsDeleteService(ohos_test_env.service_id, ohos_test_env.srvcHandle);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_srv_delete", "none", bt_ble_srv_delete);

static void bt_ble_srv_dis_conn(int argc, char *argv[])
{
    BleGattsDisconnect(ohos_test_env.service_id, ohos_test_env.bdAddr, ohos_test_env.service_conn_id);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_srv_dis_conn", "none", bt_ble_srv_dis_conn);

static void bt_ble_uuid_tran_test(int argc, char *argv[])
{
    uint8_t uuid_len;
    char uuid_tring[37] = {0};
    uint8_t uuid[16] = {0};

    LOG_I("[%s][%d]: %s", __func__, __LINE__ , SOFTBUS_SERVICE_UUID);

    uuid_len = BleGattsUuidCharToHex(SOFTBUS_SERVICE_UUID, uuid);
    LOG_I("[%s][%d]: %d", __func__, __LINE__ , uuid_len);
    DUMP8("%02x ", uuid, uuid_len);

    uuid_len = BleGattsUuidHexToChar(uuid, uuid_len, uuid_tring);
    LOG_I("[%s][%d]: %d", __func__, __LINE__ , uuid_len);
    LOG_I("[%s][%d]: %s", __func__, __LINE__ , uuid_tring);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "bt_ble_uuid_tran_test", "none", bt_ble_uuid_tran_test);

static void ohos_gap_bt_on_off(int argc, char *argv[])
{
    int ret = 0;
    int enable = {0};

    bt_cmd_param_parse(argc, argv, &enable);

    if (enable)
    {
        ret = EnableBt();
    }
    else
    {
        ret = DisableBt();
    }

    LOG_I("[%s][%d]: ret=%d", __func__, __LINE__ , ret);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_bt_on_off", "none", ohos_gap_bt_on_off);

static void ohos_gap_ble_on_off(int argc, char *argv[])
{
    int ret = 0;
    int enable = {0};

    bt_cmd_param_parse(argc, argv, &enable);

    if (enable)
    {
        ret = EnableBle();
    }
    else
    {
        ret = DisableBle();
    }

    LOG_I("[%s][%d]: ret=%d", __func__, __LINE__ , ret);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_ble_on_off", "none", ohos_gap_ble_on_off);

static void ohos_gap_get_bt_state(int argc, char *argv[])
{
    LOG_I("[%s][%d]: state=%d", __func__, __LINE__ , GetBtState());
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_get_bt_state", "none", ohos_gap_get_bt_state);

static void ohos_gap_ble_is_enable(int argc, char *argv[])
{
    int en = 0;

    en = IsBleEnabled();

    LOG_I("[%s][%d]: en=%d", __func__, __LINE__ , en);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_ble_is_enable", "none", ohos_gap_ble_is_enable);

static void ohos_gap_get_local_addr(int argc, char *argv[])
{
    bool state;
    uint8_t addr[6] = {0};

    state = GetLocalAddr(addr, 6);

    LOG_I("[%s][%d]: state=%d", __func__, __LINE__ , state);
    DUMP8("%02x ", addr, 6);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_get_local_addr", "none", ohos_gap_get_local_addr);

static void ohos_gap_set_local_name(int argc, char *argv[])
{
    bool state;

    state = SetLocalName((uint8_t *)SCAN_FILTER_DEV_NAME1, strlen(SCAN_FILTER_DEV_NAME1));

    LOG_I("[%s][%d]: state=%d", __func__, __LINE__ , state);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_set_local_name", "none", ohos_gap_set_local_name);

static void ohos_gap_set_bt_scan_mode(int argc, char *argv[])
{
    bool state;
    int param[2] = {0};

    bt_cmd_param_parse(argc, argv, param);
    state = SetBtScanMode(param[0], param[1]);

    LOG_I("[%s][%d]: state=%d", __func__, __LINE__ , state);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_set_bt_scan_mode", "none", ohos_gap_set_bt_scan_mode);


static void ohos_gap_set_fast_scan(int argc, char *argv[])
{
    int isEnable;
    bool state;

    bt_cmd_param_parse(argc, argv, &isEnable);

    state = SetFastScan(isEnable);

    LOG_I("[%s][%d]: state=%d", __func__, __LINE__ , state);
}
ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_BT_COMMON, "ohos_gap_set_fast_scan", "none", ohos_gap_set_fast_scan);

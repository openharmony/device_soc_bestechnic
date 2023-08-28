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
#include "harmony_utils.h"

#include "customer_api_test.h"
#include "bt_adaptor_dbg.h"

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
#define SCAN_FILTER_DEV_NAME2 "fjr_ble_test2"

/****************************Variable defination************************/
static const char scan_filter_addr1[6] = {0x91,0x33,0x33,0x23,0x22,0x11};
static const char scan_filter_addr2[6] = {0x92,0x33,0x33,0x23,0x22,0x11};

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
    LOG_I("[%s][%d]: result=%d, en=%d", __func__, __LINE__, resultCode, isStartScan);
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

static const BtGattClientCallbacks ohos_client_callback =
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

void OhosServiceStartCb(int status, int serverId, int srvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d", __func__, __LINE__, status, srvcHandle);
}

void OhosServiceStopCb(int status, int serverId, int srvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d", __func__, __LINE__, status, srvcHandle);
}

void OhosServiceDeleteCb(int status, int serverId, int srvcHandle)
{
    LOG_I("[%s][%d]: state=%d,srv_hdl=%d", __func__, __LINE__, status, srvcHandle);
}

void OhosRequestReadCb(BtReqReadCbPara readCbPara)
{
    LOG_I("[%s][%d]: conn=%d,attr_hdl=%d", __func__, __LINE__, readCbPara.connId, readCbPara.attrHandle);
}

void OhosRequestWriteCb(BtReqWriteCbPara writeCbPara)
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

void OhosResponseConfirmationCb(int status, int handle)
{
    LOG_I("[%s][%d]: attr_hdl=%d", __func__, __LINE__, handle);
}

void OhosIndicationSentCb(int connId, int status)
{
    LOG_I("[%s][%d]: status=%d", __func__, __LINE__, status);
}

void OhosMtuChangeCb(int connId, int mtu)
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

static void ohos_adv_start_handle(uint32_t BufPtr, uint32_t BufLen)
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

static void ohos_adv_stop_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleStopAdv(ohos_test_env.adv_id);
}

static void ohos_adv_set_data_handle(uint32_t BufPtr, uint32_t BufLen)
{
    int data_len;
    unsigned char advData[20] = {0};
    //unsigned char advRspData[20] = {0};
    StartAdvRawData rawData = {0};

    advData[0] = strlen(SCAN_FILTER_DEV_NAME1)+1;
    advData[1] = 0X08;
    memcpy(&advData[2], SCAN_FILTER_DEV_NAME1, advData[0]-1);
    data_len = advData[0] + 1;

    rawData.advData =advData;
    rawData.advDataLen =data_len;
    //rawData.rspData = advRspData;
    //rawData.rspDataLen = sizeof(advRspData);
    rawData.rspData = NULL;
    rawData.rspDataLen = 0;

    BleSetAdvData(ohos_test_env.adv_id, rawData);
}

static void ohos_scan_start_handle(uint32_t BufPtr, uint32_t BufLen)
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

static void ohos_scan_stop_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleStopScan(ohos_test_env.adv_id);
}

static void ohos_cli_creat_conn_handle(uint32_t BufPtr, uint32_t BufLen)
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

static void ohos_cli_set_priority_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleGattcSetPriority(ohos_test_env.client_id, (BdAddr *)scan_filter_addr1, OHOS_BT_GATT_PRIORITY_HIGH);
}

static void ohos_cli_creat_conn_fast_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleGattcSetFastestConn(ohos_test_env.client_id, true);
}

static void ohos_cli_cancel_handle(uint32_t BufPtr, uint32_t BufLen)
{
    ohos_cli_creat_conn_handle(BufPtr, BufLen);
    BleGattcDisconnect(ohos_test_env.client_id);
}

static void ohos_cli_disconn_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleGattcDisconnect(ohos_test_env.client_id);
}

static void ohos_cli_Search_srv_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleGattcSearchServices(ohos_test_env.client_id);
}

static void ohos_cli_get_srv_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BtUuid Uuid;

    Uuid.uuidLen = strlen(SOFTBUS_SERVICE_UUID);
    Uuid.uuid    = SOFTBUS_SERVICE_UUID;
    BleGattcGetService(ohos_test_env.client_id, Uuid);
}

static void ohos_cli_read_handle(uint32_t BufPtr, uint32_t BufLen)
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

static void ohos_cli_wirte_handle(uint32_t BufPtr, uint32_t BufLen)
{
    int char_num = 0;
    int writeType = 0;
    BtGattCharacteristic char_uuid = {0};
    const char value[] = {5,6,7,8,10};

    sscanf((char *)BufPtr, "%d,%d", &char_num, &writeType);
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

static void ohos_cli_reg_ntf_handle(uint32_t BufPtr, uint32_t BufLen)
{
    int ret;
    int char_num = 0;
    BtGattCharacteristic Uuid;

    sscanf((char *)BufPtr, "%d", &char_num);

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

static void ohos_srv_add_srv_handle(uint32_t BufPtr, uint32_t BufLen)
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
    permissions = SOFTBUS_GATT_PERMISSION_WRITE, SOFTBUS_CHARA_BLECONN_UUID;
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
    permissions = SOFTBUS_GATT_PERMISSION_WRITE, SOFTBUS_CHARA_BLECONN_UUID;
    BleGattsAddCharacteristic(ohos_test_env.service_id, ohos_test_env.srvcHandle, Uuid, properties, permissions);

    // add Descriptor
    Uuid.uuidLen = strlen(SOFTBUS_DESCRIPTOR_CONFIGURE_UUID);
    Uuid.uuid    = SOFTBUS_DESCRIPTOR_CONFIGURE_UUID;
    permissions = SOFTBUS_GATT_PERMISSION_READ | SOFTBUS_GATT_PERMISSION_WRITE;
    BleGattsAddDescriptor(ohos_test_env.service_id, ohos_test_env.srvcHandle, Uuid, permissions);
}

static void ohos_srv_start_handle(uint32_t BufPtr, uint32_t BufLen)
{
    int ret;

    ret = BleGattsStartService(ohos_test_env.service_id, ohos_test_env.srvcHandle);
    LOG_I("[%s][%d]: clientId=%d, state=%d", __func__, __LINE__, ohos_test_env.client_id, ret);
}

static void ohos_srv_stop_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleGattsStopService(ohos_test_env.service_id, ohos_test_env.srvcHandle);
}

static void ohos_srv_send_ntf_handle(uint32_t BufPtr, uint32_t BufLen)
{
    int char_num = 0;
    GattsSendIndParam param;
    char send_data[] = {1,2,3,4};

    param.connectId  = ohos_test_env.service_conn_id;
    sscanf((char *)BufPtr, "%d,%d", &char_num, &param.confirm);

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

static void ohos_srv_delete_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleGattsDeleteService(ohos_test_env.service_id, ohos_test_env.srvcHandle);
}

static void ohos_srv_dis_conn_handle(uint32_t BufPtr, uint32_t BufLen)
{
    BleGattsDisconnect(ohos_test_env.service_id, ohos_test_env.bdAddr, ohos_test_env.service_conn_id);
}

static void ohos_uuid_tran_test_handle(uint32_t BufPtr, uint32_t BufLen)
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

static const customer_adapt_handle_t bluet_adapt_test_handle[]=
{
    // gap cmd

    // gatt cmd
    {"ohos_adv_start", ohos_adv_start_handle},
    {"ohos_adv_set_data", ohos_adv_set_data_handle},
    {"ohos_adv_stop", ohos_adv_stop_handle},
    {"ohos_scan_start", ohos_scan_start_handle},
    {"ohos_scan_stop", ohos_scan_stop_handle},

    // gatt client cmd
    {"ohos_cli_creat_conn", ohos_cli_creat_conn_handle},
    {"ohos_cli_set_filter_list", ohos_cli_set_priority_handle},
    {"ohos_cli_creat_conn_fast", ohos_cli_creat_conn_fast_handle},
    {"ohos_cli_cancel_conn", ohos_cli_cancel_handle},
    {"ohos_cli_disconn", ohos_cli_disconn_handle},
    {"ohos_cli_search", ohos_cli_Search_srv_handle},
    {"ohos_cli_get_srv", ohos_cli_get_srv_handle},
    {"ohos_cli_reg_ntf", ohos_cli_reg_ntf_handle},
    {"ohos_cli_read", ohos_cli_read_handle},
    {"ohos_cli_wirte", ohos_cli_wirte_handle},

    // gatt server cmd
    {"ohos_srv_add_srv", ohos_srv_add_srv_handle},
    {"ohos_srv_start_srv", ohos_srv_start_handle},
    {"ohos_srv_send_ntf", ohos_srv_send_ntf_handle},
    {"ohos_srv_stop_srv", ohos_srv_stop_handle},
    {"ohos_srv_delete_srv", ohos_srv_delete_handle},
    {"ohos_srv_disconn", ohos_srv_dis_conn_handle},

    {"ohos_uuid_tran_test", ohos_uuid_tran_test_handle},
};

uint8_t ohos_get_cmd_list(const customer_adapt_handle_t **cmd_list)
{
    // gatt callback
    BleGattRegisterCallbacks(&ohos_adv_callback);
    BleRegisterScanCallbacks(&ohos_scan_callback, &ohos_test_env.scanner_id);

    // gatts callback
    BtUuid appUuid;
    char uuid[2] = {0x01, 0x02};
    appUuid.uuidLen = 2;
    appUuid.uuid = uuid;
    BleGattsRegisterCallbacks(&ohos_service_callback);
    ohos_test_env.service_id = BleGattsRegister(appUuid);

    *cmd_list = bluet_adapt_test_handle;
    return ARRAY_SIZE(bluet_adapt_test_handle);
}

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
#include "string.h"
#include "plat_types.h"
#include "hal_trace.h"
#include "ohos_bt_gatt_server.h"
#include "harmony_utils.h"
#include "gatt_service.h"
#include "bt_adaptor_dbg.h"

/****************************Macro defination***************************/
#define OHOS_GATTS_EVENT_MAX        10
#define OHOS_GATTS_ID_INVALIDE      0
#define OHOS_GATTS_ID               1
#define OHOS_GATTS_INVALID_CONN_ID  0xFFFFFFFF

#define OHOS_SRV_ATT_LIST_MAX     10
#define OHOS_SRV_ATT_MTU_DEF      512
/****************************Type defination****************************/
typedef struct
{
    gatt_server_event_t event;
    int (*BleGattsCallback)(gatt_svc_t *svc, gatt_server_callback_param_t param);
} OhosGattsStackEventList_t;

typedef struct
{
    bool Used;
    int  ConnIdx;
    int  ReadAttrHandle;
    gatt_server_read_ctx_t *ReadContext;
    int  WriteAttrHandle;
    gatt_server_write_ctx_t *WriteContext;
} OhosBleConnInfo_t;

typedef struct
{
    osMutexDef_t mutex;
    osMutexId    mutex_id;

    // ohos info
    int ServiceId;
    BtUuid AppUser;
    BtGattServerCallbacks *EventCb;

    // host stack info
    int StackEventNum;
    uint32_t NtfConnBf;
    uint8_t ConnNum;
    OhosBleConnInfo_t ConnInfo[OHOS_CONN_MAX];

    int AttNum;
    int AttSrvHdl;
    int AttAddCount;
    uint8_t AttrData[OHOS_SRV_ATT_LIST_MAX][25];
    gatt_attribute_t SrvAttrList[OHOS_SRV_ATT_LIST_MAX];
} OhosGattsEnv_t;
/****************************Function declaration***********************/

/****************************Variable defination************************/
static OhosGattsEnv_t BleGattsEnv = {0};

/****************************Private function implementation************/
static inline int BleGattsGetUserAttHdl(const gatt_attribute_t *char_attr)
{
    int i=0;

    for (i=0; BleGattsEnv.AttAddCount > i; ++i)
    {
        if (&BleGattsEnv.SrvAttrList[i] == char_attr)
        {
            break;
        }
    }

    return i+1;
}

static inline OhosBleConnInfo_t* BleGattsGetBleConnInfo(int connId)
{
    int i=0;
    OhosBleConnInfo_t* ConnInfo = NULL;

    for (i=0; i < OHOS_CONN_MAX; ++i)
    {
        if(connId == OHOS_GATTS_INVALID_CONN_ID)
        {
            if (!BleGattsEnv.ConnInfo[i].Used)
            {
                ConnInfo = &BleGattsEnv.ConnInfo[i];
                break;
            }
        }
        else
        {
            if (BleGattsEnv.ConnInfo[i].ConnIdx == connId)
            {
                ConnInfo = &BleGattsEnv.ConnInfo[i];
                break;
            }
        }
    }

    return ConnInfo;
}

static int BleGattsConnOpenedCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    OhosBleConnInfo_t* ConnInfo = NULL;

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    ConnInfo = BleGattsGetBleConnInfo(OHOS_GATTS_INVALID_CONN_ID);
    osMutexRelease(BleGattsEnv.mutex_id);
    if (ConnInfo)
    {
        osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
        ConnInfo->Used = true;
        ConnInfo->ConnIdx = param.opened->conn->connhdl;
        ConnInfo->ReadAttrHandle = 0;
        ConnInfo->ReadContext = NULL;
        ConnInfo->WriteAttrHandle = 0;
        ConnInfo->WriteContext = NULL;
        osMutexRelease(BleGattsEnv.mutex_id);

        BleGattsEnv.EventCb->connectServerCb(param.opened->conn->connhdl,
            BleGattsEnv.ServiceId, (const BdAddr *)param.opened->conn->peer_addr.address);
    }



    return BT_STS_SUCCESS;
}

static int BleGattsConnClosedCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    OhosBleConnInfo_t* ConnInfo = NULL;

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    ConnInfo = BleGattsGetBleConnInfo(param.closed->conn->connhdl);
    osMutexRelease(BleGattsEnv.mutex_id);

    if (ConnInfo)
    {
        osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
        ConnInfo->Used = false;
        osMutexRelease(BleGattsEnv.mutex_id);
        BleGattsEnv.EventCb->disconnectServerCb(param.closed->conn->connhdl, BleGattsEnv.ServiceId,
            (const BdAddr *)param.closed->conn->peer_addr.address);
    }

    return BT_STS_SUCCESS;
}

static int BleGattsCharReadCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    BtReqReadCbPara readCbPara = {0};
    OhosBleConnInfo_t* ConnInfo = NULL;

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    ConnInfo = BleGattsGetBleConnInfo(param.closed->conn->connhdl);
    osMutexRelease(BleGattsEnv.mutex_id);

    if (ConnInfo)
    {
        osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
        readCbPara.connId = svc->con_idx;
        readCbPara.transId = OHOS_BT_TRANSPORT_LE;
        readCbPara.bdAddr = (BdAddr *)&param.char_read->conn->peer_addr;
        readCbPara.attrHandle = BleGattsGetUserAttHdl(param.char_read->char_attr);
        readCbPara.offset = param.char_read->value_offset;
        readCbPara.isLong = false;

        ConnInfo->ReadAttrHandle = readCbPara.attrHandle;
        ConnInfo->ReadContext = param.char_read->ctx;
        osMutexRelease(BleGattsEnv.mutex_id);

        BleGattsEnv.EventCb->requestReadCb(readCbPara);
    }

    return BT_STS_SUCCESS;
}

static int BleGattsDescReadCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    uint16_t uuid16 = 0;
    BtReqReadCbPara readCbPara = {0};
    OhosBleConnInfo_t* ConnInfo = NULL;

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    ConnInfo = BleGattsGetBleConnInfo(param.closed->conn->connhdl);
    osMutexRelease(BleGattsEnv.mutex_id);

    if (ConnInfo)
    {
        uuid16 = (param.desc_read->descriptor[3] << 8) | (param.desc_read->descriptor[2]);
        if (uuid16 != GATT_DESC_UUID_CHAR_CLIENT_CONFIG)
        {
            osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
            readCbPara.connId = svc->con_idx;
            readCbPara.transId = OHOS_BT_TRANSPORT_LE;
            readCbPara.bdAddr = (BdAddr *)&param.desc_read->conn->peer_addr;
            readCbPara.attrHandle = BleGattsGetUserAttHdl(param.desc_read->char_attr);
            readCbPara.offset = param.desc_read->value_offset;
            readCbPara.isLong = false;

            ConnInfo->ReadAttrHandle = readCbPara.attrHandle;
            ConnInfo->ReadContext = param.desc_read->ctx;
            osMutexRelease(BleGattsEnv.mutex_id);

            BleGattsEnv.EventCb->requestReadCb(readCbPara);
        }
    }

    return BT_STS_SUCCESS;
}

static int BleGattsCharWriteCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    BtReqWriteCbPara writeCbPara = {0};
    OhosBleConnInfo_t* ConnInfo = NULL;

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    ConnInfo = BleGattsGetBleConnInfo(param.closed->conn->connhdl);
    osMutexRelease(BleGattsEnv.mutex_id);
    if (ConnInfo)
    {
        osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
        writeCbPara.connId   = svc->con_idx;
        writeCbPara.transId  = OHOS_BT_TRANSPORT_LE;
        writeCbPara.bdAddr   = (BdAddr *)param.char_write->conn->peer_addr.address;
        writeCbPara.attrHandle = BleGattsGetUserAttHdl(param.char_write->char_attr);
        writeCbPara.offset   = param.char_write->value_offset;
        writeCbPara.length   = param.char_write->value_len;
        writeCbPara.needRsp  = false;
        writeCbPara.isPrep   = false;
        writeCbPara.value    = (unsigned char *)param.char_write->value;

        ConnInfo->WriteAttrHandle = writeCbPara.attrHandle;
        ConnInfo->WriteContext = param.char_write->ctx;
        osMutexRelease(BleGattsEnv.mutex_id);

        BleGattsEnv.EventCb->requestWriteCb(writeCbPara);
    }
    return BT_STS_SUCCESS;
}

static int BleGattsDescWriteCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    uint16_t uuid16 = 0;
    BtReqWriteCbPara writeCbPara = {0};
    OhosBleConnInfo_t* ConnInfo = NULL;

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    ConnInfo = BleGattsGetBleConnInfo(param.closed->conn->connhdl);
    osMutexRelease(BleGattsEnv.mutex_id);
    if (ConnInfo)
    {
        uuid16 = (param.desc_write->descriptor[3] << 8) | (param.desc_write->descriptor[2]);
        if (uuid16 != GATT_DESC_UUID_CHAR_CLIENT_CONFIG)
        {
            osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
            writeCbPara.connId   = svc->con_idx;
            writeCbPara.transId  = OHOS_BT_TRANSPORT_LE;
            writeCbPara.bdAddr   = (BdAddr *)param.desc_write->conn->peer_addr.address;
            writeCbPara.attrHandle = BleGattsGetUserAttHdl(param.desc_write->char_attr);
            writeCbPara.offset   = param.desc_write->value_offset;
            writeCbPara.length   = param.desc_write->value_len;
            writeCbPara.needRsp  = false;
            writeCbPara.isPrep   = false;
            writeCbPara.value    = (unsigned char *)param.desc_write->value;

            ConnInfo->WriteAttrHandle = writeCbPara.attrHandle;
            ConnInfo->WriteContext = param.desc_write->ctx;
            osMutexRelease(BleGattsEnv.mutex_id);

            BleGattsEnv.EventCb->requestWriteCb(writeCbPara);
        }
        else
        {
            osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
            BleGattsEnv.NtfConnBf |= (1 << ((param.desc_write->conn->connhdl)));
            osMutexRelease(BleGattsEnv.mutex_id);
            gatts_send_write_rsp(param.desc_write->ctx, ATT_ERROR_NO_ERROR);
        }
    }

    return BT_STS_SUCCESS;
}

static int BleGattsNtfTxDoneCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    BleGattsEnv.EventCb->indicationSentCb(svc->con_idx, OHOS_BT_STATUS_SUCCESS);
    return BT_STS_SUCCESS;
}

static int BleGattsIndicateCfmCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    BleGattsEnv.EventCb->indicationSentCb(svc->con_idx, param.cfm->error_code);
    return BT_STS_SUCCESS;
}

static int BleGattsMtuChangedCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    BleGattsEnv.EventCb->mtuChangeCb(svc->con_idx, param.mtu_changed->mtu);
    return BT_STS_SUCCESS;
}

static int BleGattsConnUpdateCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    LOG_I("[%s][%d]: interval=%d", __func__, __LINE__,
        param.conn_updated->conn->timing.conn_interval_1_25ms);
    return BT_STS_SUCCESS;
}

static int BleGattsConnEncryptedCb(gatt_svc_t *svc, gatt_server_callback_param_t param)
{
    LOG_I("[%s][%d]: error_code=%d", __func__, __LINE__,
        param.conn_encrypted->error_code);

    return BT_STS_SUCCESS;
}


static const OhosGattsStackEventList_t GattsStackEventList[] =
{
    {GATT_SERV_EVENT_CONN_OPENED, BleGattsConnOpenedCb},
    {GATT_SERV_EVENT_CONN_CLOSED, BleGattsConnClosedCb},
    {GATT_SERV_EVENT_CHAR_READ, BleGattsCharReadCb},
    {GATT_SERV_EVENT_DESC_READ, BleGattsDescReadCb},
    {GATT_SERV_EVENT_CHAR_WRITE, BleGattsCharWriteCb},
    {GATT_SERV_EVENT_DESC_WRITE, BleGattsDescWriteCb},
    {GATT_SERV_EVENT_NTF_TX_DONE, BleGattsNtfTxDoneCb},
    {GATT_SERV_EVENT_INDICATE_CFM, BleGattsIndicateCfmCb},
    {GATT_SERV_EVENT_MTU_CHANGED, BleGattsMtuChangedCb},
    {GATT_SERV_EVENT_CONN_UPDATED, BleGattsConnUpdateCb},
    {GATT_SERV_EVENT_CONN_ENCRYPTED, BleGattsConnEncryptedCb},
};

static int BleGattsStackCallback(gatt_svc_t *svc, gatt_server_event_t event, gatt_server_callback_param_t param)
{
    int ret = BT_STS_SUCCESS;

    for (int i=0; i< BleGattsEnv.StackEventNum; ++i)
    {
        if (event == GattsStackEventList[i].event)
        {
            ret = GattsStackEventList[i].BleGattsCallback(svc, param);
            break;
        }
    }

    return ret;
}

/****************************Api function implementation****************************/
int BleGattsRegisterCallbacks(BtGattServerCallbacks *func)
{
    BleGattsEnv.EventCb = func;

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsRegister(BtUuid appUuid)
{
    if (!BleGattsEnv.mutex_id)
    {
        BleGattsEnv.mutex_id = osMutexCreate(&BleGattsEnv.mutex);
    }

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    BleGattsEnv.AppUser =appUuid;
    BleGattsEnv.StackEventNum = ARRAY_SIZE(GattsStackEventList);
    BleGattsEnv.NtfConnBf = 0;
    BleGattsEnv.ServiceId = OHOS_GATTS_ID;
    osMutexRelease(BleGattsEnv.mutex_id);

    BleGattsEnv.EventCb->registerServerCb(OHOS_BT_STATUS_SUCCESS, BleGattsEnv.ServiceId, &BleGattsEnv.AppUser);

    return BleGattsEnv.ServiceId;
}

int BleGattsUnRegister(int serverId)
{
    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    memset(&BleGattsEnv.AppUser, 0, sizeof(BleGattsEnv.AppUser));
    BleGattsEnv.EventCb = NULL;
    osMutexRelease(BleGattsEnv.mutex_id);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsAddService(int serverId, BtUuid srvcUuid, bool isPrimary, int number)
{
    uint8_t uuid_len;
    uint8_t uuid[16] = {0};

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    BleGattsEnv.AttNum = number;
    uuid_len = BleGattsUuidCharToHex(srvcUuid.uuid, uuid);
    //GATT_DECL_SEC_SERVICE(AIC_SERVICE, GATT_UUID_AIC_SERVICE);

    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0] = CO_UINT8_VALUE(0);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][1] = CO_UINT8_VALUE(ATT_RD_PERM);
    if (isPrimary)
    {
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][2] = CO_UINT8_VALUE(GATT_UUID_PRI_SERVICE);
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][3] = CO_UINT8_VALUE((GATT_UUID_PRI_SERVICE)>>8);
    }
    else
    {
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][2] = CO_UINT8_VALUE(GATT_UUID_SEC_SERVICE);
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][3] = CO_UINT8_VALUE((GATT_UUID_SEC_SERVICE)>>8);
    }

    memcpy (&BleGattsEnv.AttrData[0][4], uuid, uuid_len);

    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].attr_data = (const attr_byte_array_t *)&BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0];
    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].data_len = 4+uuid_len;
    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].inst_id  = 0;
    BleGattsEnv.AttAddCount++;

    BleGattsEnv.AttSrvHdl = BleGattsEnv.AttAddCount;
    osMutexRelease(BleGattsEnv.mutex_id);

    BleGattsEnv.EventCb->serviceAddCb(OHOS_BT_STATUS_SUCCESS, serverId, &srvcUuid, BleGattsEnv.AttAddCount);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsAddCharacteristic(int serverId, int srvcHandle, BtUuid characUuid,
                              int properties, int permissions)
{
    uint8_t uuid_len;
    uint8_t uuid[16] = {0};

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    uuid_len = BleGattsUuidCharToHex(characUuid.uuid, uuid);
    //GATT_DECL_CHAR(AUD_INPUT_STATE, GATT_CHAR_UUID_AUDIO_INPUT_STATE, GATT_RD_REQ | GATT_NTF_PROP, ATT_RD_ENC);

    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0] = CO_UINT8_VALUE(0);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][1] = CO_UINT8_VALUE(ATT_RD_PERM);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][2] = CO_UINT8_VALUE(GATT_UUID_CHAR_DECLARE);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][3] = CO_UINT8_VALUE((GATT_UUID_CHAR_DECLARE)>>8);

    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][4] = properties;

    if (uuid_len == 2)
    {
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][5] = CO_UINT8_VALUE(0);
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][6] = permissions;
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][7] = uuid[0];
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][8] = uuid[1];

        BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].data_len = 9;
    }
    else
    {
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][5] = CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID);
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][6] = permissions;
        memcpy(&BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][7], uuid, uuid_len);

        BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].data_len = 23;
    }

    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].attr_data = (const attr_byte_array_t *)&BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0];
    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].inst_id  = 0;
    BleGattsEnv.AttAddCount++;


    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0] = CO_UINT8_VALUE(0);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][1] = permissions;
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][2] = CO_UINT8_VALUE(GATT_DESC_UUID_CHAR_CLIENT_CONFIG);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][3] = CO_UINT8_VALUE((GATT_DESC_UUID_CHAR_CLIENT_CONFIG)>>8);

    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].attr_data = (const attr_byte_array_t *)&BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0];
    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].data_len = 4;
    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].inst_id  = 0;

    BleGattsEnv.AttAddCount++;
    osMutexRelease(BleGattsEnv.mutex_id);

    BleGattsEnv.EventCb->characteristicAddCb(OHOS_BT_STATUS_SUCCESS, serverId, &characUuid, BleGattsEnv.AttSrvHdl, BleGattsEnv.AttAddCount-1);
    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsAddDescriptor(int serverId, int srvcHandle, BtUuid descUuid, int permissions)
{
    uint8_t uuid_len;
    uint8_t uuid[16] = {0};

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    uuid_len = BleGattsUuidCharToHex(descUuid.uuid, uuid);
    //GATT_DECL_CCCD_DESCRIPTOR(AUD_INPUT_STATE_CCCD, ATT_RD_ENC | ATT_WR_ENC);
    //ATT_128BE_UUID

    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0] = CO_UINT8_VALUE(0);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][1] = permissions;
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][2] = CO_UINT8_VALUE(GATT_DESC_UUID_CHAR_USER_DESC);
    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][3] = CO_UINT8_VALUE((GATT_DESC_UUID_CHAR_USER_DESC)>>8);

    BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][4] = ATT_RD_PERM|ATT_WR_PERM;

    if (uuid_len == 2)
    {
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][5] = CO_UINT8_VALUE(0);
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][6] = permissions;
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][7] = uuid[0];
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][8] = uuid[1];

        BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].data_len = 9;
    }
    else
    {
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][5] = CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID);
        BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][6] = permissions;
        memcpy(&BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][7], uuid, uuid_len);

        BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].data_len = 23;
    }

    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].attr_data =
        (const attr_byte_array_t *)&BleGattsEnv.AttrData[BleGattsEnv.AttAddCount][0];
    BleGattsEnv.SrvAttrList[BleGattsEnv.AttAddCount].inst_id  = 0;
    BleGattsEnv.AttAddCount++;
    osMutexRelease(BleGattsEnv.mutex_id);

    BleGattsEnv.EventCb->descriptorAddCb(OHOS_BT_STATUS_SUCCESS, serverId, &descUuid, BleGattsEnv.AttSrvHdl, BleGattsEnv.AttAddCount);
    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsStartService(int serverId, int srvcHandle)
{
    int ret;

    gatts_cfg_t cfg =
    {
        .eatt_preferred = true,
        .preferred_mtu = OHOS_SRV_ATT_MTU_DEF,
        .svc_size = 0,
        .service_inst_id = 0,
    };

    ret = gatts_register_service(BleGattsEnv.SrvAttrList, BleGattsEnv.AttAddCount, BleGattsStackCallback, &cfg);
    if (ret != BT_STS_SUCCESS)
    {
        return BT_STS_FAILED;
    }
    ret = gatts_ctrl_service(BleGattsEnv.SrvAttrList, true);
    if (ret != BT_STS_SUCCESS)
    {
        return BT_STS_FAILED;
    }

    BleGattsEnv.EventCb->serviceStartCb(OHOS_BT_STATUS_SUCCESS, BleGattsEnv.ServiceId, BleGattsEnv.AttSrvHdl);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsStopService(int serverId, int srvcHandle)
{
    int ret;

    ret = gatts_ctrl_service(BleGattsEnv.SrvAttrList, false);
    if (ret != BT_STS_SUCCESS)
    {
        return BT_STS_FAILED;
    }

    BleGattsEnv.EventCb->serviceStopCb(OHOS_BT_STATUS_SUCCESS, BleGattsEnv.ServiceId, BleGattsEnv.AttSrvHdl);
    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsDeleteService(int serverId, int srvcHandle)
{
    int ret;

    ret = gatts_unregister_service(BleGattsEnv.SrvAttrList);
    if (ret != BT_STS_SUCCESS)
    {
        return BT_STS_FAILED;
    }

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    BleGattsEnv.AttNum = 0;
    BleGattsEnv.AttSrvHdl = 0;
    BleGattsEnv.AttAddCount = 0;
    memset(BleGattsEnv.AttrData, 0, sizeof(BleGattsEnv.AttrData));
    memset(BleGattsEnv.SrvAttrList, 0, sizeof(BleGattsEnv.SrvAttrList));
    osMutexRelease(BleGattsEnv.mutex_id);

    BleGattsEnv.EventCb->serviceDeleteCb(OHOS_BT_STATUS_SUCCESS, BleGattsEnv.ServiceId, BleGattsEnv.AttSrvHdl);
    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsDisconnect(int serverId, BdAddr bdAddr, int connId)
{
    gap_terminate_connection(connId, 0);
    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsSendResponse(int serverId, GattsSendRspParam *param)
{
    int ret = BT_STS_SUCCESS;
    OhosBleConnInfo_t* ConnInfo = NULL;

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    ConnInfo = BleGattsGetBleConnInfo(param->connectId);
    osMutexRelease(BleGattsEnv.mutex_id);

    if (ConnInfo->ReadAttrHandle == param->attrHandle)
    {
        ret = gatts_write_read_rsp_data(ConnInfo->ReadContext, (uint8_t *)param->value, param->valueLen);
        osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
        ConnInfo->ReadAttrHandle = 0;
        ConnInfo->ReadContext    = NULL;
        osMutexRelease(BleGattsEnv.mutex_id);
    }
    if (ConnInfo->WriteAttrHandle == param->attrHandle)
    {
        ret = gatts_send_write_rsp(ConnInfo->WriteContext, param->status);
        osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
        ConnInfo->WriteAttrHandle = 0;
        ConnInfo->WriteContext    = NULL;
        osMutexRelease(BleGattsEnv.mutex_id);
    }

    if (BT_STS_SUCCESS == ret)
    {
        BleGattsEnv.EventCb->responseConfirmationCb(OHOS_BT_STATUS_SUCCESS, param->attrHandle);
    }
    else
    {
        BleGattsEnv.EventCb->responseConfirmationCb(OHOS_BT_STATUS_FAIL, param->attrHandle);
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsSendIndication(int serverId, GattsSendIndParam *param)
{
    int ret;
    uint32_t con_idx_bf = 0;
    gap_conn_item_t *conn = NULL;
    gatt_char_notify_t character = {0};

    osMutexWait(BleGattsEnv.mutex_id, osWaitForever);
    if (!(BleGattsEnv.NtfConnBf & (1<<param->connectId)))
    {
        LOG_I("[%s][%d]: this conn not reg ntf! conn=%d", __func__, __LINE__, param->connectId);
        return OHOS_BT_STATUS_FAIL;
    }

    conn = gap_get_conn_item(param->connectId);
    con_idx_bf = gap_conn_bf(conn->con_idx);

    character.service   = &BleGattsEnv.AttrData[0][0];
    character.character = &BleGattsEnv.AttrData[param->attrHandle-1][0];
    character.service_inst_id  = 0;
    character.char_instance_id = 0;
    character.force_send_value = true;
    osMutexRelease(BleGattsEnv.mutex_id);

    if (param->confirm)
    {
        ret = gatts_send_value_indication(con_idx_bf, &character, (uint8_t *)param->value, param->valueLen);
    }
    else
    {
        ret = gatts_send_value_notification(con_idx_bf, &character, (uint8_t *)param->value, param->valueLen);
    }

    if (ret != BT_STS_SUCCESS)
    {
        LOG_E("[%s][%d]: send fail ret=%d", __func__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }

    return OHOS_BT_STATUS_SUCCESS;
}

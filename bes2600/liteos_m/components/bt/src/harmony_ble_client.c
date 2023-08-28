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
#include <string.h>
#include "plat_types.h"
#include "hal_trace.h"
#include "ohos_bt_gatt_client.h"
#include "harmony_utils.h"
#include  "gatt_service.h"
#include "bt_adaptor_dbg.h"

/****************************Macro defination***************************/
#define OHOS_CACHE_SRV_MAX      20
#define OHOS_CACHE_CHAR_MAX     10

#define OHOS_ClIENT_INVALID_ID  0xFFFFFFFF
/****************************Type defination****************************/
typedef struct
{
    gatt_profile_event_t event;
    int (*BleGattcCallback)(gatt_prf_t *prf, gatt_profile_callback_param_t param);
} ohos_gattc_stack_event_list_t;

typedef struct
{
    gap_init_event_t event;
    int (*BleInitCallback)(uintptr_t init, gap_init_callback_param_t param);
} ohos_init_stack_event_list_t;

typedef struct {
    gap_conn_item_t *Conn;
    att_error_code_t ErrorCode;
    bool DiscEnd;
    uint16_t StartHdl;
    uint16_t EndHdl;
    uint16_t UuidLen;
    uint8_t  Uuid[16];
    gatt_peer_service_t *Service;
} GattProfileAllServiceCache_t;

typedef struct {
    gap_conn_item_t *Conn;
    att_error_code_t ErrorCode;
    bool DiscEnd;
    uint16_t ValueHandle;
    uint8_t UuidLen;
    uint8_t CharUuid[16]; // 16-bit or 128-bit characteristic uuid
    gatt_peer_character_t *Character;
} GattProfileAllCharacteristicCache_t;

typedef struct {
    uint8_t  PrfId;
    uint8_t  ConnIdx;
    uint16_t Connhdl;
    uint8_t  UserId;
} OhosGattcPrf_t;

typedef struct {
    /** UUID length */
    unsigned char UuidLen;
    /** UUID field */
    char Uuid[16];
} OhosAppGattcUser_t;

typedef struct
{
    bool Registered;
    bool FastestConnFlag;
    int  ClientId;
    int  ConnState;
    OhosAppGattcUser_t AppUser;
    uint8_t  PrfId;
    gatt_profile_param_t StackPrf;
    BtGattClientCallbacks *UserFuncCb;
    // stack info
    gap_conn_param_t ConnInfo;
    uint8_t FindSrvCount;
    uint8_t CacheSrvNum;
    GattProfileAllServiceCache_t CacheSrv[OHOS_CACHE_SRV_MAX];
    uint8_t FindCharCount;
    uint8_t CacheCharNum;
    gatt_peer_service_t *CharByService;
    GattProfileAllCharacteristicCache_t CacheChar[OHOS_CACHE_CHAR_MAX];
} OhosGattcObj_t;

typedef struct
{
    osMutexDef_t mutex;
    osMutexId    mutex_id;

    char    CharUuid1[37];
    char    CharUuid2[37];
    int     CreatConnCliId;
    uint8_t InitEventNum;
    uint8_t GattcEventNum;
    uint8_t UserClientNum;
    OhosGattcObj_t UserClient[OHOS_CONN_MAX];
} OhosGattcEnv_t;

/****************************Function declaration***********************/

/****************************Variable defination************************/
static OhosGattcEnv_t BleGattcEnv = {
    .CreatConnCliId = OHOS_ClIENT_INVALID_ID,
};

/****************************Private function implementation************************/
static inline gatt_peer_service_t* BleGattcFindSrvByUuid(OhosGattcObj_t *UserGattc, uint8_t uuid_len, uint8_t *uuid)
{
    gatt_peer_service_t*srv = NULL;

    for (int i=0; i<UserGattc->CacheSrvNum; ++i)
    {
        if ((uuid_len == UserGattc->CacheSrv[i].UuidLen) &&
            (!memcmp(UserGattc->CacheSrv[i].Uuid, uuid, uuid_len)))
        {
            srv = UserGattc->CacheSrv[i].Service;
            break;
        }
    }

    return srv;
}

static inline gatt_peer_character_t *BleGattcFindcharByUuid(OhosGattcObj_t *UserGattc, uint8_t uuid_len, uint8_t *uuid)
{
    gatt_peer_character_t *char_ptr = NULL;

    for (int i=0; i<UserGattc->CacheCharNum; ++i)
    {
        if ((uuid_len == UserGattc->CacheChar[i].UuidLen) &&
            (!memcmp(UserGattc->CacheChar[i].CharUuid, uuid, uuid_len)))
        {
            char_ptr = UserGattc->CacheChar[i].Character;
            break;
        }
    }

    return char_ptr;
}

static inline GattProfileAllServiceCache_t* BleGattcFindSrvByHdl(OhosGattcObj_t *UserGattc, gatt_peer_service_t *service)
{
    for (int i=0; i<UserGattc->CacheSrvNum; ++i)
    {
        if (service == UserGattc->CacheSrv[i].Service)
        {
            return &UserGattc->CacheSrv[i];
        }
    }

    return NULL;
}

static inline GattProfileAllCharacteristicCache_t *BleGattcFindcharByHdl(OhosGattcObj_t *UserGattc, gatt_peer_character_t *character)
{
    for (int i = 0; i < UserGattc->CacheCharNum; i++)
    {
        if (UserGattc->CacheChar[i].Character == character)
        {
            return &UserGattc->CacheChar[i];
        }
    }

    return NULL;
}

static int BleGattcOpenedCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{
    OhosGattcPrf_t *MsgPrf = (OhosGattcPrf_t *)prf;

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    for (int i=0; i<OHOS_CONN_MAX; ++i)
    {
        if (BleGattcEnv.UserClient[i].PrfId == prf->prf_id)
        {
            memcpy(&BleGattcEnv.UserClient[i].StackPrf, param.opened, sizeof(gatt_profile_param_t));
            MsgPrf->UserId = i;
            break;
        }
    }
    osMutexRelease(BleGattcEnv.mutex_id);

    return BT_STS_SUCCESS;
}

static int BleGattcClosedCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{
    OhosGattcPrf_t *MsgPrf = (OhosGattcPrf_t *)prf;

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    memset(&BleGattcEnv.UserClient[MsgPrf->UserId], 0, sizeof(OhosGattcObj_t));
    osMutexRelease(BleGattcEnv.mutex_id);

    return BT_STS_SUCCESS;
}

static int BleGattcAllServiceCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{
    OhosGattcPrf_t *MsgPrf = (OhosGattcPrf_t *)prf;
    OhosGattcObj_t *UserGattc = NULL;

    UserGattc = &BleGattcEnv.UserClient[MsgPrf->UserId];
    LOG_I("discovery all end=%d, %d" ,param.all_service->disc_end, UserGattc->CacheSrvNum);

    if (!param.all_service->disc_end)
    {
        osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
        memcpy(&UserGattc->CacheSrv[UserGattc->CacheSrvNum],
            param.all_service, sizeof(gatt_profile_all_service_t));
        memcpy(UserGattc->CacheSrv[UserGattc->CacheSrvNum].Uuid,
            param.all_service->uuid, param.all_service->uuid_len);
        UserGattc->CacheSrvNum++;
        osMutexRelease(BleGattcEnv.mutex_id);
    } else {
        if (UserGattc->CacheSrvNum)
        {
            UserGattc->FindSrvCount++;
            if (UserGattc->CacheSrv[0].UuidLen == 2)
            {
                uint16_t *uuid = (uint16 *)UserGattc->CacheSrv[0].Uuid;
                gattc_discover_service(UserGattc->StackPrf.prf, *uuid, NULL);
            }
            else
            {
                gattc_discover_service(UserGattc->StackPrf.prf, 0, UserGattc->CacheSrv[0].Uuid);
            }
        }
    }

    return BT_STS_SUCCESS;
}

static int BleGattcServiceCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{
    OhosGattcPrf_t *MsgPrf = (OhosGattcPrf_t *)prf;
    OhosGattcObj_t *UserGattc = NULL;

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[MsgPrf->UserId];
    LOG_I("discovery srv uuid=0x%x, %p" ,param.service->service_uuid, param.service->service);

    if (!param.service->error_code)
    {
        UserGattc->CacheSrv[UserGattc->FindSrvCount-1].Service = param.service->service;
    }
    osMutexRelease(BleGattcEnv.mutex_id);

    if (UserGattc->CacheSrvNum > UserGattc->FindSrvCount)
    {
        UserGattc->FindSrvCount++;
        if (UserGattc->CacheSrv[UserGattc->FindSrvCount-1].UuidLen == 2)
        {
            uint16_t *uuid = (uint16 *)UserGattc->CacheSrv[UserGattc->FindSrvCount-1].Uuid;
            gattc_discover_service(UserGattc->StackPrf.prf, *uuid, NULL);
        }
        else
        {
            gattc_discover_service(UserGattc->StackPrf.prf, 0,
                UserGattc->CacheSrv[UserGattc->FindSrvCount-1].Uuid);
        }
    }
    else
    {
        UserGattc->UserFuncCb->searchServiceCompleteCb(UserGattc->ClientId, OHOS_BT_STATUS_SUCCESS);
    }

    return BT_STS_SUCCESS;
}

static int BleGattcIncludeCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleGattcAllCharacterCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{
    OhosGattcPrf_t *MsgPrf = (OhosGattcPrf_t *)prf;
    OhosGattcObj_t *UserGattc = NULL;

    UserGattc = &BleGattcEnv.UserClient[MsgPrf->UserId];
    LOG_I("discovery All char dis, %d, %d!!", UserGattc->CacheCharNum, param.all_character->disc_end);
    DUMP8("0x%02x ", param.all_character->char_uuid, 16);


    if (!param.all_character->disc_end)
    {
        osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
        memcpy(&UserGattc->CacheChar[UserGattc->CacheCharNum],
            param.all_character, sizeof(gatt_profile_all_service_t));
        memcpy(UserGattc->CacheChar[UserGattc->CacheCharNum].CharUuid,
            param.all_character->char_uuid, param.all_character->uuid_len);
        UserGattc->CacheCharNum++;
        osMutexRelease(BleGattcEnv.mutex_id);
    } else {
        if (UserGattc->CacheCharNum)
        {
            UserGattc->FindCharCount++;
            if (UserGattc->CacheChar[0].UuidLen == 2)
            {
                uint16_t *uuid = (uint16 *)UserGattc->CacheChar[0].CharUuid;
                gattc_discover_character(UserGattc->StackPrf.prf, UserGattc->CharByService, *uuid, NULL);
            }
            else
            {
                gattc_discover_character(UserGattc->StackPrf.prf, UserGattc->CharByService,
                    0, UserGattc->CacheChar[0].CharUuid);
            }
        }
    }

    return BT_STS_SUCCESS;
}


static int BleGattcCharacterCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{
    int ret = BT_STS_SUCCESS;
    OhosGattcPrf_t *MsgPrf = (OhosGattcPrf_t *)prf;
    OhosGattcObj_t *UserGattc = NULL;

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[MsgPrf->UserId];
    LOG_I("discovery char count=%d, num=0x%x" , UserGattc->FindCharCount, UserGattc->CacheCharNum);
    DUMP8("0x%02x ", param.character->uuid_le, 16);

    if (!param.character->error_code)
    {
        UserGattc->CacheChar[UserGattc->FindCharCount-1].Character = param.character->character;
    }
    osMutexRelease(BleGattcEnv.mutex_id);

    if (UserGattc->CacheCharNum > UserGattc->FindCharCount)
    {
        UserGattc->FindCharCount++;
        if (UserGattc->CacheChar[UserGattc->FindCharCount-1].UuidLen == 2)
        {
            uint16_t *uuid = (uint16 *)UserGattc->CacheChar[UserGattc->FindCharCount-1].CharUuid;
            ret = gattc_discover_character(UserGattc->StackPrf.prf, UserGattc->CharByService, *uuid, NULL);
        }
        else
        {
            ret = gattc_discover_character(UserGattc->StackPrf.prf, UserGattc->CharByService, 0,
                UserGattc->CacheChar[UserGattc->FindCharCount-1].CharUuid);
        }
        LOG_I("discovery srv ret =%d" ,ret);
    }
    return BT_STS_SUCCESS;
}

static int BleGattcCharReadRspCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleGattcDescReadRspCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleGattcCharWriteRspCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleGattcDescWriteRspCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleGattcNotifyCb(gatt_prf_t *prf, gatt_profile_callback_param_t param)
{
    OhosGattcPrf_t *MsgPrf = (OhosGattcPrf_t *)prf;
    OhosGattcObj_t *UserGattc = NULL;
    GattProfileAllServiceCache_t* srv_info = NULL;
    GattProfileAllCharacteristicCache_t * char_info = NULL;
    BtGattReadData notifyData = {0};

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[MsgPrf->UserId];
    srv_info = BleGattcFindSrvByHdl(UserGattc, param.notify->service);
    char_info = BleGattcFindcharByHdl(UserGattc, param.notify->character);

    if ((!srv_info) || (!char_info))
    {
        LOG_E("[%s][%d]: not find,srv=%p srv_hdl=%p, char=%p, char_hdl=%p",
            __func__, __LINE__, srv_info, param.notify->service, char_info, param.notify->character);
        return BT_STS_SUCCESS;
    }

    notifyData.attribute.characteristic.serviceUuid.uuidLen =
        BleGattsUuidHexToChar(srv_info->Uuid, srv_info->UuidLen, BleGattcEnv.CharUuid1);
    notifyData.attribute.characteristic.serviceUuid.uuid    = BleGattcEnv.CharUuid1;

    notifyData.attribute.characteristic.characteristicUuid.uuidLen =
        BleGattsUuidHexToChar(char_info->CharUuid, char_info->UuidLen, BleGattcEnv.CharUuid2);;
    notifyData.attribute.characteristic.characteristicUuid.uuid = BleGattcEnv.CharUuid2;

    notifyData.dataLen = param.notify->value_len;
    notifyData.data    = (unsigned char *)param.notify->value;
    osMutexRelease(BleGattcEnv.mutex_id);

    UserGattc->UserFuncCb->notificationCb(UserGattc->ClientId, &notifyData, OHOS_BT_STATUS_SUCCESS);

    return BT_STS_SUCCESS;
}


static const ohos_gattc_stack_event_list_t GattcStackEventList[] =
{
    {GATT_PROF_EVENT_OPENED, BleGattcOpenedCb},
    {GATT_PROF_EVENT_CLOSED, BleGattcClosedCb},
    {GATT_PROF_EVENT_ALL_SERVICE, BleGattcAllServiceCb},
    {GATT_PROF_EVENT_SERVICE, BleGattcServiceCb},
    {GATT_PROF_EVENT_INCLUDE, BleGattcIncludeCb},
    {GATT_PROF_EVENT_ALL_CHARACTER, BleGattcAllCharacterCb},
    {GATT_PROF_EVENT_CHARACTER, BleGattcCharacterCb},
    {GATT_PROF_EVENT_CHAR_READ_RSP, BleGattcCharReadRspCb},
    {GATT_PROF_EVENT_DESC_READ_RSP, BleGattcDescReadRspCb},
    {GATT_PROF_EVENT_CHAR_WRITE_RSP, BleGattcCharWriteRspCb},
    {GATT_PROF_EVENT_DESC_WRITE_RSP, BleGattcDescWriteRspCb},
    {GATT_PROF_EVENT_NOTIFY, BleGattcNotifyCb},
};

static int BleGattcProfileCallback(gatt_prf_t *prf, gatt_profile_event_t event, gatt_profile_callback_param_t param)
{
    int ret = BT_STS_SUCCESS;

    for (int i=0; i< BleGattcEnv.GattcEventNum; ++i)
    {
        if (event == GattcStackEventList[i].event)
        {
            ret = GattcStackEventList[i].BleGattcCallback(prf, param);
            break;
        }
    }

    return ret;
}

static int BleInitScanStartedCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitScanStopedCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitScanAdvReportCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitStartedCb(uintptr_t init, gap_init_callback_param_t param)
{
    OhosGattcObj_t *UserGattc = NULL;

    if (param.init_started->error_code)
    {
        osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
        UserGattc = &BleGattcEnv.UserClient[BleGattcEnv.CreatConnCliId];
        UserGattc->ConnState = OHOS_STATE_DISCONNECTED;
        osMutexRelease(BleGattcEnv.mutex_id);
        UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
            UserGattc->ConnState, OHOS_BT_STATUS_FAIL);
    }

    return BT_STS_SUCCESS;
}

static int BleInitStopedCb(uintptr_t init, gap_init_callback_param_t param)
{
    OhosGattcObj_t *UserGattc = NULL;

    UserGattc = &BleGattcEnv.UserClient[BleGattcEnv.CreatConnCliId];
    if (param.init_stopped->error_code)
    {
        UserGattc->ConnState = OHOS_STATE_DISCONNECTED;
        UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
            UserGattc->ConnState, OHOS_BT_STATUS_FAIL);
    }
    else
    {
        if (UserGattc->ConnState == OHOS_STATE_DISCONNECTING)
        {
            UserGattc->ConnState = OHOS_STATE_DISCONNECTED;
            UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
                UserGattc->ConnState, OHOS_BT_STATUS_SUCCESS);
        }
    }
    //BleGattcEnv.CreatConnCliId = OHOS_ClIENT_INVALID_ID;

    return BT_STS_SUCCESS;
}

static int BleInitConnOpenedCb(uintptr_t init, gap_init_callback_param_t param)
{
    OhosGattcObj_t *UserGattc = NULL;

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[BleGattcEnv.CreatConnCliId];
    memcpy(&UserGattc->ConnInfo, param.conn_opened, sizeof(gap_conn_param_t));
    UserGattc->ConnState = OHOS_STATE_CONNECTED;
    BleGattcEnv.CreatConnCliId = OHOS_ClIENT_INVALID_ID;
    osMutexRelease(BleGattcEnv.mutex_id);

    UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
        UserGattc->ConnState, OHOS_BT_STATUS_SUCCESS);

    return BT_STS_SUCCESS;
}

static int BleInitConnClosedCb(uintptr_t init, gap_init_callback_param_t param)
{
    int state;
    OhosGattcObj_t *UserGattc = NULL;

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    for (int i=0; i<OHOS_CONN_MAX; ++i)
    {
        if (BleGattcEnv.UserClient[i].ConnInfo.connhdl == param.conn_closed->connhdl)
        {
            UserGattc = &BleGattcEnv.UserClient[i];
            break;
        }
    }

    if (UserGattc->ConnState == OHOS_STATE_DISCONNECTING)
    {
        state = OHOS_BT_STATUS_SUCCESS;
    }
    else
    {
        state  =OHOS_BT_STATUS_FAIL;
    }

    memset(&UserGattc->ConnInfo, 0, sizeof(gap_conn_param_t));
    UserGattc->ConnState = OHOS_STATE_DISCONNECTED;
    osMutexRelease(BleGattcEnv.mutex_id);
    UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
        UserGattc->ConnState, state);

    return BT_STS_SUCCESS;
}

static int BleInitConnFailedCb(uintptr_t init, gap_init_callback_param_t param)
{
    OhosGattcObj_t *UserGattc = NULL;

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[BleGattcEnv.CreatConnCliId];
    memset(&UserGattc->ConnInfo, 0, sizeof(gap_conn_param_t));
    BleGattcEnv.CreatConnCliId = OHOS_ClIENT_INVALID_ID;
    osMutexRelease(BleGattcEnv.mutex_id);

    if (UserGattc->ConnState != OHOS_STATE_DISCONNECTED)
    {
        UserGattc->ConnState = OHOS_STATE_DISCONNECTED;
        UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
            UserGattc->ConnState, OHOS_BT_STATUS_FAIL);
    }

    return BT_STS_SUCCESS;
}

static int BleInitConnMtuExchangedCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitUserConfirmCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitConnEncryptedCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitRecvKeyDistCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitConnUpdateReqCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitConnParmsUpdateCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitConnSubrateChangeCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitConnPhyUpdateCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static int BleInitTxPowerReportCb(uintptr_t init, gap_init_callback_param_t param)
{

    return BT_STS_SUCCESS;
}

static const ohos_init_stack_event_list_t InitStackEventList[] =
{
    {GAP_INIT_EVENT_SCAN_STARTED, BleInitScanStartedCb},
    {GAP_INIT_EVENT_SCAN_STOPPED, BleInitScanStopedCb},
    {GAP_INIT_EVENT_SCAN_ADV_REPORT, BleInitScanAdvReportCb},
    {GAP_INIT_EVENT_INIT_STARTED, BleInitStartedCb},
    {GAP_INIT_EVENT_INIT_STOPPED, BleInitStopedCb},
    {GAP_INIT_EVENT_CONN_OPENED, BleInitConnOpenedCb},
    {GAP_INIT_EVENT_CONN_CLOSED, BleInitConnClosedCb},
    {GAP_INIT_EVENT_CONN_FAILED, BleInitConnFailedCb},
    {GAP_INIT_EVENT_CONN_MTU_EXCHANGED, BleInitConnMtuExchangedCb},
    {GAP_INIT_EVENT_USER_CONFIRM, BleInitUserConfirmCb},
    {GAP_INIT_EVENT_CONN_ENCRYPTED, BleInitConnEncryptedCb},
    {GAP_INIT_EVENT_RECV_KEY_DIST, BleInitRecvKeyDistCb},
    {GAP_INIT_EVENT_CONN_UPDATE_REQ, BleInitConnUpdateReqCb},
    {GAP_INIT_EVENT_CONN_PARMS_UPDATE, BleInitConnParmsUpdateCb},
    {GAP_INIT_EVENT_CONN_SUBRATE_CHANGE, BleInitConnSubrateChangeCb},
    {GAP_INIT_EVENT_CONN_PHY_UPDATE, BleInitConnPhyUpdateCb},
    {GAP_INIT_EVENT_TX_POWER_REPORT, BleInitTxPowerReportCb},
};

static int BleGapConnCallback(uintptr_t init, gap_init_event_t event, gap_init_callback_param_t param)
{
    int ret = BT_STS_SUCCESS;

    for (int i=0; i< BleGattcEnv.InitEventNum; ++i)
    {
        if (event == InitStackEventList[i].event)
        {
            ret = InitStackEventList[i].BleInitCallback(init, param);
            break;
        }
    }

    return ret;
}

/****************************Api function implementation****************************/
int BleGattcRegister(BtUuid appUuid)
{
    int clientId = OHOS_ClIENT_INVALID_ID;

    if (!BleGattcEnv.mutex_id)
    {
        BleGattcEnv.mutex_id = osMutexCreate(&BleGattcEnv.mutex);
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    for (int i=0; i<OHOS_CONN_MAX; ++i)
    {
        if (!BleGattcEnv.UserClient[i].Registered)
        {
            clientId = i;
            break;
        }
    }

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        LOG_W("[%s][%d]: gattc register fail!!!", __func__, __LINE__);
        osMutexRelease(BleGattcEnv.mutex_id);
        return clientId;
    }

    if (!BleGattcEnv.GattcEventNum)
    {
        BleGattcEnv.GattcEventNum = ARRAY_SIZE(GattcStackEventList);
    }
    if (!BleGattcEnv.InitEventNum)
    {
        BleGattcEnv.InitEventNum = ARRAY_SIZE(InitStackEventList);
    }

    BleGattcEnv.UserClient[clientId].Registered = true;
    BleGattcEnv.UserClient[clientId].FastestConnFlag = false;
    BleGattcEnv.UserClient[clientId].ClientId = clientId;
    BleGattcEnv.UserClient[clientId].ConnState = OHOS_STATE_DISCONNECTED;
    BleGattcEnv.UserClient[clientId].AppUser.UuidLen = appUuid.uuidLen;
    memcpy(BleGattcEnv.UserClient[clientId].AppUser.Uuid, appUuid.uuid, appUuid.uuidLen);
    BleGattcEnv.UserClient[clientId].UserFuncCb = NULL;
    BleGattcEnv.UserClientNum++;
    osMutexRelease(BleGattcEnv.mutex_id);

    BleGattcEnv.UserClient[clientId].PrfId = gattc_register_profile(sizeof(OhosGattcPrf_t), BleGattcProfileCallback, true);

    return clientId;
}

int BleGattcUnRegister(int clientId)
{
    gattc_unregister_profile(BleGattcEnv.UserClient[clientId].StackPrf.prf->prf_id);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattcConnect(int clientId, BtGattClientCallbacks *func, const BdAddr *bdAddr,
    bool isAutoConnect, BtTransportType transport)
{
    int ret;
    OhosGattcObj_t *UserGattc = NULL;
    gap_init_param_t param = {0};

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    if (BleGattcEnv.CreatConnCliId != OHOS_ClIENT_INVALID_ID)
    {
        osMutexRelease(BleGattcEnv.mutex_id);
        return OHOS_BT_STATUS_BUSY;
    }

    UserGattc = &BleGattcEnv.UserClient[clientId];
    if (OHOS_STATE_DISCONNECTED != UserGattc->ConnState)
    {
        osMutexRelease(BleGattcEnv.mutex_id);
        LOG_E("gattc conn is no disconnect, not new connection!");
        return OHOS_BT_STATUS_FAIL;
    }

    // save user param
    UserGattc->UserFuncCb = func;

    // call stack api
    param.peer_type = BT_ADDR_TYPE_PUBLIC;
    memcpy(param.peer_addr.address, bdAddr->addr, 6);
    param.use_filter_list = UserGattc->FastestConnFlag;
    param.continue_initiating = 0;
    param.has_custom_scan_timing = 0;
    param.has_custom_init_timing = 0;
    osMutexRelease(BleGattcEnv.mutex_id);

    ret = gap_start_initiating(BT_ADDR_TYPE_PUBLIC, &param, BleGapConnCallback);

    if (BT_STS_SUCCESS != ret)
    {
        LOG_I("[%s][%d]: state=%d", __func__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    BleGattcEnv.CreatConnCliId = UserGattc->ClientId;
    UserGattc->ConnState = OHOS_STATE_CONNECTING;
    osMutexRelease(BleGattcEnv.mutex_id);
    UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
            UserGattc->ConnState, OHOS_BT_STATUS_SUCCESS);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattcSetPriority(int clientId, const BdAddr *bdAddr, BtGattPriority priority)
{
    int ret;

    gap_filter_list_clear();
    ret = gap_filter_list_add(BT_ADDR_TYPE_PUBLIC, (bt_bdaddr_t *)bdAddr);
    if (BT_STS_SUCCESS != ret)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattcSetFastestConn(int clientId, bool fastestConnFlag)
{
    OhosGattcObj_t *UserGattc = NULL;

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[clientId];
    UserGattc->FastestConnFlag = fastestConnFlag;
    osMutexRelease(BleGattcEnv.mutex_id);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattcDisconnect(int clientId)
{
    int ret = BT_STS_FAILED;
    OhosGattcObj_t *UserGattc = NULL;

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[clientId];
    osMutexRelease(BleGattcEnv.mutex_id);

    if (UserGattc->ConnState > OHOS_STATE_CONNECTED)
    {
        LOG_E("gattc conn is disconnected!");
        return OHOS_BT_STATUS_FAIL;
    }

    if (UserGattc->ConnState == OHOS_STATE_CONNECTING)
    {
        ret = gap_cancel_initiating();
    } else if (UserGattc->ConnState == OHOS_STATE_CONNECTED)
    {
        ret = gap_terminate_connection(UserGattc->ConnInfo.connhdl);
    }

    if (BT_STS_SUCCESS != ret)
    {
        LOG_I("[%s][%d]: state=%d", __func__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }

    UserGattc->ConnState = OHOS_STATE_DISCONNECTING;
    UserGattc->UserFuncCb->ConnectionStateCb(UserGattc->ClientId,
        UserGattc->ConnState, OHOS_BT_STATUS_SUCCESS);

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattcSearchServices(int clientId)
{
    int ret;
    OhosGattcObj_t *UserGattc = NULL;

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[clientId];
    UserGattc->FindSrvCount = 0;
    UserGattc->CacheSrvNum = 0;
    UserGattc->FindCharCount = 0;
    UserGattc->CacheCharNum = 0;
    UserGattc->CharByService = NULL;
    memset(UserGattc->CacheSrv, 0, sizeof(UserGattc->CacheSrv));
    memset(UserGattc->CacheChar, 0, sizeof(UserGattc->CacheChar));
    osMutexRelease(BleGattcEnv.mutex_id);

    ret = gatt_discover_all_primary_services(UserGattc->StackPrf.prf);

    if (BT_STS_SUCCESS != ret)
    {
        LOG_I("[%s][%d]: state=%d", __func__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }

    return OHOS_BT_STATUS_SUCCESS;
}

bool BleGattcGetService(int clientId, BtUuid serviceUuid)
{
    int ret = BT_STS_FAILED;
    uint8_t uuid_len;
    uint8_t uuid[16] = {0};
    gatt_peer_service_t *srv_ptr = NULL;
    OhosGattcObj_t *UserGattc = NULL;

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[clientId];
    uuid_len = BleGattsUuidCharToHex(serviceUuid.uuid, uuid);

    srv_ptr = BleGattcFindSrvByUuid(UserGattc, uuid_len, uuid);
    osMutexRelease(BleGattcEnv.mutex_id);

    //ret = gattc_get_service(ble_gattc_env.stack_prf.prf, serviceUuid.uuidLen, (uint8_t *)serviceUuid.uuid, &srv_ptr);
    if (!srv_ptr)
    {
        LOG_E("[%s][%d]: not find srv", __func__, __LINE__);
        DUMP8("%02x ", uuid, uuid_len);
        return false;
    }
    else
    {
        if (!UserGattc->FindCharCount)
        {
            LOG_I("start discovery char uuid_len=0x%x" ,uuid_len);
            DUMP8("0x%02x ", uuid, uuid_len);
            UserGattc->CharByService = srv_ptr;
            ret = gatt_client_discover_all_characters(UserGattc->StackPrf.prf->prf_id,
                UserGattc->StackPrf.prf->connhdl, srv_ptr);
        }
    }

    if (BT_STS_SUCCESS != ret)
    {
        LOG_I("[%s][%d]: state=%d", __func__, __LINE__, ret);
        return false;
    }

    return true;
}

int BleGattcRegisterNotification(int clientId, BtGattCharacteristic characteristic, bool enable)
{
    int ret;
    gatt_peer_service_t* srv_ptr = NULL;
    gatt_peer_character_t* char_ptr = NULL;
    uint8_t srv_uuid_len;
    uint8_t srv_uuid[16] = {0};
    uint8_t char_uuid_len;
    uint8_t char_uuid[16] = {0};
    OhosGattcObj_t *UserGattc = NULL;

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[clientId];
    srv_uuid_len = BleGattsUuidCharToHex(characteristic.serviceUuid.uuid, srv_uuid);
    char_uuid_len = BleGattsUuidCharToHex(characteristic.characteristicUuid.uuid, char_uuid);

    srv_ptr = BleGattcFindSrvByUuid(UserGattc,srv_uuid_len, srv_uuid);
    //ret = gattc_get_service(ble_gattc_env.stack_prf.prf, srv_uuid_len, srv_uuid, &srv_ptr);
    if (!srv_ptr)
    {
        osMutexRelease(BleGattcEnv.mutex_id);
        return OHOS_BT_STATUS_FAIL;
    }
    char_ptr = BleGattcFindcharByUuid(UserGattc, char_uuid_len, char_uuid);
    //ret = gattc_get_character(srv_ptr, char_uuid_len, char_uuid, &char_ptr);
    if (!char_ptr)
    {
        osMutexRelease(BleGattcEnv.mutex_id);
        return OHOS_BT_STATUS_FAIL;
    }
    osMutexRelease(BleGattcEnv.mutex_id);

    ret = gattc_write_cccd_descriptor(UserGattc->StackPrf.prf, char_ptr, true, false);

    if (BT_STS_SUCCESS != ret)
    {
        LOG_I("[%s][%d]: state=%d", __func__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }
    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattcWriteCharacteristic(int clientId, BtGattCharacteristic characteristic,
    BtGattWriteType writeType, int len, const char *value)
{
    int ret = BT_STS_FAILED;
    gatt_peer_service_t *srv_ptr = NULL;
    gatt_peer_character_t* char_ptr = NULL;
    uint8_t srv_uuid_len;
    uint8_t srv_uuid[16] = {0};
    uint8_t char_uuid_len;
    uint8_t char_uuid[16] = {0};
    OhosGattcObj_t *UserGattc = NULL;

    if (clientId == OHOS_ClIENT_INVALID_ID)
    {
        return OHOS_BT_STATUS_FAIL;
    }

    osMutexWait(BleGattcEnv.mutex_id, osWaitForever);
    UserGattc = &BleGattcEnv.UserClient[clientId];
    srv_uuid_len = BleGattsUuidCharToHex(characteristic.serviceUuid.uuid, srv_uuid);
    char_uuid_len = BleGattsUuidCharToHex(characteristic.characteristicUuid.uuid, char_uuid);

    srv_ptr = BleGattcFindSrvByUuid(UserGattc, srv_uuid_len, srv_uuid);
    //ret = gattc_get_service(ble_gattc_env.stack_prf.prf, srv_uuid_len, srv_uuid, &srv_ptr);
    if (!srv_ptr)
    {
        osMutexRelease(BleGattcEnv.mutex_id);
        return OHOS_BT_STATUS_FAIL;
    }
    char_ptr = BleGattcFindcharByUuid(UserGattc,char_uuid_len, char_uuid);
    //ret = gattc_get_character(srv_ptr, char_uuid_len, char_uuid, &char_ptr);
    if (!char_ptr)
    {
        osMutexRelease(BleGattcEnv.mutex_id);
        return OHOS_BT_STATUS_FAIL;
    }
    osMutexRelease(BleGattcEnv.mutex_id);

    switch (writeType)
    {
        case OHOS_GATT_WRITE_NO_RSP:
        {
            ret =gattc_write_character_command(UserGattc->StackPrf.prf, char_ptr, (uint8_t *)value, len, false);
        } break;
        case OHOS_GATT_WRITE_DEFAULT:
        case OHOS_GATT_WRITE_PREPARE:
        {
            ret = gattc_write_character_value(UserGattc->StackPrf.prf, char_ptr, (uint8_t *)value, len);
        } break;
        case OHOS_GATT_WRITE_SIGNED:
        {
            ret = gattc_write_character_command(UserGattc->StackPrf.prf, char_ptr, (uint8_t *)value, len, true);
        } break;
        default:
            LOG_E("unknow write type=%d!", writeType);
    }

    if (BT_STS_SUCCESS != ret)
    {
        LOG_E("[%s][%d]: write fail, error_code=%d", __func__, __LINE__, ret);
        return OHOS_BT_STATUS_FAIL;
    }

    return OHOS_BT_STATUS_SUCCESS;
}

int BleGattcConfigureMtuSize(int clientId, int mtuSize)
{
    return OHOS_BT_STATUS_SUCCESS;
}

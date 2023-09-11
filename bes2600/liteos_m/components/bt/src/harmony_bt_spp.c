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

#include "ohos_bt_gap.h"

#include "gap_service.h"
#include "bt_adaptor_dbg.h"

/****************************Macro defination***************************/


/****************************Type defination****************************/
typedef struct
{

} BtSppEnv_t;
/****************************Function declaration***********************/

/****************************Variable defination************************/
static BtGapEnv_t BtSppEnv = {0};

/****************************Private function implementation************************/


/****************************Api function implementation****************************/
int SppServerCreate(BtCreateSocketPara *socketPara, const char *name, unsigned int len)
{
}

int SppServerAccept(int serverId)
{
}

int SppServerClose(int serverId)
{
}

int SppConnect(BtCreateSocketPara *socketPara, const BdAddr *bdAddr)
{
}

int SppDisconnect(int clientId)
{
}

bool IsSppConnected(int clientId)
{
}

int SppGetRemoteAddr(int clientId, BdAddr *remoteAddr)
{
}

int SppRead(int clientId, char *buf, const unsigned int bufLen)
{
}

int SppWrite(int clientId, const char *data, const unsigned int len)
{

}

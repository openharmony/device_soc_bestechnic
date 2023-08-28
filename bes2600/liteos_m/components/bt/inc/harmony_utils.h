#ifndef __HARMONY_UTILS_H__
#define __HARMONY_UTILS_H__
#include "plat_types.h"
#include "ohos_bt_def.h"
#include "bt_adaptor_dbg.h"
#include "adapter_service.h"

#define OHOS_CONN_MAX           5

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

//16 bit string format "XXXX"
//128 bit string format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
int BleGattsUuidCharToHex(char* uuid_str, uint8_t *uuid);
int BleGattsUuidHexToChar(const uint8_t *uuid, uint8_t uuid_len, char *uuid_string);
BtStatus bes_status_to_ohos(bt_status_t bes_status);

#endif

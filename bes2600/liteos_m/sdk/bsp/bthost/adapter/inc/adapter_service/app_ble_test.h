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
#ifndef __APP_BLE_TEST_H__
#define __APP_BLE_TEST_H__
#ifdef __cplusplus
extern "C"{
#endif

void app_ble_test_cmd_init(void);
void ble_audio_dflt_update_role(void);
void ble_audio_dflt_config(void);
bool ble_audio_dflt_check_device_is_master(uint8_t *address);
const unsigned char* ble_audio_dflt_read_peer_tws_addr(void);
void ble_audio_test_config_dynamic_audio_sharing_master(void);

#ifdef __cplusplus
}
#endif
#endif /* __APP_BLE_TEST_H__ */

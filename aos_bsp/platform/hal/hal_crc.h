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
#ifndef __HAL_CRC_H__
#define __HAL_CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"

struct HAL_CRC_CFG_T
{
    bool type_crc32;/*crc type 0:crc16 1:crc32*/
    uint32_t poly;
    uint32_t init;/*initial value*/
    bool refin;
    bool refout;
    bool xorout_flag;
};

typedef void (*HAL_CRC_RUN_CB_T)(const uint32_t res, uint32_t error);

void hal_crc_open(void);
void hal_crc_close(void);

/*common crc function*/
uint32_t hal_crc_start(const struct HAL_CRC_CFG_T* cfg, void *data, uint16_t len, HAL_CRC_RUN_CB_T cb);

/*crc16*/
uint16_t hal_crc16_ibm(void *data, uint16_t len);
uint16_t hal_crc16_maxim(void *data, uint16_t len);
uint16_t hal_crc16_usb(void *data, uint16_t len);
uint16_t hal_crc16_modbus(void *data, uint16_t len);
uint16_t hal_crc16_ccitt(void *data, uint16_t len);
uint16_t hal_crc16_ccitt_false(void *data, uint16_t len);
uint16_t hal_crc16_x25(void *data, uint16_t len);
uint16_t hal_crc16_xmodem(void *data, uint16_t len);
uint16_t hal_crc16_dnp(void *data, uint16_t len);

/*crc32*/
uint32_t hal_crc32(void *data, uint16_t len);
uint32_t hal_crc32_mpeg(void *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif

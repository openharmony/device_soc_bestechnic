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
#ifndef __HAL_I3C_H__
#define __HAL_I3C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_types.h"
#include "plat_addr_map.h"

enum HAL_I3C_ID_T {
    HAL_I3C_ID_0,
#ifdef I3C1_BASE
    HAL_I3C_ID_1,
#endif

    HAL_I3C_ID_NUM,
};

//Error Types
enum HAL_I3C_ERR {
    I3C_ERR_NONE             = 0,
    I3C_ERR_NULL_POINTER     = -1,
    I3C_ERR_INVALID_MODE     = -2,
    I3C_ERR_SYS_BUS_FAILURE  = -3,
    I3C_ERR_INVALID_PARA     = -4,
    I3C_ERR_INVALID_VALUE    = -5,
    I3C_ERR_INVALID_CMD_ATRB = -6,
    I3C_ERR_INVALID_DATA_LEN = -7,
    I3C_ERR_INVALID_RING_ID  = -8,
    I3C_ERR_INVALID_CAP_ID   = -9,
    I3C_ERR_DAA_EXCEED       = -10,
    I3C_ERR_ENUM_FAILURE     = -11,
    I3C_ERR_INTR_NOT_SET     = -12,
    I3C_ERR_DEV_NOT_FOUND    = -13,
    I3C_ERR_TID_EXCEED       = -14,
    I3C_ERR_THLD_EXCEED      = -15,
    I3C_ERR_ENTRIES_EXCEED   = -16,
    I3C_ERR_DEVICES_EXCEED   = -17,
    I3C_ERR_INPUT_CFG        = -18,
    I3C_ERR_HOST_ID          = -19,
    I3C_ERR_TIMEOUT          = -20,
    I3C_ERR_TRANSFER         = -21,
};

//CCC definitions
enum HAL_I3C_ENUM_CMD_T {
    HAL_I3C_ENUM_CMD_DAA = 0x07,
    HAL_I3C_ENUM_CMD_SA  = 0x87,
};

enum HAL_I3C_MODE_T {
    HAL_I3C_MODE_SDR0 = 0,
    HAL_I3C_MODE_SDR1 = 1,
    HAL_I3C_MODE_SDR2 = 2,
    HAL_I3C_MODE_SDR3 = 3,
    HAL_I3C_MODE_SDR4 = 4,
    HAL_I3C_MODE_HDR_TSX = 5,//TSP/TSP
    HAL_I3C_MODE_HDR_DDR = 6,
    HAL_I3C_MODE_HDR_BT = 7,
};

enum HAL_I3C_CCC_CMD_T {
    //CCC definitions
    CCC_BC_ENEC         = 0x00, //Enable events Cmd
    CCC_BC_DISEC        = 0x01, //Disable events
    CCC_BC_ENTAS0       = 0x02, //Enter Activity mode state 0
    CCC_BC_ENTAS1       = 0x03,
    CCC_BC_ENTAS2       = 0x04,
    CCC_BC_ENTAS3       = 0x05,
    CCC_BC_RSTDAA       = 0x06,
    CCC_BC_ENTDAA       = 0x07,
    CCC_BC_DEFTGTS      = 0x08,
    CCC_BC_SETMWL       = 0x09, //Set Max write length
    CCC_BC_SETMRL       = 0x0A, //Set Max read length
    CCC_BC_ENTTM        = 0x0B, //Enter Test mode
    CCC_BC_SETBUSCON    = 0x0C, //Set Bus Context
    CCC_BC_ENDXFER      = 0x12, //Data Transfer Ending Procedure Control
    CCC_BC_ENTHDR0      = 0x20,
    CCC_BC_ENTHDR1      = 0x21,
    CCC_BC_ENTHDR2      = 0x22,
    CCC_BC_ENTHDR3      = 0x23,
    CCC_BC_ENTHDR4      = 0x24,
    CCC_BC_ENTHDR5      = 0x25,
    CCC_BC_ENTHDR6      = 0x26,
    CCC_BC_ENTHDR7      = 0x27,
    CCC_BC_SETXTIME     = 0x28,
    CCC_BC_SETAASA      = 0x29,
    CCC_BC_RSTACT       = 0x2A,
    CCC_BC_DEFGRPA      = 0x2B,
    CCC_BC_RSTGRPA      = 0x2C,
    CCC_BC_MLANE        = 0x2D,
    CCC_DIR_ENEC        = 0x80, //Enable events Cmd
    CCC_DIR_DISEC       = 0x81, //Disable events
    CCC_DIR_ENTAS0      = 0x82, //Enter Activity mode state 0
    CCC_DIR_ENTAS1      = 0x83,
    CCC_DIR_ENTAS2      = 0x84,
    CCC_DIR_ENTAS3      = 0x85,
    CCC_DIR_RSTDAA      = 0x86,
    CC_DS_SETDASA       = 0x87,
    CCC_DS_SETNEWDA     = 0x88,
    CCC_DS_SETMWL       = 0x89,
    CCC_DS_SETMRL       = 0x8A,
    CCC_DG_GETMWL       = 0x8B,
    CCC_DG_GETMRL       = 0x8C,
    CCC_DG_GETPID       = 0x8D,
    CCC_DG_GETBCR       = 0x8E,
    CCC_DG_GETDCR       = 0x8F,
    CCC_DG_GETSTS       = 0x90,
    CCC_DG_GETACCCR     = 0x91,
    CCC_DIR_ENDXFER     = 0x92,
    CCC_DS_SETBRGTGT    = 0x93,
    CCC_DG_GETMXDS      = 0x94,
    CCC_DG_GETCAPS      = 0x95,
    CCC_DIR_SETROUTE    = 0x96,
    CCC_DIR_D2DXFER     = 0x97,
    CCC_DIR_SETXTIME    = 0x98,
    CCC_DIR_GETXTIME    = 0x99,
    CCC_DIR_RSTACT      = 0x9A,
    CCC_DIR_SETGRPA     = 0x9B,
    CCC_DIR_RSTGRPA     = 0x9C,
    CCC_DIR_MLANE       = 0x9D,
};

struct HAL_I3C_CONFIG_T {
    enum HAL_I3C_MODE_T mode;
    uint32_t speed;
    uint8_t dev_cnt : 4;           //i3c + i2c device count
    uint8_t use_dma : 1;
    uint8_t i2c_dev_present : 1;   //i2c device present
    uint8_t inc_broadcast_addr : 1;//Include I3C Broadcast Address

    uint8_t *static_addr;
};

//Funtion prototypes
enum HAL_I3C_ERR hal_i3c_open(enum HAL_I3C_ID_T id, const struct HAL_I3C_CONFIG_T *cfg);
void hal_i3c_close(enum HAL_I3C_ID_T id);

enum HAL_I3C_ERR hal_i3c_bus_enumeration(enum HAL_I3C_ID_T id, enum HAL_I3C_ENUM_CMD_T enum_cmd);
enum HAL_I3C_ERR hal_i3c_ccc_read(enum HAL_I3C_ID_T id, uint8_t dev_index, enum HAL_I3C_CCC_CMD_T cmd, uint16_t data_len, uint8_t *data);

enum HAL_I3C_ERR hal_i3c_send(enum HAL_I3C_ID_T id, uint8_t dev_index, const void *buf, uint16_t reg_len, uint16_t data_len);
enum HAL_I3C_ERR hal_i3c_recv(enum HAL_I3C_ID_T id, uint8_t dev_index, uint8_t *buf, uint16_t reg_len, uint16_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_I3C_H__ */

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
#ifndef __BT_GATT_SERVICE_H__
#define __BT_GATT_SERVICE_H__
#include "gap_service.h"
#include "l2cap_service.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ATT_ERROR_NO_ERROR                  = 0x00,
    ATT_ERROR_INVALID_HANDLE            = 0x01,
    ATT_ERROR_RD_NOT_PERMITTED          = 0x02,
    ATT_ERROR_WR_NOT_PERMITTED          = 0x03,
    ATT_ERROR_INVALID_PDU               = 0x04,
    ATT_ERROR_INSUFF_AUTHEN             = 0x05,
    ATT_ERROR_REQ_NOT_SUPPORT           = 0x06,
    ATT_ERROR_INVALID_OFFSET            = 0x07,
    ATT_ERROR_INSUFF_AUTHOR             = 0x08,
    ATT_ERROR_PREPARE_QUEUE_FULL        = 0x09,
    ATT_ERROR_ATTRIBUTE_NOT_FOUND       = 0x0A,
    ATT_ERROR_ATTRIBUTE_NOT_LONG        = 0x0B,
    ATT_ERROR_ENC_KEY_TOO_SHORT         = 0x0C,
    ATT_ERROR_INVALID_ATTR_VALUE_LEN    = 0x0D,
    ATT_ERROR_UNLIKELY_ERROR            = 0x0E,
    ATT_ERROR_INSUFF_ENCRYPT            = 0x0F,
    ATT_ERROR_UNSUPP_GROUP_TYPE         = 0x10,
    ATT_ERROR_INSUFF_RESOURCES          = 0x11,
    ATT_ERROR_DATABASE_OUT_OF_SYNC      = 0x12,
    ATT_ERROR_VALUE_NOT_ALLOWED         = 0x13,
    ATT_ERROR_APPLICATION_ERROR         = 0x80, // 0x80 ~ 0x9F application error code defined by a higher layer spec
    ATT_ERROR_UNKNOWN_ERROR             = 0x81,
    ATT_ERROR_SEND_FAILED               = 0x82,
    ATT_ERROR_MEMORY_FULL               = 0x83,
    ATT_ERROR_RESPONSE_TIMEOUT          = 0x84,
    ATT_ERROR_BEARER_CLOSED             = 0x85,
    ATT_ERROR_CONN_CLOSED               = 0x86,
    ATT_ERROR_UUID_NOT_FOUND            = 0x87,
    ATT_ERROR_RSP_DATA_INVALID          = 0x88,
    ATT_ERROR_RSP_INVALID_HANDLE        = 0x89,
    ATT_ERROR_INVALID_CONN              = 0x8A,
    ATT_ERROR_INVALID_MTU               = 0x8B,
    ATT_ERROR_INVALID_UUID_LEN          = 0x8C,
    ATT_ERROR_INVALID_VALUE_LEN         = 0x8D,
    ATT_ERROR_INVALID_CACHE_HANDLE      = 0x8E,
    ATT_ERROR_MUST_SHORT_VALUE          = 0x8F,
    ATT_ERROR_WRITE_DECLARE_ATTR        = 0x90,
    ATT_ERROR_NO_INCLUDE_EXIST          = 0x91,
    ATT_ERROR_ADD_INCL_SERVICE_FAILED   = 0x92,
    ATT_ERROR_NOT_FOUND                 = 0x93,
    ATT_ERROR_IBRT_SLAVE_CANT_SEND      = 0x94,
    ATT_ERROR_PEER_SERVICE_CHANGED      = 0x95,
    ATT_ERROR_COMMON_PROFILE_SERVICE    = 0xE0, // 0xE0 ~ 0xFF common profile and service error codes defined in CSS
    ATT_ERROR_COMMON_PROFILE_SERVICE_RFU= 0xFB, // 0xE0 ~ 0xFB reserved for future use
    ATT_ERROR_WRITE_REQ_REJECTED        = 0xFC,
    ATT_ERROR_CCCD_IMPROPER_CONFIGED    = 0xFD,
    ATT_ERROR_PROC_ALREADY_IN_PROGRESS  = 0xFE,
    ATT_ERROR_OUT_OF_RANGE              = 0xFF,
} att_error_code_t;

#define ATT_NO_PERM         0x00
#define ATT_SEC_NONE        0x00
#define ATT_RD_PERM         0x01
#define ATT_RD_MITM_AUTH    0x02
#define ATT_RD_AUTHOR       0x04
#define ATT_RD_ENC          0x08
#define ATT_WR_PERM         0x10
#define ATT_WR_MITM_AUTH    0x20
#define ATT_WR_AUTHOR       0x40
#define ATT_WR_ENC          0x80

#define ATT_FLAG_IS_128_UUID 0x01
#define ATT_FLAG_CONST_VALUE 0x02
#define ATT_FLAG_OPTION_CHAR 0x04
#define ATT_FLAG_ARRAY_VALID 0x08
#define ATT_FLAG_IND_AUTH    0x10
#define ATT_FLAG_IND_ENC     0x20
#define ATT_FLAG_NTF_AUTH    0x40
#define ATT_FLAG_NTF_ENC     0x80

#define GATT_NO_PROP   0x00
#define GATT_BROADCAST 0x01 // permit broadcasts of char value using SCCD, if set, SCCD shall exist
#define GATT_RD_REQ    0x02 // permit read req of the char value
#define GATT_WR_CMD    0x04 // permit write cmd of the char value
#define GATT_WR_REQ    0x08 // permit write req of the char value
#define GATT_NTF_PROP  0x10 // permit notification of the char value, if set, CCCD shall exist
#define GATT_IND_PROP  0x20 // permit indication of the char value with ack, if set, CCCD shall exist
#define GATT_SIGNED_WR 0x40 // permit authenticated signed write of the char value
#define GATT_EXT_PROP  0x80 // additional char properties are defined in the CEPD, if set, CEPD shall exist
#define GATT_EXT_PROP_RELIABLE_WRITE 0x0001 // permit reliable writes of the char value
#define GATT_EXT_PROP_WRITABLE_AUX   0x0002 // permit write CUSD descriptor

#define ATT_RW_PERM_FROM_PROP(p) \
    ((((p) & (GATT_RD_REQ)) ? ATT_RD_PERM : 0) | \
     (((p) & (GATT_WR_CMD|GATT_WR_REQ|GATT_SIGNED_WR)) ? ATT_WR_PERM : 0))

#define ATT_16BIT_UUID_WITH_FLAGS(uuid, perm, flags) \
    CO_UINT8_VALUE(flags), \
    CO_UINT8_VALUE(perm), \
    CO_SPLIT_UINT16_LE(uuid)

#define ATT_128BE_UUID_WITH_FLAGS(uuid_128_be, perm, flags) \
    CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID|flags), \
    CO_UINT8_VALUE(perm), \
    CO_SPLIT_128_UUID_LE(uuid_128_be)

#define ATT_16BIT_UUID(uuid, perm) \
    ATT_16BIT_UUID_WITH_FLAGS(uuid, perm, 0)

#define ATT_128BE_UUID(uuid_128_be, perm) \
    ATT_128BE_UUID_WITH_FLAGS(uuid_128_be, perm, 0)

#define ATT_16BIT_CONST_VALUE(uuid, perm) \
    ATT_16BIT_UUID_WITH_FLAGS(uuid, perm, ATT_FLAG_CONST_VALUE)

#define ATT_128BE_CONST_VALUE(uuid_128_be, perm) \
    ATT_128BE_UUID_WITH_FLAGS(uuid_128_be, perm, ATT_FLAG_CONST_VALUE)

#define GATT_EXTERN_SERVICE(name) \
    extern const uint8_t name[]

#define GATT_DECL_PRI_SERVICE(name, service_uuid) \
    const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_UUID_PRI_SERVICE, ATT_RD_PERM), \
        CO_SPLIT_UINT16_LE(service_uuid), \
    }

#define GATT_DECL_128_BE_PRI_SERVICE(name, service_128_uuid_be) \
    const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_UUID_PRI_SERVICE, ATT_RD_PERM), \
        CO_SPLIT_128_UUID_LE(service_128_uuid_be), \
    }

#define GATT_DECL_128_LE_PRI_SERVICE(name, service_128_uuid_le) \
    const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_UUID_PRI_SERVICE, ATT_RD_PERM), \
        service_128_uuid_le, \
    }

#define GATT_DECL_SEC_SERVICE(name, service_uuid) \
    const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_UUID_SEC_SERVICE, ATT_RD_PERM), \
        CO_SPLIT_UINT16_LE(service_uuid), \
    }

#define GATT_DECL_128_BE_SEC_SERVICE(name, service_128_uuid_be) \
    const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_UUID_SEC_SERVICE, ATT_RD_PERM), \
        CO_SPLIT_128_UUID_LE(service_128_uuid_be), \
    }

#define GATT_DECL_128_LE_SEC_SERVICE(name, service_128_uuid_le) \
    const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_UUID_SEC_SERVICE, ATT_RD_PERM), \
        service_128_uuid_le, \
    }

#define GATT_DECL_INC_SERVICE(name, included_service_name) \
    static const gatt_decl_inc_service_t name = { \
        ATT_16BIT_UUID(GATT_UUID_INCLUDE_SERVICE, ATT_RD_PERM), \
        (uint32_t)(uintptr_t)(included_service_name), /* included service address */ \
    }

#define GATT_DECL_CHAR(char_name, char_uuid, char_prop, char_value_sec_perm) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        ATT_16BIT_UUID(char_uuid, ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
    }

#define GATT_DECL_CHAR_WITH_FLAG(char_name, char_uuid, char_prop, char_value_sec_perm, flag) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), (flag) | \
        ATT_16BIT_UUID(char_uuid, ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
    }

// if the value is read-only, but the server can change it, it is not const value
#define GATT_DECL_CHAR_WITH_CONST_VALUE(char_name, char_uuid, char_prop, char_value_sec_perm, ...) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        ATT_16BIT_CONST_VALUE(char_uuid, ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        __VA_ARGS__, \
    }

#define GATT_DECL_OPTIONAL_CHAR(char_name, char_uuid, char_prop, char_value_sec_perm) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), ATT_FLAG_OPTION_CHAR | \
        ATT_16BIT_UUID(char_uuid, ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
    }

#define GATT_DECL_OPTIONAL_CHAR_WITH_FLAG(char_name, char_uuid, char_prop, char_value_sec_perm, flag) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), ATT_FLAG_OPTION_CHAR | (flag) | \
        ATT_16BIT_UUID(char_uuid, ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
    }

// if the value is read-only, but the server can change it, it is not const value
#define GATT_DECL_OPTIONAL_CHAR_WITH_CONST_VALUE(char_name, char_uuid, char_prop, char_value_sec_perm, ...) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), ATT_FLAG_OPTION_CHAR | \
        ATT_16BIT_CONST_VALUE(char_uuid, ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        __VA_ARGS__, \
    }

#define GATT_DECL_128_BE_CHAR(char_name, char_128_uuid_be, char_prop, char_value_sec_perm) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        CO_SPLIT_128_UUID_LE(char_128_uuid_be), \
    }

#define GATT_DECL_128_LE_CHAR(char_name, char_128_uuid_le, char_prop, char_value_sec_perm) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        char_128_uuid_le, \
    }

// if the value is read-only, but the server can change it, it is not const value
#define GATT_DECL_128_BE_CHAR_WITH_CONST_VALUE(char_name, char_128_uuid_be, char_prop, char_value_sec_perm, ...) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID|ATT_FLAG_CONST_VALUE), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        CO_SPLIT_128_UUID_LE(char_128_uuid_be), \
        __VA_ARGS__, \
    }

// if the value is read-only, but the server can change it, it is not const value
#define GATT_DECL_128_LE_CHAR_WITH_CONST_VALUE(char_name, char_128_uuid_le, char_prop, char_value_sec_perm, ...) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID|ATT_FLAG_CONST_VALUE), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        char_128_uuid_le, \
        __VA_ARGS__, \
    }

#define GATT_DECL_OPTIONAL_128_BE_CHAR(char_name, char_128_uuid_be, char_prop, char_value_sec_perm) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID|ATT_FLAG_OPTION_CHAR), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        CO_SPLIT_128_UUID_LE(char_128_uuid_be), \
    }

#define GATT_DECL_OPTIONAL_128_LE_CHAR(char_name, char_128_uuid_le, char_prop, char_value_sec_perm) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID|ATT_FLAG_OPTION_CHAR), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        char_128_uuid_le, \
    }

// if the value is read-only, but the server can change it, it is not const value
#define GATT_DECL_OPTIONAL_128_BE_CHAR_WITH_CONST_VALUE(char_name, char_128_uuid_be, char_prop, char_value_sec_perm, ...) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID|ATT_FLAG_OPTION_CHAR|ATT_FLAG_CONST_VALUE), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        CO_SPLIT_128_UUID_LE(char_128_uuid_be), \
        __VA_ARGS__, \
    }

// if the value is read-only, but the server can change it, it is not const value
#define GATT_DECL_OPTIONAL_128_LE_CHAR_WITH_CONST_VALUE(char_name, char_128_uuid_le, char_prop, char_value_sec_perm, ...) \
    static const uint8_t char_name[] = { \
        ATT_16BIT_UUID(GATT_UUID_CHAR_DECLARE, ATT_RD_PERM), \
        CO_UINT8_VALUE(char_prop), \
        CO_UINT8_VALUE(ATT_FLAG_IS_128_UUID|ATT_FLAG_OPTION_CHAR|ATT_FLAG_CONST_VALUE), \
        CO_UINT8_VALUE(ATT_RW_PERM_FROM_PROP(char_prop)|char_value_sec_perm), \
        char_128_uuid_le, \
        __VA_ARGS__, \
    }

// Characteristic Extended Properties Descriptor (CEPD)
#define GATT_DECL_CEPD_DESCRIPTOR(name, char_ext_prop) \
    static const uint8_t name[] = { \
        ATT_16BIT_CONST_VALUE(GATT_DESC_UUID_CHAR_EXT_PROP, ATT_RD_PERM), \
        CO_SPLIT_UINT16_LE(char_ext_prop), \
    }

// Characteristic User Description Descriptor (CUDD)
#define GATT_DECL_CUDD_DESCRIPTOR(name, perm) \
    static const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_DESC_UUID_CHAR_USER_DESC, ATT_RD_PERM|perm), \
    }

#define GATT_DECL_CONST_CUDD_DESCRIPTOR(name, ...) \
    static const uint8_t name[] = { \
        ATT_16BIT_CONST_VALUE(GATT_DESC_UUID_CHAR_USER_DESC, ATT_RD_PERM), \
        __VA_ARGS__ \
    }

// Client Characteristic Configuration Descriptor (CCCD)
#define GATT_DECL_CCCD_DESCRIPTOR(name, write_sec_perm) \
    static const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_DESC_UUID_CHAR_CLIENT_CONFIG, ATT_RD_PERM|ATT_WR_PERM|(write_sec_perm)), \
    }

// Server Characteristic Configuration Descriptor (SCCD)
#define GATT_DECL_SCCD_DESCRIPTOR(name, write_sec_perm) \
    static const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_DESC_UUID_CHAR_SERVER_CONFIG, ATT_RD_PERM|ATT_WR_PERM|(write_sec_perm)), \
    }

// Characteristic Presentation Format Descriptor (CPFD)
#define GATT_DECL_CPFD_DESCRIPTOR(name) \
    static const uint8_t name[] = { \
        ATT_16BIT_UUID(GATT_DESC_UUID_EXT_REPORT_REFERENCE, ATT_RD_PERM), \
    }

#define GATT_DECL_CONST_CPFD_DESCRIPTOR(name, format, exponent, unit, name_space, description) \
    static const uint8_t name[] = { \
        ATT_16BIT_CONST_VALUE(GATT_DESC_UUID_EXT_REPORT_REFERENCE, ATT_RD_PERM), \
        CO_UINT8_VALUE(format), \
        CO_UINT8_VALUE(exponent), \
        CO_SPLIT_UINT16_LE(unit), \
        CO_UINT8_VALUE(name_space), \
        CO_SPLIT_UINT16_LE(description), \
    }

// External Report Reference Characteristic Descriptor (ERRD)
#define GATT_DECL_ERRD_DESCRIPTOR(name, ext_refer_char_uuid) \
    static const uint8_t name[] = { \
        ATT_16BIT_CONST_VALUE(GATT_DESC_UUID_EXT_REPORT_REFERENCE, ATT_RD_PERM), \
        CO_SPLIT_UINT16_LE(ext_refer_char_uuid), \
    }

// Report Reference Characteristic Descriptor (RRCD)
#define GATT_DECL_RRCD_DESCRIPTOR(name) \
    static const uint8_t name[] = { \
        ATT_16BIT_CONST_VALUE(GATT_DESC_UUID_REPORT_REFERENCE, ATT_RD_PERM), \
    }

#define GATT_DECL_COSNT_RRCD_DESCRIPTOR(name, report_id, report_type) \
    static const uint8_t name[] = { \
        ATT_16BIT_CONST_VALUE(GATT_DESC_UUID_REPORT_REFERENCE, ATT_RD_PERM), \
        CO_UINT8_VALUE(report_id), \
        CO_UINT8_VALUE(report_type), \
    }

// Encrypted Data Key Material Characteristic
#define GATT_DECL_ENC_DATA_KEY_MATERIAL_CHAR(name) \
    GATT_DECL_CHAR_WITH_FLAG(name, \
        GATT_CHAR_UUID_ENCRYPTED_DATA_KEY_MATERIAL, \
        GATT_RD_REQ|GATT_IND_PROP, \
        ATT_RD_ENC|ATT_RD_AUTHOR, \
        ATT_FLAG_IND_ENC)

#define GATT_DECL_DESCRIPTOR(name, uuid, att_perm) \
    static const uint8_t name[] = { \
        ATT_16BIT_UUID(uuid, att_perm), \
    }

#define gatt_include_attribute(a) {(attr_byte_array_t*)(&a), sizeof(gatt_decl_inc_service_t)}

#define gatt_include_instance(a, inst_id) {(attr_byte_array_t*)(&a), sizeof(gatt_decl_inc_service_t), inst_id}

#define gatt_char_attribute(a, inst_id) {(attr_byte_array_t*)(a), sizeof(a), inst_id}

#define gatt_attribute(a) {(attr_byte_array_t*)(a), sizeof(a)}

typedef struct {
    uint8_t format;
    uint8_t exponent;
    uint16_t unit;
    uint8_t name_space;
    uint16_t description;
} __attribute__ ((packed)) gatt_cpfd_data_t;

typedef struct {
    uint8_t attr_flags;
    uint8_t attr_perm;
    uint8_t uuid_le_l;
    uint8_t uuid_le_h;
    uint32_t inc_srvc_addr;
} gatt_decl_inc_service_t;

typedef struct {
    uint8_t report_id;
    uint8_t report_type;
} __attribute__ ((packed)) gatt_rrcd_value_t;

typedef struct {
    uint8_t attr_flags;
    uint8_t attr_perm;
    uint8_t uuid_le[2];
} __attribute__ ((packed)) attr_byte_array_t;

typedef struct {
    attr_byte_array_t *attr_data; // [perm:1B][flags:1B][uuid:2B or 16B][attr_value:nB]
    uint16_t data_len;
    uint16_t inst_id;
} gatt_attribute_t;

typedef struct gatt_service_t gatt_service_t;
typedef struct gatt_character_t gatt_character_t;

#define GATT_SERVER_FEAT_EATT_BEARER       0x01
#define GATT_CLIENT_FEAT_ROBUST_CACHING    0x01
#define GATT_CLIENT_FEAT_EATT_BEARER       0x02
#define GATT_CLIENT_FEAT_MULT_VALUE_NOTIFY 0x04

#define GATT_CCCD_SET_NOTIFICATION 0x01 // the char value shall be notified, can only be set if char ntf prop set
#define GATT_CCCD_SET_INDICATION 0x02 // the char value shall be indicated, can only be set if char ind prop set

#define GAP_SERVICE_START_HANDLE    0x01
#define GAP_SERVICE_END_HANDLE      0x1F
#define GATT_SERVICE_START_HANDLE   0x20
#define GATT_SERVICE_END_HANDLE     0x3F

void gatt_init(void);

typedef struct {
    const uint8_t *character;
    const uint8_t *value;
    uint16_t value_len;
    uint8_t char_instance_id;
    const uint8_t *service;
    uint8_t service_inst_id;
    /* below is for internal use */
    uint16_t cccd_config;
    gatt_service_t *s;
    gatt_character_t *c;
} gatt_multi_notify_item_t;

typedef struct {
    void *read_context;
    uint32_t token;
} gatt_server_read_ctx_t;

typedef struct {
    void *write_context;
    uint32_t token;
} gatt_server_write_ctx_t;

typedef struct {
    uint8_t prf_id;
    uint8_t con_idx;
    uint16_t connhdl;
} gatt_prf_t;

typedef struct {
    uint8_t service_inst_id;
    uint8_t con_idx;
    uint16_t connhdl;
} gatt_svc_t;

typedef struct {
    const gatt_attribute_t *service;
    bt_status_t status;
} gatt_server_register_cmp_t;

typedef struct {
    uint8_t con_idx;
    uint16_t value_offset;
    gap_conn_item_t *conn;
    const uint8_t *service;
    const uint8_t *character;
    const gatt_attribute_t *char_attr;
    att_error_code_t rsp_error_code;
    gatt_server_read_ctx_t *ctx;
} gatt_server_char_read_t;

typedef struct {
    uint8_t con_idx;
    uint16_t value_offset;
    gap_conn_item_t *conn;
    const uint8_t *service;
    const uint8_t *character;
    const gatt_attribute_t *char_attr;
    const uint8_t *value;
    uint16_t value_len;
    att_error_code_t rsp_error_code;
    gatt_server_write_ctx_t *ctx;
} gatt_server_char_write_t;

typedef struct {
    uint8_t con_idx;
    uint16_t value_offset;
    gap_conn_item_t *conn;
    const uint8_t *service;
    const uint8_t *character;
    const gatt_attribute_t *char_attr;
    att_error_code_t rsp_error_code;
    gatt_server_read_ctx_t *ctx;
    const uint8_t *descriptor;
} gatt_server_desc_read_t;

typedef struct {
    uint8_t con_idx;
    uint16_t value_offset;
    gap_conn_item_t *conn;
    const uint8_t *service;
    const uint8_t *character;
    const gatt_attribute_t *char_attr;
    const uint8_t *value;
    uint16_t value_len;
    att_error_code_t rsp_error_code;
    gatt_server_write_ctx_t *ctx;
    const uint8_t *descriptor;
} gatt_server_desc_write_t;

typedef struct {
    uint8_t con_idx;
    att_error_code_t error_code;
    gap_conn_item_t *conn;
    const uint8_t *service;
    const uint8_t *character;
    const gatt_attribute_t *char_attr;
} gatt_server_indicate_cfm_t;

typedef struct {
    gap_conn_item_t *conn;
    const uint8_t *service;
    uint8_t con_idx;
    uint8_t error_code;
    uint16_t mtu;
} gatt_server_conn_param_t;

typedef gatt_server_conn_param_t gatt_server_conn_opened_t;
typedef gatt_server_conn_param_t gatt_server_conn_closed_t;
typedef gatt_server_conn_param_t gatt_server_mtu_changed_t;
typedef gatt_server_conn_param_t gatt_server_conn_updated_t;
typedef gatt_server_conn_param_t gatt_server_conn_encrypted_t;

typedef union {
    void *param_ptr;
    gatt_server_register_cmp_t *resgiter_cmp;
    gatt_server_conn_param_t *conn_param;
    gatt_server_conn_opened_t *opened;
    gatt_server_conn_closed_t *closed;
    gatt_server_char_read_t *char_read;
    gatt_server_desc_read_t *desc_read;
    gatt_server_char_write_t *char_write;
    gatt_server_desc_write_t *desc_write;
    gatt_server_indicate_cfm_t *confirm;
    gatt_server_mtu_changed_t *mtu_changed;
    gatt_server_conn_updated_t *conn_updated;
    gatt_server_conn_encrypted_t *conn_encrypted;
} gatt_server_callback_param_t;

typedef enum {
    GATT_SERV_EVENT_REGISTER_CMP,
    GATT_SERV_EVENT_CONN_OPENED = BT_EVENT_GATT_SERV_OPENED,
    GATT_SERV_EVENT_CONN_CLOSED,
    GATT_SERV_EVENT_CHAR_READ,
    GATT_SERV_EVENT_DESC_READ,
    GATT_SERV_EVENT_CHAR_WRITE,
    GATT_SERV_EVENT_DESC_WRITE,
    GATT_SERV_EVENT_NTF_TX_DONE,
    GATT_SERV_EVENT_INDICATE_CFM,
    GATT_SERV_EVENT_MTU_CHANGED,
    GATT_SERV_EVENT_CONN_UPDATED,
    GATT_SERV_EVENT_CONN_ENCRYPTED,
} gatt_server_event_t;

typedef struct {
    uint16_t svc_size;
    uint16_t preferred_mtu;
    uint8_t eatt_preferred: 1;
    uint8_t btgatt_add_sdp: 1;
    uint8_t dont_delay_report_conn_open: 1;
    uint8_t service_inst_id;
} gatts_cfg_t;

typedef struct {
    const uint8_t *service;
    const uint8_t *character;
    uint8_t service_inst_id;
    uint8_t char_instance_id;
} gatt_char_notify_t;

typedef struct {
    uint16_t char_value_handle;
    uint16_t value_len;
    const uint8_t *value;
} gatt_char_notify_by_handle_t;

typedef int (*gatt_server_callback_t)(gatt_svc_t *svc, gatt_server_event_t event, gatt_server_callback_param_t param);

bt_status_t gatts_register_service(const gatt_attribute_t *service, uint16_t attr_count, gatt_server_callback_t cb, const gatts_cfg_t *cfg);
bt_status_t gatts_unregister_service(const gatt_attribute_t *service);
bt_status_t gatts_control_service(const gatt_attribute_t *service, uint16_t service_uuid_16, bool visible, uint16_t connhdl);
bt_status_t gatts_write_read_rsp_data(gatt_server_read_ctx_t *ctx, const uint8_t *data, uint16_t len);
bt_status_t gatts_send_write_rsp(gatt_server_write_ctx_t *write_ctx, uint16_t err_code);
bt_status_t gatts_send_defer_read_rsp(uint16_t connhdl, uint32_t token, uint8_t error_code, const uint8_t *rsp_data, uint16_t data_len);
bt_status_t gatts_send_defer_write_rsp(uint16_t connhdl, uint32_t token, uint8_t error_code);
bt_status_t gatts_send_indication(uint32_t con_bfs, const uint8_t *character, const uint8_t *data, uint16_t len);
bt_status_t gatts_send_notification(uint32_t con_bfs, const uint8_t *character, const uint8_t *data, uint16_t len);
bt_status_t gatts_send_multi_notifications(uint32_t con_bfs, gatt_multi_notify_item_t *notify, uint16_t count);
bt_status_t gatts_send_value_indication(uint32_t con_bfs, const gatt_char_notify_t *character, const uint8_t *data, uint16_t len);
bt_status_t gatts_send_value_notification(uint32_t con_bfs, const gatt_char_notify_t *character, const uint8_t *data, uint16_t len);
bt_status_t gatts_send_character_value(uint32_t con_bfs, const gatt_char_notify_t *character, const uint8_t *data, uint16_t len);
bt_status_t gatts_send_indication_by_handle(uint32_t con_bfs, const gatt_char_notify_by_handle_t *character);
bt_status_t gatts_send_notification_by_handle(uint32_t con_bfs, const gatt_char_notify_by_handle_t *character);
bt_status_t gatts_send_multi_notifications_by_handle(uint32_t con_bfs, const gatt_char_notify_by_handle_t *character, uint16_t count);
uint16_t gatts_get_char_value_handle(const uint8_t *service, uint8_t service_inst_id, const uint8_t *character, uint8_t char_inst_id);
uint16_t gatts_get_char_byte_16_bit_uuid(const uint8_t *character);
uint16_t gatts_get_char_byte_16_bit_part_uuid(const uint8_t *character);
uint16_t gatts_get_character_16_bit_part_uuid(const gatt_attribute_t *attr);
uint16_t gatts_get_service_16_bit_part_uuid(const gatt_attribute_t *attr);
gatt_svc_t *gatts_get_service(uint16_t connhdl, const uint8_t *service, uint8_t service_inst_id);
uint8_t *gatts_put_read_rsp_buffer(gatt_server_read_ctx_t *ctx, uint16_t len);
void gatts_gen_local_database_hash(void (*cb)(void *priv, int error_code, const uint8_t *hash));
void gatts_gen_visible_svc_database_hash(uint16_t connhdl, void (*cb)(void *priv, int error_code, const uint8_t *hash));
bt_status_t gatts_server_cache_restore(uint16_t connhdl, const gatt_server_cache_t *server_cache);

#define GATT_PRF_NONE 0x00
#define GATT_PRF_LAST_ID 0x7E
#define GATT_PRF_ID_MASK 0x7F
#define GATT_PRF_NOT_RPT 0x80
#define GATT_PRF_INVALID 0xFF

typedef struct {
    uint8_t uuid_le[16];
    uint8_t uuid_128_bit;
    uint8_t uuid_len;
} gatt_char_uuid_t;

typedef struct {
    gatt_prf_t *prf;
    gap_conn_item_t *conn;
    uint16_t mtu;
} gatt_profile_param_t;

struct gatt_peer_service_t;
struct gatt_peer_srvc_node_t;
struct gatt_peer_char_node_t;

typedef struct {
    struct gatt_peer_srvc_node_t *next;
    struct gatt_peer_srvc_node_t *tail;
} gatt_peer_srvc_list_t;

typedef struct {
    struct gatt_peer_char_node_t *next;
} gatt_peer_char_list_t;

typedef struct {
    struct gatt_peer_service_t *incl_service;
    uint16_t incl_handle;
    uint16_t start_handle;
    uint16_t end_handle;
    uint16_t service_uuid; // set when 16-bit uuid
} gatt_peer_include_t;

typedef struct {
    uint16_t desc_uuid;
    uint16_t desc_handle;
} gatt_peer_descriptor_t;

typedef struct {
    gatt_peer_descriptor_t *desc_list;
    uint8_t desc_is_discovered: 1;
    uint8_t desc_disc_pending: 1;
    uint8_t peer_not_exist: 1;
    uint8_t desc_count;
    uint8_t char_index;
    uint8_t end_hdl_offset;
    uint16_t char_prop;
    uint16_t char_value_handle;
} gatt_peer_character_t;

typedef struct gatt_peer_service_t {
    gatt_peer_include_t *incl_list;
    gatt_peer_char_list_t char_list;
    uint8_t incl_is_discovered: 1;
    uint8_t incl_disc_pending: 1;
    uint8_t peer_not_exist: 1;
    uint8_t incl_count;
    uint8_t srvc_index;
    uint8_t count; // same uuid service count
    uint16_t service_uuid; // set when 16-bit uuid
    uint16_t attr_handle;
    uint16_t end_handle;
} gatt_peer_service_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    att_error_code_t error_code;
    uint8_t count; // same uuid service count
    uint16_t service_uuid; // set when 16-bit uuid
    uint8_t discover_cmpl;
} gatt_profile_service_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *owner_service;
    gatt_peer_service_t *service;
    att_error_code_t error_code;
    uint8_t discover_cmpl;
    uint16_t service_uuid; // set when 16-bit uuid
} gatt_profile_include_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    gatt_peer_character_t *character;
    att_error_code_t error_code;
    uint16_t char_prop;
    uint16_t char_value_handle;
    uint16_t char_end_handle;
    uint8_t discover_cmpl;
    uint8_t discover_idx;
    uint8_t count; // same uuid characteristic count
    uint16_t char_uuid; // set when 16-bit uuid
    uint8_t uuid_le[16]; // always set
} gatt_profile_character_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_character_t *owner_character;
    att_error_code_t error_code;
    uint8_t discover_cmpl;
    uint16_t char_prop;
    uint16_t char_value_handle;
    uint16_t desc_handle;
    uint16_t desc_uuid; // set when 16-bit uuid
    const uint8_t *desc_uuid_le; // always set
} gatt_profile_descriptor_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    gatt_peer_character_t *character;
    att_error_code_t error_code;
    uint16_t attr_handle;
    uint16_t value_len;
    const uint8_t *value;
} gatt_profile_char_read_rsp_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    gatt_peer_character_t *character;
    att_error_code_t error_code;
    uint16_t char_value_handle;
    uint16_t desc_uuid;
    uint16_t value_len;
    const uint8_t *value;
} gatt_profile_desc_read_rsp_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    gatt_peer_character_t *character;
    att_error_code_t error_code;
    uint16_t attr_handle;
    bool prepare_mismatch;
    bool reliable_write_done;
    uint16_t value_offset;
    uint16_t value_write_len;
} gatt_profile_char_write_rsp_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    gatt_peer_character_t *character;
    att_error_code_t error_code;
    uint16_t attr_handle;
    uint16_t desc_uuid;
} gatt_profile_desc_write_rsp_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    gatt_peer_character_t *character;
    att_error_code_t error_code;
} gatt_profile_write_exec_rsp_t;

typedef struct {
    gap_conn_item_t *conn;
    gatt_peer_service_t *service;
    gatt_peer_character_t *character;
    bool is_indicate;
    uint16_t value_len;
    const uint8_t *value;
} gatt_profile_recv_notify_t;

typedef struct {
    uint16_t char_uuid; // only for 16-bit uuid, 0 for 128-bit uuid
    uint8_t uuid_16_le[2];
    const uint8_t *uuid_le;
} gatt_peer_character_uuid_t;

typedef struct {
    uint16_t service_uuid; // only for 16-bit uuid, 0 for 128-bit uuid
    uint8_t uuid_16_le[2];
    const uint8_t *uuid_le;
} gatt_peer_service_uuid_t;

typedef struct {
    uint16_t prf_size;
    uint16_t preferred_mtu;
    bool eatt_preferred;
    bool enc_required; // encrypt required to read/write peer service
} gattc_cfg_t;

typedef union {
    void *param_ptr;
    gatt_profile_param_t *opened;
    gatt_profile_param_t *closed;
    gatt_profile_param_t *mtu_changed;
    gatt_profile_param_t *conn_encrypted;
    gatt_profile_param_t *conn_updated;
    gatt_profile_param_t *service_changed;
    gatt_profile_service_t *service;
    gatt_profile_include_t *incl_service;
    gatt_profile_character_t *character;
    gatt_profile_descriptor_t *descriptor;
    gatt_profile_char_read_rsp_t *char_read_rsp;
    gatt_profile_desc_read_rsp_t *desc_read_rsp;
    gatt_profile_char_write_rsp_t *char_write_rsp;
    gatt_profile_desc_write_rsp_t *desc_write_rsp;
    gatt_profile_write_exec_rsp_t *write_exec_rsp;
    gatt_profile_recv_notify_t *notify;
} gatt_profile_callback_param_t;

typedef enum {
    GATT_PROF_EVENT_OPENED = BT_EVENT_GATT_PROF_OPENED,
    GATT_PROF_EVENT_CLOSED,
    GATT_PROF_EVENT_MTU_CHANGED,
    GATT_PROF_EVENT_CONN_ENCRYPTED,
    GATT_PROF_EVENT_CONN_UPDATED,
    GATT_PROF_EVENT_SERVICE_CHANGED,
    GATT_PROF_EVENT_SERVICE,
    GATT_PROF_EVENT_INCLUDE,
    GATT_PROF_EVENT_CHARACTER,
    GATT_PROF_EVENT_DESCRIPTOR,
    GATT_PROF_EVENT_CHAR_READ_RSP,
    GATT_PROF_EVENT_DESC_READ_RSP,
    GATT_PROF_EVENT_CHAR_WRITE_RSP,
    GATT_PROF_EVENT_DESC_WRITE_RSP,
    GATT_PROF_EVENT_WRITE_EXEC_RSP,
    GATT_PROF_EVENT_NOTIFY, // profile shall check notify belong self or not
} gatt_profile_event_t;

#define GATTC_PEER_CHAR_END_HDL(peer_char)           (peer_char->char_value_handle != 0 ? \
                                                            (peer_char->char_value_handle + peer_char->end_hdl_offset) : 0)

typedef int (*gatt_profile_callback_t)(gatt_prf_t *prf, gatt_profile_event_t event, gatt_profile_callback_param_t param);

uint8_t gattc_register_profile(gatt_profile_callback_t cb, const gattc_cfg_t *cfg);
bool gatt_is_user_profile_registered(void);
void gatt_start_att_mtu_exchange(uint16_t connhdl, uint16_t pref_mtu_size);
gatt_prf_t *gattc_get_profile(uint8_t prf_id, uint16_t connhdl);
bool gattc_has_profile_eatt_preferred(uint16_t connhdl);
bt_status_t gattc_get_service(gatt_prf_t *prf, uint16_t uuid_len, const uint8_t *uuid, gatt_peer_service_t ** srv_ptr);
bt_status_t gattc_get_character(gatt_peer_service_t *srv, uint16_t uuid_len, const uint8_t *uuid, gatt_peer_character_t** char_ptr);
void gattc_get_character_uuid(const gatt_peer_character_t *c, gatt_peer_character_uuid_t *param);
void gattc_get_service_uuid(const gatt_peer_service_t *s, gatt_peer_service_uuid_t *param);
const uint8_t *gatt_srvc_128_uuid_le(const gatt_peer_service_t *s);
uint16_t gatt_get_srvc_uuid_len(const gatt_peer_service_t *s);
uint8_t gatt_get_same_uuid_char_count(const gatt_peer_character_t *c);
uint16_t gatt_char_16_uuid_le(const gatt_peer_character_t *c);
const uint8_t *gatt_char_128_uuid_le(const gatt_peer_character_t *c);
uint16_t gatt_get_char_uuid_len(const gatt_peer_character_t *c);
bt_status_t gatt_create_att_bearer(uint16_t connhdl);
bt_status_t gatt_disconnect_att_bearers(uint16_t connhdl, bool include_no_initiator_att, uint8_t reason);
bt_status_t gatt_create_eatt_bearer(uint16_t connhdl);
bt_status_t gatt_disconnect_eatt_bearers(uint16_t connhdl, bool include_no_initiator_eatt);
bt_status_t gattc_activate_profile(uint8_t prf_id, bool active);
bt_status_t gattc_unregister_profile(uint8_t prf_id);
bt_status_t gatt_discover_all_primary_services(uint8_t prf_id, uint16_t connhdl);
bt_status_t gatt_client_discover_all_characters(uint8_t prf_id, uint16_t connhdl, gatt_peer_service_t *s);
bt_status_t gattc_discover_all_services(uint8_t prf_id, uint16_t connhdl);
bt_status_t gattc_discover_service(gatt_prf_t *prf, uint16_t service_uuid, const uint8_t *uuid_128_le);
bt_status_t gattc_discover_include(gatt_prf_t *prf, gatt_peer_service_t *s, uint16_t service_uuid, const uint8_t *uuid_128_le);
bt_status_t gattc_discover_all_include_of_service(gatt_prf_t *prf, gatt_peer_service_t *s);
bt_status_t gattc_discover_character(gatt_prf_t *prf, gatt_peer_service_t *s, uint16_t char_uuid, const uint8_t *uuid_128_le);
bt_status_t gattc_discover_multi_characters(gatt_prf_t *prf, gatt_peer_service_t *s, const uint16_t *char_uuid, uint16_t count);
bt_status_t gattc_discover_all_characters(gatt_prf_t *prf, gatt_peer_service_t *s);
bt_status_t gattc_discover_multi_128_characters(gatt_prf_t *prf, gatt_peer_service_t *s, const gatt_char_uuid_t *char_uuid, uint16_t count);
bt_status_t gattc_discover_descriptor_by_value_handle(gatt_prf_t *prf, uint16_t char_value_handle, uint16_t char_end_handle, uint16_t char_prop);
bt_status_t gattc_discover_descriptor_by_character(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_character_value(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_character_by_uuid(gatt_prf_t *prf, gatt_peer_service_t *s, uint16_t char_uuid, const uint8_t *uuid_128_le);
bt_status_t gattc_write_character_value(gatt_prf_t *prf, gatt_peer_character_t *c, const uint8_t *data, uint16_t len);
bt_status_t gattc_write_character_command(gatt_prf_t *prf, gatt_peer_character_t *c, const uint8_t *data, uint16_t len, bool signed_write);
bt_status_t gattc_reliable_write_character(gatt_prf_t *prf, gatt_peer_character_t *c, uint16_t offset, const uint8_t *data, uint16_t len);
bt_status_t gattc_reliable_write_execute(gatt_prf_t *prf, gatt_peer_character_t *c, bool execute);
bt_status_t gattc_write_cudd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c, const uint8_t *data, uint16_t len);
bt_status_t gattc_write_cccd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c, bool notify, bool indicate);
bt_status_t gattc_write_sccd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c, bool broadcast);
bt_status_t gattc_write_descriptor_value(gatt_prf_t *prf, gatt_peer_character_t *c, uint16_t desc_uuid, const uint8_t *data, uint16_t len);
bt_status_t gattc_read_cepd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_cudd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_errd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_rrcd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_cpfd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_cafd_descriptor(gatt_prf_t *prf, gatt_peer_character_t *c);
bt_status_t gattc_read_descriptor_value(gatt_prf_t *prf, gatt_peer_character_t *c, uint16_t desc_uuid);
bt_status_t gattc_read_desc_from_cpfd_handle(gatt_prf_t *prf, gatt_peer_character_t *c, uint16_t cpfd_handle);
bt_status_t gattc_read_character_by_uuid_of_service(gatt_prf_t *prf, uint16_t service_start_handle, uint16_t service_end_handle, uint16_t char_uuid, const uint8_t *uuid_128_le);
bt_status_t gattc_read_character_by_handle(gatt_prf_t *prf, uint16_t char_value_handle, bool long_value);
bt_status_t gattc_read_descriptor_by_handle(gatt_prf_t *prf, uint16_t desc_handle, bool long_value);
bt_status_t gattc_read_multi_char_values(gatt_prf_t *prf, bool multi_var_req, uint16_t count, uint16_t *handles, uint16_t *value_lens);
bt_status_t gattc_send_signed_write_cmd(gatt_prf_t *prf, uint16_t attr_handle, const uint8_t *data, uint16_t len);
bt_status_t gattc_write_character_by_handle(gatt_prf_t *prf, uint16_t attr_handle, const uint8_t *data, uint16_t len, bool write_cmd);
bt_status_t gattc_write_descriptor_by_handle(gatt_prf_t *prf, uint16_t desc_handle, const uint8_t *data, uint16_t len);
bt_status_t gattc_prepare_write_by_handle(gatt_prf_t *prf, uint16_t attr_handle, uint16_t offset, const uint8_t *data, uint16_t len);
bt_status_t gattc_prepare_write_execute(gatt_prf_t *prf, bool execute);
bt_status_t gattc_send_l2cap_packet(uint16_t connhdl, const uint8_t *data, uint16_t len, bool eatt_preferred);
bt_status_t gattc_recv_peer_service_changed(uint16_t connhdl, uint16_t start_handle, uint16_t end_handle);
bt_status_t gattc_recv_peer_database_hash(uint16_t connhdl, const uint8_t *hash, bool check_hash_change);
#ifdef BLE_GATT_CLIENT_CACHE
bt_status_t gattc_cache_restore_handles(uint16_t connhdl, gatt_client_cache_t *client_cache);
#endif /* BLE_GATT_CLIENT_CACHE */

typedef struct att_conn_item_t att_conn_item_t;
typedef struct gatt_proc_node_t gatt_proc_node_t;

typedef struct {
    uint8_t is_initiator: 1;
    uint8_t is_eatt: 1;
    uint8_t is_mtu_exchanged: 1;
    uint8_t tx_pending: 1;
    uint8_t for_prep_write: 1;
} __attribute__ ((packed)) att_bearer_flag_t;

typedef struct att_bearer_t {
    struct att_bearer_t *next;
    att_conn_item_t *conn;
    att_bearer_flag_t flag;
    uint8_t bearer_id;
    uint8_t rsp_wait_timer;
    int32_t free_credits;
    uint16_t att_mtu;
    uint16_t connhdl;
    uint32_t l2cap_handle;
    uint32_t defer_token;
} att_bearer_t;

typedef struct {
    att_bearer_t *next;
} att_bearer_q_t;

typedef struct gatt_prf_reg_t {
    struct gatt_prf_reg_t *next;
    uint8_t multi_device_supp: 1;
    uint8_t eatt_preferred: 1;
    uint8_t enc_required: 1;
    uint8_t is_active: 1;
    uint8_t connected_count;
    uint8_t prf_id;
    uint8_t prf_size_div_4;
    uint16_t preferred_mtu;
    gatt_profile_callback_t profile_callback;
} gatt_prf_reg_t;

typedef struct {
    gatt_prf_reg_t *next;
} gatt_prf_reg_list_t;

typedef struct gatt_prf_node_t {
    struct gatt_prf_node_t *next;
    gatt_prf_reg_t *reg;
    gatt_prf_t prf_head;
} gatt_prf_node_t;

typedef struct {
    gatt_prf_node_t *next;
} gatt_prf_list_t;

typedef struct gatt_svc_node_t {
    struct gatt_svc_node_t *next;
    gatt_service_t *service;
    uint16_t reported_mtu;
    uint8_t conn_open_reported: 1;
    uint8_t encrypt_reported: 1;
    gatt_svc_t svc_head;
} gatt_svc_node_t;

typedef struct {
    gatt_svc_node_t *next;
} gatt_svc_list_t;

typedef struct gatt_peer_char_node_t {
    struct gatt_peer_char_node_t *next;
    gatt_peer_service_t *service;
    uint8_t count; // same uuid char count
    uint16_t char_uuid; // set when 16-bit uuid
    gatt_peer_character_t character[1];
} gatt_peer_char_node_t;

typedef struct gatt_peer_srvc_node_t {
    struct gatt_peer_srvc_node_t *next;
    gatt_peer_service_t service[1];
} gatt_peer_srvc_node_t;

typedef struct att_prep_write_t {
    struct att_prep_write_t *next;
    uint16_t attr_handle;
    uint16_t value_offset;
    uint16_t value_len;
} att_prep_write_t;

typedef struct {
    att_prep_write_t *next;
} att_prep_wr_q_t;

typedef struct {
    struct single_link_node_t node;
    struct pp_buff *ppb;
    uint8_t bearer_id;
} att_rx_req_node_t;

typedef struct att_conn_item_t {
    gap_conn_item_t head;
    gatt_prf_list_t prf_list;
    gatt_svc_list_t svc_list;
    att_bearer_q_t bearer_q;
    att_prep_wr_q_t prep_wr_q;
    uint8_t bearer_id_seed;
    uint8_t last_tx_bearer;
    uint8_t bredr_att_added: 1;
    uint8_t use_prev_tx_bearer: 1;
    uint8_t mtu_ready_to_report: 1;
    uint8_t real_mtu_exchanged: 1;
    uint8_t mtu_wait_report: 1;
    uint8_t encrypt_wait_report: 1;
    uint8_t user_profile_started: 1;
    uint8_t profile_is_allocated: 1;
    // Every proc in different queue own a bearer id
    struct single_link_head_t rx_req_pendq;
    struct single_link_head_t req_ind_tx_q;
    struct single_link_head_t cmd_ntf_tx_q;
    struct single_link_head_t cmd_ntf_tx_wait_done_q;
} att_conn_item_t;

typedef struct {
    // 16-bit/128-bit uuid services should be grouped together
    // the same service may be exist multiple instances
    struct list_node local_service_database;
    uint16_t local_cust_16bit_srvc_count;
    uint16_t local_cust_128bit_srvc_count;
    uint8_t prf_id_seed;
    uint8_t gatt_proc_seqn_seed;
    bool is_initialized;
    bool has_user_profile_registered;
    gatt_prf_reg_list_t prf_reg_list;
} gatt_global_t;

#if !defined(BLE_ONLY_ENABLED)
gap_conn_item_t *gatt_over_bredr_is_profile_connected(uint16_t connhdl);
void gatt_over_bredr_force_disconnect_profile(uint16_t connhdl, uint8_t reason);
#ifdef IBRT
uint32_t gatt_over_bredr_profile_save_ctx(btif_remote_device_t *rem_dev, uint8_t *buf, uint32_t buf_len);
uint32_t gatt_over_bredr_profile_restore_ctx(const bt_bdaddr_t *remote, uint8_t *buf, uint32_t buf_len);
void gatt_over_bredr_report_profile_connected(gap_conn_item_t *conn);
void gatt_over_bredr_report_profile_disconnected(gap_conn_item_t *conn);
#endif
#endif

/* GATT SERVICES */
#define GATT_UUID_GAP_SERVICE   0x1800 // Generic Access
#define GATT_UUID_GATT_SERVICE  0x1801 // Generic Attribute
#define GATT_UUID_IAP_SERVICE   0x1802 // Immediate Alert
#define GATT_UUID_LLP_SERVICE   0x1803 // Link Loss
#define GATT_UUID_TXP_SERVICE   0x1804 // Tx Power
#define GATT_UUID_CTP_SERVICE   0x1805 // Current Time
#define GATT_UUID_RTU_SERVICE   0x1806 // Reference Time Update
#define GATT_UUID_NDT_SERVICE   0x1807 // Next DST Change
#define GATT_UUID_GLU_SERVICE   0x1808 // Glucose
#define GATT_UUID_HTP_SERVICE   0x1809 // Health Thermometer
#define GATT_UUID_DIP_SERVICE   0x180A // Device Information
#define GATT_UUID_HRP_SERVICE   0x180D // Heart Rate
#define GATT_UUID_PAS_SERVICE   0x180E // Phone Alert Status
#define GATT_UUID_BAT_SERVICE   0x180F // Battery
#define GATT_UUID_BLP_SERVICE   0x1810 // Blood Pressure
#define GATT_UUID_ANP_SERVICE   0x1811 // Alert Notification
#define GATT_UUID_HID_SERVICE   0x1812 // Human Interface Device
#define GATT_UUID_SCN_SERVICE   0x1813 // Scan Parameters
#define GATT_UUID_RSC_SERVICE   0x1814 // Running Speed and Cadence
#define GATT_UUID_AIO_SERVICE   0x1815 // Automation IO
#define GATT_UUID_CSC_SERVICE   0x1816 // Cycling Speed and Cadence
#define GATT_UUID_CPW_SERVICE   0x1818 // Cycling Power
#define GATT_UUID_LNP_SERVICE   0x1819 // Location and Navigation
#define GATT_UUID_ESP_SERVICE   0x181A // Environment Sensing
#define GATT_UUID_BCP_SERVICE   0x181B // Body Composition
#define GATT_UUID_UDP_SERVICE   0x181C // User Data
#define GATT_UUID_WSP_SERVICE   0x181D // Weight Scale
#define GATT_UUID_BOM_SERVICE   0x181E // Bond Management
#define GATT_UUID_CGM_SERVICE   0x181F // Continuous Glucose Monitoring
#define GATT_UUID_IPS_SERVICE   0x1820 // Internet Protocol Support
#define GATT_UUID_IPP_SERVICE   0x1821 // Indoor Posiitoning
#define GATT_UUID_POP_SERVICE   0x1822 // Pulse Oximeter
#define GATT_UUID_HTT_SERVICE   0x1823 // HTTP Proxy
#define GATT_UUID_TDP_SERVICE   0x1824 // Transport Discovery
#define GATT_UUID_OTP_SERVICE   0x1825 // Object Transfer
#define GATT_UUID_FIT_SERVICE   0x1826 // Fitness Machine
#define GATT_UUID_MPV_SERVICE   0x1827 // Mesh Provisioning
#define GATT_UUID_MPX_SERVICE   0x1828 // Mesh Proxy
#define GATT_UUID_RCN_SERVICE   0x1829 // Reconnnection Configuration
#define GATT_UUID_IDP_SERVICE   0x183A // Insulin Delivery
#define GATT_UUID_BSN_SERVICE   0x183B // Binary Sensor
#define GATT_UUID_ECP_SERVICE   0x183C // Emergency Configuration
#define GATT_UUID_PAM_SERVICE   0x183e // Physical Activity Monitor
#define GATT_UUID_AIC_SERVICE   0x1843 // Audio Input Control
#define GATT_UUID_VCP_SERVICE   0x1844 // Volume Control
#define GATT_UUID_VOC_SERVICE   0x1845 // Volume Offset Control
#define GATT_UUID_CSI_SERVICE   0x1846 // Coordinated Set Identification
#define GATT_UUID_DTP_SERVICE   0x1847 // Device Time
#define GATT_UUID_MCP_SERVICE   0x1848 // Media Control
#define GATT_UUID_GMC_SERVICE   0x1849 // Generic Media Control
#define GATT_UUID_CTE_SERVICE   0x184A // Constant Tone Extension
#define GATT_UUID_TBP_SERVICE   0x184B // Telephone Bearer
#define GATT_UUID_GTP_SERVICE   0x184C // Generic Telephone Bearer
#define GATT_UUID_MIC_SERVICE   0x184D // Microphone Control
#define GATT_UUID_ASC_SERVICE   0x184E // Audio Stream Control
#define GATT_UUID_BAS_SERVICE   0x184F // Broadcast Audio Scan
#define GATT_UUID_PAC_SERVICE   0x1850 // Published Audio Capabilites
#define GATT_UUID_BAA_SERVICE   0x1851 // Basic Audio Announcement
#define GATT_UUID_BCA_SERVICE   0x1852 // Broadcast Audio Announcement
#define GATT_UUID_CAP_SERVICE   0x1853 // Common Audio
#define GATT_UUID_HAP_SERVICE   0x1854 // Hearing Access
#define GATT_UUID_TMA_SERVICE   0x1855 // Telephone and Media
#define GATT_UUID_PBA_SERVICE   0x1856 // Public Broadcast Announcement
#define GATT_UUID_GMA_SERVICE   0x1858 // Gaming Audio

/* DECLARATIONS */
#define GATT_UUID_PRI_SERVICE           0x2800 // Primary Service
#define GATT_UUID_SEC_SERVICE           0x2801 // Secondary Service
#define GATT_UUID_INCLUDE_SERVICE       0x2802 // Include
#define GATT_UUID_CHAR_DECLARE          0x2803 // Characteristic

/* DESCRIPTORS */
#define GATT_DESC_UUID_CHAR_EXT_PROP         0x2900 // Characteristic Extended Properties
#define GATT_DESC_UUID_CHAR_USER_DESC        0x2901 // Characteristic User Description
#define GATT_DESC_UUID_CHAR_CLIENT_CONFIG    0x2902 // Client Characteristic Configuration
#define GATT_DESC_UUID_CHAR_SERVER_CONFIG    0x2903 // Server Characteristic Configuration
#define GATT_DESC_UUID_CHAR_PRESENT_FORMAT   0x2904 // Characteristic Presentation Format
#define GATT_DESC_UUID_CHAR_AGGREGATE_FORMAT 0x2905 // Characteristic Aggregate Format
#define GATT_DESC_UUID_CHAR_VALID_RANGE      0x2906 // Characteristic Valid Range
#define GATT_DESC_UUID_EXT_REPORT_REFERENCE  0x2907 // External Report Reference
#define GATT_DESC_UUID_REPORT_REFERENCE      0x2908 // Report Reference
#define GATT_DESC_UUID_NUM_DIGITALS          0x2909 // Number of Digitals
#define GATT_DESC_UUID_VALUE_TRIGGER_SETTING 0x290A // Value Trigger Setting
#define GATT_DESC_UUID_ES_CONFIGURATION      0x290B // Environmental Sensing Configuration
#define GATT_DESC_UUID_ES_MEASUREMENT        0x290C // Environmental Sensing Measurement
#define GATT_DESC_UUID_ES_TRIGGER_SETTING    0x290D // Envrionmental Sensing Trigger Setting
#define GATT_DESC_UUID_TIME_TRIGGER_SETTING  0x290E // Time Trigger Setting
#define GATT_DESC_UUID_COMPLETE_BREDR_TBD    0x290F // Copmlete BR-EDR Transport Block Data

/* CHARATERISTICS */
#define GATT_CHAR_UUID_DEVICE_NAME                                       0x2A00
#define GATT_CHAR_UUID_APPEARANCE                                        0x2A01
#define GATT_CHAR_UUID_PERIPH_PRIV_FLAG                                  0x2A02
#define GATT_CHAR_UUID_RECONN_ADDRESS                                    0x2A03
#define GATT_CHAR_UUID_PERIPH_PREFER_CONN_PARAMS                         0x2A04
#define GATT_CHAR_UUID_SERVICE_CHANGED                                   0x2A05
#define GATT_CHAR_UUID_ALERT_LEVEL                                       0x2A06
#define GATT_CHAR_UUID_TX_PWR_LEVEL                                      0x2A07
#define GATT_CHAR_UUID_DATE_TIME                                         0x2A08
#define GATT_CHAR_UUID_DAY_OF_WEEK                                       0x2A09
#define GATT_CHAR_UUID_DAY_DATE_TIME                                     0x2A0A
#define GATT_CHAR_UUID_EXACT_TIME_256                                    0x2A0C
#define GATT_CHAR_UUID_DST_OFFSET                                        0x2A0D
#define GATT_CHAR_UUID_TIME_ZONE                                         0x2A0E
#define GATT_CHAR_UUID_LOCAL_TIME_INFO                                   0x2A0F
#define GATT_CHAR_UUID_TIME_WITH_DST                                     0x2A11
#define GATT_CHAR_UUID_TIME_ACCURACY                                     0x2A12
#define GATT_CHAR_UUID_TIME_SOURCE                                       0x2A13
#define GATT_CHAR_UUID_REFER_TIME_INFO                                   0x2A14
#define GATT_CHAR_UUID_TIME_UPDATE_CTRL_POINT                            0x2A16
#define GATT_CHAR_UUID_TIME_UPDATE_STATE                                 0x2A17
#define GATT_CHAR_UUID_GLU_MEASURE                                       0x2A18
#define GATT_CHAR_UUID_BATT_LEVEL                                        0x2A19
#define GATT_CHAR_UUID_TEMPERATURE_MEASURE                               0x2A1C
#define GATT_CHAR_UUID_TEMPERATURE_TYPE                                  0x2A1D
#define GATT_CHAR_UUID_INTERMEDIATE_TEMPERATURE                          0x2A1E
#define GATT_CHAR_UUID_MEASURE_INTERVAL                                  0x2A21
#define GATT_CHAR_UUID_BOOT_KEYBOARD_INPUT_REPORT                        0x2A22
#define GATT_CHAR_UUID_SYSTEM_ID                                         0x2A23
#define GATT_CHAR_UUID_MODEL_NUMBER_STRING                               0x2A24
#define GATT_CHAR_UUID_SERIAL_NUMBER_STRING                              0x2A25
#define GATT_CHAR_UUID_FW_REVISION_STRING                                0x2A26
#define GATT_CHAR_UUID_HW_REVISION_STRING                                0x2A27
#define GATT_CHAR_UUID_SW_REVISION_STRING                                0x2A28
#define GATT_CHAR_UUID_MANUFACTURER_NAME_STRING                          0x2A29
#define GATT_CHAR_UUID_IEEE_CERT_DATA_LIST                               0x2A2A
#define GATT_CHAR_UUID_CURRENT_TIME                                      0x2A2B
#define GATT_CHAR_UUID_SCAN_REFRESH                                      0x2A31
#define GATT_CHAR_UUID_BOOT_KEYBOARD_OUTPUT_REPORT                       0x2A32
#define GATT_CHAR_UUID_BOOT_MOUSE_INPUT_REPORT                           0x2A33
#define GATT_CHAR_UUID_GLU_MEASURE_CONTEXT                               0x2A34
#define GATT_CHAR_UUID_BLOOD_PRESSURE_MEASURE                            0x2A35
#define GATT_CHAR_UUID_INTERMEDIATE_CUFF_PRESSURE                        0x2A36
#define GATT_CHAR_UUID_HEART_RATE_MEASURE                                0x2A37
#define GATT_CHAR_UUID_BODY_SENSOR_LOCATION                              0x2A38
#define GATT_CHAR_UUID_HEART_RATE_CTRL_POINT                             0x2A39
#define GATT_CHAR_UUID_ALERT_STATUS                                      0x2A3F
#define GATT_CHAR_UUID_RINGER_CTRL_POINT                                 0x2A40
#define GATT_CHAR_UUID_RINGER_SETTING                                    0x2A41
#define GATT_CHAR_UUID_ALERT_CATE_ID_BIT_MASK                            0x2A42
#define GATT_CHAR_UUID_ALERT_CATE_ID                                     0x2A43
#define GATT_CHAR_UUID_ALERT_NOTIFY_CTRL_POINT                           0x2A44
#define GATT_CHAR_UUID_UNREAD_ALERT_STATUS                               0x2A45
#define GATT_CHAR_UUID_NEW_ALERT                                         0x2A46
#define GATT_CHAR_UUID_SUPP_NEW_ALERT_CATE                               0x2A47
#define GATT_CHAR_UUID_SUPP_UNREAD_ALERT_CATE                            0x2A48
#define GATT_CHAR_UUID_BLOOD_PRESSURE_FEATURE                            0x2A49
#define GATT_CHAR_UUID_HID_INFORMATION                                   0x2A4A
#define GATT_CHAR_UUID_HID_REPORT_MAP                                    0x2A4B
#define GATT_CHAR_UUID_HID_CTRL_POINT                                    0x2A4C
#define GATT_CHAR_UUID_HID_REPORT                                        0x2A4D
#define GATT_CHAR_UUID_HID_PROTOCOL_MODE                                 0x2A4E
#define GATT_CHAR_UUID_SCAN_INTERVAL_WINDOW                              0x2A4F
#define GATT_CHAR_UUID_PNP_ID                                            0x2A50
#define GATT_CHAR_UUID_GLU_FEATURE                                       0x2A51
#define GATT_CHAR_UUID_RECORD_ACCESS_CTRL_POINT                          0x2A52
#define GATT_CHAR_UUID_RSC_MEASURE                                       0x2A53
#define GATT_CHAR_UUID_RSC_FEATURE                                       0x2A54
#define GATT_CHAR_UUID_SC_CTRL_POINT                                     0x2A55
#define GATT_CHAR_UUID_AGGREGAT                                          0x2A5A
#define GATT_CHAR_UUID_CSC_MEASURE                                       0x2A5B
#define GATT_CHAR_UUID_CSC_FEATURE                                       0x2A5C
#define GATT_CHAR_UUID_SENS_OR_LOCATION                                  0x2A5D
#define GATT_CHAR_UUID_PLX_SPOT_CHECK_MEASURE                            0x2A5E
#define GATT_CHAR_UUID_PLX_CONT_MEASURE                                  0x2A5F
#define GATT_CHAR_UUID_PLX_FEATURES                                      0x2A60
#define GATT_CHAR_UUID_CYCL_PWR_MEASURE                                  0x2A63
#define GATT_CHAR_UUID_CYCL_PWR_VECTOR                                   0x2A64
#define GATT_CHAR_UUID_CYCL_PWR_FEATURE                                  0x2A65
#define GATT_CHAR_UUID_CYCL_PWR_CTRL_POINT                               0x2A66
#define GATT_CHAR_UUID_LOCATION_AND_SPEED                                0x2A67
#define GATT_CHAR_UUID_NAVIGATION                                        0x2A68
#define GATT_CHAR_UUID_POSITION_QUALITY                                  0x2A69
#define GATT_CHAR_UUID_LN_FEATURE                                        0x2A6A
#define GATT_CHAR_UUID_LN_CTRL_POINT                                     0x2A6B
#define GATT_CHAR_UUID_ELEVATION                                         0x2A6C
#define GATT_CHAR_UUID_PRESSURE                                          0x2A6D
#define GATT_CHAR_UUID_TEMPERATURE                                       0x2A6E
#define GATT_CHAR_UUID_HUMIDITY                                          0x2A6F
#define GATT_CHAR_UUID_TRUE_WIND_SPEED                                   0x2A70
#define GATT_CHAR_UUID_TRUE_WIND_DIRECTION                               0x2A71
#define GATT_CHAR_UUID_APPARENT_WIND_SPEED                               0x2A72
#define GATT_CHAR_UUID_APPARENT_WIND_DIRECTION                           0x2A73
#define GATT_CHAR_UUID_GUST_FACTOR                                       0x2A74
#define GATT_CHAR_UUID_POLLEN_CONCENTRATION                              0x2A75
#define GATT_CHAR_UUID_UV_INDEX                                          0x2A76
#define GATT_CHAR_UUID_IRRADIANCE                                        0x2A77
#define GATT_CHAR_UUID_RAINFALL                                          0x2A78
#define GATT_CHAR_UUID_WIND_CHILL                                        0x2A79
#define GATT_CHAR_UUID_HEAT_INDEX                                        0x2A7A
#define GATT_CHAR_UUID_DEW_POINT                                         0x2A7B
#define GATT_CHAR_UUID_DESC_VALUE_CHANGED                                0x2A7D
#define GATT_CHAR_UUID_AEROBIC_HEART_RATE_LOWER_LIMIT                    0x2A7E
#define GATT_CHAR_UUID_AEROBIC_THRESHOLD                                 0x2A7F
#define GATT_CHAR_UUID_AGE                                               0x2A80
#define GATT_CHAR_UUID_ANAEROBIC_HEART_RATE_LOWER_LIMIT                  0x2A81
#define GATT_CHAR_UUID_ANAEROBIC_HEART_RATE_UPPER_LIMIT                  0x2A82
#define GATT_CHAR_UUID_ANAEROBIC_THRESHOLD                               0x2A83
#define GATT_CHAR_UUID_AEROBIC_HEART_RATE_UPPER_LIMIT                    0x2A84
#define GATT_CHAR_UUID_DATE_OF_BIRTH                                     0x2A85
#define GATT_CHAR_UUID_DATE_OF_THRESHOLD_ASSESS                          0x2A86
#define GATT_CHAR_UUID_EMAIL_ADDRESS                                     0x2A87
#define GATT_CHAR_UUID_FAT_BURN_HEART_RATE_LOWER_LIMIT                   0x2A88
#define GATT_CHAR_UUID_FAT_BURN_HEART_RATE_UPPER_LIMIT                   0x2A89
#define GATT_CHAR_UUID_FIRST_NAME                                        0x2A8A
#define GATT_CHAR_UUID_FIVE_ZONE_HEART_RATE_LIMITS                       0x2A8B
#define GATT_CHAR_UUID_GENDER                                            0x2A8C
#define GATT_CHAR_UUID_HEART_RATE_MAX                                    0x2A8D
#define GATT_CHAR_UUID_HEIGHT                                            0x2A8E
#define GATT_CHAR_UUID_HIP_CIRCUMFERENCE                                 0x2A8F
#define GATT_CHAR_UUID_LAST_NAME                                         0x2A90
#define GATT_CHAR_UUID_MAX_RECOMMEND_HEART_RATE                          0x2A91
#define GATT_CHAR_UUID_RESTING_HEART_RATE                                0x2A92
#define GATT_CHAR_UUID_SPORT_TYPE_FOR_AEAN_THRESHOLDS                    0x2A93
#define GATT_CHAR_UUID_THREE_ZONE_HEART_RATE_LIMITS                      0x2A94
#define GATT_CHAR_UUID_TWO_ZONE_HEART_RATE_LIMITS                        0x2A95
#define GATT_CHAR_UUID_VO2_MAX                                           0x2A96
#define GATT_CHAR_UUID_WAIST_CIRCUMFERENCE                               0x2A97
#define GATT_CHAR_UUID_WEIGHT                                            0x2A98
#define GATT_CHAR_UUID_DATABASE_CHANGE_INC                               0x2A99
#define GATT_CHAR_UUID_USER_INDEX                                        0x2A9A
#define GATT_CHAR_UUID_BODY_COMPOSITION_FEATURE                          0x2A9B
#define GATT_CHAR_UUID_BODY_COMPOSITION_MEASURE                          0x2A9C
#define GATT_CHAR_UUID_WEIGHT_MEASURE                                    0x2A9D
#define GATT_CHAR_UUID_WEIGHT_SCALE_FEATURE                              0x2A9E
#define GATT_CHAR_UUID_USER_CTRL_POINT                                   0x2A9F
#define GATT_CHAR_UUID_MAGNET_FLUX_DENS_2D                               0x2AA0
#define GATT_CHAR_UUID_MAGNET_FLUX_DENS_3D                               0x2AA1
#define GATT_CHAR_UUID_LANGUAGE                                          0x2AA2
#define GATT_CHAR_UUID_BAROMETRIC_PRESSURE_TREND                         0x2AA3
#define GATT_CHAR_UUID_BOND_MNG_CTRL_POINT                               0x2AA4
#define GATT_CHAR_UUID_BOND_MNG_FEATURE                                  0x2AA5
#define GATT_CHAR_UUID_CENTRAL_ADDRESS_RESOLUTION                        0x2AA6
#define GATT_CHAR_UUID_CGM_MEASURE                                       0x2AA7
#define GATT_CHAR_UUID_CGM_FEATURE                                       0x2AA8
#define GATT_CHAR_UUID_CGM_STATUS                                        0x2AA9
#define GATT_CHAR_UUID_CGM_SESSION_START_TIME                            0x2AAA
#define GATT_CHAR_UUID_CGM_SESSION_RUN_TIME                              0x2AAB
#define GATT_CHAR_UUID_CGM_SPECIFIC_OPS_CTRL_POINT                       0x2AAC
#define GATT_CHAR_UUID_INDOOR_POSITION_CONFIG                            0x2AAD
#define GATT_CHAR_UUID_LATITUDE                                          0x2AAE
#define GATT_CHAR_UUID_LONGITUDE                                         0x2AAF
#define GATT_CHAR_UUID_LOCAL_NORTH_COORD                                 0x2AB0
#define GATT_CHAR_UUID_LOCAL_EAST_COORD                                  0x2AB1
#define GATT_CHAR_UUID_FLOOR_NUMBER                                      0x2AB2
#define GATT_CHAR_UUID_ALTITUDE                                          0x2AB3
#define GATT_CHAR_UUID_UNCERTAINTY                                       0x2AB4
#define GATT_CHAR_UUID_LOCATION_NAME                                     0x2AB5
#define GATT_CHAR_UUID_URI                                               0x2AB6
#define GATT_CHAR_UUID_HTTP_HEADERS                                      0x2AB7
#define GATT_CHAR_UUID_HTTP_STATUS_CODE                                  0x2AB8
#define GATT_CHAR_UUID_HTTP_ENTITY_BODY                                  0x2AB9
#define GATT_CHAR_UUID_HTTP_CTRL_POINT                                   0x2ABA
#define GATT_CHAR_UUID_HTTPS_SECURITY                                    0x2ABB
#define GATT_CHAR_UUID_TDS_CTRL_POINT                                    0x2ABC
#define GATT_CHAR_UUID_OTS_FEATURE                                       0x2ABD
#define GATT_CHAR_UUID_OBJECT_NAME                                       0x2ABE
#define GATT_CHAR_UUID_OBJECT_TYPE                                       0x2ABF
#define GATT_CHAR_UUID_OBJECT_SIZE                                       0x2AC0
#define GATT_CHAR_UUID_OBJECT_FIRST_CREATED                              0x2AC1
#define GATT_CHAR_UUID_OBJECT_LAST_MODIFIED                              0x2AC2
#define GATT_CHAR_UUID_OBJECT_ID                                         0x2AC3
#define GATT_CHAR_UUID_OBJECT_PROP                                       0x2AC4
#define GATT_CHAR_UUID_OBJECT_ACTION_CTRL_POINT                          0x2AC5
#define GATT_CHAR_UUID_OBJECT_LIST_CTRL_POINT                            0x2AC6
#define GATT_CHAR_UUID_OBJECT_LIST_FILTER                                0x2AC7
#define GATT_CHAR_UUID_OBJECT_CHANGED                                    0x2AC8
#define GATT_CHAR_UUID_RPA_ONLY                                          0x2AC9
#define GATT_CHAR_UUID_UNSPECIFIED                                       0x2ACA
#define GATT_CHAR_UUID_DIRECTORY_LISTING                                 0x2ACB
#define GATT_CHAR_UUID_FITNESS_MACHINE_FEATURE                           0x2ACC
#define GATT_CHAR_UUID_TREADMILL_DATA                                    0x2ACD
#define GATT_CHAR_UUID_CROSS_TRAINER_DATA                                0x2ACE
#define GATT_CHAR_UUID_STEP_CLIMBER_DATA                                 0x2ACF
#define GATT_CHAR_UUID_STAIR_CLIMBER_DATA                                0x2AD0
#define GATT_CHAR_UUID_ROWER_DATA                                        0x2AD1
#define GATT_CHAR_UUID_INDOOR_BIKE_DATA                                  0x2AD2
#define GATT_CHAR_UUID_TRAINING_STATUS                                   0x2AD3
#define GATT_CHAR_UUID_SUPP_SPEED_RANGE                                  0x2AD4
#define GATT_CHAR_UUID_SUPP_INCLINATION_RANGE                            0x2AD5
#define GATT_CHAR_UUID_SUPP_RESIST_LEVEL_RANGE                           0x2AD6
#define GATT_CHAR_UUID_SUPP_HEART_RATE_RANGE                             0x2AD7
#define GATT_CHAR_UUID_SUPP_POWER_RANGE                                  0x2AD8
#define GATT_CHAR_UUID_FITNESS_MACHINE_CTRL_POINT                        0x2AD9
#define GATT_CHAR_UUID_FITNESS_MACHINE_STATUS                            0x2ADA
#define GATT_CHAR_UUID_MESH_PROVISION_DATA_IN                            0x2ADB
#define GATT_CHAR_UUID_MESH_PROVISION_DATA_OUT                           0x2ADC
#define GATT_CHAR_UUID_MESH_PROXY_DATA_IN                                0x2ADD
#define GATT_CHAR_UUID_MESH_PROXY_DATA_OUT                               0x2ADE
#define GATT_CHAR_UUID_AVERAGE_CURRENT                                   0x2AE0
#define GATT_CHAR_UUID_AVERAGE_VOLTAGE                                   0x2AE1
#define GATT_CHAR_UUID_BOOLEAN                                           0x2AE2
#define GATT_CHAR_UUID_CHROMATIC_DISTANCE_FROM_PLANCKIAN                 0x2AE3
#define GATT_CHAR_UUID_CHROMATIC_COORDINATES                             0x2AE4
#define GATT_CHAR_UUID_CHROMATIC_IN_CCT_AND_DUV_VALUES                   0x2AE5
#define GATT_CHAR_UUID_CHROMATIC_TOLERANCE                               0x2AE6
#define GATT_CHAR_UUID_CIE_COLOR_RENDERING_INDEX                         0x2AE7
#define GATT_CHAR_UUID_COEFFICIENT                                       0x2AE8
#define GATT_CHAR_UUID_CORRELATED_COLOR_TEMPERATURE                      0x2AE9
#define GATT_CHAR_UUID_COUNT_16                                          0x2AEA
#define GATT_CHAR_UUID_COUNT_24                                          0x2AEB
#define GATT_CHAR_UUID_COUNTRY_CODE                                      0x2AEC
#define GATT_CHAR_UUID_DATE_UTC                                          0x2AED
#define GATT_CHAR_UUID_ELE_CURRENT                                       0x2AEE
#define GATT_CHAR_UUID_ELE_CURRENT_RANGE                                 0x2AEF
#define GATT_CHAR_UUID_ELE_CURRENT_SPECIFIC                              0x2AF0
#define GATT_CHAR_UUID_ELE_CURRENT_STATS                                 0x2AF1
#define GATT_CHAR_UUID_ENERGY                                            0x2AF2
#define GATT_CHAR_UUID_ENERGY_IN_A_PERIOD_OF_DAY                         0x2AF3
#define GATT_CHAR_UUID_EVENT_STATS                                       0x2AF4
#define GATT_CHAR_UUID_FIXED_STRING_16                                   0x2AF5
#define GATT_CHAR_UUID_FIXED_STRING_24                                   0x2AF6
#define GATT_CHAR_UUID_FIXED_STRING_36                                   0x2AF7
#define GATT_CHAR_UUID_FIXED_STRING_8                                    0x2AF8
#define GATT_CHAR_UUID_GENERIC_LEVEL                                     0x2AF9
#define GATT_CHAR_UUID_GLOBAL_TRADE_ITEM_NUMBER                          0x2AFA
#define GATT_CHAR_UUID_ILLUMINANCE                                       0x2AFB
#define GATT_CHAR_UUID_LUMI_EFFICACY                                     0x2AFC
#define GATT_CHAR_UUID_LUMI_ENERGY                                       0x2AFD
#define GATT_CHAR_UUID_LUMI_EXPOSURE                                     0x2AFE
#define GATT_CHAR_UUID_LUMI_FLUX                                         0x2AFF
#define GATT_CHAR_UUID_LUMI_FLUX_RANGE                                   0x2B00
#define GATT_CHAR_UUID_LUMI_INTENSITY                                    0x2B01
#define GATT_CHAR_UUID_MASS_FLOW                                         0x2B02
#define GATT_CHAR_UUID_PERCEIVED_LIGHTNESS                               0x2B03
#define GATT_CHAR_UUID_PERCENTAGE_8                                      0x2B04
#define GATT_CHAR_UUID_POWER                                             0x2B05
#define GATT_CHAR_UUID_POWER_SPECIFIC                                    0x2B06
#define GATT_CHAR_UUID_REL_RUNTIME_IN_A_CURENT_RANGE                     0x2B07
#define GATT_CHAR_UUID_REL_RUNTIME_IN_A_GENERIC_LEVEL_RANGE              0x2B08
#define GATT_CHAR_UUID_REL_VALUE_IN_A_VOLT_RANGE                         0x2B09
#define GATT_CHAR_UUID_REL_VALUE_IN_A_ILLUMI_RANGE                       0x2B0A
#define GATT_CHAR_UUID_REL_VALUE_IN_A_PERIOD_OF_DAY                      0x2B0B
#define GATT_CHAR_UUID_REL_VALUE_IN_A_TEMPERATURE_RANGE                  0x2B0C
#define GATT_CHAR_UUID_TEMPERATURE_8                                     0x2B0D
#define GATT_CHAR_UUID_TEMPERATURE_8_IN_A_PERIOD_OF_DAY                  0x2B0E
#define GATT_CHAR_UUID_TEMPERATURE_8_STATS                               0x2B0F
#define GATT_CHAR_UUID_TEMPERATURE_RANGE                                 0x2B10
#define GATT_CHAR_UUID_TEMPERATURE_STATS                                 0x2B11
#define GATT_CHAR_UUID_TIME_DECIHOUR_8                                   0x2B12
#define GATT_CHAR_UUID_TIME_EXP_8                                        0x2B13
#define GATT_CHAR_UUID_TIME_HOUR_24                                      0x2B14
#define GATT_CHAR_UUID_TIME_MILLISECOND_24                               0x2B15
#define GATT_CHAR_UUID_TIME_SECOND_16                                    0x2B16
#define GATT_CHAR_UUID_TIME_SECOND_8                                     0x2B17
#define GATT_CHAR_UUID_VOLTAGE                                           0x2B18
#define GATT_CHAR_UUID_VOLTAGE_SPECIFIC                                  0x2B19
#define GATT_CHAR_UUID_VOLTAGE_STATS                                     0x2B1A
#define GATT_CHAR_UUID_VOLTAGE_FLOW                                      0x2B1B
#define GATT_CHAR_UUID_CHROMATIC_COORDINATE                              0x2B1C
#define GATT_CHAR_UUID_RC_FEATURE                                        0x2B1D
#define GATT_CHAR_UUID_RC_SETTINGS                                       0x2B1E
#define GATT_CHAR_UUID_RECONN_CONFIG_CTRL_POINT                          0x2B1F
#define GATT_CHAR_UUID_IDD_STATUS_CHANGED                                0x2B20
#define GATT_CHAR_UUID_IDD_STATUS                                        0x2B21
#define GATT_CHAR_UUID_IDD_ANNUNC_STATUS                                 0x2B22
#define GATT_CHAR_UUID_IDD_FEATURES                                      0x2B23
#define GATT_CHAR_UUID_IDD_STATUS_READER_CTRL_POINT                      0x2B24
#define GATT_CHAR_UUID_IDD_COMMAND_CTRL_POINT                            0x2B25
#define GATT_CHAR_UUID_IDD_COMMAND_DATA                                  0x2B26
#define GATT_CHAR_UUID_IDD_RECORD_ACCESS_CTRL_POINT                      0x2B27
#define GATT_CHAR_UUID_IDD_HISTORY_DATA                                  0x2B28
#define GATT_CHAR_UUID_CLIENT_SUPP_FEATURES                              0x2B29
#define GATT_CHAR_UUID_DATABASE_HASH                                     0x2B2A
#define GATT_CHAR_UUID_BSS_CTRL_POINT                                    0x2B2B
#define GATT_CHAR_UUID_BSS_RESPONSE                                      0x2B2C
#define GATT_CHAR_UUID_EMERGENCY_ID                                      0x2B2D
#define GATT_CHAR_UUID_EMERGENCY_TEXT                                    0x2B2E
#define GATT_CHAR_UUID_ENH_BLOOD_PRESSURE_MEASURE                        0x2B34
#define GATT_CHAR_UUID_ENH_INTERMEDIATE_CUFF_PRESSURE                    0x2B35
#define GATT_CHAR_UUID_BLOOD_PRESSURE_RECORD                             0x2B36
#define GATT_CHAR_UUID_BREDR_HANDOVER_DATA                               0x2B38
#define GATT_CHAR_UUID_BLUETOOTH_SIG_DATA                                0x2B39
#define GATT_CHAR_UUID_SERVER_SUPP_FEATURES                              0x2B3A
#define GATT_CHAR_UUID_PHY_ACTIVITY_MONITOR_FEATURES                     0x2B3B
#define GATT_CHAR_UUID_GENERAL_ACTIVITY_INSTANT_DATA                     0x2B3C
#define GATT_CHAR_UUID_GENERAL_ACTIVITY_SUMMARY_DATA                     0x2B3D
#define GATT_CHAR_UUID_CARDIORESP_ACTIVITY_INSTANT_DATA                  0x2B3E
#define GATT_CHAR_UUID_CARDIORESP_ACTIVITY_SUMMARY_DATA                  0x2B3F
#define GATT_CHAR_UUID_SETP_COUNTER_ACTIVITY_SUMMARY_DATA                0x2B40
#define GATT_CHAR_UUID_SLEEP_ACTIVITY_INSTANT_DATA                       0x2B41
#define GATT_CHAR_UUID_SLEEP_ACTIVITY_SUMMARY_DATA                       0x2B42
#define GATT_CHAR_UUID_PHY_ACTIVITY_MONITOR_CTRL_POINT                   0x2B43
#define GATT_CHAR_UUID_ACTIVITY_CURRENT_SESSION                          0x2B44
#define GATT_CHAR_UUID_PHY_ACTIVITY_SESSION_DESC                         0x2B45
#define GATT_CHAR_UUID_PREFER_UNITS                                      0x2B46
#define GATT_CHAR_UUID_HIGH_RESO_HEIGHT                                  0x2B47
#define GATT_CHAR_UUID_MIDDLE_NAME                                       0x2B48
#define GATT_CHAR_UUID_STRIDE_LENGTH                                     0x2B49
#define GATT_CHAR_UUID_HANDEDNESS                                        0x2B4A
#define GATT_CHAR_UUID_DEVICE_WEARING_POSITION                           0x2B4B
#define GATT_CHAR_UUID_FOUR_ZONE_HEART_RATE_LIMITS                       0x2B4C
#define GATT_CHAR_UUID_HIGH_INTENSITY_EXERCISE_THRESHOLD                 0x2B4D
#define GATT_CHAR_UUID_ACTIVITY_GOAL                                     0x2B4E
#define GATT_CHAR_UUID_SEDENTARY_INTERVAL_NOTIFY                         0x2B4F
#define GATT_CHAR_UUID_CALORIC_INTAKE                                    0x2B50
#define GATT_CHAR_UUID_TMAP_ROLE                                         0x2B51
#define GATT_CHAR_UUID_AUDIO_INPUT_STATE                                 0x2B77
#define GATT_CHAR_UUID_GAIN_SETTINGS_ATTRIBUTE                           0x2B78
#define GATT_CHAR_UUID_AUDIO_INPUT_TYPE                                  0x2B79
#define GATT_CHAR_UUID_AUDIO_INPUT_STATUS                                0x2B7A
#define GATT_CHAR_UUID_AUDIO_INPUT_CTRL_POINT                            0x2B7B
#define GATT_CHAR_UUID_AUDIO_INPUT_DESC                                  0x2B7C
#define GATT_CHAR_UUID_VOLUME_STATE                                      0x2B7D
#define GATT_CHAR_UUID_VOLUME_CTRL_POINT                                 0x2B7E
#define GATT_CHAR_UUID_VOLUME_FLAGS                                      0x2B7F
#define GATT_CHAR_UUID_VOLUME_OFFSET_STATE                               0x2B80
#define GATT_CHAR_UUID_AUDIO_LOCATION                                    0x2B81
#define GATT_CHAR_UUID_VOLUME_OFFSET_CTRL_POINT                          0x2B82
#define GATT_CHAR_UUID_AUDIO_OUTPUT_DESC                                 0x2B83
#define GATT_CHAR_UUID_SET_IRK                                           0x2B84
#define GATT_CHAR_UUID_COORD_SET_SIZE                                    0x2B85
#define GATT_CHAR_UUID_SET_MEMBER_LOCK                                   0x2B86
#define GATT_CHAR_UUID_SET_MEMBER_RANK                                   0x2B87
#define GATT_CHAR_UUID_ENCRYPTED_DATA_KEY_MATERIAL                       0x2B88
#define GATT_CHAR_UUID_DEVICE_TIME_FEATURE                               0x2B8E
#define GATT_CHAR_UUID_DEVICE_TIME_PARAMS                                0x2B8F
#define GATT_CHAR_UUID_DEVICE_TIME                                       0x2B90
#define GATT_CHAR_UUID_DEVICE_TIME_CTRL_POINT                            0x2B91
#define GATT_CHAR_UUID_TIME_CHANGE_LOG_DATA                              0x2B92
#define GATT_CHAR_UUID_MEDIA_PLAYER_NAME                                 0x2B93
#define GATT_CHAR_UUID_MEDIA_PLAYER_ICON_OBJECT_ID                       0x2B94
#define GATT_CHAR_UUID_MEDIA_PLAYER_ICON_URL                             0x2B95
#define GATT_CHAR_UUID_TRACK_CHANGED                                     0x2B96
#define GATT_CHAR_UUID_TRACK_TITLE                                       0x2B97
#define GATT_CHAR_UUID_TRACK_DURATION                                    0x2B98
#define GATT_CHAR_UUID_TRACK_POSITION                                    0x2B99
#define GATT_CHAR_UUID_PLAYBACK_SPEED                                    0x2B9A
#define GATT_CHAR_UUID_SEEKING_SPEED                                     0x2B9B
#define GATT_CHAR_UUID_CURRENT_TRACK_SEG_OBJECT_ID                       0x2B9C
#define GATT_CHAR_UUID_CURRENT_TRACK_OBJECT_ID                           0x2B9D
#define GATT_CHAR_UUID_NEXT_TRACK_OBJECT_ID                              0x2B9E
#define GATT_CHAR_UUID_PARENT_GROUP_OBJECT_ID                            0x2B9F
#define GATT_CHAR_UUID_CURRENT_GROUP_OBJECT_ID                           0x2BA0
#define GATT_CHAR_UUID_PLAYING_ORDER                                     0x2BA1
#define GATT_CHAR_UUID_PLAYING_ORDERS_SUPP                               0x2BA2
#define GATT_CHAR_UUID_MEDIA_STATE                                       0x2BA3
#define GATT_CHAR_UUID_MEDIA_CTRL_POINT                                  0x2BA4
#define GATT_CHAR_UUID_MEDIA_CTRL_POINT_OPCODES_SUPP                     0x2BA5
#define GATT_CHAR_UUID_SEARCH_RESULTS_OBJECT_ID                          0x2BA6
#define GATT_CHAR_UUID_SEARCH_CTRL_POINT                                 0x2BA7
#define GATT_CHAR_UUID_MEDIA_PLAYER_ICON_OBJECT_TYPE                     0x2BA9
#define GATT_CHAR_UUID_TRACK_SEG_OBJECT_TYPE                             0x2BAA
#define GATT_CHAR_UUID_TRACK_OBJECT_TYPE                                 0x2BAB
#define GATT_CHAR_UUID_GROUP_OBJECT_TYPE                                 0x2BAC
#define GATT_CHAR_UUID_CTE_ENABLE                                        0x2BAD
#define GATT_CHAR_UUID_ADV_CTE_MIN_LENGTH                                0x2BAE
#define GATT_CHAR_UUID_ADV_CTE_MIN_TX_COUNT                              0x2BAF
#define GATT_CHAR_UUID_ADV_CTE_TX_DURATION                               0x2BB0
#define GATT_CHAR_UUID_ADV_CTE_INTERVAL                                  0x2BB1
#define GATT_CHAR_UUID_ADV_CTE_PHY                                       0x2BB2
#define GATT_CHAR_UUID_BEARER_PROVIDER_NAME                              0x2BB3
#define GATT_CHAR_UUID_BEARER_UCI                                        0x2BB4
#define GATT_CHAR_UUID_BEARER_TECHNOLOGY                                 0x2BB5
#define GATT_CHAR_UUID_BEARER_URI_SCHEMES_SUPP_LIST                      0x2BB6
#define GATT_CHAR_UUID_BEARER_SIGNAL_STENGTH                             0x2BB7
#define GATT_CHAR_UUID_BEARER_SIGNAL_STENGTH_REPORT_INTERVAL             0x2BB8
#define GATT_CHAR_UUID_BEARER_LIST_CURRENT_CALLS                         0x2BB9
#define GATT_CHAR_UUID_CONTENT_CONTROL_ID                                0x2BBA
#define GATT_CHAR_UUID_STATUS_FLAGS                                      0x2BBB
#define GATT_CHAR_UUID_INCOMING_CALL_TARGET_BEARER_URI                   0x2BBC
#define GATT_CHAR_UUID_CALL_STATE                                        0x2BBD
#define GATT_CHAR_UUID_CALL_CTRL_POINT                                   0x2BBE
#define GATT_CHAR_UUID_CALL_CTRL_POINT_OPTIONAL_OPCODES                  0x2BBF
#define GATT_CHAR_UUID_TERMINATION_REASON                                0x2BC0
#define GATT_CHAR_UUID_INCOMING_CALL                                     0x2BC1
#define GATT_CHAR_UUID_CALL_FRIENDLY_NAME                                0x2BC2
#define GATT_CHAR_UUID_MUTE                                              0x2BC3
#define GATT_CHAR_UUID_SINK_ASE                                          0x2BC4
#define GATT_CHAR_UUID_SOURCE_ASE                                        0x2BC5
#define GATT_CHAR_UUID_ASE_CTRL_POINT                                    0x2BC6
#define GATT_CHAR_UUID_BC_AUDIO_SCAN_CTRL_POINT                          0x2BC7
#define GATT_CHAR_UUID_BC_RECEIVE_STATE                                  0x2BC8
#define GATT_CHAR_UUID_SINK_PAC                                          0x2BC9
#define GATT_CHAR_UUID_SINK_AUDIO_LOCATIONS                              0x2BCA
#define GATT_CHAR_UUID_SOURCE_PAC                                        0x2BCB
#define GATT_CHAR_UUID_SOURCE_AUDIO_LOCATIONS                            0x2BCC
#define GATT_CHAR_UUID_AVAIL_AUDIO_CONTEXTS                              0x2BCD
#define GATT_CHAR_UUID_SUPP_AUDIO_CONTEXTS                               0x2BCE
#define GATT_CHAR_UUID_AMMONIA_CONCENTRATION                             0x2BCF
#define GATT_CHAR_UUID_CARBON_MONOXIDE_CONCENTRATION                     0x2BD0
#define GATT_CHAR_UUID_METHANE_CONCENTRATION                             0x2BD1
#define GATT_CHAR_UUID_NITROGEN_DIOXIDE_CONCENTRATION                    0x2BD2
#define GATT_CHAR_UUID_NON_METHANE_VOLATILE_ORGANIC_COMP_CONCENTRATION   0x2BD3
#define GATT_CHAR_UUID_OZON_CONCENTRATION                                0x2BD4
#define GATT_CHAR_UUID_PART_MATTER_PM1_CONCENTRATION                     0x2BD5
#define GATT_CHAR_UUID_PART_MATTER_PM25_CONCENTRATION                    0x2BD6
#define GATT_CHAR_UUID_PART_MATTER_PM10_CONCENTRATION                    0x2BD7
#define GATT_CHAR_UUID_SULFUR_DIOXIDE_CONCENTRATION                      0x2BD8
#define GATT_CHAR_UUID_SULFUR_HEXAFLU_CONCENTRATION                      0x2BD9
#define GATT_CHAR_UUID_HEARING_AID_FEATURES                              0x2BDA
#define GATT_CHAR_UUID_HEARING_AID_PRESET_CTRL_POINT                     0x2BDB
#define GATT_CHAR_UUID_ACTIVE_PRESET_INDEX                               0x2BDC
#define GATT_CHAR_UUID_FIXED_STRING_64                                   0x2BDE
#define GATT_CHAR_UUID_HIGH_TEMPERATURE                                  0x2BDF
#define GATT_CHAR_UUID_HIGH_VOLTAGE                                      0x2BE0
#define GATT_CHAR_UUID_LIGHT_DISTRIBUTION                                0x2BE1
#define GATT_CHAR_UUID_LIGHT_OUTPUT                                      0x2BE2
#define GATT_CHAR_UUID_LIGHT_SOURCE_TYPE                                 0x2BE3
#define GATT_CHAR_UUID_NOISE                                             0x2BE4
#define GATT_CHAR_UUID_RELATIVE_RUNTIME_IN_A_CCTR                        0x2BE5
#define GATT_CHAR_UUID_TIME_SECOND_32                                    0x2BE6
#define GATT_CHAR_UUID_VOC_CONCENTRATION                                 0x2BE7
#define GATT_CHAR_UUID_VOLTAGE_FREQUENCY                                 0x2BE8
#define GATT_CHAR_UUID_BATT_CRITICAL_STATUS                              0x2BE9
#define GATT_CHAR_UUID_BATT_HEALTH_STATUS                                0x2BEA
#define GATT_CHAR_UUID_BATT_HEALTH_INFO                                  0x2BEB
#define GATT_CHAR_UUID_BATT_INFORMATION                                  0x2BEC
#define GATT_CHAR_UUID_BATT_LEVEL_STATUS                                 0x2BED
#define GATT_CHAR_UUID_BATT_TIME_STATUS                                  0x2BEE
#define GATT_CHAR_UUID_ESTIMATED_SERVICE_DATE                            0x2BEF
#define GATT_CHAR_UUID_BATT_ENERGY_STATUS                                0x2BF0
#define GATT_CHAR_UUID_OBSERVATION_SCHE_CHANGED                          0x2BF1
#define GATT_CHAR_UUID_CURR_ELAPSED_TIME                                 0x2BF2
#define GATT_CHAR_UUID_HEALTH_SENSOR_FEATURES                            0x2BF3
#define GATT_CHAR_UUID_GHS_CONTROL_POINT                                 0x2BF4
#define GATT_CHAR_UUID_LE_GATT_SECURITY_LEVELS                           0x2BF5
#define GATT_CHAR_UUID_ESL_ADDRESS                                       0x2BF6
#define GATT_CHAR_UUID_AP_SYNC_KEY_MATERIAL                              0x2BF7
#define GATT_CHAR_UUID_ESL_RSP_KEY_MATERIAL                              0x2BF8
#define GATT_CHAR_UUID_ESL_CURR_ABSOLUTE_TIME                            0x2BF9
#define GATT_CHAR_UUID_ESL_DISPLAY_INFO                                  0x2BFA
#define GATT_CHAR_UUID_ESL_IMAGE_INFO                                    0x2BFB
#define GATT_CHAR_UUID_ESL_SENSOR_INFO                                   0x2BFC
#define GATT_CHAR_UUID_ESL_LED_INFO                                      0x2BFD
#define GATT_CHAR_UUID_ESL_CONTROL_POINT                                 0x2BFE
#define GATT_CHAR_UUID_UDI_FOR_MEDICAL_DEVICES                           0x2BFF
#define GATT_CHAR_UUID_GMAP_ROLE                                         0x2C00
#define GATT_CHAR_UUID_GMAP_UGG_FEATURES                                 0x2C01
#define GATT_CHAR_UUID_GMAP_UGT_FEATURES                                 0x2C02
#define GATT_CHAR_UUID_GMAP_BGS_FEATURES                                 0x2C03
#define GATT_CHAR_UUID_GMAP_BGR_FEATURES                                 0x2C04

/* GATT UNIT */
#define GATT_UNIT_UUID_UNITLESS                                          0x2700
#define GATT_UNIT_UUID_LEN_METRE                                         0x2701
#define GATT_UNIT_UUID_MASS_KG                                           0x2702
#define GATT_UNIT_UUID_TIME_SEC                                          0x2703
#define GATT_UNIT_UUID_ELE_CURRENT_AMPERE                                0x2704
#define GATT_UNIT_UUID_TEMPERATURE_KELVIN                                0x2705
#define GATT_UNIT_UUID_MOLE                                              0x2706
#define GATT_UNIT_UUID_CANDELA                                           0x2707
#define GATT_UNIT_UUID_AREA_M2                                           0x2710
#define GATT_UNIT_UUID_VOLUME_M3                                         0x2711
#define GATT_UNIT_UUID_VELOCITY_METRES_PER_SEC                           0x2712
#define GATT_UNIT_UUID_ACCELERATION_METRES_PER_SEC2                      0x2713
#define GATT_UNIT_UUID_WAVENUMBER                                        0x2714
#define GATT_UNIT_UUID_DENSITY_KG_PER_M3                                 0x2715
#define GATT_UNIT_UUID_SURFACE_DENSITY_KG_PER_M2                         0x2716
#define GATT_UNIT_UUID_M3_PER_KG                                         0x2717
#define GATT_UNIT_UUID_CURRENT_DENSITY_AMPERE_PER_M2                     0x2718
#define GATT_UNIT_UUID_MAG_FIELD_STRENGTH_AMPERE_PER_METRE               0x2719
#define GATT_UNIT_UUID_MOLE_PER_M3                                       0x271A
#define GATT_UNIT_UUID_MASS_CONCENTRATION_KG_PER_M3                      0x271B
#define GATT_UNIT_UUID_LUMINANCE_CANDELA_PER_M2                          0x271C
#define GATT_UNIT_UUID_REFRA_INDEX                                       0x271D
#define GATT_UNIT_UUID_REL_PERMEABILITY                                  0x271E
#define GATT_UNIT_UUID_ANGLE_RADIAN                                      0x2720
#define GATT_UNIT_UUID_ANGLE_STERADIAN                                   0x2721
#define GATT_UNIT_UUID_FERQ_HZ                                           0x2722
#define GATT_UNIT_UUID_FORCE_N                                           0x2723
#define GATT_UNIT_UUID_PRESSURE_PASCAL                                   0x2724
#define GATT_UNIT_UUID_ENERGY_JOULE                                      0x2725
#define GATT_UNIT_UUID_POWER_WATT                                        0x2726
#define GATT_UNIT_UUID_ELE_CHARGE_COULOMB                                0x2727
#define GATT_UNIT_UUID_ELE_POTENTIAL_DIFF_VOLT                           0x2728
#define GATT_UNIT_UUID_CAPA_FARAD                                        0x2729
#define GATT_UNIT_UUID_ELE_RESIST_OHM                                    0x272A
#define GATT_UNIT_UUID_ELE_CONDUCT_SIEMENS                               0x272B
#define GATT_UNIT_UUID_MAG_FLUX_WEBER                                    0x272C
#define GATT_UNIT_UUID_MAG_FLUX_DENSITY_TESLA                            0x272D
#define GATT_UNIT_UUID_INDUCTANCE_HENRY                                  0x272E
#define GATT_UNIT_UUID_TEMPERATURE_DEGREE_CELSIUS                        0x272F
#define GATT_UNIT_UUID_LUMINOUS_FLUX_LUMEN                               0x2730
#define GATT_UNIT_UUID_ILLUMINANCE_LUX                                   0x2731
#define GATT_UNIT_UUID_ACTIVITY_REFERRED_BECQUEREL                       0x2732
#define GATT_UNIT_UUID_ABSORBED_DOSE_GRAY                                0x2733
#define GATT_UNIT_UUID_DOSE_EQUIVALENT_SIEVERT                           0x2734
#define GATT_UNIT_UUID_CATALYTIC_ACTIVITY_KATAL                          0x2735
#define GATT_UNIT_UUID_DYN_VISCOSITY_PASCAL_SEC                          0x2740
#define GATT_UNIT_UUID_MOMENT_OF_FORCE_N_METRE                           0x2741
#define GATT_UNIT_UUID_SURFACE_TENSION_N_PER_METRE                       0x2742
#define GATT_UNIT_UUID_ANG_VELOCITY_RADIAN_PER_SEC                       0x2743
#define GATT_UNIT_UUID_ANG_ACCELERATION_RADIAN_PER_SEC2                  0x2744
#define GATT_UNIT_UUID_HEAT_FLUX_DESNSITY_WATT_PER_M2                    0x2745
#define GATT_UNIT_UUID_HEAT_CAPA_JOULE_PER_K                             0x2746
#define GATT_UNIT_UUID_HEAT_CAPA_JOULE_PER_KG_K                          0x2747
#define GATT_UNIT_UUID_ENERGY_JOULE_PER_KG                               0x2748
#define GATT_UNIT_UUID_THERMAL_CONDUCT_WATT_PER_METRE_K                  0x2749
#define GATT_UNIT_UUID_ENERGY_DENSITY_JOULE_PER_M3                       0x274A
#define GATT_UNIT_UUID_ELE_FIELD_STRENGTH_VOLT_PER_METRE                 0x274B
#define GATT_UNIT_UUID_ELE_CHARGE_DENSITY_COULOMB_PER_M3                 0x274C
#define GATT_UNIT_UUID_SURFACE_CHARGE_DENSITY_COULOMB_PER_M2             0x274D
#define GATT_UNIT_UUID_ELE_FLUX_DENSITY_COULOMB_PER_M2                   0x274E
#define GATT_UNIT_UUID_PERMITTIVITY_FARAD_PER_METRE                      0x274F
#define GATT_UNIT_UUID_PERMEABILITY_HENRY_PER_METRE                      0x2750
#define GATT_UNIT_UUID_MOLAR_ENERGY_JOULE_PER_MOLE                       0x2751
#define GATT_UNIT_UUID_MOLAR_ENTROPY_JOULE_PER_MOLE_K                    0x2752
#define GATT_UNIT_UUID_EXPOSURE_COULOMB_PER_KG                           0x2753
#define GATT_UNIT_UUID_ABSORBED_DOSE_RATE_GRAY_PER_SEC                   0x2754
#define GATT_UNIT_UUID_RADIANT_INTENSITY_WATT_PER_STERADIAN              0x2755
#define GATT_UNIT_UUID_RADIANCE_WATT_PER_M2_STERADIAN                    0x2756
#define GATT_UNIT_UUID_KATAL_PER_M3                                      0x2757
#define GATT_UNIT_UUID_TIME_MINUTE                                       0x2760
#define GATT_UNIT_UUID_TIME_HOUR                                         0x2761
#define GATT_UNIT_UUID_TIME_DAY                                          0x2762
#define GATT_UNIT_UUID_PLANE_ANGLE_DEGREE                                0x2763
#define GATT_UNIT_UUID_PLANE_ANGLE_MINUTE                                0x2764
#define GATT_UNIT_UUID_PLANE_ANGLE_SECOND                                0x2765
#define GATT_UNIT_UUID_AREA_HECTARE                                      0x2766
#define GATT_UNIT_UUID_VOLUME_LITRE                                      0x2767
#define GATT_UNIT_UUID_MASS_TONNE                                        0x2768
#define GATT_UNIT_UUID_PRESSURE_BAR                                      0x2780
#define GATT_UNIT_UUID_PRESSURE_MILLIMETRE_OF_MERCURY                    0x2781
#define GATT_UNIT_UUID_LEN_ANGSTROM                                      0x2782
#define GATT_UNIT_UUID_LEN_NAUTICAL_MILE                                 0x2783
#define GATT_UNIT_UUID_AREA_BAM                                          0x2784
#define GATT_UNIT_UUID_VELOCITY_KNOT                                     0x2785
#define GATT_UNIT_UUID_READIO_QUANTITY_NEPER                             0x2786
#define GATT_UNIT_UUID_READIO_QUANTITY_BEL                               0x2787
#define GATT_UNIT_UUID_LEN_YARD                                          0x27A0
#define GATT_UNIT_UUID_LEN_PARSEC                                        0x27A1
#define GATT_UNIT_UUID_LEN_INCH                                          0x27A2
#define GATT_UNIT_UUID_LEN_FOOT                                          0x27A3
#define GATT_UNIT_UUID_LEN_MILE                                          0x27A4
#define GATT_UNIT_UUID_PRESSURE_POUND_FORCE_PER_INCH2                    0x27A5
#define GATT_UNIT_UUID_VELOCITY_KM_PER_HOUR                              0x27A6
#define GATT_UNIT_UUID_VELOCITY_MILE_PER_HOUR                            0x27A7
#define GATT_UNIT_UUID_ANG_VELOCITY_REVOLUTION_PER_MIN                   0x27A8
#define GATT_UNIT_UUID_ENERGY_GRAM_CALORIE                               0x27A9
#define GATT_UNIT_UUID_ENERGY_KG_CALORIE                                 0x27AA
#define GATT_UNIT_UUID_ENERGY_KILOWATT_HOUR                              0x27AB
#define GATT_UNIT_UUID_TEMPERATURE_DEGREE_FAHRENHEIT                     0x27AC
#define GATT_UNIT_UUID_PERCENT                                           0x27AD
#define GATT_UNIT_UUID_PERMILLE                                          0x27AE
#define GATT_UNIT_UUID_PERIOD_BEATS_PER_MIN                              0x27AF
#define GATT_UNIT_UUID_ELE_CHARGE_AMPERE_HOURS                           0x27B0
#define GATT_UNIT_UUID_MASS_DENSITY_MILLIGRAM_PER_DECILITRE              0x27B1
#define GATT_UNIT_UUID_MASS_DENSITY_MILLIMOLE_PER_LITRE                  0x27B2
#define GATT_UNIT_UUID_TIME_YEAR                                         0x27B3
#define GATT_UNIT_UUID_TIME_MONTH                                        0x27B4
#define GATT_UNIT_UUID_COUNT_PER_M3                                      0x27B5
#define GATT_UNIT_UUID_WATT_PER_M2                                       0x27B6
#define GATT_UNIT_UUID_PER_KG_PER_MINUTE                                 0x27B7
#define GATT_UNIT_UUID_MASS_POUND                                        0x27B8
#define GATT_UNIT_UUID_METABOLIC_EQUIVALENT                              0x27B9
#define GATT_UNIT_UUID_STEP_PER_MINUTE                                   0x27BA
#define GATT_UNIT_UUID_STROKE_PER_MINUTE                                 0x27BC
#define GATT_UNIT_UUID_PACE_KM_PER_MINUTE                                0x27BD
#define GATT_UNIT_UUID_LUMEN_PER_WATT                                    0x27BE
#define GATT_UNIT_UUID_LUMEN_HOUR                                        0x27BF
#define GATT_UNIT_UUID_LUX_HOUR                                          0x27C0
#define GATT_UNIT_UUID_GRAM_PER_SEC                                      0x27C1
#define GATT_UNIT_UUID_LITRE_PER_SEC                                     0x27C2
#define GATT_UNIT_UUID_SOUND_PRESSURE_DECIBEL                            0x27C3
#define GATT_UNIT_UUID_PARTS_PER_MILLION                                 0x27C4
#define GATT_UNIT_UUID_PARTS_PER_BILLION                                 0x27C5

typedef enum {
    GATT_BOOL       = 0x01,
    GATT_UINT2      = 0x02, // no exponent value
    GATT_UINT4      = 0x03, // no exponent value
    GATT_UINT8      = 0x04,
    GATT_UINT12     = 0x05,
    GATT_UINT16     = 0x06,
    GATT_UINT24     = 0x07,
    GATT_UINT32     = 0x08,
    GATT_UINT48     = 0x09,
    GATT_UINT64     = 0x0A,
    GATT_UINT128    = 0x0B,
    GATT_SINT8      = 0x0C,
    GATT_SINT12     = 0x0D,
    GATT_SINT16     = 0x0E,
    GATT_SINT24     = 0x0F,
    GATT_SINT32     = 0x10,
    GATT_SINT48     = 0x11,
    GATT_SINT64     = 0x12,
    GATT_SINT128    = 0x13,
    GATT_FLOAT32    = 0x14, // IEEE-754 32-bit floating point
    GATT_FLOAT64    = 0x15, // IEEE-754 64-bit floating point
    GATT_SFLOAT16   = 0x16, // IEEE-11073 16-bit SFLOAT
    GATT_SFLOAT32   = 0x17, // IEEE-11073 32-bit FLOAT
    GATT_DUINT16    = 0x18, // IEEE-20601 format
    GATT_UTF8       = 0x19,
    GATT_UTF16      = 0x1A,
    GATT_STRUCT     = 0x1B, // opaque structure
} gatt_format_type_t;

typedef enum {
    GATT_NS_BLUETOOTH_SIG = 0x01,
} gatt_namespace_t;

typedef enum {
    GATT_SIG_NS_UNKNOWN         = 0x0000,
    GATT_SIG_NS_FRONT           = 0x0100,
    GATT_SIG_NS_BACK            = 0x0101,
    GATT_SIG_NS_TOP             = 0x0102,
    GATT_SIG_NS_BOTTOM          = 0x0103,
    GATT_SIG_NS_UPPER           = 0x0104,
    GATT_SIG_NS_LOWER           = 0x0105,
    GATT_SIG_NS_MAIN            = 0x0106,
    GATT_SIG_NS_BACKUP          = 0x0107,
    GATT_SIG_NS_AUXILIARY       = 0x0108,
    GATT_SIG_NS_SUPPLEMENTARY   = 0x0109,
    GATT_SIG_NS_FLASH           = 0x010A,
    GATT_SIG_NS_INSIDE          = 0x010B,
    GATT_SIG_NS_OUTSIDE         = 0x010C,
    GATT_SIG_NS_LEFT            = 0x010D,
    GATT_SIG_NS_RIGHT           = 0x010E,
    GATT_SIG_NS_INTERNAL        = 0x010F,
    GATT_SIG_NS_EXTERNAL        = 0x0110,
    // integer from 1 to 250
} gatt_sig_ns_desc_t;

/**
 * The exponent field is only used on integer format types. The exponent
 * field has type sint8.
 *
 *      actual value = Characteristic Value * 10^exponent
 *
 * The namespace field is used to identify the organization. The description
 * field is an enumerated value from the organization identified by the
 * namespace field.
 *
 */

typedef struct {
    uint8_t format;
    uint8_t exponet;
    uint16_t unit;
    uint8_t name_space;
    uint16_t description;
} __attribute__ ((packed)) gatt_present_format_t;

/* APPEARANCE */
#define GATT_APPEARANCE_CATEGORY_MASK 0xFFC0
#define GATT_APPEARANCE_SUBCTGRY_MASK 0x3F
#define GATT_APPEARANCE_SUBCTGRY_BITS 6

#define GATT_APPEARANCE_GENERIC_UNKNOWN                     0x0000
#define GATT_APPEARANCE_GENERIC_PHONE                       0x0040
#define GATT_APPEARANCE_GENERIC_COMPUTER                    0x0080
#define GATT_APPEARANCE_GENERIC_WATCH                       0x00C0
#define GATT_APPEARANCE_GENERIC_CLOCK                       0x0100
#define GATT_APPEARANCE_GENERIC_DISPLAY                     0x0140
#define GATT_APPEARANCE_GENERIC_REMOTE_CONTROL              0x0180
#define GATT_APPEARANCE_GENERIC_EYE_GLASSES                 0x01C0
#define GATT_APPEARANCE_GENERIC_TAG                         0x0200
#define GATT_APPEARANCE_GENERIC_KEYRING                     0x0240
#define GATT_APPEARANCE_GENERIC_MEDIA_PLAYER                0x0280
#define GATT_APPEARANCE_GENERIC_BARCODE_SCANNER             0x02C0
#define GATT_APPEARANCE_GENERIC_THERMOMETER                 0x0300
#define GATT_APPEARANCE_GENERIC_HEART_RATE_SENSOR           0x0340
#define GATT_APPEARANCE_GENERIC_BLOOD_PRESSURE              0x0380
#define GATT_APPEARANCE_GENERIC_HID                         0x03C0
#define GATT_APPEARANCE_HID_KEYBOARD                        0x03C1
#define GATT_APPEARANCE_HID_MOUSE                           0x03C2
#define GATT_APPEARANCE_HID_JOYSTICK                        0x03C3
#define GATT_APPEARANCE_HID_GAMEPAD                         0x03C4
#define GATT_APPEARANCE_HID_DIGITIZER_TABLET                0x03C5
#define GATT_APPEARANCE_HID_CARD_READER                     0x03C6
#define GATT_APPEARANCE_HID_DIGITAL_PEN                     0x03C7
#define GATT_APPEARANCE_HID_BARCODE_SCANNER                 0x03C8
#define GATT_APPEARANCE_HID_TOUCHPAD                        0x03C9
#define GATT_APPEARANCE_HID_PRESENTATION_REMOTE             0x03CA
#define GATT_APPEARANCE_GENERIC_GLUCOSE_METER               0x0400
#define GATT_APPEARANCE_GENERIC_RUNNING_WALING_SENSOR       0x0440
#define GATT_APPEARANCE_GENERIC_CYCLING                     0x0480
#define GATT_APPEARANCE_GENERIC_CONTROL_DEVICE              0x04C0
#define GATT_APPEARANCE_GENERIC_NETWORK_DEVICE              0x0500
#define GATT_APPEARANCE_GENERIC_SENSOR                      0x0540
#define GATT_APPEARANCE_GENERIC_LIGHT_FIXTURES              0x0580
#define GATT_APPEARANCE_GENERIC_FAN                         0x05C0
#define GATT_APPEARANCE_GENERIC_HVAC                        0x0600
#define GATT_APPEARANCE_GENERIC_AIR_CONDITIONING            0x0640
#define GATT_APPEARANCE_GENERIC_HUMIDIFIER                  0x0680
#define GATT_APPEARANCE_GENERIC_HEATING                     0x06C0
#define GATT_APPEARANCE_GENERIC_ACCESS_CONTROL              0x0700
#define GATT_APPEARANCE_GENERIC_MOTORIZED_DEVICE            0x0740
#define GATT_APPEARANCE_GENERIC_POWER_DEVICE                0x0780
#define GATT_APPEARANCE_GENERIC_LIGHT_SOURCE                0x07C0
#define GATT_APPEARANCE_GENERIC_WINDOW_COVERING             0x0800
#define GATT_APPEARANCE_GENERIC_AUDIO_SINK                  0x0840
#define GATT_APPEARANCE_GENERIC_AUDIO_SOURCE                0x0880
#define GATT_APPEARANCE_GENERIC_MOTORIZED_VEHICLE           0x08C0
#define GATT_APPEARANCE_GENERIC_DOMESTIC_APPLIANCE          0x0900
#define GATT_APPEARANCE_GENERIC_WEARABLE_AUDIO_DEVICE       0x0940
#define GATT_APPEARANCE_WEARABLE_EARBUD                     0x0941
#define GATT_APPEARANCE_WEARABLE_HEADSET                    0x0942
#define GATT_APPEARANCE_WEARABLE_HEADPHONES                 0x0943
#define GATT_APPEARANCE_WEARABLE_NECK_BAND                  0x0944
#define GATT_APPEARANCE_GENERIC_AIRCRAFT                    0x0980
#define GATT_APPEARANCE_GENERIC_AV_EQUIPMENT                0x09C0
#define GATT_APPEARANCE_GENERIC_DISPLAY_EQUIPMENT           0x0A00
#define GATT_APPEARANCE_GENERIC_HEARING_AID                 0x0A40
#define GATT_APPEARANCE_GENERIC_GAMING                      0x0A80
#define GATT_APPEARANCE_GENERIC_SIGNAGE                     0x0AC0
#define GATT_APPEARANCE_GENERIC_PULSE_OXIMETER              0x0C40
#define GATT_APPEARANCE_GENERIC_WEIGHT_SCALE                0x0C80
#define GATT_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE    0x0CC0
#define GATT_APPEARANCE_GENERIC_CONT_GLUCOSE_MONITOR        0x0D00
#define GATT_APPEARANCE_GENERIC_INSULIN_PUMP                0x0D40
#define GATT_APPEARANCE_GENERIC_MEDICATION_DELIVERY         0x0D80
#define GATT_APPEARANCE_GENERIC_OUTDOOR_SPORTS_ACTIVITY     0x1440

#ifdef __cplusplus
}
#endif
#endif /* __BT_GATT_SERVICE_H__ */

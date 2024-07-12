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
#ifndef __BT_GAP_SERVICE_H__
#define __BT_GAP_SERVICE_H__
#include "adapter_service.h"
#ifdef __cplusplus
extern "C" {
#endif

#define GAP_UNKNOWN_TX_POWER 0x7F
#define GAP_UNKNOWN_RSSI 0x7F
#define GAP_UNKNOWN_CONN_INTERVAL 0xFFFF

#define GAP_INVALID_CONIDX 0xFF
#define GAP_INVALID_ADV_HANDLE 0xFF
#define GAP_INVALID_SCAN_HANDLE 0xFF
#define GAP_INVALID_INIT_HANDLE 0xFF
#define GAP_INVALID_ADV_SET_ID 0xFF
#define GAP_PA_NO_SUBEVENT 0xFF

#define GAP_INVALID_SYNC_HANDLE 0xFFFF
#define GAP_INVALID_CONN_HANDLE 0xFFFF

#define GAP_KEY_LEN (16)
#define GAP_PUB_KEY_LEN (32)
#define GAP_ADDR_LEN (6)

#define GAP_ADV_SID_MAX (0xF)

#define GAP_DEFAULT_RPA_TIMEOUT (900) // 15min
#define GAP_MAX_DEVICE_NAME_LEN 248

#define gap_continue_loop (true)
#define gap_end_loop (false)

typedef enum {
    GAP_PHY_LE_UNKNOWN = 0x00,
    GAP_PHY_LE_1M = 0x01,
    GAP_PHY_LE_2M = 0x02,
    GAP_PHY_LE_CODED = 0x03,
} gap_le_phy_t;

typedef enum {
    GAP_LE_PHY_UNKNOWN = 0x00,
    GAP_LE_PHY_1M = 0x01,
    GAP_LE_PHY_2M = 0x02,
    GAP_LE_PHY_CODED_S8_CODING = 0x03,
    GAP_LE_PHY_CODED_S2_CODING = 0x04,
} gap_le_detail_phy_t;

typedef enum {
    GAP_PHY_BIT_LE_1M = 0x01,      // bit 0
    GAP_PHY_BIT_LE_2M = 0x02,      // bit 1
    GAP_PHY_BIT_LE_CODED = 0x04,   // bit 2
#if mHDT_LE_SUPPORT
    GAP_PHY_BIT_LE_MHDT = 0x80,   // bit 7
    GAP_PHY_BIT_ALL_MASK = 0x87,
#else
    GAP_PHY_BIT_ALL_MASK = 0x07,
#endif
} gap_phy_bit_t;

typedef enum {
    GAP_CODED_PHY_NO_PREFER_CODING = 0x00,
    GAP_CODED_PHY_PREFER_S2_CODING = 0x01,
    GAP_CODED_PHY_PREFER_S8_CODING = 0x02,
    GAP_CODED_PHY_REQUIR_S2_CODING = 0x03,
    GAP_CODED_PHY_REQUIR_S8_CODING = 0x04,
} gap_coded_phy_prefer_t;

#define SMP_MIN_ENC_KEY_SIZE (7)
#define SMP_MAX_ENC_KEY_SIZE (16)

typedef enum {
    SMP_IO_DISPLAY_ONLY        = 0x00,
    SMP_IO_DISPLAY_YES_NO      = 0x01, // 'yes'could be indicated by pressing a button within a certim time limit otherwise 'no' would be assumed
    SMP_IO_KEYBOARD_ONLY       = 0x02,
    SMP_IO_NO_INPUT_NO_OUTPUT  = 0x03,
    SMP_IO_KEYBOARD_DISPLAY    = 0x04,
    SMP_IO_MAX_CPAS,
} smp_io_cap_t;

typedef enum {
    SMP_METHOD_NONE             = 0x00,
    SMP_JUST_WORKS              = 0x01,
    SMP_PASSKEY_ENTRY           = 0x02,
    SMP_OOB_METHOD              = 0x03,
    SMP_NUMERIC_COMPARE         = 0x04,
    SMP_SECURE_JUST_WORKS       = 0x11,
    SMP_SECURE_PASSKEY_ENTRY    = 0x12,
    SMP_SECURE_OOB_METHOD       = 0x13,
    SMP_SECURE_NUMERIC_COMPARE  = 0x14,
} smp_pairing_method_t;

typedef enum {
    SMP_KDIST_ENC_KEY  = 0x01, // LTK EDIV Rand (legacy pairing), or br gen LTK from Link Key
    SMP_KDIST_ID_KEY   = 0x02, // IRK IA
    SMP_KDIST_SIGN_KEY = 0x04, // CSRK
    SMP_KDIST_LINK_KEY = 0x08, // le gen Link Key from LTK
} smp_key_dist_t;

#define SMP_AUTH_BONDING_MASK    0x03
#define SMP_AUTH_NON_BONDING     0x00
#define SMP_AUTH_BONDING         0x01
#define SMP_AUTH_MITM_PROTECT    0x04
#define SMP_AUTH_SC_SUPPORT      0x08 // LE Secure Connection Pairing support flag
#define SMP_AUTH_KEYPRESS_NOTIFY 0x10 // PairingKeypressNotification shall be generated if both side set support
#define SMP_AUTH_CT2_SUPPORT     0x20 // support for the h7 function or not

typedef struct {
    smp_io_cap_t io_cap;
    uint8_t has_oob_data;
    uint8_t auth_req;
    uint8_t max_enc_key_size;
    uint8_t init_key_dist;
    uint8_t resp_key_dist;
} smp_requirements_t;

typedef struct {
    uint16_t conn_interval_min_1_25ms; // 0x06 to 0x0C80 * 1.25ms, 0xFFFF no specific value
    uint16_t conn_interval_max_1_25ms; // 0x06 to 0x0C80 * 1.25ms, 0xFFFF no specific value
} gap_conn_interval_t;

typedef struct gap_conn_prefer_params_t {
    uint16_t conn_interval_min_1_25ms; // connection interal = interval * 1.25ms
    uint16_t conn_interval_max_1_25ms; // connection interal = interval * 1.25ms
    uint16_t max_peripheral_latency; // 0x00 to 0x01F3, max peripheral latency in units of subrated conn intervals
    uint16_t superv_timeout_ms; // 0x0A to 0x0C80 * 10ms, 100ms to 32s
} gap_conn_prefer_params_t;

typedef struct {
    uint16_t conn_interval_min_1_25ms; // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4000ms
    uint16_t conn_interval_max_1_25ms; // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4000ms
    uint16_t max_peripheral_latency; // 0x00 to 0x01F3, max peripheral latency in units of subrated conn intervals
    uint16_t superv_timeout_ms; // 0x0A to 0x0C80 * 10ms, 100ms to 32s
    uint16_t min_ce_length_slots; // min len of connection event, 0x00 to 0xFFFF * 0.625ms
    uint16_t max_ce_length_slots; // max len of connection event, 0x00 to 0xFFFF * 0.625ms
    uint8_t l2cap_trans_id;
    bool update_using_l2cap_req;
} gap_update_params_t;

typedef struct {
    uint16_t subrate_factor_min; // min subrate factor to be applied to the underlying conn interval, 0x01 to 0x01F4
    uint16_t subrate_factor_max; // max subrate factor to be applied to the underlying conn interval, 0x01 to 0x01F4
    uint16_t max_peripheral_latency; // 0x00 to 0x01F3, max peripheral latency in units of subrated conn intervals
    uint16_t conn_continuation_number; // 0x00 to 0x01F3, default 0x00, num of underlying conn events to remain active after a packet contain a LL PDU with non-zero length is sent or received
    uint16_t superv_timeout_ms; // 0x0A to 0x0C80 * 10ms, 100ms to 32s
} gap_subrate_params_t;

/**
 * LE security mode 1 levels:
 *      Level 1 - No security (no authentication and no encryption)
 *      Level 2 - Unauthenticated pairing with encryption
 *      Level 3 - Authenticated pairing with encryption
 *      Level 4 - Authenticated LE Secure Connections pairing with encryption using a 128-bit enc key
 * LE security mode 2 levels:
 *      shall not be used when a connection is operating in mode 1 level 2/3/4
 *      Level 1 - Unauthenticated pairing with data signing
 *      Level 2 - Authenticated pairing with data signing
 * LE security mode 3 levels:
 *      shall be used to broadcast a BIG in an ISO Broadcaster or receive a BIS in a Synced Receiver
 *      Level 1 - No security (no authentication and no encryption)
 *      Level 2 - Use of unauthenticated Broadcast_Code
 *      Level 3 - Use of authenticated Broadcast_Code
 * LE Secure Connecitons Only mode (LE security mode 1 level 4)
 *
 */

typedef enum {
    GAP_SEC_NO_SECURITY         = 0x01,
    GAP_SEC_UNAUTHENTICATED     = 0x02,
    GAP_SEC_AUTHENTICATED       = 0x03,
    GAP_SEC_SC_AUTHENTICATED    = 0x04,
} gap_link_sec_level_t;

typedef enum {
    GAP_DATA_SIGN_NO_SECURITY         = 0x00,
    GAP_DATA_SIGN_UNAUTHENTICATED     = 0x01,
    GAP_DATA_SIGN_AUTHENTICATED       = 0x02,
} gap_data_sign_level_t;

typedef enum {
    GAP_BIG_SEC_NO_SECURITY     = 0x01,
    GAP_BIG_SEC_UNAUTHENTICATED = 0x02,
    GAP_BIG_SEC_AUTHENTICATED   = 0x03,
} gap_big_sec_level_t;

typedef enum {
    GAP_SAM_JUST_WORKS              = 0x01,
    GAP_SAM_PASSKEY_ENTRY           = 0x02,
    GAP_SAM_OOB_METHOD              = 0x04,
    GAP_SAM_NUMERIC_COMPARE         = 0x08,
    GAP_SAM_SECURE_JUST_WORKS       = 0x10,
    GAP_SAM_SECURE_PASSKEY_ENTRY    = 0x20,
    GAP_SAM_SECURE_OOB_METHOD       = 0x40,
    GAP_SAM_SECURE_NUMERIC_COMPARE  = 0x80,
} gap_sec_accept_method_t;

typedef struct {
    gap_link_sec_level_t link_sec;
    gap_data_sign_level_t data_sign_sec;
    uint8_t enc_key_size;
    uint8_t accept_method;
} gap_sec_level_t;

typedef struct {
    uint8_t link_sec_level: 3; // 0 not exist, 1 no security, 2 unauth, 3 auth, 4 sc auth
    uint8_t big_sec_level: 2; // 0 not exist, 1 no security, 2 unauth, 3 auth
    uint8_t data_sign_sec_level: 2; // 0 no security, 1 unauth, 2 auth
    uint8_t data_sign_sec_exist: 1;
} gap_security_levels_t;

typedef struct {
    uint8_t session_key_le[16]; // the shared session key
    uint8_t iv_le[8]; // initialization vector, used as the IV for encrypting and authenticating the Encrypted Data
}  __attribute__ ((packed)) gap_key_material_t;

typedef struct {
    gap_key_material_t data;
    bool is_exist;
} gap_encrypted_data_key_material_t;

#define GATT_MAX_SERVER_CCCD 36
#define GATT_MAX_CACHE_SERVICES 8
#define GATT_MAX_CACHE_CHARS 64
#define GATT_MAX_CACHE_128_SERVICES 3
#define GATT_MAX_CACHE_128_CHARS 8

typedef struct {
    uint16_t char_end_handle;
    uint16_t char_value_handle;
    uint16_t char_cccd_handle;
    uint8_t service_seqn: 5;
    uint8_t in_128_service: 1;
    uint8_t reliable_write: 1;
    uint8_t writable_aux: 1;
    uint8_t char_prop;
    uint16_t char_uuid;
} __attribute__ ((packed)) gatt_char_cache_t;

typedef struct {
    uint16_t char_end_handle;
    uint16_t char_value_handle;
    uint16_t char_cccd_handle;
    uint8_t service_seqn: 5;
    uint8_t in_128_service: 1;
    uint8_t reliable_write: 1;
    uint8_t writable_aux: 1;
    uint8_t char_prop;
    uint8_t uuid_128_le[16];
} __attribute__ ((packed)) gatt_128_char_cache_t;

typedef struct {
    uint16_t start_handle;
    uint16_t end_handle;
    uint16_t service_uuid;
} __attribute__ ((packed)) gatt_service_cache_t;

typedef struct {
    uint16_t start_handle;
    uint16_t end_handle;
    uint8_t uuid_128_le[16];
} __attribute__ ((packed)) gatt_128_service_cache_t;

typedef struct {
    uint8_t server_cache_is_exist: 1;
    uint8_t service_change_unaware: 1;
    uint8_t gatt_client_supp_robust_caching: 1;
    uint8_t gatt_client_supp_eatt_bearer: 1;
    uint8_t gatt_client_supp_recv_multi_notify: 1;
    uint8_t gatt_server_supp_eatt_bearer: 1;
    uint8_t gatt_server_has_service_changed_char: 1;
    uint8_t gatt_server_has_database_hash_char: 1;
}  __attribute__ ((packed)) gatt_server_cache_t;

typedef struct {
    uint8_t client_cache_seqn;
    uint8_t peer_database_hash[16];
    gatt_service_cache_t peer_service[GATT_MAX_CACHE_SERVICES];
    gatt_128_service_cache_t peer_128_service[GATT_MAX_CACHE_128_SERVICES];
    gatt_char_cache_t peer_character[GATT_MAX_CACHE_CHARS];
    gatt_128_char_cache_t peer_128_character[GATT_MAX_CACHE_128_CHARS];
}  __attribute__ ((packed)) gatt_client_cache_t;

typedef struct {
    bt_bdaddr_t peer_addr;
    bool trusted;
    bool for_bt_source;
    uint8_t link_key[16];
    uint8_t key_type;
    uint8_t cod[3];
} gap_bredr_sec_t;

typedef struct {
    bt_bdaddr_t peer_addr;
    bt_addr_type_t peer_type;
    uint8_t device_paired: 1; // pairing done successfully or prev bonded
    uint8_t device_bonded: 1; // device paired but may be not bonded due to bonding_type is non bonding or LE legacy pairing LTK not distributed
    uint8_t secure_pairing: 1;
    uint8_t bonded_with_num_compare: 1;
    uint8_t bonded_with_passkey_entry: 1;
    uint8_t bonded_with_oob_method: 1;
    uint8_t peer_irk_distributed: 1;
    uint8_t peer_csrk_distributed: 1;
    uint8_t peer_irk[16];
    uint8_t peer_csrk[16];
    uint8_t ltk[16];
    uint8_t local_ltk[16];
    uint8_t rand[8];
    uint8_t local_rand[8];
    uint8_t ediv[2];
    uint8_t local_ediv[2];
    uint8_t enc_key_size;
} gap_bond_sec_t;

typedef struct {
    uint32_t is_opened: 1;
    uint32_t is_central: 1;
    uint32_t conn_ready: 1;
    uint32_t authencated: 1;
    uint32_t encrypted: 1;
    uint32_t directed: 1;
    uint32_t new_link_key_notified: 1;
    uint32_t smp_let_app_confirm: 1;
    uint32_t smp_pairing_ongoing: 1;
    uint32_t smp_encrypt_ongoing: 1;
    uint32_t auth_mitm_protection: 1;
    uint32_t reconn_eatt_after_enc: 1;
    uint32_t att_tx_sec_block: 1;
    uint32_t att_rx_sec_block: 1;
    uint32_t peer_pairing_is_bonding: 1;
    uint32_t local_pairing_is_bonding: 1;
    uint32_t peer_pairing_support_ct2: 1;
    uint32_t local_pairing_support_ct2: 1;
    uint32_t rx_peer_pairing_fail: 1;
    uint32_t set_use_passkey_entry: 1;
    uint32_t set_use_oob_method: 1;
    uint32_t peer_legacy_ltk_received: 1;
    uint32_t peer_legacy_ediv_rand_received: 1;
    uint32_t peer_irk_received: 1;
    uint32_t peer_ia_received: 1;
    uint32_t peer_bt_supp_eatt_fc_mode: 1;
    uint32_t peer_bt_supp_fixed_smp_cid: 1;
    uint32_t ble_gen_link_key_wait_report: 1;
    uint32_t bredr_gen_ltk_wait_report: 1;
    uint32_t bredr_smp_wait_read_key_size: 1;
    uint32_t peer_rpa_is_resolving: 1;
    uint32_t encrypt_wait_report: 1;
} __attribute__ ((packed)) gap_conn_flag_t;

typedef struct {
    uint16_t central_addr_reso_support: 1;
    uint16_t only_use_rpa_after_bonding: 1;
    uint16_t gatt_client_supp_robust_caching: 1;
    uint16_t gatt_client_supp_eatt_bearer: 1;
    uint16_t gatt_client_supp_recv_multi_notify: 1;
    uint16_t gatt_server_supp_eatt_bearer: 1;
    uint16_t gatt_server_has_service_changed_char: 1;
    uint16_t gatt_server_has_database_hash_char: 1;
    uint16_t gatt_service_change_unaware: 1;
    uint8_t name_length; // max 248 utf-8 string
    gap_security_levels_t sec_levels;
    uint32_t le_features;
    uint32_t high_le_features;
    uint8_t *device_name;
    uint8_t service_database_hash[16];
    struct list_node srvc_list;
} gap_peer_info_t;

/**
 * IRK can be assigned, or randomly generated by the device during manufacturing, or some other method could
 * be used. CSRK is used to sign data in a connection. A device that has received CSRK can verify signatures
 * generated by the distributing device. CSRK can be assigned or randomly generated by the device during
 * manufacturing, or some other method could be used.
 *
 * LE legacy pairing generation of LTK, EDIV and Rand, new values of LTK, EDIV, and Rand shall be generated
 * each time they are distributed. The generated LTK size shall not be longer then the negotiated encryption
 * key size, so its size may need to be shortened.
 *
 */
typedef struct {
    bt_bdaddr_t factory_bt_addr;
    bt_bdaddr_t factory_le_addr;
    bt_bdaddr_t host_local_rpa;
    bt_bdaddr_t le_static_addr;
    const bt_bdaddr_t *hci_bt_addr;
    const bt_bdaddr_t *hci_le_addr;
    const bt_bdaddr_t *hci_random_addr;
    // Used for speed up smp pairing
    // when using DHkey generation
    const uint8_t *secret_key_256;
    const uint8_t *public_key_256;
    uint32_t sign_counter;
    uint8_t irk[16];
    uint8_t csrk[16];
    uint8_t local_database_hash[16];
    uint8_t max_filter_list_size;
    uint8_t max_resolving_list_size;
    uint8_t max_pa_list_size;
    uint8_t max_num_adv_sets;
    uint8_t max_size_att_prep_q;
    uint8_t gatt_self_prf_id;
    uint16_t max_adv_data_len;
    uint32_t le_features;
    uint32_t high_le_features;
    uint8_t adv_coding_sel_support; // core spec v5.4
    uint8_t short_name_max_len;
    uint16_t data_sign_support: 1;
    uint16_t only_use_rpa_after_bonding: 1;
    uint16_t server_database_hash_support: 1;
    uint16_t gatt_server_supp_eatt_bearer: 1;
    uint16_t gatt_client_supp_eatt_bearer: 1;
    uint16_t gatt_client_supp_robust_caching: 1;
    uint16_t gatt_client_supp_recv_multi_notify: 1;
    uint16_t use_random_identity_address: 1;
    uint16_t reso_address_by_host: 1;
    uint16_t address_reso_support: 1;
    uint16_t address_reso_enable_forever: 1;
    gap_security_levels_t sec_levels;
    gap_encrypted_data_key_material_t key_material;
} gap_local_info_t;

typedef struct {
    uint16_t conn_interval_1_25ms; // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4000ms
    uint16_t peripheral_latency; // num of subrated conn events, 0x00 to 0x1F3 (499)
    uint16_t superv_timeout_ms; // 0x0A to 0x0C80 * 10ms, 100ms to 32s
    uint16_t central_clock_accuracy; // only valid on Peripheral, on Central shall be set to 0x00
    uint16_t subrate_factor; // 0x01 to 0x01F4, subrate factor applied to the specified underlying conn interval
    uint16_t conn_continuation_number; // 0x00 to 0x01F3, num of underlying conn events to remain active after a packet contain a LL PDU with non-zero length is sent or received
} gap_conn_timing_t;

typedef struct {
    uint16_t conn_interval_min_1_25ms;  // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4000ms
    uint16_t conn_interval_max_1_25ms;  // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4000ms
    uint16_t max_peripheral_latency;    // 0x00 to 0x01F3, max peripheral latency in units of subrated conn intervals
    uint16_t superv_timeout_ms;         // default 5s, 0x0A to 0x0C80 * 10ms, 100ms to 32s
    uint16_t min_ce_length_slots;       // default 0, min len of connection event, 0x00 to 0xFFFF * 0.625ms
    uint16_t max_ce_length_slots;       // default 8, max len of connection event, 0x00 to 0xFFFF * 0.625ms
} gap_conn_timing_config_t;

struct gap_conn_item_t;

typedef struct {
    uint8_t error_code;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
} gap_conn_param_t;

typedef struct {
    uint8_t error_code;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    uint16_t mtu;
} gap_conn_mtu_exchanged_t;

typedef struct {
    uint8_t error_code;
    uint8_t role; // 0 central, 1 perpherial
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
} gap_conn_failed_t;

typedef struct {
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    const gatt_server_cache_t *srv_cache;
    const gatt_client_cache_t *cli_cache;
} gap_conn_cache_ind_t;

typedef enum {
    GAP_USER_NUMERIC_CONFIRM = 1,
    GAP_USER_LTK_REQUEST_CONFIRM,
    GAP_USER_INPUT_6_DIGIT_PASSKEY,
    GAP_USER_DISPLAY_6_DIGIT_PASSKEY,
    GAP_USER_GET_OOB_LEGACY_TK_VALUE,
    GAP_USER_GET_LOCAL_OOB_AUTH_DATA,
    GAP_USER_GET_PEER_OOB_AUTH_DATA,
} gap_user_confirm_type_t;

typedef struct {
    // hci_ev_le_ltk_request last two params
    uint8_t rand[8];
    uint8_t ediv[2];
} gap_ltk_enc_info_t;

typedef struct {
    gap_user_confirm_type_t type;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    union
    {
        uint32_t numeric_confirm_value; // 6 digits
        uint32_t display_passkey; // 6 digits
        gap_ltk_enc_info_t ltk_req;
    } data;
} gap_user_confirm_t;

typedef struct {
    uint8_t error_code;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    gap_update_params_t params_req;
} gap_conn_update_req_t;

typedef struct {
    uint8_t error_code;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    bool phy_update; // false indicate host read
    gap_le_phy_t tx_phy;
    gap_le_phy_t rx_phy;
} gap_conn_phy_update_t;

typedef enum {
    GAP_LE_LOCAL_TX_POWER_RANGE = 1,
    GAP_LE_ADV_TX_POWER_LEVEL,
    GAP_LE_CONN_LOCAL_TX_POWER,
    GAP_LE_CONN_REMOTE_TX_POWER,
} gap_le_tx_power_type_t;

typedef struct {
    gap_le_tx_power_type_t type;
    union {
    struct {
        int8_t min_tx_power; // -127dBm to 20dBm
        int8_t max_tx_power; // -127dBm to 20dBm
    } range;
    struct {
        uint8_t adv_handle;
        int8_t curr_tx_power;
    } adv;
    struct {
        uint16_t connhdl;
        gap_le_detail_phy_t phy;
        int8_t curr_tx_power; // -127dBm to 20dBm
        bool is_at_min_level;
        bool is_at_max_level;
        int8_t delta; // 0x7F change is not available or is out of range, 0xXX zero unchanged, positive increased, negative decreased
        int8_t max_tx_power; // -127dBm to 20dBm, or 0x7F GAP_UNKNOWN_TX_POWER
    } local;
    struct {
        uint16_t connhdl;
        gap_le_detail_phy_t phy;
        int8_t curr_tx_power; // -127dBm to 20dBm
        bool is_at_min_level;
        bool is_at_max_level;
        int8_t delta; // 0x7F change is not available or is out of range, 0xXX zero unchanged, positive increased, negative decreased
    } remote;
    } u;
} gap_le_tx_power_param_t;

typedef struct {
    uint8_t error_code;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    gap_le_tx_power_param_t param;
} gap_le_tx_power_report_t;

typedef enum {
    GAP_RECV_PEER_IRK_IDENTITY = 1,
    GAP_RECV_PEER_CSRK,
    GAP_RECV_DERIVED_BT_LINK_KEY,
} gap_key_dist_type_t;

typedef struct {
    gap_key_dist_type_t key_type;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    const uint8_t *recv_key;
    bt_addr_type_t recv_ia_type;
    bt_bdaddr_t recv_ia_addr;
    uint8_t enc_key_size;
    bool link_key_generated_but_still_wait_peer_ia;
    struct gap_conn_item_t *conn;
} gap_recv_key_dist_t;

typedef struct {
    uint8_t error_code;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    const gap_key_material_t *key_material;
} gap_recv_key_material_t;

typedef struct {
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    uint8_t auth_req;
} gap_recv_sec_request_t;

typedef struct {
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
    bool pairing_req_or_rsp;
    smp_requirements_t requirements;
} gap_recv_smp_requirements_t;

typedef struct {
    uint8_t err_code;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    struct gap_conn_item_t *conn;
} gap_smp_pairing_cmp_t;

typedef union {
    void *param_ptr;
    gap_conn_param_t *opened;
    gap_conn_param_t *closed;
    gap_conn_failed_t *conn_failed;
    gap_conn_cache_ind_t *conn_cache;
    gap_conn_mtu_exchanged_t *mtu_exchanged;
    gap_user_confirm_t *user_confirm;
    gap_conn_param_t *encrypted;
    gap_recv_key_dist_t *recv_key_dist;
    gap_conn_update_req_t *update_req;
    gap_conn_param_t *params_update;
    gap_conn_param_t *subrate_change;
    gap_conn_phy_update_t *phy_update;
    gap_le_tx_power_report_t *tx_power;
    gap_recv_key_material_t *recv_key_material;
    gap_recv_sec_request_t *recv_security_req;
    gap_recv_smp_requirements_t *smp_requirements;
    gap_smp_pairing_cmp_t *smp_pairing_cmp;
} gap_conn_callback_param_t;

typedef enum {
    GAP_CONN_EVENT_OPENED = BT_EVENT_GAP_CONN_EVENT_START,
    GAP_CONN_EVENT_CLOSED,
    GAP_CONN_EVENT_FAILED,
    GAP_CONN_EVENT_CACHE,
    GAP_CONN_EVENT_MTU_EXCHANGED,
    GAP_CONN_EVENT_USER_CONFIRM,
    GAP_CONN_EVENT_ENCRYPTED,
    GAP_CONN_EVENT_RECV_KEY_DIST,
    GAP_CONN_EVENT_UPDATE_REQ,
    GAP_CONN_EVENT_PARAMS_UPDATE,
    GAP_CONN_EVENT_SUBRATE_CHANGE,
    GAP_CONN_EVENT_PHY_UPDATE,
    GAP_CONN_EVENT_TX_POWER_REPORT,
    GAP_CONN_EVENT_RECV_KEY_METERIAL,
    GAP_CONN_EVENT_RECV_SEC_REQUEST,
    GAP_CONN_EVENT_RECV_SMP_REQUIRE,
    GAP_CONN_EVENT_PAIRING_COMPLETE,
} gap_conn_event_t;

typedef int (*gap_conn_callback_t)(uintptr_t connhdl, gap_conn_event_t event, gap_conn_callback_param_t param);

typedef struct gap_conn_item_t {
    hci_conn_type_t conn_type;
    uint8_t con_idx;
    uint16_t connhdl;
    bt_bdaddr_t own_addr;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    uint16_t sync_handle; // PAwR central role: sync_handle
    bt_bdaddr_t peer_rpa;
    uint8_t ctkd_enc_key_size;
    uint8_t error_code;
    bt_bdaddr_t own_rpa;
    uint8_t adv_handle; // peripheral role: conn from adv_handle
    gap_conn_flag_t conn_flag;
    gap_sec_level_t sec_level;
    gap_conn_timing_t timing;
    gap_conn_callback_t conn_callback;
    gap_peer_info_t peer;
    gap_bond_sec_t sec;
    void *smp_conn;
    void *bap_callback;
    uint8_t *link_key;
} gap_conn_item_t;

typedef struct {
    const uint8_t *ltk;
    uint8_t enc_key_size;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    bool ltk_generated_but_still_wait_peer_kdist;
    struct gap_conn_item_t *bredr_conn;
} gap_recv_derived_ltk_t;

typedef union {
    void *param_ptr;
    gap_le_tx_power_param_t *tx_power;
    gap_recv_derived_ltk_t *recv_derived_ltk;
} gap_global_event_param_t;

typedef enum {
    GAP_EVENT_RECV_STACK_READY = BT_EVENT_GAP_GLOBAL_EVENT_START,
    GAP_EVENT_REFRESH_ADVERTISING,
    GAP_EVENT_RECV_TX_POWER_LEVEL,
    GAP_EVENT_RECV_DERIVED_BLE_LTK,
} gap_global_event_t;

typedef struct gap_external_function_callbacks {
    void (*nv_get_local_csrk)(uint8_t *csrk);
    bool (*nv_get_ble_device_by_index)(uint32_t i, gap_bond_sec_t *out);
    bool (*nv_get_ble_device_by_addr)(const gap_conn_item_t *conn, bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, gap_bond_sec_t *out);
    void (*nv_add_ble_device)(const gap_conn_item_t *conn, const gap_bond_sec_t *bond);
    void (*smp_get_upper_requirements)(const gap_conn_item_t *conn, smp_requirements_t *p_requirements);
    void (*gap_get_upper_specific_irk_ia)(const gap_conn_item_t *conn, uint8_t *irk, ble_bdaddr_t *ia);
    bool (*nv_get_bt_device_by_addr)(const bt_bdaddr_t *bd_addr, gap_bredr_sec_t *out);
    bool (*gatt_need_start_mtu_exchange)(const gap_conn_item_t *conn);
    bool (*btgatt_allowed_send)(uint8_t conidx);
} gap_ext_func_cbs_t;

typedef int (*gap_global_callback_t)(uintptr_t priv, gap_global_event_t event, gap_global_event_param_t param);

typedef bool (*gap_conn_foreach_func_t)(gap_conn_item_t *conn, void *priv);

typedef struct
{
    bt_l2cap_config_t cfg;
    uint8_t address_reso_support: 1;
    uint8_t address_reso_enable_forever: 1;
    uint8_t use_random_identity_address: 1;
    uint8_t server_database_hash_support: 1;
} gap_config_t;

#define GAP_ALL_CONNS 0xFFFFFFFF
#define gap_zero_based_conidx(con_idx) ((con_idx) & 0xf)
#define gap_zero_based_conidx_to_ble_conidx(conidx) (BLE_DEVICE_ID_BASE + ((conidx) & 0xf))
#define gap_zero_based_ble_conidx_as_hdl(conidx) gap_conn_idx_as_hdl(gap_zero_based_conidx_to_ble_conidx(conidx))

uint32_t gap_conn_bf(uint8_t con_idx);
uint16_t gap_conn_hdl(uint8_t con_idx);
uint16_t gap_conn_idx_as_hdl(uint8_t con_idx);

void gap_init(const gap_config_t *cfg, gap_global_callback_t global_callback, const gap_ext_func_cbs_t *p_ext_func_cbs);
gap_config_t gap_get_config(void);
gap_local_info_t *gap_local_info(void);
bool gap_stack_is_ready(void);
gap_conn_item_t *gap_get_conn_item(uint16_t connhdl);
gap_conn_item_t *gap_get_conn_by_bt_address(const bt_bdaddr_t *remote);
gap_conn_item_t *gap_get_conn_by_le_address(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr);
uint8_t gap_count_ble_connections(void);
void gap_dump_ble_status(void);

int gap_conn_foreach(gap_conn_foreach_func_t func, void *param);
const char *gap_local_le_name(uint8_t *len);
const bt_bdaddr_t *gap_hci_bt_address(void);
const bt_bdaddr_t *gap_hci_le_address(void);
const bt_bdaddr_t *gap_factory_bt_address(void);
const bt_bdaddr_t *gap_factory_le_address(void);
const bt_bdaddr_t *gap_public_identity_address(void);
const bt_bdaddr_t *gap_random_identity_address(void);
void gap_recv_local_rpa(void *priv, const bt_bdaddr_t *local_rpa);
const bt_bdaddr_t *gap_local_host_rpa(void);
ble_bdaddr_t gap_local_identity_address(bt_addr_type_t own_addr_type);
ble_bdaddr_t gap_conn_own_identity_address(const gap_conn_item_t *conn);
bt_status_t gap_set_public_address(const bt_bdaddr_t *public_addr, bool is_ble);
bt_status_t gap_set_bt_public_address(const bt_bdaddr_t *public_addr);
bt_status_t gap_set_le_public_address(const bt_bdaddr_t *public_addr);
bool gap_get_ble_device_record_by_index(uint32_t i, gap_bond_sec_t *out);
bool gap_find_ble_device_record(const gap_conn_item_t *conn, bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, gap_bond_sec_t *out);
gap_conn_timing_config_t gap_default_conn_timing(void);
ble_bdaddr_t gap_get_peer_resolved_address(uint16_t connhdl);
void gap_set_send_sec_error_rsp_directly(uint8_t send_sec_error_rsp);
void gap_peer_security_levels_received(gap_conn_item_t *conn, gap_security_levels_t sec_levels);
void gap_peer_enc_data_key_material_received(gap_conn_item_t *conn, const gap_key_material_t *key, uint8_t error_code);
ble_bdaddr_t gap_ctkd_get_le_identity_address(const gap_conn_item_t *conn);

bt_status_t gap_set_local_secret_public_key_pair(const uint8_t *p_sec_key_256, const uint8_t *p_pub_key_256);
bt_status_t gap_gen_local_rpa(void (*cmpl)(void *priv, const bt_bdaddr_t *local_rpa), void *priv);
bt_status_t gap_resolve_rpa(const bt_bdaddr_t *rpa, void (*cmpl)(void *priv, const gap_bond_sec_t *out), void *priv);
bt_status_t gap_terminate_connection(uint16_t connhdl, uint8_t reason);
bt_status_t gap_terminate_all_ble_connection(void);
bt_status_t gap_update_le_conn_parameters(uint16_t connhdl, const gap_update_params_t *params);
bt_status_t gap_accept_le_conn_parameters(uint16_t connhdl, const gap_update_params_t *params, bool accept);
bt_status_t gap_set_default_subrate(const gap_subrate_params_t *params);
bt_status_t gap_update_subrate_parameters(uint16_t connhdl, const gap_subrate_params_t *params);
bt_status_t gap_read_le_conn_phy(uint16_t connhdl);
bt_status_t gap_set_le_default_phy(uint8_t tx_phy_bits, uint8_t rx_phy_bits);
bt_status_t gap_set_le_conn_phy(uint16_t connhdl, uint8_t tx_phy_bits, uint8_t rx_phy_bits, gap_coded_phy_prefer_t coded_prefer);
bt_status_t gap_send_raw_hci_cmd(uint16_t cmd_opcode, uint16_t cmd_len, const uint8_t *data_little_endian, hci_cmd_evt_func_t cmd_cb, void *priv);
bt_status_t gap_send_raw_hci_cmd_extra(uint16_t cmd_opcode, uint16_t cmd_len, const uint8_t *data_le, hci_cmd_evt_func_t cmd_cb, void *priv, void *cont, void *cmpl);
bt_status_t gap_read_le_tx_power_range(void);
bt_status_t gap_read_legacy_adv_tx_power(void);
bt_status_t gap_read_le_adv_tx_power(uint8_t adv_handle);
bt_status_t gap_read_conn_local_tx_power(uint16_t connhdl, gap_le_detail_phy_t phy);
bt_status_t gap_read_conn_remote_tx_power(uint16_t connhdl, gap_le_detail_phy_t phy);
bt_status_t gap_set_conn_tx_power_report(uint16_t connhdl, bool local_enable, bool remote_enable);
bt_status_t gap_set_short_name_max_length(uint8_t len);
bt_status_t gap_set_server_database_hash_support(bool support);
bt_status_t gap_set_client_robust_caching_support(bool support);
bt_status_t gap_clear_client_cache_by_handle(uint16_t connhdl);

/*
 * Features
 *
*/

#define GAP_FEAT_LE_ENCRYPTION              0x00000001 /* send to peer (Y) host controlled (N) */
#define GAP_FEAT_CONN_PARAM_REQ_PROC        0x00000002 /* (Y) (N) */
#define GAP_FEAT_EXT_REJ_INDICATION         0x00000004 /* (Y) (N) */
#define GAP_FEAT_PERIPH_INIT_FEAT_EXCHG     0x00000008 /* (Y) (N) */
#define GAP_FEAT_LE_PING                    0x00000010 /* (O) (N) */
#define GAP_FEAT_LE_DATA_LENGTH_EXT         0x00000020 /* (Y) (N) */
#define GAP_FEAT_LL_PRIVACY                 0x00000040 /* (O) (N) */
#define GAP_FEAT_EXT_SCAN_FILTER_POLICIES   0x00000080 /* (O) (N) */
#define GAP_FEAT_LE_2M_PHY                  0x00000100 /* (Y) (N) */
#define GAP_FEAT_TX_STABLE_MOD_INDEX        0x00000200 /* (Y) (N) */
#define GAP_FEAT_RX_STABLE_MOD_INDEX        0x00000400 /* (Y) (N) */
#define GAP_FEAT_LE_CODED_PHY               0x00000800 /* (Y) (N) */
#define GAP_FEAT_LE_EXT_ADVERTISING         0x00001000 /* (O) (N) */
#define GAP_FEAT_LE_PERIODIC_ADVERTISING    0x00002000 /* (O) (N) */
#define GAP_FEAT_CHANNEL_SEL_ALGO_2         0x00004000 /* (Y) (N) */
#define GAP_FEAT_LE_POWER_CLASS_1           0x00008000 /* (Y) (N) */
#define GAP_FEAT_MIN_NUM_USED_CHANS_PROC    0x00010000 /* (Y) (N) */
#define GAP_FEAT_CONN_CTE_REQUEST           0x00020000 /* (Y) (N) */
#define GAP_FEAT_CONN_CTE_RESPONSE          0x00040000 /* (Y) (N) */
#define GAP_FEAT_CONNLESS_CTE_TX            0x00080000 /* (O) (N) */
#define GAP_FEAT_CONNLESS_CTE_RX            0x00100000 /* (O) (N) */
#define GAP_FEAT_CTE_TX_ANTENNA_SWITCH_AOD  0x00200000 /* (O) (N) */
#define GAP_FEAT_CTE_RX_ANTENNA_SWITCH_AOA  0x00400000 /* (O) (N) */
#define GAP_FEAT_RX_CONSTANT_TONE_EXTS      0x00800000 /* (Y) (N) */
#define GAP_FEAT_PAST_SENDER                0x01000000 /* (Y) (N) */
#define GAP_FEAT_PAST_RECIPIENT             0x02000000 /* (Y) (N) */
#define GAP_FEAT_SLEEP_CLOCK_ACC_UPDATES    0x04000000 /* (Y) (N) */
#define GAP_FEAT_REMOTE_PUB_KEY_VALIDATION  0x08000000 /* (N) (N) */
#define GAP_FEAT_CIS_CENTRAL                0x10000000 /* (Y) (N) */
#define GAP_FEAT_CIS_PERIPHERAL             0x20000000 /* (Y) (N) */
#define GAP_FEAT_ISO_BROADCASTER            0x40000000 /* (Y) (N) */
#define GAP_FEAT_SYNCED_RECEIVER            0x80000000 /* (Y) (N) */

#define GAP_HIGH_FEAT_CIS_HOST_SUPPORT      0x00000001 /* (Y) (Y) */
#define GAP_HIGH_FEAT_LE_POWER_CTRL_REQ     0x00000002 /* (Y) (N) */ // shall always have the same value as next bit
#define GAP_HIGH_FEAT_LE_POWER_CTRL_RQS     0x00000004 /* (Y) (N) */ // shall always have the same value as prev bit
#define GAP_HIGH_FEAT_LE_PATH_LOSS_MONITOR  0x00000008 /* (Y) (N) */
#define GAP_HIGH_FEAT_PA_ADI_SUPPORT        0x00000010 /* (O) (N) */
#define GAP_HIGH_FEAT_CONN_SUBRATING        0x00000020 /* (Y) (N) */
#define GAP_HIGH_FEAT_CONN_SUBRAT_HOST_SUPP 0x00000040 /* (Y) (Y) */
#define GAP_HIGH_FEAT_CHANNEL_CLASSIFY      0x00000080 /* (Y) (N) */
#define GAP_HIGH_FEAT_ADV_CODING_SEL        0x00000100 /* (Y) (N) */
#define GAP_HIGH_FEAT_ADV_CODSEL_HOST_SUPP  0x00000200 /* (Y) (Y) */
#define GAP_HIGH_FEAT_PA_WITH_RSP_ADVTISER  0x00000400 /* (Y) (N) */
#define GAP_HIGH_FEAT_PA_WITH_RSP_SCANNER   0x00000800 /* (Y) (N) */

#define GAP_HOST_FEAT_BIT_CIS_HOST_SUPPORT      32
#define GAP_HOST_FEAT_BIT_CONN_SUBRAT_HOST_SUPP 38
#define GAP_HOST_FEAT_BIT_ADV_CODSEL_HOST_SUPP  41

uint32_t gap_local_le_features(void);
uint32_t gap_local_le_high_features(void);
bt_status_t gap_set_le_host_feature_support(uint8_t bit_number, uint8_t enable);

/**
 * Security
 *
 */

typedef enum {
    SMP_ERROR_NO_ERROR                  = 0x00,
    SMP_ERROR_PASSKEY_ENTRY_FAILED      = 0x01,
    SMP_ERROR_OOB_NOT_AVAILABLE         = 0x02,
    SMP_ERROR_AUTH_REQUIREMENTS         = 0x03,
    SMP_ERROR_CONFIRM_VALUE_FAILED      = 0x04,
    SMP_ERROR_PAIRING_NOT_SUPPORT       = 0x05,
    SMP_ERROR_ENC_KEY_SIZE              = 0x06,
    SMP_ERROR_CMD_NOT_SUPPORT           = 0x07,
    SMP_ERROR_UNSPECIFIED_REASON        = 0x08,
    SMP_ERROR_REPEATED_ATTEMPTS         = 0x09,
    SMP_ERROR_INVALID_PARAMS            = 0x0a,
    SMP_ERROR_DHKEY_CHECK_FAILED        = 0x0b,
    SMP_ERROR_NUM_COMPARISON_FAILED     = 0x0c,
    SMP_ERROR_BEEDR_PAIRING_IN_PROGRESS = 0x0d,
    SMP_ERROR_CTKD_NOT_ALLOWED          = 0x0e,
    SMP_ERROR_KEY_REJECTED              = 0x0f,
    SMP_ERROR_STATUS_CONTINUE           = 0x80, // local use error code
    SMP_ERROR_STATUS_FINISHED           = 0x81,
    SMP_ERROR_STATUS_FAILED             = 0x82,
    SMP_ERROR_INVALID_SMP_CONN          = 0x83,
    SMP_ERROR_INVALID_RESULT            = 0x84,
    SMP_ERROR_HCI_STATUS_ERROR          = 0x85,
    SMP_ERROR_AES_RESULT_FAILED         = 0x86,
    SMP_ERROR_AES_CMAC_FAILED           = 0x87,
    SMP_ERROR_LE_ENC_CMD_FAILED         = 0x88,
    SMP_ERROR_GEN_TK_RAND_FAILED        = 0x89,
    SMP_ERROR_GEN_RANDOM_FAILED         = 0x8A,
    SMP_ERROR_GEN_CFM_VALUE_FAILED      = 0x8B,
    SMP_ERROR_GEN_STK_FAILED            = 0x8C,
    SMP_ERROR_GEN_PUB_KEY_FAILED        = 0x8D,
    SMP_ERROR_GEN_USER_VALUE_FAILED     = 0x8E,
    SMP_ERROR_GEN_DHKEY_FAILED          = 0x8F,
    SMP_ERROR_GEN_F5_KEY_T_FAILED       = 0x90,
    SMP_ERROR_GEN_MACKEY_FAILED         = 0x91,
    SMP_ERROR_GEN_LTK_FAILED            = 0x92,
    SMP_ERROR_GEN_CHECK_VALUE_FAILED    = 0x93,
    SMP_ERROR_ENABLE_ENC_FAILED         = 0x94,
    SMP_ERROR_GEN_ILK_FAILED            = 0x95,
    SMP_ERROR_GEN_ILTK_FAILED           = 0x96,
    SMP_ERROR_LK_FROM_ILK_FAILED        = 0x97,
    SMP_ERROR_LTK_FROM_ILTK_FAILED      = 0x98,
    SMP_ERROR_SMP_TIMER_TIMEOUT         = 0x99,
    SMP_ERROR_ENCRYPTION_FAILED         = 0x9A,
    SMP_ERROR_GET_OOB_DATA_FAILED       = 0x9B,
    SMP_ERROR_RX_PEER_PAIRING_FAIL      = 0x9C,
    SMP_ERROR_MIC_ERROR                 = 0x9D
} smp_error_code_t;

typedef struct {
    uint8_t pkx[GAP_PUB_KEY_LEN];
    uint8_t pky[GAP_PUB_KEY_LEN];
    uint8_t rand[GAP_KEY_LEN];
    uint8_t confirm[GAP_KEY_LEN];
} gap_smp_oob_auth_data_t;

typedef enum {
    GAP_AUTH_STARTED_BY_NONE                = 0x00,
    GAP_AUTH_STARTED_BY_BLE_TEST            = 0x01,
    GAP_AUTH_STARTED_BY_GATT_READY          = 0x02,
    GAP_AUTH_STARTED_BY_UPPER_APP           = 0x03,
    GAP_AUTH_STARTED_BY_ADDRESS             = 0x04,
    GAP_AUTH_STARTED_BY_RECV_SEC_LEVELS     = 0x05,
    GAP_AUTH_STARTED_BY_INIT_COMPLETE       = 0x06,
    GAP_AUTH_STARTED_BY_NOTIFY_SEC_CHECK    = 0x07,
    GAP_ATUH_STARTED_BY_RX_RSP_SEC_ERROR    = 0x08,
    GAP_ATUH_STARTED_BY_RX_REQ_SEC_ERROR    = 0x09,
    GAP_AUTH_STARTED_BY_EATT_SEC_FAIL       = 0x0A,
} gap_who_started_auth_t;

const uint8_t *gap_get_local_irk(void);
void gap_set_local_irk(const uint8_t *p_irk);
const uint8_t *gap_get_local_csrk(void);
bt_status_t gap_bredr_start_ltk_derivation(uint16_t connhdl);
bt_status_t gap_set_security_level(uint16_t connhdl, gap_link_sec_level_t sec_level);
bt_status_t gap_set_data_sign_security(uint16_t connhdl, gap_data_sign_level_t data_sign_level);
bt_status_t gap_set_key_strength(uint16_t connhdl, uint8_t key_size);
bt_status_t gap_set_accepted_pairing_method(uint16_t connhdl, uint8_t accept_method);
bt_status_t gap_start_authentication(uint16_t connhdl, gap_who_started_auth_t who);
bt_status_t gap_start_authentication_by_address(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr);
bt_status_t gap_start_enable_encryption(uint16_t connhdl, const gap_ltk_enc_info_t *p_enc_data, const uint8_t *p_ltk);
bt_status_t gap_set_use_passkey_entry_method(uint16_t connhdl, bool passkey_entry);
bt_status_t gap_set_use_passkey_entry_method_by_address(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, bool passkey_entry);
bt_status_t gap_input_numeric_confirm(uint16_t peer_type_or_connhdl, const bt_bdaddr_t *peer_addr, bool user_confirmed);
bt_status_t gap_reply_peer_ltk_request(uint16_t connhdl, bool negative_reply, const uint8_t *p_ltk_reply);
bt_status_t gap_reply_peer_pairing_request(uint16_t connhdl, bool pairing_failed, smp_error_code_t err_code);
bt_status_t gap_input_6_digit_passkey(uint16_t peer_type_or_connhdl, const bt_bdaddr_t *peer_addr, uint32_t passkey);
bt_status_t gap_input_oob_legacy_tk(uint16_t peer_type_or_connhdl, const bt_bdaddr_t *peer_addr, const uint8_t *tk);
bt_status_t gap_input_local_oob_auth_data(uint16_t peer_type_or_connhdl, const bt_bdaddr_t *peer_addr, const gap_smp_oob_auth_data_t *data);
bt_status_t gap_input_peer_oob_auth_data(uint16_t peer_type_or_connhdl, const bt_bdaddr_t *peer_addr, const gap_smp_oob_auth_data_t *data);

/**
 * Data types for AdvData, ScanRsp, ACAD, OOB, EIR
 *
 */

#define GAP_DT_FLAGS                    0x01 // all numerical multi-byte entities and values shall use little-endian byte order
#define GAP_DT_SRVC_UUID_16_INCP_LIST   0x02
#define GAP_DT_SRVC_UUID_16_CMPL_LIST   0x03
#define GAP_DT_SRVC_UUID_32_INCP_LIST   0x04
#define GAP_DT_SRVC_UUID_32_CMPL_LIST   0x05
#define GAP_DT_SRVC_UUID_128_INCP_LIST  0x06
#define GAP_DT_SRVC_UUID_128_CMPL_LIST  0x07
#define GAP_DT_SHORT_LOCAL_NAME         0x08
#define GAP_DT_COMPLETE_LOCAL_NAME      0x09
#define GAP_DT_TX_POWER_LEVEL           0x0A
#define GAP_DT_CLASS_OF_DEVICE          0x0D
#define GAP_DT_SSP_HASH_C               0x0E
#define GAP_DT_SSP_HASH_C_192           0x0E
#define GAP_DT_SSP_RANDOMIZER_R         0x0F
#define GAP_DT_SSP_RANDOMIZER_R_192     0x0F
#define GAP_DT_DEVICE_ID                0x10
#define GAP_DT_SM_TK_VALUE              0x10
#define GAP_DT_SM_OOB_FLAGS             0x11
#define GAP_DT_PERIPERAL_CONN_INTERVAL  0x12
#define GAP_DT_SRVC_SOLI_16_UUID_LIST   0x14
#define GAP_DT_SRVC_SOLI_128_UUID_LIST  0x15
#define GAP_DT_SERVICE_DATA             0x16
#define GAP_DT_SERVICE_DATA_16BIT_UUID  0x16
#define GAP_DT_PUBLIC_TARGET_ADDRESS    0x17
#define GAP_DT_RANDOM_TARGET_ADDRESS    0x18
#define GAP_DT_APPEARANCE               0x19
#define GAP_DT_ADV_INTERVAL             0x1A
#define GAP_DT_LE_DEVICE_ADDRESS        0x1B
#define GAP_DT_LE_ROLE                  0x1C
#define GAP_DT_SSP_HASH_C_256           0x1D
#define GAP_DT_SSP_RANDOMIZER_R_256     0x1E
#define GAP_DT_SRVC_SOLI_32_UUID_LIST   0x1F
#define GAP_DT_SERVICE_DATA_32BIT_UUID  0x20
#define GAP_DT_SERVICE_DATA_128BIT_UUID 0x21
#define GAP_DT_LE_SC_CONFIRM_VALUE      0x22
#define GAP_DT_LE_SC_RANDOM_VALUE       0x23
#define GAP_DT_URI                      0x24
#define GAP_DT_INDOOR_POSITIONING       0x25
#define GAP_DT_TRANSPORT_DISCOVERY_DATA 0x26
#define GAP_DT_LE_SUPPORTED_FEATURES    0x27
#define GAP_DT_CHAN_MAP_UPDATE_IND      0x28
#define GAP_DT_MESH_PB_ADV              0x29
#define GAP_DT_MESH_MESSAGE             0x2A
#define GAP_DT_MESH_BEACON              0x2B
#define GAP_DT_BIG_INFO                 0x2C
#define GAP_DT_BROADCAST_CODE           0x2D
#define GAP_DT_RESOLVABLE_SET_ID        0x2E
#define GAP_DT_ADV_INTERVAL_LONG        0x2F
#define GAP_DT_BROADCAST_NAME           0x30
#define GAP_DT_ENCRYPTED_ADV_DATA       0x31
#define GAP_DT_PA_RSP_TIMING_INFO       0x32
#define GAP_DT_3D_INFO_DATA             0x3D
#define GAP_DT_MANUFACTURER_DATA        0xFF

/**
 * Simultaneous LE and BR/EDR to Same Device Capable (Controller) indicates
 * that the Controller supports simultaneous LE and BR/EDR links to the
 * same remote device. The local Host uses this feature bit to determine
 * whether the Controller has capable of or not. A remote device does not
 * use this feature bit. This bit requires LE Supported (Controller) feature
 * bit. The Host shall ignore the 'Simultaneous LE and BR/EDR to Same Device
 * Capable (Controller)' bit in the Flags AD type.
 *
 */
#define GAP_FLAGS_LE_NON_DISCOVERABLE_MODE      0x00
#define GAP_FLAGS_LE_LIMITED_DISCOVERABLE_MODE  0x01
#define GAP_FLAGS_LE_GENERAL_DISCOVERABLE_MODE  0x02
#define GAP_FLAGS_BR_EDR_NOT_SUPPORT            0x04
#define GAP_FLAGS_SIMU_LE_BREDR_TO_SAME_DEVICE  0x08

typedef struct {
    uint8_t length;
    uint8_t ad_type;
} __attribute__ ((packed)) gap_dt_head_t;

typedef struct {
    struct pp_buff *ppb;
} gap_dt_buf_t;

typedef struct {
    uint8_t uuid_le[16];
} __attribute__ ((packed)) gap_uuid_128_t;

/**
 * The Encrypted Data data type consists of an encrypted payload secured with a pre-shared
 * session key, a pre-shared initializaiton vector, and randomizer. It is authenticated using
 * a message integerity check (MIC). The session key and iv can be shared by GATT Encrypted
 * Data Key Meterial Characteristic. The Encrypted Data data type shall contain Randomizer,
 * Payload, and MIC fields. The Payload field shall contain a sequence of one or more AD
 * structures that are encrypted.
 *
 * The Payload and MIC fields shall be encrypted using the CCM algorithm with following changes:
 *      The packetCounter and directionBit of the CCM nonce shall be set to the Randomizer field.
 *      In the B1 block, octet 2 shall equal 0xEA.
 *
 * LL CCM 13-byte nonce: From MSB to LSB
 * IV[7][6][5][4][3][2][1][0]|[directionBit|packetCounter[4]&7]|packetCounter[3][2][1][0]
 *
 * AD Type 13-byte nonce: From MSB to LSB
 * IV[7][6][5][4][3][2][1][0]|Randomizer[4]Randomizer[3]Randomizer[2]Randomizer[1]Randomizer[0]
 *
 * <<Encrypted Data>> AD Type:
 * [Len|ED Tag|---------------------Encrypted Data------------------------]
 *            |Randomizer 5B|-----------Payload--------------------|MIC 4B]
 *                          [Len|ESL TAG|ESL][Len|LN Tag|Local Name]
 */

#define GAP_ED_TYPE_ADD_AUTH_DATA 0xEA

typedef struct {
    gap_dt_head_t head;
    uint8_t randomizer_le[5];
} __attribute__ ((packed)) gap_enc_data_t;

typedef void (*gap_key_callback_t)(void *priv, int error_code, const uint8_t *key);
typedef void (*gap_dt_encrypt_callback_t)(void *priv, int error_code, const gap_enc_data_t *encrypted_data);

bool gap_dt_start_encrypt_data(const gap_key_material_t *key, const uint8_t *data, uint16_t len, gap_dt_encrypt_callback_t func, void *priv);
bool gap_dt_start_decrypt_data(const gap_key_material_t *key, const gap_enc_data_t *p, gap_key_callback_t func, void *priv);

bool gap_dt_add_data_type(gap_dt_buf_t *buf, uint8_t type, const uint8_t *data, uint16_t len);
bool gap_dt_add_raw_data(gap_dt_buf_t *buf, const uint8_t *data, uint16_t len);
bool gap_dt_add_flags(gap_dt_buf_t *buf, uint8_t discoverable, bool simu_bredr_support);
bool gap_dt_add_local_le_name(gap_dt_buf_t *buf, bool use_legacy_pdu, const char *cust_le_name);
bool gap_dt_add_tx_power(gap_dt_buf_t *buf, uint8_t tx_power_level);
bool gap_dt_add_manufacturer_data(gap_dt_buf_t *buf, uint16_t company_identifier_code, const uint8_t *data, uint8_t len);
bool gap_dt_add_peripheral_conn_interval(gap_dt_buf_t *buf, uint16_t min_interval_1_25ms, uint16_t max_interval_1_25ms);
bool gap_dt_add_service_data(gap_dt_buf_t *buf, uint16_t service_uuid, const uint8_t *data, uint8_t len);
bool gap_set_le_service_16_uuid(gap_dt_buf_t *buf, const uint16_t *uuid, uint16_t count);
bool gap_set_le_service_128_uuid(gap_dt_buf_t *buf, const gap_uuid_128_t *uuid_le, uint16_t count);

bool gap_dt_buf_is_empty(const gap_dt_buf_t *buf);
const uint8_t *gap_dt_buf_data(const gap_dt_buf_t *buf);
uint16_t gap_dt_buf_len(const gap_dt_buf_t *buf);
void gap_dt_buf_clear(gap_dt_buf_t *buf);
const gap_dt_head_t *gap_dt_buf_find_type(const gap_dt_buf_t *buf, uint8_t ad_type, uint8_t ad_type_2);

typedef struct {
    uint16_t le_limited_discoverable_mode: 1;
    uint16_t le_general_discoverable_mode: 1;
    uint16_t simu_bredr_support: 1;
    uint16_t srvc_16_uuid_cmpl: 1;
    uint16_t srvc_128_uuid_cmpl: 1;
    uint16_t device_name_cmpl: 1;
    uint16_t has_le_features: 1;
    uint16_t has_manufacturer_data: 1;
    uint16_t has_tx_power_level: 1;
    uint16_t has_periph_prefer_conn_interval: 1;
    uint16_t has_le_role: 1;
    uint16_t has_le_supp_features: 1;
} gap_dt_flags_t;

typedef struct {
    uint8_t has_oob_flags: 1;
    uint8_t oob_data_present: 1;
    uint8_t le_host_support: 1;
    uint8_t is_random_address: 1;
} gap_dt_oob_flags_t;

typedef struct {
    gap_dt_flags_t flags;
    uint16_t device_apperance; // valid when non-zero
    uint32_t adv_interval_slots; // >= 40.96s <<advertising interval - long>> data type shall be used
    uint8_t le_role; // 0x00 periph, 0x01 central, 0x02 both and periph preferred for conn, 0x03 both and central preferred for conn
    uint8_t num_srvc_16_uuids;
    uint8_t num_srvc_128_uuids;
    uint8_t num_public_target_addr;
    uint8_t num_random_target_addr;
    uint8_t device_name_len;
    uint8_t big_info_data_len;
    int8_t tx_power_level;
    bt_addr_type_t le_addr_type;
    gap_dt_oob_flags_t smp_oob_flags;
    uint16_t manufacturer_company_id;
    const uint8_t *manufacturer_data;
    const uint16_t *srvc_16_uuid_le;
    const gap_uuid_128_t *srvc_128_uuid;
    const uint8_t *device_name;
    uint32_t le_features;
    uint32_t high_le_features;
    gap_conn_interval_t periph_prefer_conn_interval; // valid when interval_min is not 0xffff
    bt_bdaddr_t *le_bdaddr;
    bt_bdaddr_t *public_target_addr;
    bt_bdaddr_t *random_target_addr;
    const uint8_t *chm_indication;
    const uint8_t *ascs_srvc_data;
    // Broadcast Audio Announcement: Broadcast ID
    const uint8_t *broadcast_id;
    // Basic Audio Announcement: BASE
    struct basic_audio_annoucement
    {
        uint8_t length;
        const uint8_t *base;
    } base_info;
    // Public Broadcast Announcement: PBA
    struct public_broadcast_annoucement
    {
        uint8_t length;
        const uint8_t *pba;
    } pba_info;
    // Broadcast Name
    struct broadcast_name
    {
        uint8_t length;
        const uint8_t *bc_name;
    } broadcast_name;
    const uint8_t *smp_tk;
    const uint8_t *big_info;
    const uint8_t *broadcast_code;
    const uint8_t *rsi; // resolvable set identifier - 6 bytes
} gap_dt_parsed_t;

typedef struct {
    uint16_t extended_adv: 1;
    uint16_t legacy_pdu: 1;
    uint16_t connectable: 1;
    uint16_t scannable: 1;
    uint16_t directed: 1;
    uint16_t scan_rsp: 1;
    uint16_t cmpl_adv_data: 1;
    uint16_t incmpl_adv_data: 1;
    uint16_t has_more_data: 1;
    uint16_t peer_anonymous: 1;
    uint16_t direct_unresolv_addr: 1;
    uint16_t primary_phy: 2;
    uint16_t secondary_phy: 2;
} gap_adv_flags_t;

typedef enum {
    GAP_CTE_TYPE_AOA = 0x00,
    GAP_CTE_TYPE_AOA_1US_SLOTS = 0x01,
    GAP_CTE_TYPE_AOA_2US_SLOTS = 0x02,
    GAP_CTE_TYPE_NO_CTE = 0xFF,
} gap_le_cte_type_t;

typedef struct {
    gap_adv_flags_t adv;
    bt_bdaddr_t peer_addr;
    bt_addr_type_t peer_type;
    bt_addr_type_t direct_addr_type;
    bt_bdaddr_t direct_addr; // TargetA or Public IA or Random IA
    const uint8_t *data;
    uint8_t data_length;
    uint8_t adv_set_id;
    uint16_t sync_handle;
    uint16_t pa_interval;
    int8_t tx_power;
    int8_t rssi;
    uint16_t pa_event_counter;
    uint8_t subevent;
    uint8_t response_slot; // 0x00 to 0xFF, the response slot the data was received in
    bool aux_sync_subevent_ind_not_transmitted;
    bool fail_rx_aux_sync_subevent_rsp;
    bool fail_rx_aux_sync_subevent_ind;
    gap_le_cte_type_t cte_type; // 0x00 to 0x02, 0xFF no cte
    gap_coded_phy_prefer_t primary_coding;
    gap_coded_phy_prefer_t secondary_coding;
    gap_dt_parsed_t parsed;
} gap_adv_report_t;

/**
 * Filter Accept List
 *
 */

#define GAP_FILTER_INVALID_USER 0x7f

typedef struct {
    bt_bdaddr_t peer_addr;
    bt_addr_type_t peer_type;
    uint8_t user: 7;
    uint8_t is_inuse: 1;
} gap_filter_item_t;

bt_status_t gap_filter_list_add(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr);
bt_status_t gap_filter_list_add_user_item(uint8_t user, const ble_bdaddr_t *addr_list, uint16_t count);
bt_status_t gap_filter_list_add_anonymous(void);
bt_status_t gap_filter_list_remove(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr);
bt_status_t gap_filter_list_remove_user_item(uint8_t user);
bt_status_t gap_filter_list_remove_anonymous(void);
bt_status_t gap_filter_list_remove_by_index(int index);
bt_status_t gap_filter_list_clear(void);
bt_status_t gap_filter_list_set_devices(const gap_filter_item_t *items, uint16_t count);
bt_status_t gap_filter_list_set_bonded_devices(void);
const gap_filter_item_t *gap_filter_list_get_item(int index);
bool gap_filter_list_user_item_exist(uint8_t user);
uint8_t gap_filter_list_curr_size(void);
uint8_t gap_filter_list_max_size(void);
bool gap_get_irk_distributed_bond_device(gap_bond_sec_t *out_record);

/**
 * Resovling List
 *
 */

typedef struct {
    bt_bdaddr_t peer_addr;
    bt_addr_type_t peer_type;
    uint8_t device_privacy_mode: 1;
    uint8_t is_inuse: 1;
    uint8_t peer_irk[16];
} gap_resolv_item_t;

bt_status_t gap_resolving_list_add(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, const uint8_t *peer_irk, bool device_privacy_mode, bool auto_restore_reso_enable, bool set_reso_enable);
bt_status_t gap_resolving_list_add_item(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, const uint8_t *peer_irk,
                                        const uint8_t *local_irk, bool device_privacy_mode, bool auto_restore_reso_enable, bool set_reso_enable);
bt_status_t gap_resolving_list_remove(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, bool auto_restore_reso_enable, bool set_reso_enable);
bt_status_t gap_resolving_list_remove_by_index(int index, bool auto_restore_reso_enable, bool set_reso_enable);
bt_status_t gap_enable_address_resolution(bool enable);
bt_status_t gap_resolving_list_clear(void);
bt_status_t gap_set_network_privacy_mode(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr);
bt_status_t gap_set_device_privacy_mode(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, bool auto_disable_reso_first, bool set_addr_reso_enable);
bt_status_t gap_set_all_network_privacy_mode(void);
bt_status_t gap_set_all_device_privacy_mode(void);
bt_status_t gap_read_peer_rpa(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, hci_cmd_evt_func_t cmd_cb, void *priv);
bt_status_t gap_read_local_rpa(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, hci_cmd_evt_func_t cmd_cb, void *priv);
bt_status_t gap_set_rpa_timeout(uint16_t timeout_sec);
const gap_resolv_item_t *gap_resolving_list_find_item(bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr);
const gap_resolv_item_t *gap_resolving_list_get_item(int index);
bool gap_address_reso_is_enabled(void);
bool gap_is_refresh_adv_after_addr_reso_enable(void);
uint8_t gap_resolving_list_curr_size(void);
uint8_t gap_resolving_list_max_size(void);

/**
 * BLE Advertising
 *
 */

#define GAP_DYN_ADV_HANDLE_START (0x10)
#define GAP_MAX_ADV_HANDLE_VALUE (0xEF)
#define GAP_MAX_ADV_ACTIVITY (5)

#define GAP_MAX_LEGACY_ADV_DATA_LEN 31
#define GAP_MAX_EXTEND_ADV_DATA_LEN 251

#define GAP_LE_LIMIT_ADV_TIMEOUT (30*1000) // max 180s, max advertising time in limited discoverable mode, required value

typedef enum {
    GAP_ADV_DATA      = 0x00,
    GAP_ADV_RSP_DATA,
    GAP_ADV_PER_DATA,
    GAP_ADV_UNKNOW_DATA,
} gap_adv_data_type_t;

typedef enum {
    GAP_ADV_ACCEPT_ALL_CONN_SCAN_REQS = 0x00,
    GAP_ADV_ACCEPT_ALL_CONN_REQS_BUT_ONLY_SCAN_REQS_IN_LIST = 0x01,
    GAP_ADV_ACCEPT_ALL_SCAN_REQS_BUT_ONLY_CONN_REQS_IN_LIST = 0x02,
    GAP_ADV_ACCEPT_ALL_CONN_SCAN_REQS_IN_LIST = 0x03,
} gap_adv_policy_t;

typedef enum {
    GAP_ADV_CHANNEL_UNKNOWN = 0x00,
    GAP_ADV_CHANNEL_37 = 0x01,
    GAP_ADV_CHANNEL_38 = 0x02,
    GAP_ADV_CHANNEL_39 = 0x04,
} gap_adv_channel_t;

typedef enum {
    GAP_ADV_DATA_INTER_FRAGMENT = 0x00,
    GAP_ADV_DATA_FIRST_FRAGMENT = 0x01,
    GAP_ADV_DATA_LAST_FRAGMENT = 0x02,
    GAP_ADV_DATA_COMPLETE = 0x03,
    GAP_ADV_DATA_UNCHANGED = 0x04, // just update Advertising DID of the adv
} gap_adv_data_op_t;

typedef enum {
    GAP_ADV_DISABLE_BY_NONE                     = 0x00,
    GAP_ADV_DISABLE_BY_APP_BLE                  = 0x01,
    GAP_ADV_DISABLE_BY_SEND_CMD_FAIL            = 0x02,
    GAP_ADV_DISABLE_BY_SET_PARAM_FAIL           = 0x03,
    GAP_ADV_DISABLE_BY_SEND_ENABLE_FAIL         = 0x04,
    GAP_ADV_DISABLE_BY_ADV_TIMEOUT              = 0x05,
    GAP_ADV_DISABLE_BY_BIG_CLEANUP              = 0x06,
    GAP_ADV_DISABLE_BY_BIG_DISABLE              = 0x07,
    GAP_ADV_DISABLE_ALL_BY_BLE_TEST             = 0x08,
    GAP_ADV_DISABLE_ALL_BY_UPPER_APP            = 0x09,
    GAP_ADV_DISABLE_ALL_BY_RESOLVE              = 0x0A,
    GAP_ADV_DISABLE_ALL_BY_FILTER               = 0x0B,
    GAP_ADV_DISABLE_LOW_PRIORITY_BUT_NOT_FREE   = 0x0C,
    GAP_ADV_DISABLE_OLD_PARAM_BUT_NOT_FREE      = 0x0D,
    GAP_ADV_TIMEOUT_AND_RESTART_NOT_FREE        = 0x0E,
    GAP_ADV_DISABLE_BY_LEGACY_CONN_COMPLETE     = 0x0F,
    GAP_ADV_DISABLE_BY_ADV_SET_TERMINATE        = 0x10,
    GAP_ADV_DISABLE_BY_DISABLE_COMPLETE         = 0x11,
} gap_who_disable_adv_t;

typedef struct {
    uint8_t adv_handle;
    bt_status_t error_code;
} gap_adv_started_t;

typedef struct {
    uint8_t adv_handle;
    bt_status_t error_code;
} gap_adv_stopped_t;

typedef struct {
    uint8_t adv_handle;
    uint8_t data_type;
    bt_status_t error_code;
} gap_adv_set_data_t;

typedef struct {
    uint8_t adv_handle;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
} gap_adv_scan_req_t;

typedef struct {
    uint8_t adv_handle;
    bt_status_t error_code;
} gap_pa_enabled_t;

typedef struct {
    uint8_t adv_handle;
    bt_status_t error_code;
} gap_pa_disabled_t;

typedef struct {
    uint8_t adv_handle;
    uint8_t subevent_start; // 0x00 to 0x7F, the first subevent that data is requested for
    uint8_t subevent_data_count; // 0x01 to 0x80, num of subevents that data is requested for
} gap_pa_subevent_data_req_t;

typedef gap_adv_report_t gap_pa_response_report_t;

typedef union {
    void *param_ptr;
    gap_adv_started_t *adv_started;
    gap_adv_stopped_t *adv_stopped;
    gap_adv_set_data_t *adv_set_data;
    gap_adv_scan_req_t *scan_req;
    gap_pa_enabled_t *pa_enabled;
    gap_pa_disabled_t *pa_disable;
    gap_pa_subevent_data_req_t *pa_subevent_data_req;
    gap_pa_response_report_t *pa_response_report;
    gap_conn_param_t *conn_opened;
    gap_conn_param_t *conn_closed;
    gap_conn_failed_t *conn_failed;
    gap_conn_cache_ind_t *conn_cache;
    gap_conn_mtu_exchanged_t *mtu_exchanged;
    gap_user_confirm_t *user_confirm;
    gap_conn_param_t *conn_encrypted;
    gap_recv_key_dist_t *recv_key_dist;
    gap_conn_update_req_t *conn_update_req;
    gap_conn_param_t *conn_params_update;
    gap_conn_param_t *conn_subrate_change;
    gap_conn_phy_update_t *conn_phy_update;
    gap_le_tx_power_report_t *tx_power;
    gap_recv_key_material_t *recv_key_material;
    gap_recv_sec_request_t *recv_security_req;
    gap_recv_smp_requirements_t *smp_requirements;
    gap_smp_pairing_cmp_t *pairing_cmp;
} gap_adv_callback_param_t;

typedef enum {
    GAP_ADV_EVENT_STARTED = BT_EVENT_GAP_ADV_EVENT_START,
    GAP_ADV_EVENT_STOPPED,
    GAP_ADV_EVENT_SET_DATA,
    GAP_ADV_EVENT_SCAN_REQ,
    GAP_ADV_EVENT_PA_ENABLED,
    GAP_ADV_EVENT_PA_DISABLED,
    GAP_ADV_EVENT_PA_SUBEVENT_DATA_REQ,
    GAP_ADV_EVENT_PA_RESPONSE_REPORT,
    GAP_ADV_EVENT_CONN_OPENED = BT_EVENT_GAP_CONN_EVENT_START,
    GAP_ADV_EVENT_CONN_CLOSED,
    GAP_ADV_EVENT_CONN_FAILED,
    GAP_ADV_EVENT_CONN_CACHE,
    GAP_ADV_EVENT_CONN_MTU_EXCHANGED,
    GAP_ADV_EVENT_USER_CONFIRM,
    GAP_ADV_EVENT_CONN_ENCRYPTED,
    GAP_ADV_EVENT_RECV_KEY_DIST,
    GAP_ADV_EVENT_CONN_UPDATE_REQ,
    GAP_ADV_EVENT_CONN_PARAMS_UPDATE,
    GAP_ADV_EVENT_CONN_SUBRATE_CHANGE,
    GAP_ADV_EVENT_CONN_PHY_UPDATE,
    GAP_ADV_EVENT_TX_POWER_REPORT,
    GAP_ADV_EVENT_RECV_KEY_METERIAL,
    GAP_ADV_EVENT_RECV_SEC_REQUEST,
    GAP_ADV_EVENT_RECV_SMP_REQUIRE,
    GAP_ADV_EVENT_PAIRING_COMPLETE,
} gap_adv_event_t;

typedef int (*gap_adv_callback_t)(uintptr_t adv, gap_adv_event_t event, gap_adv_callback_param_t param);

typedef struct {
    /**
     * Recommended advertising interval for user initiated connectable modes:
     *      connectable undirected mode
     *      limited discoverable and connectable undirected mode
     *      general discoverable and connectable undirected mode
     *      connectable directed low duty cycle mode
     */
    uint32_t min_adv_fast_interval_ms; // LE 1M PHY
    uint32_t max_adv_fast_interval_ms; // LE 1M PHY
    uint32_t min_adv_fast_interval_coded_ms; // LE Coded PHY
    uint32_t max_adv_fast_interval_coded_ms; // LE Coded PHY
    /**
     * Recommended advertising interval for user initiated non-connectable modes:
     *      non-discoverable mode
     *      non-connectable mode
     *      limited discoverable mode
     *      general discoverable mode
     * Note: When advertising interval values of less than 100ms are used for
     * non-connectable or scannable undirected advertising in environments where
     * the advertiser can interfere with other devices, it is recommended that
     * steps be taken to minimize the interference. For example, the advertising
     * might be alternately enabled for only a few seconds and disabled for
     * several minutes.
     */
    uint32_t min_adv_slow_interval_ms; // LE 1M PHY
    uint32_t max_adv_slow_interval_ms; // LE 1M PHY
    uint32_t min_adv_slow_interval_coded_ms; // LE Coded PHY
    uint32_t max_adv_slow_interval_coded_ms; // LE Coded PHY
    /**
     * Recommended advertising interval for background advertising other then Directed Connectable High Duty Mode:
     */
    uint32_t min_adv_bg_interval_ms; // LE 1M PHY
    uint32_t max_adv_bg_interval_ms; // LE 1M PHY
    uint32_t min_adv_bg_interval_coded_ms; // LE Coded PHY
    uint32_t max_adv_bg_interval_coded_ms; // LE Coded PHY
} gap_adv_timing_t;

typedef struct {
    bool force_start;
    bool connectable;
    bool scannable;
    bool directed_adv;
    bool anonymous_adv;
    bool high_duty_directed_adv; // <= 3.75ms adv interval
    bool use_legacy_pdu;
    bool include_tx_power_data; // include TxPower in the ext header of at least one adv pdu
    bool own_addr_use_rpa;
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    gap_adv_policy_t policy;
    bool use_custom_local_addr;
    bool use_fake_btc_rpa_when_no_irk_exist;
    bt_bdaddr_t custom_local_addr;
    bool limited_discoverable_mode;
    bool fast_advertising;
    bool continue_advertising;
    bool start_bg_advertising;
    bool has_prefer_adv_tx_power;
    bool has_custom_adv_timing;
    bool enable_scan_req_notify;
    int8_t adv_tx_power;
    uint8_t activity_priority;
    uint8_t adv_channel_map;
    uint8_t secondary_adv_max_skip;
    gap_le_phy_t primary_adv_phy;
    gap_coded_phy_prefer_t primary_coded_phy_prefer;
    gap_le_phy_t secondary_adv_phy;
    gap_coded_phy_prefer_t secondary_coded_phy_prefer;
    gap_adv_timing_t adv_timing;
    gap_dt_buf_t adv_data;
    gap_dt_buf_t scan_rsp_data;
    uint32_t duration_ms;
} gap_adv_param_t;

typedef struct {
    uint32_t adv_interval_min;
    uint32_t adv_interval_max;
    uint16_t adv_event_props;
    uint8_t own_addr_type;
    uint8_t peer_type;
    uint8_t adv_channel_map;
    uint8_t adv_filter_policy;
    bt_bdaddr_t local_addr;
    bt_bdaddr_t peer_addr;
    int8_t adv_tx_power;
    uint8_t primary_adv_phy;
    uint8_t primary_coded_phy_prefer;
    uint8_t secondary_adv_phy;
    uint8_t secondary_coded_phy_prefer;
    uint8_t secondary_adv_max_skip;
    uint8_t scan_req_notify_enable;
} gap_adv_set_param_t;

typedef struct {
    bool adv_params_set;
    bool adv_enabled;
    uint8_t adv_set_id;
    uint8_t adv_handle;
    bool legacy_advertising;
    int8_t selected_tx_power;
    bool fast_advertising;
    bool continue_advertising;
    bool start_bg_advertising;
    bool directed_advertising;
    uint8_t activity_priority;
    uint8_t adv_stop_timer;
    bool pa_is_enabled;
    uint32_t duration_ms;
    gap_dt_buf_t adv_data;
    gap_dt_buf_t scan_rsp_data;
    gap_adv_callback_t adv_callback;
    gap_adv_set_param_t set_param;
    gap_adv_timing_t timing_cfg;
} gap_advertising_t;

uint8_t gap_set_adv_parameters(const gap_adv_param_t *param, gap_adv_callback_t cb);
void gap_set_advertising(uint8_t adv_handle, const gap_adv_param_t *param, gap_adv_callback_t cb);
void gap_refresh_advertising(uint8_t adv_handle, const gap_adv_param_t *param, gap_adv_callback_t cb);
uint8_t gap_legacy_non_discoverable_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_legacy_directed_advertising(bt_addr_type_t own_addr_type, bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, gap_adv_callback_t cb);
uint8_t gap_legacy_connectable_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_legacy_scannable_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_legacy_connectable_filter_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_legacy_scannable_filter_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_start_non_discoverable_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_start_directed_advertising(bt_addr_type_t own_addr_type, bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, gap_adv_callback_t cb);
uint8_t gap_start_connectable_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_start_scannable_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_start_connectable_filter_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
uint8_t gap_start_scannable_filter_advertising(bt_addr_type_t own_addr_type, bool continue_advertising, gap_adv_callback_t cb);
bt_status_t gap_set_adv_random_address(uint8_t adv_handle, const bt_bdaddr_t *random);
bt_status_t gap_set_adv_data_unchanged(uint8_t adv_handle); // unchanged data, just update the Advertising DID
bt_status_t gap_set_adv_data(uint8_t adv_handle, const uint8_t *data, uint16_t len);
bt_status_t gap_set_scan_rsp_data(uint8_t adv_handle, const uint8_t *data, uint16_t len);
bt_status_t gap_set_ext_adv_data(uint8_t adv_handle, gap_adv_data_op_t op, const uint8_t *data, uint16_t len);
bt_status_t gap_set_ext_scan_rsp_data(uint8_t adv_handle, gap_adv_data_op_t op, const uint8_t *data, uint16_t len);
bt_status_t gap_set_ext_adv_enable(uint8_t adv_handle, uint32_t duration_ms, uint8_t max_ext_adv_events);
bt_status_t gap_enable_advertising(uint8_t adv_handle, uint32_t duration_ms);
bt_status_t gap_disable_advertising(uint8_t adv_handle, gap_who_disable_adv_t who);
bt_status_t gap_disable_all_advertising(gap_who_disable_adv_t who);
bt_status_t gap_update_advertising_rpa_when(uint8_t adv_handle, bool adv_data_change, bool scan_rsp_change);
gap_advertising_t *gap_get_advertising_set(uint8_t adv_handle);
bool gap_is_advertising_enabled(uint8_t adv_handle);
gap_adv_timing_t gap_default_adv_timing(void);

/**
 * BLE Scanning
 *
 */

typedef enum {
    GAP_DUP_FILTER_DISABLE = 0,
    GAP_DUP_FILTER_ENABLED = 1,
    GAP_DUP_FILTER_ENABLED_AND_RESET_ON_EACH_PERIOD = 2,
} gap_dup_filter_op_t;

typedef struct {
    bt_status_t error_code;
} gap_scan_started_t;

typedef struct {
    bt_status_t error_code;
} gap_scan_stopped_t;

typedef struct {
    uint16_t sync_handle; // 0x0000 to 0x0EFE, sync_handle identifying the pa train
    uint8_t num_bis; // 0x01 to 0x1F, value of the Num_BIS subfield of the BIGInfo field
    uint8_t nse; // 0x01 to 0x1F, value of the NSE subfield of the BIGInfo field
    uint16_t iso_interval; // iso_interval subfield of the BIGInfo field
    uint8_t bn; // 0x01 to 0x07, BN subfield of the BIGInfo field
    uint8_t pto; // 0x00 to 0x0F, pre-transmission offset, PTO subfield of the BIGInfo field
    uint8_t irc; // 0x01 to 0x0F, IRC subfield of the BIGInfo field
    uint16_t max_pdu; // 0x01 to 0xFB, max_pdu subfiled of the BIGInfo
    uint32_t sdu_interval; // sdu_interval subfield of the BIGInfo field
    uint16_t max_sdu; // 0x01 to 0x0FFF, max_sdu subfield of the BIGInfo
    gap_le_phy_t big_phy; // 0x01 the BIG is transmitted on the LE 1M PHY, 0x02 LE 2M, 0x03 LE Coded
    uint8_t framed; // 0x00 unframed 0x01 framed
    uint8_t encrypted; // 0x00 BIG carries unencrypted data, 0x01 BIG carries encrypted data
} gap_big_info_adv_report_t;

typedef struct gap_pa_sync_establish_t gap_pa_sync_establish_t;
typedef struct gap_pa_sync_terminated_t gap_pa_sync_terminated_t;

typedef union {
    void *param_ptr;
    gap_scan_started_t *scan_started;
    gap_scan_stopped_t *scan_stopped;
    const gap_adv_report_t *adv_report;
    const gap_pa_sync_establish_t *pa_sync_estb;
    const gap_pa_sync_terminated_t *pa_sync_term;
    const gap_adv_report_t *per_adv_report;
    const gap_big_info_adv_report_t *big_info_report;
} gap_scan_callback_param_t;

typedef enum {
    GAP_SCAN_EVENT_STARTED = BT_EVENT_GAP_SCAN_EVENT_START,
    GAP_SCAN_EVENT_STOPPED,
    GAP_SCAN_EVENT_ADV_REPORT,
    GAP_SCAN_EVENT_PA_SYNC_ESTABLISHED,
    GAP_SCAN_EVENT_PA_SYNC_TERMINATED,
    GAP_SCAN_EVENT_PA_REPORT,
    GAP_SCAN_EVENT_BIG_INFO_REPORT,
} gap_scan_event_t;

typedef int (*gap_scan_callback_t)(uintptr_t scan, gap_scan_event_t event, gap_scan_callback_param_t param);

typedef struct {
    // user initiated discovery or connection establishment
    uint16_t fg_scan_interval_ms;               // default 30ms~60ms    LE 1M PHY
    uint16_t fg_scan_window_ms;                 // default 30ms         LE 1M PHY
    uint16_t fg_scan_interval_coded_ms;         // default 90ms~180ms   LE Coded PHY
    uint16_t fg_scan_window_coded_ms;           // default 90ms         LE Coded PHY
    uint16_t fg_slow_scan_interval_ms;          // default 30ms~60ms    LE 1M PHY
    uint16_t fg_slow_scan_window_ms;            // default 30ms         LE 1M PHY
    uint16_t fg_slow_scan_interval_coded_ms;    // default 90ms~180ms   LE Coded PHY
    uint16_t fg_slow_scan_window_coded_ms;      // default 90ms         LE Coded PHY
    uint32_t fg_scan_time_ms;                   // default 30.72s
    // background discovery or connection establishment
    uint16_t bg_scan_interval_ms;               // default 1.28s        LE 1M PHY
    uint16_t bg_scan_window_ms;                 // default 11.25ms      LE 1M PHY
    uint16_t bg_scan_interval_coded_ms;         // default 3.84s        LE Coded PHY
    uint16_t bg_scan_window_coded_ms;           // default 33.75ms      LE Coded PHY
    // slow discovery or connection establishment
    uint16_t bg_slow_scan_interval_ms;          // default 2.56s        LE 1M PHY
    uint16_t bg_slow_scan_window_ms;            // default 22.5ms       LE 1M PHY
    uint16_t bg_slow_scan_interval_coded_ms;    // default 7.68s        LE Coded PHY
    uint16_t bg_slow_scan_window_coded_ms;      // default 67.5ms       LE Coded PHY
} gap_scan_timing_t;

typedef struct {
    bt_addr_type_t own_addr_type;
    bool use_filter_list;
    bool active_scan;
    bool filter_duplicated;
    bool slow_scan;
    bool continue_scanning;
    bool has_custom_scan_timing;
    bool dont_auto_start;
    bool legacy;
    uint8_t phys;
    gap_scan_timing_t scan_timing;
    uint32_t duration_ms; // 0x01 to 0xFFFF * 10ms, 10ms to 655.35s (655350ms)
} gap_scan_param_t;

bt_status_t gap_start_scanning(const gap_scan_param_t *param, gap_scan_callback_t cb);
bt_status_t gap_start_background_scanning(const gap_scan_param_t *param, gap_scan_callback_t cb);
bt_status_t gap_start_discovery(bt_addr_type_t own_addr_type, bool slow_scan, gap_scan_callback_t cb);
bt_status_t gap_start_background_discovery(bt_addr_type_t own_addr_type, bool slow_scan, gap_scan_callback_t cb);
bt_status_t gap_set_ext_scan_enable(gap_dup_filter_op_t filter_duplicates, uint32_t duration_ms, uint32_t period_ms);
bt_status_t gap_enable_scanning(bool filter_duplicates, uint32_t duration_ms);
bt_status_t gap_disable_scanning(void);
gap_scan_timing_t gap_default_scan_timing(void);

/**
 * BLE Initiating
 *
 */

typedef struct {
    bt_status_t error_code;
} gap_init_started_t;

typedef struct {
    bt_status_t error_code;
} gap_init_stopped_t;

typedef union {
    void *param_ptr;
    gap_scan_started_t *scan_started;
    gap_scan_stopped_t *scan_stopped;
    const gap_adv_report_t *adv_report;
    gap_init_started_t *init_started;
    gap_init_stopped_t *init_stopped;
    gap_conn_param_t *conn_opened;
    gap_conn_param_t *conn_closed;
    gap_conn_failed_t *conn_failed;
    gap_conn_cache_ind_t *conn_cache;
    gap_conn_mtu_exchanged_t *mtu_exchanged;
    gap_user_confirm_t *user_confirm;
    gap_conn_param_t *conn_encrypted;
    gap_recv_key_dist_t *recv_key_dist;
    gap_conn_update_req_t *conn_update_req;
    gap_conn_param_t *conn_params_update;
    gap_conn_param_t *conn_subrate_change;
    gap_conn_phy_update_t *conn_phy_update;
    gap_le_tx_power_report_t *tx_power;
    gap_recv_key_material_t *recv_key_material;
    gap_recv_sec_request_t *recv_security_req;
    gap_recv_smp_requirements_t *smp_requirements;
    gap_smp_pairing_cmp_t *pairing_cmp;
} gap_init_callback_param_t;

typedef enum {
    GAP_INIT_EVENT_SCAN_STARTED = BT_EVENT_GAP_SCAN_EVENT_START,
    GAP_INIT_EVENT_SCAN_STOPPED,
    GAP_INIT_EVENT_SCAN_ADV_REPORT,
    GAP_INIT_EVENT_INIT_STARTED, // initiating started to start creating connection
    GAP_INIT_EVENT_INIT_STOPPED, // initiating stopped due to conn established or failed
    GAP_INIT_EVENT_CONN_OPENED = BT_EVENT_GAP_CONN_EVENT_START,
    GAP_INIT_EVENT_CONN_CLOSED,
    GAP_INIT_EVENT_CONN_FAILED,
    GAP_INIT_EVENT_CONN_CACHE,
    GAP_INIT_EVENT_CONN_MTU_EXCHANGED,
    GAP_INIT_EVENT_USER_CONFIRM,
    GAP_INIT_EVENT_CONN_ENCRYPTED,
    GAP_INIT_EVENT_RECV_KEY_DIST,
    GAP_INIT_EVENT_CONN_UPDATE_REQ,
    GAP_INIT_EVENT_CONN_PARMS_UPDATE,
    GAP_INIT_EVENT_CONN_SUBRATE_CHANGE,
    GAP_INIT_EVENT_CONN_PHY_UPDATE,
    GAP_INIT_EVENT_TX_POWER_REPORT,
    GAP_INIT_EVENT_RECV_KEY_METERIAL,
    GAP_INIT_EVENT_RECV_SEC_REQUEST,
    GAP_INIT_EVENT_RECV_SMP_REQUIRE,
    GAP_INIT_EVENT_PAIRING_COMPLETE,
} gap_init_event_t;

typedef int (*gap_init_callback_t)(uintptr_t init, gap_init_event_t event, gap_init_callback_param_t param);

typedef struct {
    uint16_t min_initial_conn_interval_1_25ms;  // default 30~50ms  LE 1M PHY
    uint16_t max_initial_conn_interval_1_25ms;  // default 30~50ms  LE 1M PHY
    uint16_t min_initial_conn_interval_coded;   // default 90~150ms LE Coded PHY
    uint16_t max_initial_conn_interval_coded;   // default 90~150ms LE Coded PHY
    uint16_t initial_superv_timeout_ms;         // 100ms to 32s
    uint16_t initial_min_ce_length_slots;       // min len of connection event, 0x00 to 0xFFFF * 0.625ms
    uint16_t initial_max_ce_length_slots;       // max len of connection event, 0x00 to 0xFFFF * 0.625ms
    uint32_t init_proc_timeout_ms;              // default 30.72s, bap 30s
} gap_init_timing_t;

typedef struct {
    bt_addr_type_t own_addr_type;
    bt_addr_type_t peer_type;
    bt_bdaddr_t peer_addr;
    bool use_filter_list;
    bool continue_initiating;
    bool has_custom_scan_timing;
    bool has_custom_init_timing;
    gap_scan_timing_t scan_timing; // initiator use scan_timing to do initiating scan
    gap_init_timing_t init_timing; // init_timing is the initial ble connection parameers
} gap_init_param_t;

bt_status_t gap_start_general_initiating(bt_addr_type_t own_addr_type, gap_init_callback_t cb);
bt_status_t gap_start_direct_initiating(bt_addr_type_t own_addr_type, bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, gap_init_callback_t cb);
bt_status_t gap_connect_ble_audio_device(bt_addr_type_t own_addr_type, bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr, gap_init_callback_t cb);
bt_status_t gap_start_auto_initiating(const gap_init_param_t *param, gap_init_callback_t cb);
bt_status_t gap_start_initiating(const gap_init_param_t *param, gap_init_callback_t cb);
bt_status_t gap_cancel_initiating(void);
bt_status_t gap_cancel_all_pend_initiating(bool also_cancel_curr_initiating);
gap_init_timing_t gap_default_init_timing(void);

/**
 * PA Source - Advertiser
 *
 */

typedef struct {
    uint8_t num_subevents; // 0x00 to 0x80
    uint8_t subevent_interval_1_25ms; // 0x06 to 0xFF * 1.25ms (7.5ms to 318.75ms)
    uint8_t response_slot_delay_1_25ms; // 0x00 no response slots, 0xXX 0x01 to 0xFE * 1.25ms (1.25ms to 317.5ms) time between the adv packet in a subevent and the 1st response slot
    uint8_t response_slot_spacing_0_125ms; // 0x00 no response slots, 0xXX 0x02 to 0xFF * 0x125ms (0x25ms to 31.875ms) time between response slots
    uint8_t num_response_slots; // 0x00 no response slots, 0xXX 0x01 to 0xFF num of subevent response slots
} gap_pa_subevent_param_t;

typedef struct {
    uint8_t subevent; // 0x00 to 0x7F subevent index
    uint8_t response_slot_start; // the first response slot to be used in the subevent
    uint8_t response_slot_count; // num of response slots to be used
    uint8_t subevent_data_len; // 0 to 251
    const uint8_t *subevent_data; // ad type formatted subevent data
} gap_pa_subevent_data_t;

bt_status_t gap_set_pa_parameters(uint8_t adv_handle, uint16_t pa_interval_1_25ms, bool include_tx_power, const gap_pa_subevent_param_t *param);
bt_status_t gap_set_pa_data(uint8_t adv_handle, gap_adv_data_op_t op, const uint8_t *pa_data, uint8_t data_len);
bt_status_t gap_set_pa_enable(uint8_t adv_handle, bool enable, bool include_adi_field);
bt_status_t gap_set_pa_subevent_data(uint8_t adv_handle, uint8_t num_subevents, const gap_pa_subevent_data_t *data);
bt_status_t gap_advertiser_start_pawr_initiating(uint8_t adv_handle, uint8_t subevent, bt_addr_type_t peer_type, const bt_bdaddr_t *peer_addr);

/**
 * PA Sink - Self Create Sync
 *
 */

typedef struct gap_pa_sync_establish_t {
    bt_status_t error_code;
    uint16_t pa_sync_hdl;
    bool is_over_past;
    uint16_t acl_conn_hdl;
    uint16_t service_data;
    uint8_t adv_sid;
    uint8_t adv_addr_type; // 0x00 public 0x01 random 0x02 public identity 0x03 random static identity
    bt_bdaddr_t adv_addr;
    uint8_t adv_phy; // 0x01 le 1m, 0x02 le 2m, 0x03 le coded
    uint16_t pa_interval_1_25ms; // 0x06 to 0xFFFF * 1.25ms (7.5ms to 81.9s)
    uint8_t adv_clock_accuracy; // 0x00 to 0x07 500ppm to 20ppm
    uint8_t num_subevents; // 0x00 no subevents, 0x01 to 0x80 num of subevents
    uint8_t subevent_interval_1_25ms; // 0x00 no subevents, 0x06 to 0xFF * 1.25ms (7.5ms to 318.75ms)
    uint8_t response_slot_delay_1_25ms; // 0x00 no response slots, 0x01 to 0xFE * 1.25ms (1.25ms to 317.6ms)
    uint8_t response_slot_spacing_0_125ms; // 0x00 no response slots, 0x02 to 0xFF * 0.125ms (0.25ms to 31.875ms)
} gap_pa_sync_establish_t;

typedef struct gap_pa_sync_terminated_t {
    uint16_t pa_sync_hdl;
    uint8_t reason;
} gap_pa_sync_terminated_t;

typedef gap_scan_event_t gap_pa_sync_event_t;
typedef gap_scan_callback_param_t gap_pa_sync_callback_param_t;
typedef gap_scan_callback_t gap_pa_sync_callback_t;

typedef struct {
    uint8_t options;
    uint8_t adv_set_id;
    bt_addr_type_t adv_addr_type;
    bt_bdaddr_t adv_addr;
    uint16_t skip;
    uint16_t sync_timeout_10ms;
    uint8_t sync_cte_type;
} gap_pa_sync_param_t;

typedef struct {
    uint8_t response_subevent; // 0x00 to 0x7F identify the subevent of PAwR train to response
    uint8_t response_slot; // 0x00 to 0xFF identify the response slot of the PAwR train
    uint8_t response_data_len; // 0 to 251
    const uint8_t *response_data; // ad type formatted response data
} gap_pa_response_data_t;

bt_status_t gap_pa_list_add(bt_addr_type_t adv_addr_type, const bt_bdaddr_t *adv_addr, uint8_t adv_set_id);
bt_status_t gap_pa_list_remove(bt_addr_type_t adv_addr_type, const bt_bdaddr_t *adv_addr, uint8_t adv_set_id);
bt_status_t gap_pa_list_clear(void);
uint8_t gap_pa_list_max_size(void);

bt_status_t gap_pa_create_sync(const gap_pa_sync_param_t *p_param, const gap_pa_sync_callback_t p_cb);
bt_status_t gap_pa_create_sync_cancel(void);
bt_status_t gap_pa_terminate_sync(uint16_t pa_sync_hdl);
bt_status_t gap_set_pa_receive_enable(uint16_t pa_sync_hdl, bool enable, bool filter_duplicated);
bt_status_t gap_set_pa_sync_subevent(uint16_t pa_sync_hdl, bool include_tx_power, uint8_t num_subevents, const uint8_t *subevent);
bt_status_t gap_set_pa_response_data(uint16_t pa_sync_hdl, uint16_t pa_event_counter, uint8_t subevent, const gap_pa_response_data_t *data);

/**
 * PA Sink - Sync from PAST
 *
 */

bt_status_t gap_pa_sync_transfer(uint8_t con_idx, uint16_t service_data, uint16_t pa_sync_hdl);
bt_status_t gap_pa_set_info_transfer(uint8_t con_idx, uint16_t service_data, uint8_t adv_hdl);
bt_status_t gap_register_past_received_callback(const gap_pa_sync_callback_t p_cb);
bt_status_t gap_set_past_parameters(uint8_t con_idx, uint8_t mode, uint16_t skip, uint16_t sync_to_10ms, uint8_t cte_type);
bt_status_t gap_set_default_past_parameters(uint8_t mode, uint16_t skip, uint16_t sync_to_10ms, uint8_t cte_type);

typedef struct {
    gap_dt_encrypt_callback_t func;
    void *priv;
} gap_enc_data_env_t;

typedef struct {
    bt_bdaddr_t local_rpa;
    void (*cmpl)(void *priv, const bt_bdaddr_t *local_rpa);
    void *priv;
} gap_gen_rpa_t;

typedef struct {
    gap_bond_sec_t record;
    bt_bdaddr_t peer_rpa;
    uint8_t record_idx;
    void (*cmpl)(void *priv, const gap_bond_sec_t *out);
    void *priv;
} gap_resolve_rpa_t;

typedef struct {
    bool scan_enabled;
    bool legacy_scanning;
    bool background_scan;
    bool initiating_scan;
    bt_addr_type_t own_addr_type;
    uint8_t scan_handle;
    uint8_t scan_stop_timer;
    uint16_t period_1_28s;
    uint32_t duration_ms;
    uint16_t scan_interval_slots;
    uint16_t scan_interval_coded;
    uint16_t scan_window_slots;
    uint16_t scan_window_coded;
    gap_scan_param_t param;
    gap_scan_callback_t scan_callback;
} gap_scanning_t;

typedef struct {
    bool legacy_initiating;
    bool is_conn_creating;
    bool is_direct_initiating;
    uint8_t peer_type;
    bt_addr_type_t own_addr_type;
    bt_bdaddr_t peer_addr;
    uint32_t init_proc_timeout_ms;
    bool timeout_and_restart_initiating;
    bool start_bg_initiating;
    bool peer_is_ble_audio;
    uint8_t init_handle;
    uint8_t init_stop_timer;
    uint8_t pa_adv_handle;
    uint8_t sd_subevent;
    uint16_t scan_interval_slots;
    uint16_t scan_interval_coded;
    uint16_t scan_window_slots;
    uint16_t scan_window_coded;
    uint16_t min_conn_interval_1_25ms;  // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4s
    uint16_t max_conn_interval_1_25ms;  // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4s
    uint16_t min_conn_interval_coded;   // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4s
    uint16_t max_conn_interval_coded;   // 0x06 to 0x0C80 * 1.25ms, 7.5ms to 4s
    uint16_t superv_timeout_ms;         // 100ms to 32s
    uint16_t min_ce_length_slots;       // min len of connection event, 0x00 to 0xFFFF * 0.625ms
    uint16_t max_ce_length_slots;       // max len of connection event, 0x00 to 0xFFFF * 0.625ms
    gap_init_callback_t init_callback;
    gap_init_param_t init_param;
} gap_initiating_t;

typedef struct {
    gap_advertising_t *adv;
} gap_advertising_item_t;

typedef struct {
    bool phy_update; // false indicate host read
    gap_le_phy_t tx_phy;
    gap_le_phy_t rx_phy;
} gap_conn_phy_param_t;

typedef struct {
    /// PA sync handle
    uint16_t pa_sync_hdl;
    /// Periodic Adv Timing
    uint8_t adv_phy; // 0x01 le 1m, 0x02 le 2m, 0x03 le coded
    uint16_t pa_interval_1_25ms; // 0x06 to 0xFFFF * 1.25ms (7.5ms to 81.9s)
    uint8_t adv_clock_accuracy; // 0x00 to 0x07 500ppm to 20ppm
    uint8_t num_subevents; // 0x00 no subevents, 0x01 to 0x80 num of subevents
    uint8_t subevent_interval_1_25ms; // 0x00 no subevents, 0x06 to 0xFF * 1.25ms (7.5ms to 318.75ms)
    uint8_t response_slot_delay_1_25ms; // 0x00 no response slots, 0x01 to 0xFE * 1.25ms (1.25ms to 317.6ms)
    uint8_t response_slot_spacing_0_125ms; // 0x00 no response slots, 0x02 to 0xFF * 0.125ms (0.25ms to 31.875ms)
    /// Sync param from upper
    gap_pa_sync_param_t pa_sync_param;
    gap_pa_sync_callback_t pa_sync_callback;
} gap_pa_sync_t;

typedef struct {
    gap_pa_sync_t *pa_sync;
} gap_pa_sync_item_t;

#define GAP_DEFAULT_SHORT_NAME_LEN (20) // max value is GAP_MAX_DEVICE_NAME_LEN 248
#define GAP_MAX_ATT_PREP_Q_SIZE (16)
#define GAP_MAX_FILTER_LIST_SIZE (10)
#define GAP_MAX_RESOLV_LIST_SIZE (10)
#define GAP_MAX_NUM_PA_SYNC_LIST (8)
#define GAP_MAX_PEND_BLE_INIT (5)
#define GAP_MAX_ADV_PARAM_FOR_CONN (GAP_MAX_ADV_ACTIVITY+2)

typedef struct {
    uint32_t adv_seqn;
    uint8_t adv_handle;
    uint8_t own_addr_type: 2;
    uint8_t directed_advertising: 1;
    bt_bdaddr_t local_addr;
    gap_adv_callback_t adv_callback;
} __attribute__ ((packed)) gap_adv_param_for_conn_t;

typedef struct {
    uint8_t is_initialized;
    uint8_t adv_handle_seed;
    uint8_t scan_handle_seed;
    uint8_t init_handle_seed;
    uint8_t resolv_list_size;
    uint8_t filter_list_size;
    uint8_t le_acl_count;
    uint8_t bt_acl_count;
    uint8_t rpa_timer_id;
    uint8_t min_conn_interval_1_25ms;
    uint8_t pts_default_enc_key_size;
    uint8_t pts_default_auth_req;
    uint32_t adv_seqn_seed;
    uint16_t rpa_timeout_sec;
    uint16_t ble_stack_is_ready: 1;
    uint16_t addr_reso_switch_on: 1;
    uint16_t refresh_adv_after_addr_reso_enable: 1;
    uint16_t send_sec_error_rsp_directly: 1;
    uint16_t pts_dont_start_smp: 1;
    uint16_t pts_set_use_passkey: 1;
    uint16_t pts_set_use_oob_method: 1;
    uint16_t pts_set_no_mitm_auth: 1;
    uint16_t pts_set_display_only: 1;
    uint16_t pts_set_keyboard_only: 1;
    uint16_t pts_set_no_bonding: 1;
    uint16_t pts_gen_link_key_from_ltk: 1;
    uint16_t pts_set_dist_irk_only: 1;
    uint16_t pts_set_dist_csrk: 1;
    uint16_t pts_ble_l2cap_test: 1;
    gap_global_callback_t global_callback;
    const gap_ext_func_cbs_t *p_external_func_cbs;
    gap_local_info_t local;
    gap_scanning_t le_scan;
    gap_initiating_t le_init;
    gap_initiating_t le_pend_init[GAP_MAX_PEND_BLE_INIT];
    gap_advertising_item_t legacy_adv;
    gap_advertising_item_t ext_adv[GAP_MAX_ADV_ACTIVITY];
    gap_adv_param_for_conn_t adv_param_for_conn[GAP_MAX_ADV_PARAM_FOR_CONN];
    gap_resolv_item_t resolv_item[GAP_MAX_RESOLV_LIST_SIZE];
    gap_filter_item_t filter_item[GAP_MAX_FILTER_LIST_SIZE];
    gap_pa_sync_item_t pa_sync[GAP_MAX_NUM_PA_SYNC_LIST];
    gap_pa_sync_callback_t past_received_cb;
} gap_global_t;

/**
 * bes tws fast pair host support apis
 * usage: set enable before le adv/scan
*/
void gap_vendor_enable_besfp(uint8_t *tws_bt_addr, uint8_t *peer_le_addr);
void gap_vendor_disable_besfp();
bool gap_vendor_is_besfp_enabled();
uint8_t *gap_vender_besfp_get_tws_bt_addr();
uint8_t *gap_vender_besfp_get_peer_le_addr();

#ifdef __cplusplus
}
#endif
#endif /* __BT_GAP_SERVICE_H__ */

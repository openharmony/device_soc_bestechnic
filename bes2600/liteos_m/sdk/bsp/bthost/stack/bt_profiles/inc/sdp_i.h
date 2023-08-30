/***************************************************************************
 *
 * Copyright 2015-2019 BES.
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
#ifndef __SDP_I_H__
#define __SDP_I_H__
#include "bluetooth.h"
#include "l2cap_i.h"
#include "sdp_service.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SDP_REQUEST_DATA_MAX_LEN (512)

enum sdp_error_code {
    SDP_Reserved_for_future_use = 0x0000,
    SDP_Invalid_SDP_version = 0x0001,
    SDP_Invalid_Service_Record_Handle = 0x0002,
    SDP_Invalid_request_syntax = 0x0003,
    SDP_Invalid_PDU_Size = 0x0004,
    SDP_Invalid_Continuation_State = 0x0005,
    SDP_Insufficient_Resources_to_satisfy_Request = 0x0006,
};

enum sdp_pdu_id {
    SDP_PDU_ErrorResponse = 0x01,
    SDP_PDU_ServiceSearchRequest = 0x02,
    SDP_PDU_ServiceSearchResponse = 0x03,
    SDP_PDU_ServiceAttributeRequest = 0x04,
    SDP_PDU_ServiceAttributeResponse = 0x05,
    SDP_PDU_ServiceSearchAttributeRequest = 0x06,
    SDP_PDU_ServiceSearchAttributeResponse = 0x07,
};

struct sdp_control_t;
void sdp_global_init(void);
bt_status_t sdp_close(struct sdp_control_t *sdp_chan);
bt_status_t sdp_client_service_search(const bt_bdaddr_t *remote, const bt_sdp_service_search_t *param);
bt_status_t sdp_client_request(const bt_bdaddr_t *remote, bt_sdp_uuid_search_t *param);
bt_status_t sdp_client_queue_request(const bt_bdaddr_t *remote, bt_sdp_service_uuid_t uuid);
bt_status_t sdp_create_record(const bt_sdp_record_attr_t *attr_list, int attr_count);
bt_status_t sdp_remove_record(const bt_sdp_record_attr_t *attr_list);
struct sdp_global_t *sdp_get_global(void);
struct sdp_proto_conn_t *sdp_get_proto_conn(const struct bdaddr_t *remote);

typedef struct {
    const uint8_t *attr_value_ptr; // attribute_value element start
    enum data_element_type elem_type; // attribute_value element type
    uint16_t elem_header_len; // attribute_value element header len
    uint16_t elem_data_len; // attribute_value element data len
} bt_sdp_attr_value_t;

typedef struct {
    const uint8_t *uuid_matched_end;
    const uint8_t *attr_value_ptr; // attribute_value element start
    enum data_element_type elem_type; // attribute_value element type
    uint16_t elem_header_len; // attribute_value element header len
    uint16_t elem_data_len; // attribute_value element data len
    uint16_t remain_data_len; // remain len including uuid attached value and left value in curr deseq
} bt_sdp_uuid_attached_value_t;

bool sdp_get_next_attribute_list(bt_sdp_attr_list_t *attr_list);
bool sdp_search_attribute_value(const bt_sdp_attr_list_t *attr_list, uint16_t attribute_id, bt_sdp_attr_value_t *out);
bool sdp_search_bool_attribute_value(bt_sdp_attr_list_t *attr_list, uint16_t attr_id, bool *out);
bool sdp_search_uint_attribute_value(bt_sdp_attr_list_t *attr_list, uint16_t attr_id, uint8_t uint_len, uint32_t *out);
bool sdp_search_text_attribute_value(bt_sdp_attr_list_t *attr_list, uint16_t attr_id, char *out_buf, uint16_t buf_max_len);
bool sdp_search_uuid_in_deseq_attribute_value(bt_sdp_attr_list_t *attr_list, uint16_t attr_id, const uint8_t *uuid, uint16_t uuid_len, uint8_t uint_len, uint32_t *out);
bool sdp_search_uuid_extra_in_deseq_attribute_value(bt_sdp_attr_list_t *attr_list, uint16_t attr_id, const uint8_t *uuid, uint16_t uuid_len, uint8_t uint_len, uint32_t *out, bt_sdp_attr_value_t *extra_value);
uint8_t sdp_search_rfcomm_server_channel_from_attr_list(bt_sdp_attr_list_t *attr_list);
bool sdp_search_uuid_from_value(const bt_sdp_attr_value_t *attr_value, const uint8_t *uuid, uint16_t uuid_len, bt_sdp_uuid_attached_value_t *out);
uint16_t sdp_get_uint_list_from_value(const bt_sdp_attr_value_t *value, uint16_t uint_len, uint32_t *out_uint_list, uint16_t max_list_len);

struct sdp_record_entry_t;
struct sdp_record_entry_t *sdp_is_record_exist(const bt_sdp_record_attr_t *attr_list);
int32 sdp_gather_global_service_uuids(uint8 in_uuid_size, uint8 *out_buff, uint32 out_buff_len, uint32 *out_len, uint32 *out_real_len);
void sdp_register_ibrt_tws_switch_protect_handle(void (*cb)(uint8_t, bt_bdaddr_t *, bool));
const char *bt_sdp_event_to_string(bt_sdp_event_t event);

#ifdef __cplusplus
}
#endif
#endif /* __SDP_I_H__ */

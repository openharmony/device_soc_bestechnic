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
#ifndef __ESHELL_H__
#define __ESHELL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "plat_types.h"

#define ESHELL_CHAR_SEP            ','

#define ONELINE_BUFF_SIZE (2048)

typedef void (*eshell_cmd_handler_t)(int argc, char *argv[]);

#undef ESHELL_GRP_DEFINE
#define ESHELL_GRP_DEFINE(n) ESHELL_CMD_GRP_##n
typedef enum {
    #include "eshell_grp.list"
    ESHELL_CMD_GRP_ID_MAXNUM,
} eshell_cmd_grp_id_e;

typedef struct {
    unsigned int grp_id;
    const char* cmd_str;
    const char* help_str;
    eshell_cmd_handler_t cmd_handler;
    int disp;
} eshell_cmd_t;

typedef void (*eshell_rpc_run_cmd_func)(char* buffer, unsigned int buff_size);

// API
void eshell_process(void);
void eshell_open(uint8_t uart_id, bool need_open_uart);
void eshell_close(void);
int eshell_rpc_execute_command(char *str, unsigned int str_len);
int eshell_run(char *cmdstr);
void eshell_rpc_run_cmd_func_cb_register(eshell_rpc_run_cmd_func op_cb, eshell_rpc_run_cmd_func result_cb);
eshell_rpc_run_cmd_func eshell_rpc_run_result_output_func_get(void);

// helper API
void eshell_param_macstr_get_array(char *params, int param_index, unsigned char *array_out);
unsigned int eshell_param_hexstr_get_uint(char *params, int param_index);
int eshell_param_get_int(char *params, int param_index);
char *eshell_param_get_start(char *params, int param_index);

#define ESHELL_PARAM_CHECK_COUNT(param,count,warn) \
    do { \
        int __i__ = 0, __len__ = 0, __count__ = 0; \
        if (param == NULL) { \
            eshell_putstring("%s:param is NULL!", warn); \
            return; \
        } \
        __len__ = strlen(param); \
        while (__i__ < __len__) { \
            if (param[__i__] == ESHELL_CHAR_SEP) \
                ++__count__; \
            ++__i__; \
        } \
        if (count != (__count__+1)) { \
            eshell_putstring("%s:wrong param number,expect=%d,give=%d!", warn, count, __count__+1); \
            return; \
        } \
    } while (0)

// platform API
void eshell_platform_init(uint8_t uart_id, bool uart_open);
void eshell_platform_wakeup(void);
void eshell_platform_sleep(void);
void eshell_platform_deinit(void);
void eshell_platform_reboot(void);
void eshell_putchar(char c);
unsigned char eshell_getchar(void);
int  eshell_tstc(void);
bool eshell_wait_data_ready(void);
int  eshell_putstring(const char *fmt, ...);
int  eshell_putstring_nl(const char *fmt, ...);
int  eshell_output_rawdata(char *data, unsigned int data_len);
void eshell_enqueue(uint8_t *buf, uint32_t size);

// helper macro
#define ESHELL_DEF_COMMAND(grp_id, cmd_str,help_str,cmd_handler)    \
    static const eshell_cmd_t cmd_handler##_eshell_def __attribute__((used, section(".eshell_command_table"))) =    \
        {(grp_id), (cmd_str), (help_str), (cmd_handler)}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __ESHELL_H__

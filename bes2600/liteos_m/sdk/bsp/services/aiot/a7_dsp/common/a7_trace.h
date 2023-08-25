#ifndef A7_TRACE_H
#define A7_TRACE_H

#include "stdarg.h"
#include "hal_trace.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ARM_ARCH_ISA_ARM
void a7_trace_init(void);
void a7_trace_tag_onoff(uint8_t onoff);
unsigned int a7_trace_fifo_size_get(void);
#else
typedef int (*HAL_TRACE_OUTPUT_HOOK_T)(const char *tag, const char *fmt, uint32_t len);
void hal_trace_register_hook(HAL_TRACE_OUTPUT_HOOK_T hook);
void hal_trace_unregister_hook(HAL_TRACE_OUTPUT_HOOK_T hook);
typedef int (*HAL_TRACE_PRINTF_HOOK_T)(const char *tag, const char *fmt, va_list ap);
void hal_trace_printf_register_hook(HAL_TRACE_PRINTF_HOOK_T hook);
void hal_trace_printf_unregister_hook(HAL_TRACE_PRINTF_HOOK_T hook);
int hal_trace_output_block(const unsigned char *buf, unsigned int len);
void hal_trace_print_a7(const unsigned char *buf, unsigned int buf_len);
int hal_trace_set_onoff(uint32_t onoff);
void hal_trace_print_a7_crash(int onoff);
void hal_trace_print_a7_tag(int onoff);
void hal_trace_register_cp_hook(HAL_TRACE_OUTPUT_HOOK_T hook);
void hal_trace_unregister_cp_hook(HAL_TRACE_OUTPUT_HOOK_T hook);
void hal_trace_register_a7_hook(HAL_TRACE_OUTPUT_HOOK_T hook);
void hal_trace_unregister_a7_hook(HAL_TRACE_OUTPUT_HOOK_T hook);
#endif
void hal_trace_print_a7_flush(int onoff);
int hal_trace_print_raw(const char *fmt, ...);
#if !defined(__NuttX__) || !defined(LIBC_ADAPT_EN)
//#define printf hal_trace_print_raw
#endif

#ifdef __cplusplus
}
#endif

#endif

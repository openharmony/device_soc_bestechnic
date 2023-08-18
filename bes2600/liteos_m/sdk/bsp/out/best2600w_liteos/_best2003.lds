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
/***************************************************************************
 *
 * Copyright 2015-2020 BES.
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
/* total ram6 size is 0x30000 */
/* rename ram6 for network heap usage */
/* Caution: Cannot use fast timer irq */
/* For linker scripts */
/* For module features */
/* Total 64M-byte space and let each flash ctrl owns 32M-byte space */
/* For boot struct version */
/* For ROM export functions */
/* Common RAM config checks (only check in lds file, the first compiled file during build) */
/* Linker script to configure memory regions. */
/* See plat_addr_map.h and common.mk for the macro definitions */
MEMORY
{
 ROM (rx) : ORIGIN = 0x00020000, LENGTH = 0x00010000
 FLASH (r) : ORIGIN = (0x2C000000 + 0x80000), LENGTH = (0x2000000 - ((0x2C000000 + 0x80000) - 0x2C000000))
 FLASH_NC (r) : ORIGIN = (((0x2C000000 + 0x80000)) - 0x2C000000 + 0x28000000), LENGTH = (0x2000000 - ((0x2C000000 + 0x80000) - 0x2C000000))
 FLASHX (rx) : ORIGIN = (((0x2C000000 + 0x80000)) - 0x2C000000 + 0x0C000000), LENGTH = (0x2000000 - ((0x2C000000 + 0x80000) - 0x2C000000))
 RAM (rwx) : ORIGIN = ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000), LENGTH = 0x200000-0x20000-0x20000-0-0-0x80000 - 0x16000
 RAMX (rx) : ORIGIN = (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000), LENGTH = 0x200000-0x20000-0x20000-0-0-0x80000 - 0x16000
 FRAMX (rwx) : ORIGIN = (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + 0x200000-0x20000-0x20000-0-0-0x80000 - 0x16000, LENGTH = 0x16000
 PSRAM (rwx) : ORIGIN = 0x34000000, LENGTH = 0x2000000-0
 PSRAM_NC (rwx) : ORIGIN = 0x30000000, LENGTH = 0x2000000-0
 PSRAMX (rx) : ORIGIN = 0x14000000, LENGTH = 0x2000000-0
 PSRAMUHS (rwx) : ORIGIN = (0x3C000000 + 0x800000-0), LENGTH = 0
 PSRAMUHS_NC (rwx) : ORIGIN = (0x38000000 + 0x800000-0), LENGTH = 0
 PSRAMUHSX (rx) : ORIGIN = (0x1C000000 + 0x800000-0), LENGTH = 0
 RAMCP (rwx) : ORIGIN = (0x00200000 + 0x20000 - 0x00200000 + 0x20000000), LENGTH = 0x20000
 RAMCPX (rwx) : ORIGIN = 0x00200000, LENGTH = 0x20000
}
/* Library configurations */
/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __export_fn_rom
 *   __exidx_start
 *   __exidx_end
 *   __copy_table_start__
 *   __copy_table_end__
 *   __zero_table_start__
 *   __zero_table_end__
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   __free_ram
 *   __factory_start
 *   __factory_end
 *   __calib_start
 *   __calib_end
 *   __flash_start
 *   __flash_end
 *   __free_flash
 *   __boot_bss_sram_start__
 *   __boot_bss_sram_end__
 *   __sram_text_start_flash__
 *   __sram_data_end_flash__
 *   __fast_sram_text_data_start__
 *   __fast_sram_text_data_end__
 *   __fast_sram_text_start_flash__
 *   __fast_sram_text_end_flash__
 *   __sram_text_data_start__
 *   __sram_bss_start__
 *   __sram_bss_end__
 *   __overlay_text_start__
 *   __overlay_text_exec_start__
 *   __overlay_data_start__
 */
ENTRY(Boot_Loader)
SECTIONS
{
 __export_fn_rom = (ORIGIN(ROM) + LENGTH(ROM) - 40 - 128);
 .boot_struct (ORIGIN(FLASH)) :
 {
  __flash_start = .;
  KEEP(*(.boot_struct))
  . = ALIGN(4);
 } > FLASH
 . = ((.) - 0x2C000000 + 0x0C000000);
 .boot_text_flash (.) : AT (((ADDR(.boot_text_flash)) - 0x0C000000 + 0x2C000000))
 {
  __flashx_start_addr__ = .;
  *(.boot_loader)
  *(.boot_text_flash*)
  *(.boot_rodata_flash*)
  . = ALIGN(4);
 } > FLASHX
 . = ((.) - 0x0C000000 + 0x2C000000);
 .got_info (.) :
 {
  __got_info_start = .;
  __got_start = .;
  *(.got)
  . = ALIGN(4);
  __got_end = .;
  __got_plt_start = .;
  *(.got.plt)
  . = ALIGN(4);
  __igot_plt_start = .;
  *(.igot.plt)
  . = ALIGN(4);
  __dynamic_start = .;
  *(.dynamic)
  . = ALIGN(4);
  __got_info_end = .;
 } > FLASH
 .vector_table (ORIGIN(RAM)) (NOLOAD) :
 {
  KEEP(*(.vector_table))
  . = 360;
  . = ALIGN(4);
 } > RAM
 .reboot_param (.) (NOLOAD) :
 {
  KEEP(*(.reboot_param))
  . = 64;
  . = ALIGN(4);
 } > RAM
 .userdata_pool (.) (NOLOAD) :
 {
  *(.userdata_pool)
  . = ALIGN(4);
 } > RAM
 __boot_text_sram_start_flash__ = LOADADDR(.got_info) + SIZEOF(.got_info);
 .boot_text_sram (ALIGN(((.) - ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) + (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000)), 32)) : AT (__boot_text_sram_start_flash__)
 {
  __boot_text_sram_start__ = .;
  /* memcpy.o or libc_nano.a(lib_a-memcpy.o/lib_a-memcpy-stub.o) */
  *:memcpy.o(.text*)
  *:lib_a-memcpy*.o(.text*)
  *:libc_rom.o(.text*)
  *:libc_rom_mem.o(.text*)
  /* memset.o or libc_nano.a(lib_a-memset.o) */
  *:memset.o(.text*)
  *:lib_a-memset*.o(.text*)
  *:hal_norflash*.o(.text*)
  *:norflash_*.o(EXCLUDE_FILE(*:norflash_api.o) .text*)
  *:hal_cmu_*.o(.text*)
  *:hal_iomux_*.o(.text*)
  *:hal_analogif_*.o(.text*)
  *:hal_spi.o(.text*)
  *:hal_timer.o(.text*)
  *:hal_sysfreq.o(.text*)
  *:hal_mcu2cp.o(.text*)
  *:wifi_agent.o(.text*)
  *:wifi_srv.o(.text*)
  *:ethernetif.o(.text*)
  *psram*.o(.text*)
  *(.boot_text_sram*)
  . = ALIGN(4);
  __boot_text_sram_end__ = .;
 } > RAMX
 __boot_text_data_sram_start__ = ((__boot_text_sram_start__) - (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000));
 __boot_text_sram_end_flash__ = __boot_text_sram_start_flash__ + SIZEOF(.boot_text_sram);
 __boot_data_sram_start_flash__ = __boot_text_sram_end_flash__;
 .boot_data_sram (((ADDR(.boot_text_sram) + SIZEOF(.boot_text_sram)) - (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000))) :
     AT (__boot_data_sram_start_flash__)
 {
  __boot_data_sram_start__ = .;
  *:hal_norflash*.o(.data* .rodata*)
  *:norflash_*.o(EXCLUDE_FILE(*:norflash_api.o) .data* EXCLUDE_FILE(*:norflash_api.o) .rodata*)
  *psram*.o(.data* .rodata*)
  *:libc_rom.o(.rodata*)
  *:libc_rom_mem.o(.rodata*)
  /* Stack protector instrumentation rodata for functions in special sections */
  *(.rodata.cst4)
  *(.boot_rodata_sram*)
  *(.boot_data*)
  . = ALIGN(4);
  __boot_data_sram_end__ = .;
 } > RAM
 __boot_data_sram_end_flash__ = __boot_data_sram_start_flash__ + SIZEOF(.boot_data_sram);
 .boot_bss_sram (.) (NOLOAD) : AT (ADDR(.boot_bss_sram))
 {
  __boot_bss_sram_start__ = .;
  *:hal_norflash*.o(.bss*)
  *:norflash_*.o(EXCLUDE_FILE(*:norflash_api.o) .bss*)
  *psram*.o(.bss*)
  *(.boot_bss*)
  . = ALIGN(32);
  __boot_bss_sram_end__ = .;
 } > RAM
 __sram_text_start_flash__ = __boot_data_sram_end_flash__;
 __sram_text_data_start__ = __boot_bss_sram_end__;
 .sram_text (((__sram_text_data_start__) - ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) + (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000))) : AT (__sram_text_start_flash__)
 {
  __sram_text_start__ = .;
  *:cp_server.o(.text*)
  *:mpu_armv8m.o(.text*)
  /* wifi adaptor */
  *:wifi_agent.o(.text*)
  *:wifi_srv.o(.text*)
  *:net_memory.o(.text*)
  *:net_timer.o(.text*)
  *:net_tasklet.o(.text*)
  *:net_mutex.o(.text*)
  *:net_workqueue.o(.text*)
  /* wifi-stack */
  *:agg-rx.o(.text*)
  *:agg-tx.o(.text*)
  *:event.o(.text*)
  *:tx.o(.text*)
  *:status.o(.text*)
  *:hw_crc.o(.text*)
  *:queue.o(.text*)
  *:rc80211_minstre*.o(.text*)
  *:mymemcpy.o(.text*)
  *:cw1200_sdio.o(.text*)
  *:net_mgmt.o(.text*)
  *:net_mgmt_data.o(.text*)
  *:skbuff.o(.text*)
  *:bh.o(.text*)
  *:txrx.o(.text*)
  *:rx.o(.text*)
  /* lwip */
  *:tcp.o(.text*)
  *:tcpip.o(.text*)
  *:tcp_in.o(.text*)
  *:tcp_out.o(.text*)
  *:udp.o(.text*)
  *:netbuf.o(.text*)
  *:ethernet.o(.text*)
  *:ethernetif.o(.text*)
  *:ip.o(.text*)
  *:ip4.o(.text*)
  *:pbuf.o(.text*)
  *:sockets.o(.text*)
  *:memp.o(.text*)
  *:mem.o(.text*)
  /* iperf */
  *:iperf_api.o(.text*)
  *:iperf_client_api.o(.text*)
  *:iperf_server_api.o(.text*)
  *:rtos_lib.o(.text*)
  /*
		   Enhance wifi throughput in the liteos system.
		   include file *los_*, *driverif*, *sys_arch*, *sockets_porting*, *ethernetif*, *tcp*
		 */
  *los_*.o(.text*)
  *driverif*.o(.text*)
  *sys_arch*.o(.text*)
  *sockets_porting*.o(.text*)
  *ethernetif*.o(.text*)
  *tcp*.o(.text*)
  *:hal_memsc.o(.text*)
  *:hal_trace*.o(.text*)
  *:mpu_armv*.o(.text*)
  *:codec_sbc.o(.text*)
  *:multi_heap.o(.text*)
  *:pool_api.o(.text*)
  *libgcc.a:(.text*)
  /* for LIBC_ROM=0 */
  *libc_nano.a:(.text*)
  /* for NOSTD=1  */
  *:atoi.o(.text*)
  *:ctype.o(.text*)
  *:memchr.o(.text*)
  *:memcmp.o(.text*)
  *:memmove.o(.text*)
  *:strcat.o(.text*)
  *:strchr.o(.text*)
  *:strcmp.o(.text*)
  *:strcpy.o(.text*)
  *:strlen.o(.text*)
  *:strncat.o(.text*)
  *:strncmp.o(.text*)
  *:strncpy.o(.text*)
  *:strnicmp.o(.text*)
  *:strstr.o(.text*)
  *:strtol.o(.text*)
  *:strtoul.o(.text*)
  *:vsprintf.o(.text*)
  *:vsscanf.o(.text*)
  /* for SCO_CP_ACCEL=1 */
  *:cmsis_dsp_lib*.o(.text*)
  *libm.a:(.text*)
  *:a2dp_encoder_cp.o(.text*)
  *:a2dp_encoder_sbc_cp.o(.text*)
  *:norflash_api.o(.text*)
  *:wifi_hal_best2003.o(.text*)
  *:wifi_mac_ind.o(.text*)
  . = ALIGN(0x2000); /*align for dubug hook*/
  /* for SCO_CP_ACCEL=1 */
  *:cmsis_dsp_lib*.o(.text*)
  *libm.a:(.text*)
  *:tcp*.o(.text*)
  *:net*.o(.text*)
  *:agg*.o(.text*)
  *:michael.o(.text*)
  *:rx.o(.text*)
  *:event.o(.text*)
  *:util.o(.text*)
  *:rate.o(.text*)
  *:rc80211*.o(.text*)
  *:tx.o(.text*)
  *:status.o(.text*)
  *:hwio.o(.text*)
  *:fwio.o(.text*)
  *:bh.o(.text*)
  *:txrx.o(.text*)
  *:skbuff.o(.text*)
  *:eth*.o(.text*)
  *:def.o(.text*)
  *:raw.o(.text*)
  *:cmsis*.o(.text*)
  *:list.o(.text*)
  *:memp*.o(.text*)
  *:mcu_slave_init.o(.text*)
  *:dsp_code_decom.o(.text*)
  *:lzma_decode.o(.text*)
  *:hal_transq.o(.text*)
  *:ftp_client.o(.text*)
  *:port_s.o(.text*)
  *:k_*.o(.text*)
  *:wifi_drv*.o(.text*)
  *:dpd*.o(.text*)
  *:flash*.o(.text*)
  /* freeRTOS kernel */
  *:heap_5.o(.text*)
  *:port.o(.text*)
  *:mpu_wrappers.o(.text*)
  *:croutine.o(.text*)
  *:event_groups.o(.text*)
  *:list.o(.text*)
  *:stream_buffer.o(.text*)
  *:tasks.o(.text*)
  *:timers.o(.text*)
  *(.sram_text*)
  /*
		   If you want protect one section by armv8's mpu, the section should
		   be aligned to 32bytes
		 */
  . = ALIGN(32);
  __sram_text_end__ = .;
 } > RAMX
 __sram_text_end_flash__ = __sram_text_start_flash__ + SIZEOF(.sram_text);
 __sram_data_start_flash__ = __sram_text_end_flash__;
 .sram_data (((ADDR(.sram_text) + SIZEOF(.sram_text)) - (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000))) :
    AT (__sram_data_start_flash__)
 {
  __sram_data_start__ = .;
  *:rtos_lib.o(.data* .rodata*)
  *:cp_server.o(.data* .rodata*)
  *:mpu_armv8m.o(.data* .rodata*)
  /*
		   Enhance wifi throughput in the liteos system.
		   include file *los_*, *driverif*, *sys_arch*, *sockets_porting*, *ethernetif*, *tcp*
		 */
  *los_*.o(.data* .rodata*)
  *driverif*.o(.data* .rodata*)
  *sys_arch*.o(.data* .rodata*)
  *sockets_porting*.o(.data* .rodata*)
  *ethernetif*.o(.data* .rodata*)
  *tcp*.o(.data* .rodata*)
  *:plc_8000.o(.data* .rodata*)
  *:Pcm8k_Cvsd.o(.data* .rodata*)
  *:cvsd_codec.o(.data* .rodata*)
  *:table.o(.data* .rodata*)
  *:plc_16000.o(.data* .rodata*)
  /* Move algos from overlay to here */
  *:utils.o(.data* .rodata*)
  *:cqueue.o(.data* .rodata*)
  *:iir_process.o(.data* .rodata*)
  *:eq_cfg.o(.data* .rodata*)
  *:sbcplc.o(.data* .rodata*)
  *:sbc.o(.data* .rodata*)
  *:sbc_math.o(.data* .rodata*)
  *:crossover_filter.o(.data* .rodata*)
  *:exp_func_tables.o(.data* .rodata*)
  *:hal_memsc.o(.rodata*)
  *:hal_trace*.o(.rodata*)
  *:mpu.o(.rodata*)
  *:multi_heap.o(.rodata*)
  *:pool_api.o(.rodata*)
  *:a2dp_decoder_cp.o(.rodata*)
  *:a2dp_encoder_cp.o(.rodata*)
  *:a2dp_encoder_sbc_cp.o(.rodata*)
  *libgcc.a:(.rodata*)
  /* for LIBC_ROM=0 */
  *libc_nano.a:(.rodata*)
  /* for NOSTD=1  */
  *:atoi.o(.rodata*)
  *:ctype.o(.rodata*)
  *:memchr.o(.rodata*)
  *:memcmp.o(.rodata*)
  *:memmove.o(.rodata*)
  *:strcat.o(.rodata*)
  *:strchr.o(.rodata*)
  *:strcmp.o(.rodata*)
  *:strcpy.o(.rodata*)
  *:strlen.o(.rodata*)
  *:strncat.o(.rodata*)
  *:strncmp.o(.rodata*)
  *:strncpy.o(.rodata*)
  *:strnicmp.o(.rodata*)
  *:strstr.o(.rodata*)
  *:strtol.o(.rodata*)
  *:strtoul.o(.rodata*)
  *:vsprintf.o(.rodata*)
  *:vsscanf.o(.rodata*)
  /* for SCO_CP_ACCEL=1 */
  *:cmsis_dsp_lib*.o(.rodata*)
  *libm.a:(.rodata*)
  *:norflash_api.o(.rodata*)
  *:tcp*.o(.data*)
  *:net*.o(.data*)
  *:agg*.o(.data*)
  *:rx.o(.data*)
  *:event.o(.data*)
  *:util.o(.data*)
  *:rate.o(.data*)
  *:rc80211*.o(.data*)
  *:tx.o(.data*)
  *:status.o(.data*)
  *:hw_crc.o(.data*)
  *:queue.o(.data*)
  *:hwio.o(.data*)
  *:fwio.o(.data*)
  *:bh.o(.data*)
  *:txrx.o(.data*)
  *:skbuff.o(.data*)
  *:eth*.o(.data*)
  *:ip*.o(.data*)
  *:def.o(.data*)
  *:pbuf.o(.data*)
  *:raw.o(.data*)
  *:sockets.o(.data*)
  *:apps.o(.data*)
  *:cmsis*.o(.data*)
  *:list.o(.data*)
  *:k_*.o(.data*)
  *:mcu_slave_init.o(.data*)
  *:dsp_code_decom.o(.data*)
  *:lzma_decode.o(.data*)
  *:hal_transq.o(.data*)
  *:ftp_client.o(.data*)
  *(.sram_data*)
  . = ALIGN(4);
 } > RAM
 __sram_data_end_flash__ = __sram_data_start_flash__ + SIZEOF(.sram_data) +
  0;
 .sram_bss (.) (NOLOAD) : AT (ADDR(.sram_bss))
 {
  __sram_bss_start__ = .;
  *:utils.o(.bss*)
  *:Pcm8k_Cvsd.o(.bss*)
  *:cvsd_codec.o(.bss*)
  *:tcp*.o(.bss*)
  *:net*.o(.bss*)
  *:agg*.o(.bss*)
  *:rx.o(.bss*)
  *:event.o(.bss*)
  *:util.o(.bss*)
  *:rate.o(.bss*)
  *:rc80211*.o(.bss*)
  *:tx.o(.bss*)
  *:status.o(.bss*)
  *:hw_crc.o(.bss*)
  *:queue.o(.bss*)
  *:hwio.o(.bss*)
  *:fwio.o(.bss*)
  *:bh.o(.bss*)
  *:txrx.o(.bss*)
  *:skbuff.o(.bss*)
  *:eth*.o(.bss*)
  *:ip*.o(.bss*)
  *:def.o(.bss*)
  *:pbuf.o(.bss*)
  *:raw.o(.bss*)
  *:sockets.o(.bss*)
  *:apps.o(.bss*)
  *:cmsis*.o(.bss*)
  *:list.o(.bss*)
  *:memp*.o(.bss*)
  *:k_*.o(.bss*)
  *:mcu_slave_init.o(.bss*)
  *:dsp_code_decom.o(.bss*)
  *:lzma_decode.o(.bss*)
  *:ftp_client.o(.bss*)
  *:hal_transq.o(.bss*)
  *:hal_mcu2cp.o(.bss*)
  *:wifi_agent.o(.bss*)
  *:wifi_srv.o(.bss*)
  *:ethernetif.o(.bss*)
  *(.sram_bss*)
  . = ALIGN(32);
  __sram_bss_end__ = .;
 } > RAM
 __fast_sram_text_start_flash__ = __sram_data_end_flash__;
 .fast_text_sram (ORIGIN(FRAMX)) : AT (__fast_sram_text_start_flash__)
 {
  __fast_sram_text_exec_start__ = .;
  *:frame_resize.o(.text*)
  *:buffer_manager.o(.text*)
  *:plc_8000.o(.text*)
  *:Pcm8k_Cvsd.o(.text*)
  *:cvsd_codec.o(.text*)
  *:dspfns.o(.text*)
  *:lpc_plc.o(.text*)
  *:plc_16000.o(.text*)
  *:sbc_filter.o(.text*)
/* Move algos from overlay to here */
  /* memmove.o or libc_nano.a(lib_a-memmove.o) */
  *:memmove.o(.text*)
  *:lib_a-memmove.o(.text*)
  *:utils.o(.text*)
  *:cqueue.o(.text*)
  *:audio_resample_ex.o(.text*)
  *:sbc.o(.text*)
  *:sbc_math.o(.text*)
  *:fixpoint_math.o(.text*)
  *:FDK_bitbuffer.o(.text*)
  *:FDK_tools_rom.o(.text*FDKgetWindowSlope*)
  *:FDK_tools_rom.o(.text*getBitstreamElementList*)
  *:dct.o(.text*dct_IV*)
  *:fft.o(.text*._Z3fftiPlPi)
  *:fft_rad2.o(.text*dit_fft*)
  *:dct.o(.text*getTables*)
  *:crossover_filter.o(.text*)
  *:data_compressor.o(.text*)
  *:drc.o(.text*)
  *:limiter.o(.text*)
  :audio_prompt_sbc.o(.text*)
  *:udp*.o(.text*)
  *:ip*.o(.text*)
  *:tcp*.o(.text*)
  *:mem*.o(.text*)
  *:pbuf.o(.text*)
  *:icmp.o(.text*)
  *:net*.o(.text*)
  *:eth*.o(.text*)
  *:iperf*.o(.text*)
  *:nts*.o(.text*)
  *:socket*.o(.text*)
  *:hw_crc.o(.text*)
  *(.fast_text_sram*)
  /*
		   If you want protect one section by armv8's mpu, the section should
		   be aligned to 32bytes
		 */
  . = ALIGN(32);
  __fast_sram_text_exec_end__ = .;
 } > FRAMX
 __fast_sram_text_end_flash__ = __fast_sram_text_start_flash__ + SIZEOF(.fast_text_sram);
 __fast_sram_text_data_start__ = ((__fast_sram_text_exec_start__) - (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000));
 __fast_sram_text_data_end__ = ((__fast_sram_text_exec_end__) - (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000));
 __cp_text_sram_start_flash__ = __fast_sram_text_end_flash__;
 .cp_text_sram (ORIGIN(RAMCPX)) : AT (__cp_text_sram_start_flash__)
 {
  __cp_text_sram_exec_start__ = .;
  *(.cp_text_sram*)
  *:cp_queue.o(.text*)
  . = ALIGN(4);
  __cp_text_sram_exec_end__ = .;
 } > RAMCPX
 __cp_text_sram_start = ((__cp_text_sram_exec_start__) - (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000));
 __cp_text_sram_end = ((__cp_text_sram_exec_end__) - (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000) + ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000));
 .vector_table_cp (ORIGIN(RAMCP)) (NOLOAD) :
 {
  KEEP(*(.vector_table_cp))
  . = 360;
  . = ALIGN(4);
 } > RAMCP
 __cp_data_sram_start_flash__ = __cp_text_sram_start_flash__ + SIZEOF(.cp_text_sram);
 .cp_data_sram (.) : AT (__cp_data_sram_start_flash__)
 {
  __cp_data_sram_start = .;
  *(.cp_data*)
  *:cp_queue.o(.data* .rodata*)
  . = ALIGN(4);
  . = ALIGN(4);
  __cp_data_sram_end = .;
 } > RAMCP
 __cp_sram_end_flash__ = __cp_data_sram_start_flash__ + SIZEOF(.cp_data_sram);
 .cp_bss_sram (.) (NOLOAD) : AT (ADDR(.cp_bss_sram))
 {
  __cp_bss_sram_start = .;
  *(.cp_bss*)
  . = ALIGN(4);
  . = ALIGN(4);
  __cp_bss_sram_end = .;
 } > RAMCP
 __overlay_text_start_flash__ = __cp_sram_end_flash__;
 __overlay_text_start__ = ALIGN(__cp_text_sram_end, 32);
 __overlay_text_exec_start__ = ((__overlay_text_start__) - ((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) + (((0x00200000 + 0x20000 - 0x00200000 + 0x20000000) + 0x20000) - 0x20000000 + 0x00200000));
 OVERLAY (__overlay_text_exec_start__) : NOCROSSREFS AT (__overlay_text_start_flash__)
 {
  .overlay_text0
  {
   LONG(0);
   *:kiss_fft.o(.text*)
   *:kiss_fftr.o(.text*)
   *:fftwrap.o(.text*)
   *:smallft.o(.text*)
   *:cmsis_fftwrap.o(.text*)
   *:arm_rfft_fast_f32.o(.text*)
   *:arm_cfft_f32.o(.text*)
   /* common */
   *:ae_common.o(.text*)
   *:ae_rand.o(.text*)
   *:fastmath.o(.text*)
   *:integer_delay.o(.text*)
   *:ringbuf.o(.text*)
   *:scale_int16.o(.text*)
   *:speech_conv.o(.text*)
   *:speech_trans_buf.o(.text*)
   *:preprocess.o(.text*)
   *:filterbank.o(.text*)
   *:mdf.o(.text*)
   *:coherence.o(.text*)
   *:echo_canceller.o(.text*)
   *:echo_suppression.o(.text*)
   *:lcmmse.o(.text*)
   *:logmmse.o(.text*)
   *:mdf.o(.text*)
   *:mdf1.o(.text*)
   *:mdf2.o(.text*)
   *:noise_generator.o(.text*)
   *:pink_noise_gen.o(.text*)
   *:white_noise_gen.o(.text*)
   *:compexp.o(.text*)
   *(.overlay_text0)
   . = ALIGN(4);
  }
  .overlay_text1
  {
   LONG(0);
   *:fixedpoint.o(.text*)
   *:replaygain.o(.text*)
   *:dsp_core.o(.text*)
   *:dsp_misc.o(.text*)
   *:dsp_filter.o(.text*)
   *:dsp_sample_input.o(.text*)
   *:dsp_sample_output.o(.text*)
   *:eq.o(.text*)
   *:pga.o(.text*)
   *:a2dp_decoder_sbc.o(.text*)
   *(.overlay_a2dp_sbc*)
   *(.overlay_text1)
   . = ALIGN(4);
  }
  .overlay_text2
  {
   LONG(0);
   *(.overlay_text2)
   . = ALIGN(4);
  }
  .overlay_text3
  {
   LONG(0);
   *:fft128dot.o(.text*)
   *(.overlay_text3)
   . = ALIGN(4);
  }
  .overlay_text4
  {
   LONG(0);
   *:block.o(.text*.*CBlock_GetEscape*)
   *:tpdec_lib.o(.text*.*transportDec_GetBitstream*)
   *:tpdec_lib.o(.text*.*transportDec_GetAuBitsRemaining*)
   *:tpdec_latm.o(.text*.*CLatmDemux_GetValue*)
   *:tpdec_latm.o(.text*.*CLatmDemux_Read*)
   *:tpdec_lib.o(.text*.*transportDec_FillData*)
   *:tpdec_lib.o(.text*.*transportDec_ReadAccessUnit*)
   *:tpdec_lib.o(.text*.*transportDec_readHeader*)
   *:block.o(.text*.*ApplyTools*)
   *:aacdec_pns.o(.text*.*CPns_Apply*)
   *:aacdec_tns.o(.text*.*CTns_Apply*)
   *:aacdec_tns.o(.text*.*CTns_Reset*)
   *:aacdec_pns.o(.text*.*CPns_ResetData*)
   *:pulsedata.o(.text*.*CPulseData_Read*)
   *:channel.o(.text*.*CChannelElement_Read*)
   *:tpdec_asc.o(.text*.*CProgramConfig_Reset*)
   *:tpdec_lib.o(.text*.*transportDec_CrcCheck*)
   *:block.o(.text*.*CBlock_FrequencyToTime*)
   *:block.o(.text*.*CBlock_ReadSectionData*)
   *:channel.o(.text*.*CChannelElement_Decode*)
   *:tpdec_lib.o(.text*.*transportDec_CrcEndReg*)
   *:aacdecoder.o(.text*.*CAacDecoder_DecodeFrame*)
   *:aac_mdct.o(.text*)
   *:block.o(.text*.*CBlock_ReadSpectralData*)
   *:block.o(.text*.*CBlock_ScaleSpectralData*)
   *:aacdec_tns.o(.text*.*CTns_ReadDataPresentFlag*)
   *:tpdec_lib.o(.text*.*transportDec_CrcStartReg*)
   *:block.o(.text*.*CBlock_ReadScaleFactorData*)
   *:tpdec_lib.o(.text*.*transportDec_EndAccessUnit*)
   *:tpdec_latm.o(.text*.*CLatmDemux_GetNrOfSubFrames*)
   *:tpdec_lib.o(.text*.*transportDec_GetAuBitsTotal*)
   *:tpdec_asc.o(.text*.*CProgramConfig_LookupElement*)
   *:tpdec_latm.o(.text*.*CLatmDemux_GetFrameLengthInBits*)
   *:tpdec_latm.o(.text*.*CLatmDemux_ReadPayloadLengthInfo*)
   *:block.o(.text*.*CBlock_InverseQuantizeSpectralData*)
   *:channelinfo.o(.text*.*IcsRead*)
   *:aacdec_tns.o(.text*.*CTns_Read*)
   *:tpdec_lib.o(.text*.*transportDec_AdjustEndOfAccessUnit*)
   *:pulsedata.o(.text*.*CPulseData_Apply*)
   *:tpdec_asc.o(.text*.*AudioSpecificConfig_Init*)
   *:tpdec_asc.o(.text*.*AudioSpecificConfig_Parse*)
   *:tpdec_latm.o(.text*.*CLatmDemux_ReadStreamMuxConfig*)
   *:tpdec_asc.o(.text*.*getSampleRate*)
   *:tpdec_asc.o(.text*.*getAOT*)
   *:aacdecoder.o(.text*.*CAacDecoder_Init*)
   *:tpdec_asc.o(.text*.*CProgramConfig_Init*)
   *:aacdecoder_lib.o(.text*.*aacDecoder_ConfigCallback*)
   *:aacdecoder.o(.text*.*CAacDecoder_ExtPayloadParse*)
   *:stereo.o(.text*.*CJointStereo_Read*)
   *:a2dp_decoder_aac_lc.o(.text*)
   *:aac_com.o(.text*)
   *:aac_enc.o(.text*)
   *:aac_dec.o(.text*)
   *(.overlay_a2dp_aac*)
   *(.overlay_text4)
   . = ALIGN(4);
  }
  .overlay_text5
  {
   LONG(0);
   *:mpa.o(.text*)
   *:bit.o(.text*)
   *:frame.o(.text*)
   *:huffman.o(.text*)
   *:layer12.o(.text*)
   *:layer3.o(.text*)
   *:stream.o(.text*)
   *:synth.o(.text*)
   *(.overlay_text5)
   . = ALIGN(4);
  }
  .overlay_text6
  {
   LONG(0);
   *:mp3_dec.o(.text*)
   *:mp3_enc.o(.text*)
   *(.overlay_text6)
   . = ALIGN(4);
  }
  .overlay_text7
  {
   LONG(0);
   *:opus_com.o(.text*)
   *(.overlay_text7)
   . = ALIGN(4);
  }
  .overlay_text8
  {
   LONG(0);
   *:ogg_com.o(.text*)
   *:vorbis_com.o(.text*)
   *(.overlay_text8)
   . = ALIGN(4);
  }
  .overlay_text9
  {
   LONG(0);
   *:alac_com.o(.text*)
   *(.overlay_text9)
   . = ALIGN(4);
  }
  .overlay_text10
  {
   LONG(0);
   *:flac_com.o(.text*)
   *(.overlay_text10)
   . = ALIGN(4);
  }
  .overlay_text11
  {
   LONG(0);
   *(.overlay_text11)
   . = ALIGN(4);
  }
  .overlay_text_last
  {
   LONG(0);
   . = ALIGN(4);
  }
 } > RAMCPX
 .dummy_overlay_text_end (.) (NOLOAD) : AT (ADDR(.dummy_overlay_text_end))
 {
  __overlay_text_exec_end__ = .;
 } > RAMCPX
 __fast_sram_end__ = __fast_sram_text_data_end__;
 ASSERT(__fast_sram_end__ - __fast_sram_text_data_start__ <= 0x16000, "fast xram sections too large")
 __free_fram = 0x16000 - (__fast_sram_end__ - __fast_sram_text_data_start__);
 __overlay_data_start__ = __cp_bss_sram_end;
 OVERLAY (__overlay_data_start__) : NOCROSSREFS AT (__load_stop_overlay_text_last)
 {
  .overlay_data0
  {
   /* Explicitly place 4 bytes at section start to avoid "section type changed to PROGBITS" warnings */
   LONG(0);
   *:bt_sco_chain.o(.data* .rodata*)
   *:bt_sco_chain_cp.o(.data* .rodata*)
   *:fftwrap.o(.data* .rodata*)
   *:kiss_fft.o(.data* .rodata*)
   *:kiss_fftr.o(.data* .rodata*)
   *:smallft.o(.data* .rodata*)
   *:CommonTables.o(.rodata.armBitRevIndexTable256)
   *:CommonTables.o(.rodata.twiddleCoef_256)
   *:CommonTables.o(.rodata.twiddleCoef_rfft_512)
   *:CommonTables.o(.rodata.armBitRevIndexTable128)
   *:CommonTables.o(.rodata.twiddleCoef_128)
   *:CommonTables.o(.rodata.twiddleCoef_rfft_256)
   /* common */
   *:fastmath_const.o(.data* .rodata*)
   *:preprocess.o(.data* .rodata*)
   *:filterbank.o(.data* .rodata*)
   *:mdf.o(.data* .rodata*)
   *:speech_win.o(.rodata.conj_fwin_60_128_cp)
   *:speech_win.o(.rodata.conj_fwin_120_256_cp)
   *:speech_win.o(.rodata.conj_fwin_128_256_cp)
   *:speech_win.o(.rodata.conj_fwin_160_256_cp)
   *:speech_win.o(.rodata.conj_fwin_240_512_cp)
   *:speech_win.o(.rodata.conj_fwin_256_512_cp)
   *(.overlay_data0 .overlay_rodata0)
   . = ALIGN(4);
   *(.overlay_sco_cache*)
   . = ALIGN(4);
  }
  .overlay_data1
  {
   LONG(0);
   *:fixedpoint.o(.data* .rodata*)
   *:replaygain.o(.data* .rodata*)
   *:dsp_core.o(.data* .rodata*)
   *:dsp_misc.o(.data* .rodata*)
   *:dsp_filter.o(.data* .rodata*)
   *:dsp_sample_input.o(.data* .rodata*)
   *:dsp_sample_output.o(.data* .rodata*)
   *:eq.o(.data* .rodata*)
   *:pga.o(.data* .rodata*)
  *:a2dp_decoder_sbc.o(.rodata*)
   *(.overlay_data1 .overlay_rodata1)
   . = ALIGN(4);
  }
  .overlay_data2
  {
   LONG(0);
   *(.overlay_data2 .overlay_rodata2)
   . = ALIGN(4);
  }
  .overlay_data3
  {
   LONG(0);
   *:fft128dot.o(.data* .rodata*)
   *(.overlay_data3 .overlay_rodata3)
   . = ALIGN(4);
  }
  .overlay_data4
  {
   LONG(0);
   *:block.o(.data* .rodata*)
   *:tpdec_lib.o(.data* .rodata*)
   *:tpdec_latm.o(.data* .rodata*)
   *:aacdec_pns.o(.data* .rodata*)
   *:aacdec_tns.o(.data* .rodata*)
   *:pulsedata.o(.data* .rodata*)
   *:channel.o(.data* .rodata*)
   *:tpdec_asc.o(.data* .rodata*)
   *:aacdecoder.o(.data* .rodata*)
   *:channelinfo.o(.data* .rodata*)
   *:aacdecoder_lib.o(.data* .rodata*)
   *:stereo.o(.data* .rodata*)
   *:fixpoint_math.o(.data* .rodata*)
   *:FDK_bitbuffer.o(.data* .rodata*)
   *:a2dp_decoder_aac_lc.o(.rodata*)
   *:aac_com.o(.data .rodata*)
   *:aac_dec.o(.data .rodata*)
   *:aac_enc.o(.data .rodata*)
   *(.overlay_data4 .overlay_rodata4)
   . = ALIGN(4);
  }
  .overlay_data5
  {
   LONG(0);
   *:mpa.o(.data* .bss*)
   *:bit.o(.data* .bss*)
   *:frame.o(.data* .bss*)
   *:huffman.o(.data* .bss*)
   *:layer12.o(.data* .bss*)
   *:layer3.o(.data* .bss*)
   *:stream.o(.data* .bss*)
   *:synth.o(.data* .bss*)
   *(.overlay_data5 .overlay_rodata5)
   . = ALIGN(4);
  }
  .overlay_data6
  {
   LONG(0);
   *:mp3_dec.o(.data* .rodata*)
   *:mp3_enc.o(.data* .rodata*)
   *(.overlay_data6 .overlay_rodata6)
   . = ALIGN(4);
  }
  .overlay_data7
  {
   LONG(0);
   *(.overlay_data7 .overlay_rodata7)
   . = ALIGN(4);
  }
  .overlay_data8
  {
   LONG(0);
   *:ogg_com.o(.text*)
   *:vorbis_com.o(.data* .rodata*)
   *(.overlay_data8 .overlay_rodata8)
   . = ALIGN(4);
  }
  .overlay_data9
  {
   LONG(0);
   *:alac_com.o(.data* .rodata*)
   *(.overlay_data9 .overlay_rodata9)
   . = ALIGN(4);
  }
  .overlay_data10
  {
   LONG(0);
   *:flac_com.o(.data* .rodata*)
   *(.overlay_data10 .overlay_rodata10)
   . = ALIGN(4);
  }
  .overlay_data11
  {
   LONG(0);
   *(.overlay_data11 .overlay_rodata11)
   . = ALIGN(4);
  }
  .overlay_data_last
  {
   LONG(0);
   . = ALIGN(4);
  }
 } > RAMCP
 .dummy_overlay_data_end (.) (NOLOAD) : AT (ADDR(.dummy_overlay_data_end))
 {
  __overlay_data_end__ = .;
 } > RAMCP
 ASSERT(__overlay_data_end__ - __overlay_data_start__ <= 0x10000, "overlay data sections too large")
 .cp_ramx_last_dummy (NOLOAD) : AT (ADDR(.cp_ramx_last_dummy))
 {
  __cp_ramx_last_dummy_start = .;
 } > RAMCPX
 .cp_ram_last_dummy (NOLOAD) : AT (ADDR(.cp_ram_last_dummy))
 {
  . = ALIGN(8);
  __cp_ram_last_dummy_start = .;
 } > RAMCP
 __cp_stack_top = ORIGIN(RAMCP) + LENGTH(RAMCP);
 __cp_stack_limit = __cp_stack_top - 0x1000;
 /* LD v2.24 (GCC 4.9) cannot handle address conversion correctly if involving immediate value.
	   E.g., <absolute_addr> + <immediate_val> + <relative_addr> */
 __free_ramcpx = ORIGIN(RAMCPX) + LENGTH(RAMCPX) - ABSOLUTE(__cp_ramx_last_dummy_start);
 ASSERT(__cp_stack_limit >= __cp_ram_last_dummy_start, "region RAMCP overflowed with stack")
 __free_ramcp = __cp_stack_limit - __cp_ram_last_dummy_start;
 __cp_mem_pool_start__ = __cp_ram_last_dummy_start;
 __cp_mem_pool_end__ = __cp_stack_limit;
 __flash_text_data_start__ = __load_stop_overlay_data_last;
 .flashx_text (((__flash_text_data_start__) - 0x2C000000 + 0x0C000000)) : AT (__flash_text_data_start__)
 {
  __flashx_text_start__ = .;
  *:notify.o(.text*)
  *:scan.o(.text*)
  *:wifi_conf_nvrecord.o(.text*)
  *:blacklist.o(.text*)
  *:ap.o(.text*)
  *:wpa_sme.o(.text*)
  *:wpas_glue.o(.text*)
  *:offchannel.o(.text*)
  *:wpa_supplicant.o(.text*)
  *:eap_register.o(.text*)
  *:wpa_interface.o(.text*)
  *:p2p_supplicant.o(.text*)
  *:config.o(.text*)
  *:sme.o(.text*)
  *:wps_supplicant.o(.text*)
  *:bss.o(.text*)
  *:trace.o(.text*)
  *:wpa_debug.o(.text*)
  *:wpabuf.o(.text*)
  *:common.o(.text*)
  *:bitfield.o(.text*)
  *:eloop.o(.text*)
  *:wpa_os.o(.text*)
  *:uuid.o(.text*)
  *:ieee802_11_common.o(.text*)
  *:wpa_common.o(.text*)
  *:ieee802_11_auth.o(.text*)
  *:wpa_auth_glue.o(.text*)
  *:eap_user_db.o(.text*)
  *:wnm_ap.o(.text*)
  *:sta_info.o(.text*)
  *:ieee802_1x.o(.text*)
  *:ctrl_iface_ap.o(.text*)
  *:wpa_auth_ft.o(.text*)
  *:wpa_auth_ie.o(.text*)
  *:preauth_auth.o(.text*)
  *:ieee802_11_ht.o(.text*)
  *:wpa_auth.o(.text*)
  *:ap_mlme.o(.text*)
  *:wmm.o(.text*)
  *:ieee802_11.o(.text*)
  *:dfs.o(.text*)
  *:hostapd.o(.text*)
  *:ieee802_11_vht.o(.text*)
  *:wps_hostapd.o(.text*)
  *:p2p_hostapd.o(.text*)
  *:drv_callbacks.o(.text*)
  *:ap_config.o(.text*)
  *:ieee802_11_shared.o(.text*)
  *:ap_drv_ops.o(.text*)
  *:tkip_countermeasures.o(.text*)
  *:peerkey_auth.o(.text*)
  *:hw_features.o(.text*)
  *:beacon.o(.text*)
  *:pmksa_cache_auth.o(.text*)
  *:utils.o(.text*)
  *:p2p_parse.o(.text*)
  *:p2p_utils.o(.text*)
  *:p2p_invitation.o(.text*)
  *:p2p.o(.text*)
  *:p2p_build.o(.text*)
  *:p2p_group.o(.text*)
  *:p2p_pd.o(.text*)
  *:p2p_dev_disc.o(.text*)
  *:p2p_go_neg.o(.text*)
  *:pmksa_cache.o(.text*)
  *:wpa_ie.o(.text*)
  *:wpa.o(.text*)
  *:preauth.o(.text*)
  *:driver_nl80211.o(.text*)
  *:nl80211.o(.text*)
  *:wpa_events.o(.text*)
  *:eap_common.o(.text*)
  *:eap_wsc_common.o(.text*)
  *:eap.o(.text*)
  *:eap_wsc.o(.text*)
  *:eap_methods.o(.text*)
  *:eapol_auth_sm.o(.text*)
  *:eapol_auth_dump.o(.text*)
  *:eapol_supp_sm.o(.text*)
  *:wps_attr_process.o(.text*)
  *:ndef.o(.text*)
  *:wps_validate.o(.text*)
  *:wps_attr_parse.o(.text*)
  *:wps_dev_attr.o(.text*)
  *:wps.o(.text*)
  *:wps_common.o(.text*)
  *:wps_registrar.o(.text*)
  *:wps_attr_build.o(.text*)
  *:wps_enrollee.o(.text*)
  *:tlsv1_server.o(.text*)
  *:pkcs8.o(.text*)
  *:x509v3.o(.text*)
  *:tlsv1_cred.o(.text*)
  *:tlsv1_client_read.o(.text*)
  *:tlsv1_client.o(.text*)
  *:rsa.o(.text*)
  *:tlsv1_client_write.o(.text*)
  *:asn1.o(.text*)
  *:tlsv1_server_read.o(.text*)
  *:pkcs1.o(.text*)
  *:pkcs5.o(.text*)
  *:tlsv1_record.o(.text*)
  *:bignum.o(.text*)
  *:tlsv1_server_write.o(.text*)
  *:tlsv1_common.o(.text*)
  *:eap_server.o(.text*)
  *:eap_server_identity.o(.text*)
  *:eap_server_methods.o(.text*)
  *:eap_server_wsc.o(.text*)
  *:hostapd_interface.o(.text*)
  *(EXCLUDE_FILE(*hal_*.o *app_sleep.o *cmsis_nvic.o *system_ARMCM4.o *system_utils.o *system_cp.o *cp_accel.o *bpabi.o *div*.o *_dvmd_tls.o *best*.o wifi_drv*.o) .text*)
  . = ALIGN(4);
  __flashx_text_end__ = .;
 }
 __flash_text_data_start__ += SIZEOF(.flashx_text);
 __psram_text_data_start__ = ORIGIN(PSRAM);
 __psram_text_data_start_flash__ = __flash_text_data_start__;
 .psram_text (((__psram_text_data_start__) - 0x34000000 + 0x14000000)) : AT (__psram_text_data_start_flash__)
 {
/*
		__psramx_text_start__ .;

		*(EXCLUDE_FILE(*hal_*.o *cmsis_nvic.o *system_ARMCM4.o *system_utils.o *system_cp.o *cp_accel.o  *bpabi.o *div*.o *_dvmd_tls.o *best*.o) .text*)
		*(.psram_text*)
		. = ALIGN(4);
		__psramx_text_end__ = .;
*/
 } > PSRAMX
 .psram_data (__psram_text_data_start__ + SIZEOF(.psram_text)) :
    AT (__psram_text_data_start_flash__ + SIZEOF(.psram_text))
 {
  *(EXCLUDE_FILE(*hal_*.o *cmsis_nvic.o *system_ARMCM4.o *system_utils.o *system_cp.o *cp_accel.o *bpabi.o *div*.o *_dvmd_tls.o *best*.o ) .data*)
  *(.psram_data*)
  . = ALIGN(4);
 } > PSRAM
 __psram_text_data_end_flash__ = __psram_text_data_start_flash__ + SIZEOF(.psram_text)+ SIZEOF(.psram_data);
 .psram_bss (.) (NOLOAD) : AT (ADDR(.psram_bss))
 {
  __psram_bss_start__ = .;
  *(.bss*)
  *(.psram_bss*)
  *:*ffmpeg*.o(COMMON)
/*#ifdef __BT_RAMRUN__*/
  . = ALIGN(4);
  __mem_pool_start__ = .;
  . += 160*1024;
  __mem_pool_end__ = .;
/*#endif*/
  . = ALIGN(64);
  __os_heap_start__ = .;
  m_aucSysMem0 = .;
  . += 0x80000;
  __os_heap_end__ = .;
  . = ALIGN(4);
  __psram_bss_end__ = .;
 } > PSRAM
 __psram_nc_data_start_flash__ = __psram_text_data_end_flash__;
 .psram_nc_data (((.) - 0x34000000 + 0x30000000)) : AT (__psram_nc_data_start_flash__)
 {
  __psram_nc_data_start__ = .;
  *(.psram_nc_rodata*)
  . = ALIGN(32);
  *(.psram_nc_data*)
  . = ALIGN(32);
  __psram_nc_data_end__ = .;
 } > PSRAM_NC
 __psram_nc_data_end_flash__ = __psram_nc_data_start_flash__ + SIZEOF(.psram_nc_data);
 .psram_nc_bss (.) (NOLOAD) : AT (ADDR(.psram_nc_bss))
 {
  __psram_nc_bss_start__ = .;
  *(.psram_nc_bss*)
  *(.sync_flags_psram*)
  . = ALIGN(32);
  __psram_nc_bss_end__ = .;
 } > PSRAM_NC
 .psram_heap_bss (((.) - 0x30000000 + 0x34000000)) (NOLOAD) : AT (ADDR(.psram_heap_bss))
 {
  __psram_reserved_start__ = .;
  *(.psram_reserved*)
  __psram_reserved_end__ = .;
 } > PSRAM
 __flash_text_data_start__ = __psram_nc_data_end_flash__;
 .text (((__flash_text_data_start__) - 0x2C000000 + 0x0C000000)) : AT (((ADDR(.text)) - 0x0C000000 + 0x2C000000))
 {
  __text_start__ = .;
  *(.text*)
  *(.flash_text*)
  KEEP(*(.init))
  KEEP(*(.fini))
  _sinit = ABSOLUTE(.);
  /* .ctors */
  *crtbegin.o(.ctors)
  *crtbegin?.o(.ctors)
  *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
  *(SORT(.ctors.*))
  *(.ctors)
  _einit = ABSOLUTE(.);
  /* .dtors */
  *crtbegin.o(.dtors)
  *crtbegin?.o(.dtors)
  *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
  *(SORT(.dtors.*))
  *(.dtors)
  __text_end__ = .;
 } > FLASHX
 .ARM.extab (.) : AT (((ADDR(.ARM.extab)) - 0x0C000000 + 0x2C000000))
 {
  *(.ARM.extab* .gnu.linkonce.armextab.*)
 } > FLASHX
 __exidx_start = .;
 /* .ARM.exidx contains R_ARM_PREL31 (+-0x40000000) offset to functions, which means
	 * the session location cannot be too far away from the function addresses */
 .ARM.exidx (.) : AT (((ADDR(.ARM.exidx)) - 0x0C000000 + 0x2C000000))
 {
  *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  __flashx_text_end__ = .;
 } > FLASHX
 __exidx_end = .;
 __flash_text_end__ = .;
 . = ((.) - 0x0C000000 + 0x2C000000);
 .rodata_nc (((.) - 0x2C000000 + 0x28000000)) : AT (((ADDR(.rodata_nc)) - 0x28000000 + 0x2C000000))
 {
  *(.rodata_nc*)
  *(.flash_nc_rodata*)
  . = ALIGN(4);
 } > FLASH_NC
 . = ((.) - 0x28000000 + 0x2C000000);
 .rodata (.) :
 {
  *(.rodata*)
  *(.flash_rodata*)
  *(.constdata)
  . = ALIGN(4);
  /* finit data */
  PROVIDE_HIDDEN (__fini_array_start = .);
  KEEP(*(SORT(.fini_array.*)))
  KEEP(*(.fini_array))
  PROVIDE_HIDDEN (__fini_array_end = .);
  . = ALIGN(4);
  __cust_cmd_hldr_tbl_start = .;
  KEEP(*(.cust_cmd_hldr_tbl))
  __cust_cmd_hldr_tbl_end = .;
  . = ALIGN(4);
  __ai_gatt_server_table_start = .;
  KEEP(*(.ai_gatt_server_table))
  __ai_gatt_server_table_end = .;
  . = ALIGN(4);
  __ai_ble_handler_table_start = .;
  KEEP(*(.ai_ble_handler_table))
  __ai_ble_handler_table_end = .;
  . = ALIGN(4);
  __ai_spp_register_table_start = .;
  KEEP(*(.ai_spp_register_table))
  __ai_spp_register_table_end = .;
  . = ALIGN(4);
  __tota_handler_table_start = .;
  KEEP(*(.tota_handler_table))
  __tota_handler_table_end = .;
  . = ALIGN(4);
  __sound_init_table_start = .;
  KEEP(*(.sound_init_table))
  __sound_init_table_end = .;
  . = ALIGN(4);
  __ai_handler_function_table_start = .;
  KEEP(*(.ai_handler_function_table))
  __ai_handler_function_table_end = .;
  . = ALIGN(4);
  __custom_handler_table_start = .;
  KEEP(*(.custom_handler_table))
  __custom_handler_table_end = .;
  . = ALIGN(4);
  __mixprompt_property_table_start = .;
  KEEP(*(.mixprompt_property_table))
  __mixprompt_property_table_end = .;
  . = ALIGN(4);
  __core_bridge_task_cmd_table_start = .;
  KEEP(*(.core_bridge_task_cmd_table))
  __core_bridge_task_cmd_table_end = .;
  . = ALIGN(4);
  __core_bridge_instant_cmd_table_start = .;
  KEEP(*(.core_bridge_instant_cmd_table))
  __core_bridge_instant_cmd_table_end = .;
  __eshell_command_table_start = .;
  KEEP(*(.eshell_command_table))
  __eshell_command_table_end = .;
  . = ALIGN(4);
  __app_bt_sync_command_handler_table_start = .;
  KEEP(*(.app_bt_sync_command_handler_table))
  __app_bt_sync_command_handler_table_end = .;
  . = ALIGN(4);
  . = ALIGN(4);
   KEEP(*(.eh_frame*))
  *(.note.gnu.build-id)
  . = ALIGN(4);
  __besinitcall_app_feature_start = .;
  KEEP(*(.besinitcall.app.feature.init))
  __besinitcall_app_feature_end = .;
  . = ALIGN(4);
  _etext = .;
 } > FLASH
 /* zInit code and data - will be freed after init */
 .zInit (.) :
 {
  __zinitcall_bsp_start = .;
  KEEP (*(.zinitcall.bsp0.init))
  KEEP (*(.zinitcall.bsp1.init))
  KEEP (*(.zinitcall.bsp2.init))
  KEEP (*(.zinitcall.bsp3.init))
  KEEP (*(.zinitcall.bsp4.init))
  __zinitcall_bsp_end = .;
  . = ALIGN(4);
  __zinitcall_device_start = .;
  KEEP (*(.zinitcall.device0.init))
  KEEP (*(.zinitcall.device1.init))
  KEEP (*(.zinitcall.device2.init))
  KEEP (*(.zinitcall.device3.init))
  KEEP (*(.zinitcall.device4.init))
  __zinitcall_device_end = .;
  . = ALIGN(4);
  __zinitcall_core_start = .;
  KEEP (*(.zinitcall.core0.init))
  KEEP (*(.zinitcall.core1.init))
  KEEP (*(.zinitcall.core2.init))
  KEEP (*(.zinitcall.core3.init))
  KEEP (*(.zinitcall.core4.init))
  __zinitcall_core_end = .;
  . = ALIGN(4);
  __zinitcall_sys_service_start = .;
  KEEP (*(.zinitcall.sys.service0.init))
  KEEP (*(.zinitcall.sys.service1.init))
  KEEP (*(.zinitcall.sys.service2.init))
  KEEP (*(.zinitcall.sys.service3.init))
  KEEP (*(.zinitcall.sys.service4.init))
  __zinitcall_sys_service_end = .;
  . = ALIGN(4);
  __zinitcall_sys_feature_start = .;
  KEEP (*(.zinitcall.sys.feature0.init))
  KEEP (*(.zinitcall.sys.feature1.init))
  KEEP (*(.zinitcall.sys.feature2.init))
  KEEP (*(.zinitcall.sys.feature3.init))
  KEEP (*(.zinitcall.sys.feature4.init))
  __zinitcall_sys_feature_end = .;
  . = ALIGN(4);
  __zinitcall_run_start = .;
  KEEP (*(.zinitcall.run0.init))
  KEEP (*(.zinitcall.run1.init))
  KEEP (*(.zinitcall.run2.init))
  KEEP (*(.zinitcall.run3.init))
  KEEP (*(.zinitcall.run4.init))
  __zinitcall_run_end = .;
  . = ALIGN(4);
  __zinitcall_app_service_start = .;
  KEEP (*(.zinitcall.app.service0.init))
  KEEP (*(.zinitcall.app.service1.init))
  KEEP (*(.zinitcall.app.service2.init))
  KEEP (*(.zinitcall.app.service3.init))
  KEEP (*(.zinitcall.app.service4.init))
  __zinitcall_app_service_end = .;
  . = ALIGN(4);
  __zinitcall_app_feature_start = .;
  KEEP (*(.zinitcall.app.feature0.init))
  KEEP (*(.zinitcall.app.feature1.init))
  KEEP (*(.zinitcall.app.feature2.init))
  KEEP (*(.zinitcall.app.feature3.init))
         KEEP (*(.zinitcall.app.feature4.init))
  __zinitcall_app_feature_end = .;
  . = ALIGN(4);
  __zinitcall_test_start = .;
  KEEP (*(.zinitcall.test0.init))
  KEEP (*(.zinitcall.test1.init))
  KEEP (*(.zinitcall.test2.init))
  KEEP (*(.zinitcall.test3.init))
  KEEP (*(.zinitcall.test4.init))
  __zinitcall_test_end = .;
  . = ALIGN(4);
  __zinitcall_exit_start = .;
  KEEP (*(.zinitcall.exit0.init))
  KEEP (*(.zinitcall.exit1.init))
  KEEP (*(.zinitcall.exit2.init))
  KEEP (*(.zinitcall.exit3.init))
  KEEP (*(.zinitcall.exit4.init))
  __zinitcall_exit_end = .;
  . = ALIGN(4);
 } > FLASH
 .interp (.) :
 {
  KEEP(*(interp))
  . = ALIGN(4);
 } > FLASH
 .dynstr (.) :
 {
  KEEP(*(dynstr))
  . = ALIGN(4);
 } > FLASH
 /* To copy multiple FLASH to RAM sections,
	 * uncomment .copy.table section and,
	 * define __STARTUP_COPY_MULTIPLE in startup_ARMCMx.S */
 /*
	.copy.table (.) :
	{
		. = ALIGN(4);
		__copy_table_start__ = .;
		LONG (__etext)
		LONG (__data_start__)
		LONG (__data_end__ - __data_start__)
		LONG (__etext2)
		LONG (__data2_start__)
		LONG (__data2_end__ - __data2_start__)
		__copy_table_end__ = .;
	} > FLASH
	*/
 /* To clear multiple BSS sections,
	 * uncomment .zero.table section and,
	 * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_ARMCMx.S */
 /*
	.zero.table (.) :
	{
		. = ALIGN(4);
		__zero_table_start__ = .;
		LONG (__bss_start__)
		LONG (__bss_end__ - __bss_start__)
		LONG (__bss2_start__)
		LONG (__bss2_end__ - __bss2_start__)
		__zero_table_end__ = .;
	} > FLASH
	*/
 /* Location counter can end up 2byte aligned with narrow Thumb code but
	   __etext is assumed by startup code to be the LMA of a section in RAM
	   which must be 4byte aligned */
 __etext = ALIGN(4);
 /* The VMA is either the end of overlay_data or the end of sram bss */
 .data : AT (__etext)
 {
  __data_start__ = .;
  _sdata = .;
  *(vtable)
  . = ALIGN(4);
  *(.data*)
  . = ALIGN(4);
  /* preinit data */
  PROVIDE_HIDDEN (__preinit_array_start = .);
  KEEP(*(.preinit_array))
  PROVIDE_HIDDEN (__preinit_array_end = .);
  . = ALIGN(4);
  /* finit data */
  PROVIDE_HIDDEN (__fini_array_start = .);
  KEEP(*(SORT(.fini_array.*)))
  KEEP(*(.fini_array))
  PROVIDE_HIDDEN (__fini_array_end = .);
  KEEP(*(.jcr*))
  /* All data end */
  . = ALIGN(4);
  *(*.got*)
  /* section information for finsh shell */
  . = ALIGN(4);
  __fsymtab_start = .;
  KEEP(*(FSymTab))
  __fsymtab_end = .;
  . = ALIGN(4);
  __vsymtab_start = .;
  KEEP(*(VSymTab))
  __vsymtab_end = .;
  /* section information for initial. */
  . = ALIGN(4);
  __rt_init_start = .;
  KEEP(*(SORT(.rti_fn*)))
  __rt_init_end = .;
  . = ALIGN(4);
  _hdf_drivers_start = .;
  KEEP(*(.hdf.driver))
  _hdf_drivers_end = .;
  __data_end__ = .;
  _edata = .;
        __shell_root_cmds_start = .;
        KEEP(*(SORT(.shell_root_cmd_*)));
        __shell_root_cmds_end = .;
 } > RAM
 .bss (.) (NOLOAD) : AT (ADDR(.bss))
 {
  . = ALIGN(4);
  __bss_start__ = .;
  _sbss = .;
  *(.bss*)
  *(COMMON)
  . = ALIGN(4);
  __bss_end__ = .;
  _ebss = .;
 } > RAM
 .reboot_custom_param (.) (NOLOAD) :
 {
  KEEP(*(.reboot_custom_param*))
  . = ALIGN(4);
  __custom_reboot_dump_start__ = .;
  . += 0x4000;
  __custom_reboot_dump_end__ = .;
  __reboot_custom_param_end__ = .;
 } > RAM
 .heap (.) (NOLOAD) :
 {
  . = ALIGN(8);
  __HeapBase = .;
  __end__ = .;
  end = __end__;
  . += 0x2000;
  . = ALIGN(8);
  __HeapLimit = .;
 } > RAM
    /* ramv will be occupied by nts heap of cp if WIFI_ON_CP defined */
 /* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
 .stack_dummy (.) (COPY) :
 {
  . = 0x2000;
  . = ALIGN(8);
 } > RAM
 /* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
 __StackTop = ORIGIN(RAM) + LENGTH(RAM);
 __StackLimit = __StackTop - SIZEOF(.stack_dummy);
 PROVIDE(__stack = __StackTop);
 _sstack = __StackLimit;
 _estack = __stack;
 /* Check if data + heap + stack exceeds RAM limit */
 ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
 __free_ram = __StackLimit - __HeapLimit;
/*
#ifndef __BT_RAMRUN__
#ifdef MEM_POOL_BASE
	.mem_pool (ORIGIN(MEM_POOL)) (NOLOAD) :
	{
		__mem_pool_start__ = .;
		. = LENGTH(MEM_POOL);
		__mem_pool_end__ = .;
	} > MEM_POOL
#elif !(defined(CP_IN_SAME_EE) && (RAMCP_SIZE > 0) && defined(MEM_POOL_IN_CP_RAM))
	__mem_pool_start__ = __HeapLimit;
	__mem_pool_end__ = __StackLimit;
#endif
#endif
*/
 . = __etext + SIZEOF(.data);
 . = ((.) - 0x2C000000 + 0x28000000);
 .build_info (.) : AT (((ADDR(.build_info)) - 0x28000000 + 0x2C000000))
 {
  KEEP(*(.build_info))
  . = ALIGN(4);
 } > FLASH_NC = 0x00000000
 __boot_struct_build_info = ADDR(.build_info);
 . = ((.) - 0x28000000 + 0x2C000000);
 /* this section should be at last in order to keep text/data result */
 .mcu_lzma_code (.) :
 {
  *(.mcu_lzma_code*)
 } > FLASH
 /* The following section MUST be the last loaded section */
 .code_start_addr (.) :
 {
  LONG(0);
  LONG(0xBE57341D);
  LONG(((ABSOLUTE(__flash_start)) - 0x2C000000 + 0x28000000));
 } > FLASH
 __flash_end = .;
 .combo (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0 - 0 - 0 -
  0 - 0 - 0x0 - 0x0 -
  0) (NOLOAD) : AT (ADDR(.combo))
 {
  __combo_start = .;
  . = 0;
  __combo_end = .;
 } > FLASH_NC
 .prompt (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0 - 0 - 0 -
  0 - 0 - 0x0 - 0x0) (NOLOAD):
 {
  __prompt_start = .;
  . = 0x0;
  __prompt_end = .;
 } > FLASH_NC
 .hotword_model (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0 - 0 - 0 -
  0 - 0 - 0x0) (NOLOAD):
 {
  __hotword_model_start = .;
  . = 0x0;
  __hotword_model_end = .;
 } > FLASH_NC
 .coredump_section (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0 - 0 - 0 -
  0 - 0) (NOLOAD) :
 {
  __coredump_section_start = .;
  . = 0;
  __coredump_section_end = .;
 } > FLASH_NC
 .ota_upgrade_log (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0 - 0 - 0 -
  0) (NOLOAD) :
 {
  __ota_upgrade_log_start = .;
  . = 0;
  __ota_upgrade_log_end = .;
 } > FLASH_NC
 .log_dump (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0 - 0 - 0) (NOLOAD) :
 {
  __log_dump_start = .;
  . = 0;
  __log_dump_end = .;
 } > FLASH_NC
 .crash_dump (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0 - 0) (NOLOAD) :
 {
  __crash_dump_start = .;
  . = 0;
  __crash_dump_end = .;
 } > FLASH_NC
 .custom_parameter (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0x1000 -
  0) (NOLOAD) :
 {
  __custom_parameter_start = .;
  . = 0x1000;
  __custom_parameter_end = .;
 } > FLASH_NC
 .lhdc_license (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2 - 0 - 0) (NOLOAD) :
 {
  __lhdc_license_start = .;
  . = 0;
  __lhdc_license_end = .;
 } > FLASH_NC
 .userdata (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0 - 0x1000*2) (NOLOAD) :
 {
  __userdata_start = .;
  . = 0x1000*2;
  __userdata_end = .;
 } > FLASH_NC
 .audio (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000 -
  0) (NOLOAD) :
 {
  __aud_start = .;
  . = 0;
  __aud_end = .;
 } > FLASH_NC
 .reserved (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000 - 0x1000) (NOLOAD) :
 {
  __reserved_start = .;
  . = 0x1000;
  __reserved_end = .;
 } > FLASH_NC
 .factory (ORIGIN(FLASH_NC) + LENGTH(FLASH_NC) - 0x1000) (NOLOAD) :
 {
  __factory_start = .;
  . = 0x1000;
  __factory_end = .;
 } > FLASH_NC
 __tail_section_start = __prompt_start;
 ASSERT(((__tail_section_start) - 0x28000000 + 0x2C000000) >= __flash_end, "region FLASH overflowed")
 __free_flash = ((__tail_section_start) - 0x28000000 + 0x2C000000) - __flash_end;
}

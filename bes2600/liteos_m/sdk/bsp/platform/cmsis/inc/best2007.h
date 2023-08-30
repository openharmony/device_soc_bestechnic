/**************************************************************************//**
 * @file     best2007.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           ARMCM4 Device Series
 * @version  V2.02
 * @date     10. September 2014
 *
 * @note     configured for CM4 with FPU
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2014 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef __BEST2007_H__
#define __BEST2007_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ASSEMBLER__
/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Cortex-M33 Processor Exceptions Numbers  ------------------ */
    NonMaskableInt_IRQn         = -14,      /*!<  2 Non Maskable Interrupt          */
    HardFault_IRQn              = -13,      /*!<  3 HardFault Interrupt             */
    MemoryManagement_IRQn       = -12,      /*!<  4 Memory Management Interrupt     */
    BusFault_IRQn               = -11,      /*!<  5 Bus Fault Interrupt             */
    UsageFault_IRQn             = -10,      /*!<  6 Usage Fault Interrupt           */
    SVCall_IRQn                 =  -5,      /*!< 11 SV Call Interrupt               */
    DebugMonitor_IRQn           =  -4,      /*!< 12 Debug Monitor Interrupt         */
    PendSV_IRQn                 =  -2,      /*!< 14 Pend SV Interrupt               */
    SysTick_IRQn                =  -1,      /*!< 15 System Tick Interrupt           */

/* ----------------------  Chip Specific Interrupt Numbers  ----------------------- */
    FPU_IRQn                    =   0,      /*!< FPU Interrupt                      */
    WAKEUP_IRQn                 =   1,      /*!< Wakeup Interrupt                   */
    AUDMA_IRQn                  =   5,      /*!< Audio DMA Interrupt                */
    USB_IRQn                    =   7,      /*!< USB Interrupt                      */
    QSPI_IRQn                   =   8,      /*!< QSPI Interrupt                     */
    USB_EXT_IRQn                =   9,      /*!< USB EXT Interrupt                  */
    IR_IRQn                     =  10,      /*!< IR Interrupt                       */
    SDIO_DEVICE_IRQn            =  11,      /*!< SDOP Interrupt                     */
    USB_AUX_IRQn                =  12,      /*!< USB AUX Interrupt                  */
    DUMP_IRQn                   =  13,      /*!< DUMP Interrupt                     */
    MCU_WDT_IRQn                =  14,      /*!< Watchdog Timer Interrupt           */
    MCU_TIMER00_IRQn            =  15,      /*!< Timer00 Interrupt                  */
    MCU_TIMER01_IRQn            =  16,      /*!< Timer01 Interrupt                  */
    MCU_TIMER10_IRQn            =  17,      /*!< Timer10 Interrupt                  */
    MCU_TIMER11_IRQn            =  18,      /*!< Timer11 Interrupt                  */
    I2C0_IRQn                   =  21,      /*!< I2C0 Interrupt                     */
    I2C1_IRQn                   =  22,      /*!< I2C1 Interrupt                     */
    SPI0_IRQn                   =  23,      /*!< SPI0 Interrupt                     */
    SPILCD_IRQn                 =  24,      /*!< SPILCD Interrupt                   */
    CAN_IRQn                    =  25,      /*!< CAN Interrupt                      */
    UART0_IRQn                  =  27,      /*!< UART0 Interrupt                    */
    UART1_IRQn                  =  28,      /*!< UART1 Interrupt                    */
    UART2_IRQn                  =  29,      /*!< UART2 Interrupt                    */
    BTPCM_IRQn                  =  30,      /*!< BTPCM Interrupt                    */
    I2S0_IRQn                   =  31,      /*!< I2S0 Interrupt                     */
    SPDIF0_IRQn                 =  32,      /*!< SPDIF0 Interrupt                   */
    CMU_SLEEP_IRQn              =  34,      /*!< CMU SLEEP Interrupt                */
    AON_GPIO_IRQn               =  35,      /*!< AON GPIO Interrupt                 */
    AON_WDT_IRQn                =  36,      /*!< AON Watchdog Timer Interrupt       */
    AON_TIMER00_IRQn            =  37,      /*!< AON Timer00 Interrupt              */
    AON_TIMER01_IRQn            =  38,      /*!< AON Timer01 Interrupt              */
    WF2MCU_LL_TX_IRQn           =  39,      /*!< WF2MCU LOWLATENCY TX Interrupt     */
    WF2MCU_LL_TXDEAD_IRQn       =  40,      /*!< WF2MCU LOWLATENCY TXDEAD Interrupt */
    WF2MCU_LL_RX_IRQn           =  41,      /*!< WF2MCU LOWLATENCY RX Interrupt     */
    MCUT2BT_DONE3_IRQn          =  42,      /*!< MCUT2BT DATA3 DONE Interrupt       */
    MCUT2BT_DONE2_IRQn          =  43,      /*!< MCUT2BT DATA2 DONE Interrupt       */
    BT2MCU_DATA3_IRQn           =  44,      /*!< BT2MCU DATA3 IND Interrupt         */
    BT2MCU_DATA2_IRQn           =  45,      /*!< BT2MCU DATA2 IND Interrupt         */
    MCU2WIFI_DONE1_IRQn         =  48,      /*!< MCUT2WIFI DATA1 DONE Interrupt     */
    MCU2WIFI_DONE_IRQn          =  49,      /*!< MCUT2WIFI DATA DONE Interrupt      */
    WIFI2MCU_DATA1_IRQn         =  50,      /*!< WIFI2MCU DATA1 IND Interrupt       */
    WIFI2MCU_DATA_IRQn          =  51,      /*!< WIFI2MCU DATA IND Interrupt        */
    WIFI_IRQn                   =  52,      /*!< WIFI to MCU Interrupt              */
    ISDONE_IRQn                 =  53,      /*!< Intersys MCU2BT Data Done Interrupt */
    ISDONE1_IRQn                =  54,      /*!< Intersys MCU2BT Data1 Done Interrupt */
    ISDATA_IRQn                 =  55,      /*!< Intersys BT2MCU Data Indication Interrupt */
    ISDATA1_IRQn                =  56,      /*!< Intersys BT2MCU Data1 Indication Interrupt */
    BT_IRQn                     =  57,      /*!< BT to MCU Interrupt                */
    USB_PIN_IRQn                =  58,      /*!< USB INT PMU Interrupt              */
    RTC_IRQn                    =  59,      /*!< RTC Interrupt                      */
    GPADC_IRQn                  =  60,      /*!< GPADC Interrupt                    */
    CHARGER_IRQn                =  61,      /*!< Charger Interrupt                  */
    PWRKEY_IRQn                 =  62,      /*!< Power key Interrupt                */
    WIFIDUMP_IRQn               =  63,      /*!< WIFIDUMP Interrupt                 */
    CHKSUM_IRQn                 =  64,      /*!< Checksum Interrupt                 */
    CRC_IRQn                    =  65,      /*!< CRC Interrupt                      */
    BT_STAMP_IRQn               =  69,      /*!< BT Playtime Stamp Interrupt        */
    WIFI_HOST_IRQn              =  70,      /*!< WIFI IPC Interrupt                 */
    BTTIME_IRQn                 =  71,      /*!< BTTIME Interrupt                   */

    USER_IRQn_QTY,
    INVALID_IRQn                = USER_IRQn_QTY,
} IRQn_Type;


#define GPIO_IRQn               AON_GPIO_IRQn
#define GPIOAUX_IRQn            AON_GPIOAUX_IRQn
#define WDT_IRQn                AON_WDT_IRQn

#include "plat_addr_map.h"

#if (TIMER0_BASE == AON_TIMER_BASE)
#define TIMER00_IRQn            AON_TIMER00_IRQn
#define TIMER01_IRQn            AON_TIMER01_IRQn
#elif (TIMER0_BASE == MCU_TIMER0_BASE)
#define TIMER00_IRQn            MCU_TIMER00_IRQn
#define TIMER01_IRQn            MCU_TIMER01_IRQn
#elif (TIMER0_BASE == MCU_TIMER1_BASE)
#define TIMER00_IRQn            MCU_TIMER10_IRQn
#define TIMER01_IRQn            MCU_TIMER11_IRQn
#elif (TIMER0_BASE == MCU_TIMER2_BASE)
#define TIMER00_IRQn            MCU_TIMER20_IRQn
#define TIMER01_IRQn            MCU_TIMER21_IRQn
#endif

#if (TIMER1_BASE == MCU_TIMER0_BASE)
#define TIMER10_IRQn            MCU_TIMER00_IRQn
#define TIMER11_IRQn            MCU_TIMER01_IRQn
#elif (TIMER1_BASE == MCU_TIMER1_BASE)
#define TIMER10_IRQn            MCU_TIMER10_IRQn
#define TIMER11_IRQn            MCU_TIMER11_IRQn
#elif (TIMER1_BASE == MCU_TIMER2_BASE)
#define TIMER10_IRQn            MCU_TIMER20_IRQn
#define TIMER11_IRQn            MCU_TIMER21_IRQn
#endif

#if (TIMER2_BASE == MCU_TIMER0_BASE)
#define TIMER20_IRQn            MCU_TIMER00_IRQn
#define TIMER21_IRQn            MCU_TIMER01_IRQn
#elif (TIMER2_BASE == MCU_TIMER1_BASE)
#define TIMER20_IRQn            MCU_TIMER10_IRQn
#define TIMER21_IRQn            MCU_TIMER11_IRQn
#elif (TIMER2_BASE == MCU_TIMER2_BASE)
#define TIMER20_IRQn            MCU_TIMER20_IRQn
#define TIMER21_IRQn            MCU_TIMER21_IRQn
#endif

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __CM33_REV                0x0000U   /* Core revision r0p1 */
#define __SAUREGION_PRESENT       1U        /* SAU regions present */
#define __MPU_PRESENT             1U        /* MPU present */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U        /* FPU present */
#define __DSP_PRESENT             1U        /* DSP extension present */

#ifdef ARM_CMNS
#if defined(RTOS) && defined(KERNEL_RTX5)
#define RTE_CMSIS_RTOS2_RTX5_ARMV8M_NS
#endif
#endif

#include "core_cm33.h"                      /* Processor and core peripherals */

#ifndef __ASSEMBLER__

#include "system_ARMCM.h"                  /* System Header                                     */

#endif

/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */

/* -------------------  Start of section using anonymous unions  ------------------ */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/* --------------------  End of section using anonymous unions  ------------------- */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif

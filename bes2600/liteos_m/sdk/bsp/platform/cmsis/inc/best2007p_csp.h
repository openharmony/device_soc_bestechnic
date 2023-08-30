/**************************************************************************//**
 * @file     best2007p_cp.h
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


#ifndef __BEST2007P_CSP_H__
#define __BEST2007P_CSP_H__

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
    AUDMA_IRQn                  =   2,      /*!< Audio DMA Interrupt                */
    CSP_WDT_IRQn                =   3,      /*!< Watchdog Timer Interrupt           */
    CSP_TIMER00_IRQn            =   4,      /*!< Timer00 Interrupt                  */
    CSP_TIMER01_IRQn            =   5,      /*!< Timer01 Interrupt                  */
    CSP_TIMER10_IRQn            =   6,      /*!< Timer10 Interrupt                  */
    CSP_TIMER11_IRQn            =   7,      /*!< Timer11 Interrupt                  */
    I2C0_IRQn                   =   8,      /*!< I2C0 Interrupt                     */
    SPI0_IRQn                   =   9,      /*!< SPI0 Interrupt                     */
    UART0_IRQn                  =  10,      /*!< UART0 Interrupt                    */
    CMU_SLEEP_IRQn              =  11,      /*!< CMU SLEEP Interrupt                */
    AON_GPIO_IRQn               =  12,      /*!< AON GPIO Interrupt                 */
    AON_WDT_IRQn                =  13,      /*!< AON Watchdog Timer Interrupt       */
    AON_TIMER00_IRQn            =  14,      /*!< AON Timer00 Interrupt              */
    AON_TIMER01_IRQn            =  15,      /*!< AON Timer01 Interrupt              */
    RTC_IRQn                    =  16,      /*!< RTC Interrupt                      */
    GPADC_IRQn                  =  17,      /*!< GPADC Interrupt                    */
    PWRKEY_IRQn                 =  18,      /*!< Power key Interrupt                */
    CSP2MCU_DATA_IRQn           =  19,      /*!< Intersys CSP2MCU Data Indication Interrupt */
    CSP2MCU_DATA1_IRQn          =  20,      /*!< Intersys CSP2MCU Data1 Indication Interrupt */
    MCU2CSP_DONE_IRQn           =  21,      /*!< Intersys MCU2CSP Data Done Interrupt */
    MCU2CSP_DONE1_IRQn          =  22,      /*!< Intersys MCU2CSP Data Done Interrupt */
    CROSSFEM_IREQ0              =  32,
    CROSSFEM_IREQ1              =  33,
    CROSSFEM_IREQ2              =  34,
    CROSSFEM_IREQ3              =  35,
    CROSSFEM_IREQ4              =  36,
    CROSSFEM_IREQ5              =  37,
    CROSSTUNER_IREQ0            =  38,
    CROSSTUNER_IREQ1            =  39,
    CROSSTUNER_IREQ2            =  40,
    CROSSTUNER_IREQ3            =  41,
    CROSSTUNER_IREQ4            =  42,

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
#elif (TIMER0_BASE == CSP_TIMER0_BASE)
#define TIMER00_IRQn            CSP_TIMER00_IRQn
#define TIMER01_IRQn            CSP_TIMER01_IRQn
#elif (TIMER0_BASE == CSP_TIMER1_BASE)
#define TIMER00_IRQn            CSP_TIMER10_IRQn
#define TIMER01_IRQn            CSP_TIMER11_IRQn
#endif

#if (TIMER1_BASE == CSP_TIMER0_BASE)
#define TIMER10_IRQn            CSP_TIMER00_IRQn
#define TIMER11_IRQn            CSP_TIMER01_IRQn
#elif (TIMER1_BASE == CSP_TIMER1_BASE)
#define TIMER10_IRQn            CSP_TIMER10_IRQn
#define TIMER11_IRQn            CSP_TIMER11_IRQn
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

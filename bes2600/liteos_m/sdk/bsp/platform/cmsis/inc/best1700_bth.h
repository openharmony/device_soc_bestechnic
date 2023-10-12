/**************************************************************************//**
 * @file     best1700_bth.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           ARMCM33 Device Series
 * @version  V2.02
 * @date     10. September 2014
 *
 * @note     configured for CM33 with FPU
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2014 ARM LIMITED
   Copyright (c) 2021 - 2022 BES

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


#ifndef __BEST1700_BTH_H__
#define __BEST1700_BTH_H__

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

/* --------------------  BEST1700 Specific Interrupt Numbers  --------------------- */
    FPU_IRQn                    =   0,      /*!< FPU Interrupt                      */
    BT_STAMP_IRQn               =   1,      /*!< BT Playtime Stamp Interrupt        */
    AON_IRQn                    =   2,      /*!< AON Interrupt                      */
    BT_IRQn                     =   3,      /*!< BT All Interrupt                   */
    SYS_IRQn                    =   4,      /*!< SYS All Interrupt                  */
    WIFI_IRQn                   =   5,      /*!< WIFI All Interrupt                 */
    BTHC1_IRQn                  =   6,      /*!< The Other Core All Interrupt       */
    WAKEUP_IRQn                 =   7,      /*!< Wakeup Interrupt                   */
    ISDONE3_IRQn                = WAKEUP_IRQn,
    AON_WDT_IRQn                =   8,      /*!< AON Watchdog Timer Interrupt       */
    BTH_WDT_IRQn                =   9,      /*!< Watchdog Timer Interrupt           */
    DSPC0_WDT_IRQn              =  10,      /*!< Watchdog Timer Interrupt           */
    M55C0_WDT_IRQn              =  11,      /*!< Watchdog Timer Interrupt           */
    SENS_WDT_IRQn               =  12,      /*!< Watchdog Timer Interrupt           */
    BTC_WDT_IRQn                =  13,      /*!< Watchdog Timer Interrupt           */
    WIFI_WDT_IRQn               =  14,      /*!< Watchdog Timer Interrupt           */
    AON_TIMER00_IRQn            =  15,      /*!< AON Timer00 Interrupt              */
    AON_TIMER01_IRQn            =  16,      /*!< AON Timer01 Interrupt              */
    AON_TIMER10_IRQn            =  17,      /*!< AON Timer10 Interrupt              */
    AON_TIMER11_IRQn            =  18,      /*!< AON Timer11 Interrupt              */
    AON_TIMER20_IRQn            =  19,      /*!< AON Timer20 Interrupt              */
    AON_TIMER21_IRQn            =  20,      /*!< AON Timer21 Interrupt              */
    AON_TIMER30_IRQn            =  21,      /*!< AON Timer30 Interrupt              */
    AON_TIMER31_IRQn            =  22,      /*!< AON Timer31 Interrupt              */
    BTH_TIMER00_IRQn            =  23,      /*!< Timer00 Interrupt                  */
    BTH_TIMER01_IRQn            =  24,      /*!< Timer01 Interrupt                  */
    BTH_TIMER10_IRQn            =  25,      /*!< Timer10 Interrupt                  */
    BTH_TIMER11_IRQn            =  26,      /*!< Timer11 Interrupt                  */
    BTH_TIMER20_IRQn            =  27,      /*!< Timer20 Interrupt                  */
    BTH_TIMER21_IRQn            =  28,      /*!< Timer21 Interrupt                  */
    GPDMA_IRQn                  =  29,      /*!< General Purpose DMA Interrupt      */
    AUDMA_IRQn                  =  30,      /*!< Audio DMA Interrupt                */
    SPI_WAKEUP_IRQn             =  31,      /*!< SPI Slave Wakeup Interrupt         */
    CODEC_IRQn                  =  32,      /*!< CODEC Interrupt                    */
    CODEC_TX_PEAK_IRQn          =  33,      /*!< CODEC TX PEAK Interrupt            */
    BTPCM_IRQn                  =  34,      /*!< BTPCM Interrupt                    */
    I2S0_IRQn                   =  35,      /*!< I2S0 Interrupt                     */
    I2S1_IRQn                   =  36,      /*!< I2S1 Interrupt                     */
    SPI0_IRQn                   =  37,      /*!< SPI0 Interrupt                     */
    ISDATA2_IRQn                = SPI0_IRQn,
    SPILCD_IRQn                 =  38,      /*!< SPILCD Interrupt                   */
    ISDATA3_IRQn                = SPILCD_IRQn,
    I2C0_IRQn                   =  39,      /*!< I2C0 Interrupt                     */
    I2C1_IRQn                   =  40,      /*!< I2C1 Interrupt                     */
    I2C2_IRQn                   =  41,      /*!< I2C2 Interrupt                     */
    I2C3_IRQn                   =  42,      /*!< I2C3 Interrupt                     */
    I2C4_IRQn                   =  43,      /*!< I2C4 Interrupt                     */
    UART0_IRQn                  =  44,      /*!< UART0 Interrupt                    */
    UART1_IRQn                  =  45,      /*!< UART1 Interrupt                    */
    UART2_IRQn                  =  46,      /*!< UART2 Interrupt                    */
    SPDIF0_IRQn                 =  47,      /*!< SPDIF0 Interrupt                   */
    SENSOR_ENG0_IRQn            =  48,      /*!< Sensor Engine Interrupt            */
    SENSOR_ENG1_IRQn            =  49,      /*!< Sensor Engine Interrupt            */
    SENSOR_ENG2_IRQn            =  50,      /*!< Sensor Engine Interrupt            */
    SENSOR_ENG3_IRQn            =  51,      /*!< Sensor Engine Interrupt            */
    LM_SENSOR_IRQn              =  52,      /*!< LM Sensor Interrupt                */
    PMU_IRQn                    =  53,      /*!< PMU Interrupt                      */
    PMU1_IRQn                   =  54,      /*!< PMU1 Interrupt                     */
    DUMP_IRQn                   =  55,      /*!< DUMP Interrupt                     */
    BTTIME_IRQn                 =  56,      /*!< BT Time Interrupt                     */
    SENS2MCU_DATA_IRQn          =  57,      /*!< Intersys SENS2BTH Data Indication Interrupt */
    SENS2MCU_DATA1_IRQn         =  58,      /*!< Intersys SENS2BTH Data Indication Interrupt */
    SENS2MCU_DATA2_IRQn         =  59,      /*!< Intersys SENS2BTH Data Indication Interrupt */
    SENS2MCU_DATA3_IRQn         =  60,      /*!< Intersys SENS2BTH Data Indication Interrupt */
    MCU2SENS_DONE_IRQn          =  61,      /*!< Intersys BTH2SENS Data Done Interrupt */
    MCU2SENS_DONE1_IRQn         =  62,      /*!< Intersys BTH2SENS Data Done Interrupt */
    MCU2SENS_DONE2_IRQn         =  63,      /*!< Intersys BTH2SENS Data Done Interrupt */
    MCU2SENS_DONE3_IRQn         =  64,      /*!< Intersys BTH2SENS Data Done Interrupt */
    BTH2SYS_DONE_IRQn           =  65,      /*!< Intersys SYS2BTH Data Done Interrupt */
    BTH2SYS_DONE1_IRQn          =  66,      /*!< Intersys SYS2BTH Data Done Interrupt */
    BTH2SYS_DONE2_IRQn          =  67,      /*!< Intersys SYS2BTH Data Done Interrupt */
    BTH2SYS_DONE3_IRQn          =  68,      /*!< Intersys SYS2BTH Data Done Interrupt */
    BTH2SYS_DONE4_IRQn          =  69,      /*!< Intersys SYS2BTH Data Done Interrupt */
    BTH2SYS_DONE5_IRQn          =  70,      /*!< Intersys SYS2BTH Data Done Interrupt */
    RESERVED_71_IRQn            =  71,      /*!< Reserved Interrupt                 */
    RESERVED_72_IRQn            =  72,      /*!< Reserved Interrupt                 */
    SYS2BTH_DATA_IRQn           =  73,      /*!< Intersys BTH2SYS Data Indication Interrupt */
    SYS2BTH_DATA1_IRQn          =  74,      /*!< Intersys BTH2SYS Data1 Indication Interrupt */
    SYS2BTH_DATA2_IRQn          =  75,      /*!< Intersys BTH2SYS Data1 Indication Interrupt */
    SYS2BTH_DATA3_IRQn          =  76,      /*!< Intersys BTH2SYS Data1 Indication Interrupt */
    SYS2BTH_DATA4_IRQn          =  77,      /*!< Intersys BTH2SYS Data1 Indication Interrupt */
    SYS2BTH_DATA5_IRQn          =  78,      /*!< Intersys BTH2SYS Data1 Indication Interrupt */
    RESERVED_79_IRQn            =  79,      /*!< Reserved Interrupt                 */
    RESERVED_80_IRQn            =  80,      /*!< Reserved Interrupt                 */
    ISDONE_IRQn                 =  81,      /*!< Intersys MCU2BT Data Done Interrupt */
    ISDONE1_IRQn                =  82,      /*!< Intersys MCU2BT Data1 Done Interrupt */
    ISDATA_IRQn                 =  83,      /*!< Intersys BT2MCU Data Indication Interrupt */
    ISDATA1_IRQn                =  84,      /*!< Intersys BT2MCU Data1 Indication Interrupt */
    MCU2WIFI_DONE_IRQn          =  85,      /*!< Intersys BTH2WIFI Data Done Interrupt */
    MCU2WIFI_DONE1_IRQn         =  86,      /*!< Intersys BTH2WIFI Data Done Interrupt */
    WIFI2MCU_DATA_IRQn          =  87,      /*!< Intersys WIFI2BTH Data Indication Interrupt */
    WIFI2MCU_DATA1_IRQn         =  88,      /*!< Intersys WIFI2BTH Data Indication Interrupt */
    SYS_USB_IRQn                =  89,      /*!< SYS USB Interrupt                  */
    SYS_USB_PIN_IRQn            =  90,      /*!< SYS_USB_PIN Interrupt              */
    SYS_SEC_ENG_IRQn            =  91,      /*!< SYS SEC ENG Interrupt              */
    SYS_SEDMA_IRQn              =  92,      /*!< SYS SEDMA Interrupt                */
    SHANHAI_NS_IRQn             =  93,      /*!< Shanhai NS Interrupt               */
    SHANHAI_S_IRQn              =  94,      /*!< Shanhai S Interrupt                */
    SYS_SDMMC_IRQn              =  95,      /*!< SYS SDMMC or EMMC Interrupt        */
    SYS_EMMC_IRQn               =  96,      /*!< SYS EMMC Interrupt                 */
    SYS_DISPLAY_IRQn            =  97,      /*!< SYS DISPLAY Interrupt              */
    SYS_DSI_IRQn                =  98,      /*!< SYS DSI Interrupt                  */
    ISDONE2_IRQn                =  99,      /*!< Intersys MCU2BT Interrupt          */
    SDMMC0_IRQn                 = 100,      /*!< SDMMC0 Interrupt                   */
    SYS_PNG_IRQn                = 101,      /*!< SYS DSI Interrupt                  */
    SYS_IMGDMA_IRQn             = 102,      /*!< SYS IMGDMA or GPU Interrupt        */
    SYS_GPU_IRQn                = 103,      /*!< SYS GPU Interrupt                  */
    SYS_DMA0_IRQn               = 104,      /*!< SYS DMA Interrupt                  */
    SYS_DMA1_IRQn               = 105,      /*!< SYS DMA Interrupt                  */
    SYS_DMA2_IRQn               = 106,      /*!< SYS DMA Interrupt                  */
    WIFI_HOST_IRQn              = 107,      /*!< WIFI Host Interrupt                */
    SYS_I2C0_IRQn               = 108,      /*!< SYS I2C Interrupt                  */
    SYS_I2C1_IRQn               = 109,      /*!< SYS I2C Interrupt                  */
    SYS_I2C2_IRQn               = 110,      /*!< SYS I2C Interrupt                  */
    SYS_I2C3_IRQn               = 111,      /*!< SYS I2C Interrupt                  */
    SYS_I2C4_IRQn               = 112,      /*!< SYS I2C Interrupt                  */
    SYS_UART0_IRQn              = 113,      /*!< SYS UART Interrupt                 */
    SYS_UART1_IRQn              = 114,      /*!< SYS UART Interrupt                 */
    SYS_UART2_IRQn              = 115,      /*!< SYS UART Interrupt                 */
    SYS_MMU_IRQn                = 116,      /*!< SYS MMU Interrupt                  */
    SYS_SCI_IRQn                = 117,      /*!< SYS SCI Interrupt                  */
    PAGE_SPY_IRQn               = 118,      /*!< Page Spy Interrupt                 */
    CMU_SLEEP_IRQn              = 119,      /*!< CMU Sleep Interrupt                */
    M55C1_WDT_IRQn              = 120,      /*!< Watchdog Timer Interrupt           */
    SYS_JPEG_IRQn               = 121,      /*!< SYS JPEG Interrupt                 */
    CORDIC_IRQn                 = 122,      /*!< Cordic Interrupt                   */
    SYS_I3C0_IRQn               = 123,      /*!< SYS I3C0 Interrupt                 */
    I3C0_IRQn                   = 124,      /*!< I3C0 Interrupt                     */
    RESERVED_125_IRQn           = 125,      /*!< Reserved Interrupt                 */
    WIFI_TSF_IRQn               = 126,      /*!< WIFI TSF Interrupt                 */

    USER_IRQn_QTY,
    INVALID_IRQn                = USER_IRQn_QTY,
} IRQn_Type;

#define CP2MCU_DATA_IRQn        SENS2MCU_DATA_IRQn
#define CP2MCU_DATA1_IRQn       SENS2MCU_DATA1_IRQn
#define CP2MCU_DATA2_IRQn       SENS2MCU_DATA2_IRQn
#define CP2MCU_DATA3_IRQn       SENS2MCU_DATA3_IRQn
#define MCU2CP_DONE_IRQn        MCU2SENS_DONE_IRQn
#define MCU2CP_DONE1_IRQn       MCU2SENS_DONE1_IRQn
#define MCU2CP_DONE2_IRQn       MCU2SENS_DONE2_IRQn
#define MCU2CP_DONE3_IRQn       MCU2SENS_DONE3_IRQn

#ifdef CHIP_ROLE_CP
#define SENS2SYS_DATA_IRQn      SYS2BTH_DATA_IRQn
#define SENS2SYS_DATA1_IRQn     SYS2BTH_DATA1_IRQn
#define SENS2SYS_DATA2_IRQn     SYS2BTH_DATA2_IRQn
#define SENS2SYS_DATA3_IRQn     SYS2BTH_DATA3_IRQn
#define SENS2SYS_DATA4_IRQn     SYS2BTH_DATA4_IRQn
#define SENS2SYS_DATA5_IRQn     SYS2BTH_DATA5_IRQn
#define SENS2SYS_DATA6_IRQn     SYS2BTH_DATA6_IRQn
#define SENS2SYS_DATA7_IRQn     SYS2BTH_DATA7_IRQn
#define SYS2SENS_DONE_IRQn      BTH2SYS_DONE_IRQn
#define SYS2SENS_DONE1_IRQn     BTH2SYS_DONE1_IRQn
#define SYS2SENS_DONE2_IRQn     BTH2SYS_DONE2_IRQn
#define SYS2SENS_DONE3_IRQn     BTH2SYS_DONE3_IRQn
#define SYS2SENS_DONE4_IRQn     BTH2SYS_DONE4_IRQn
#define SYS2SENS_DONE5_IRQn     BTH2SYS_DONE5_IRQn
#define SYS2SENS_DONE6_IRQn     BTH2SYS_DONE6_IRQn
#define SYS2SENS_DONE7_IRQn     BTH2SYS_DONE7_IRQn
#else
#define BTH2SYS_DATA_IRQn       SYS2BTH_DATA_IRQn
#define BTH2SYS_DATA1_IRQn      SYS2BTH_DATA1_IRQn
#define BTH2SYS_DATA2_IRQn      SYS2BTH_DATA2_IRQn
#define BTH2SYS_DATA3_IRQn      SYS2BTH_DATA3_IRQn
#define BTH2SYS_DATA4_IRQn      SYS2BTH_DATA4_IRQn
#define BTH2SYS_DATA5_IRQn      SYS2BTH_DATA5_IRQn
#define BTH2SYS_DATA6_IRQn      SYS2BTH_DATA6_IRQn
#define BTH2SYS_DATA7_IRQn      SYS2BTH_DATA7_IRQn
#define SYS2BTH_DONE_IRQn       BTH2SYS_DONE_IRQn
#define SYS2BTH_DONE1_IRQn      BTH2SYS_DONE1_IRQn
#define SYS2BTH_DONE2_IRQn      BTH2SYS_DONE2_IRQn
#define SYS2BTH_DONE3_IRQn      BTH2SYS_DONE3_IRQn
#define SYS2BTH_DONE4_IRQn      BTH2SYS_DONE4_IRQn
#define SYS2BTH_DONE5_IRQn      BTH2SYS_DONE5_IRQn
#define SYS2BTH_DONE6_IRQn      BTH2SYS_DONE6_IRQn
#define SYS2BTH_DONE7_IRQn      BTH2SYS_DONE7_IRQn
#endif

#define GPIO_IRQn               AON_IRQn
#define GPADC_IRQn              INVALID_IRQn
#define PWRKEY_IRQn             INVALID_IRQn
#ifdef CHIP_ROLE_CP
#define DMA0_IRQn               GPDMA_IRQn
#define TIMER00_IRQn            AON_TIMER20_IRQn
#define TIMER01_IRQn            AON_TIMER21_IRQn
#define TIMER10_IRQn            BTH_TIMER20_IRQn
#define TIMER11_IRQn            BTH_TIMER21_IRQn
#else
#define DMA0_IRQn               AUDMA_IRQn
#ifndef CP_AS_SUBSYS
#define DMA1_IRQn               GPDMA_IRQn
#endif
#define TIMER00_IRQn            AON_TIMER20_IRQn
#define TIMER01_IRQn            AON_TIMER21_IRQn
#define TIMER10_IRQn            BTH_TIMER00_IRQn
#define TIMER11_IRQn            BTH_TIMER01_IRQn
#define TIMER20_IRQn            BTH_TIMER10_IRQn
#define TIMER21_IRQn            BTH_TIMER11_IRQn
#endif
#define WDT_IRQn                AON_WDT_IRQn
#define DSP_WDT_IRQn            DSPC0_WDT_IRQn

#define USB_PIN_IRQn            SYS_USB_PIN_IRQn

#ifdef BTH_USE_SYS_PERIPH
#define EMMC_IRQn               SYS_EMMC_IRQn
#define USB_IRQn                SYS_USB_IRQn
#define SEDMA_IRQn              SYS_SEDMA_IRQn
#define SEC_ENG_IRQn            SYS_SEC_ENG_IRQn
#define SDMMC1_IRQn             SYS_SDMMC_IRQn
#endif

#endif

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __CM33_REV                0x0000U   /* Core revision r0p1 */
#define __MPU_PRESENT             1U        /* MPU present */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U        /* FPU present */
#define __DSP_PRESENT             1U        /* DSP extension present */
#define __SAUREGION_PRESENT       1U        /* SAU regions present */

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

/**************************************************************************//**
 * @file     best1700.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           ARMCM55 Device Series (configured for ARMCM55 with double precision FPU,
 *                                  DSP extension, MVE, TrustZone)
 * @version  V1.0.0
 * @date     27. March 2020
 ******************************************************************************/
/*
 * Copyright (c) 2020 Arm Limited. All rights reserved.
   Copyright (c) 2021 - 2022 BES
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __BEST1700_H__
#define __BEST1700_H__

#ifdef CHIP_SUBSYS_BTH

#include _TO_STRING(CONCAT_NAME(CHIP_ID_LITERAL, _bth.h))

#elif defined(CHIP_SUBSYS_DSP)

#include _TO_STRING(CONCAT_NAME(CHIP_ID_LITERAL, _dsp.h))

#else

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ASSEMBLER__
/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
    NonMaskableInt_IRQn         = -14,      /*!<  2 Non Maskable Interrupt          */
    HardFault_IRQn              = -13,      /*!<  3 HardFault Interrupt             */
    MemoryManagement_IRQn       = -12,      /*!<  4 Memory Management Interrupt     */
    BusFault_IRQn               = -11,      /*!<  5 Bus Fault Interrupt             */
    UsageFault_IRQn             = -10,      /*!<  6 Usage Fault Interrupt           */
    SecureFault_IRQn            =  -9,      /*!<  7 Secure Fault Interrupt          */
    SVCall_IRQn                 =  -5,      /*!< 11 SV Call Interrupt               */
    DebugMonitor_IRQn           =  -4,      /*!< 12 Debug Monitor Interrupt         */
    PendSV_IRQn                 =  -2,      /*!< 14 Pend SV Interrupt               */
    SysTick_IRQn                =  -1,      /*!< 15 System Tick Interrupt           */

/* -------------------  Processor Interrupt Numbers  ------------------------------ */
    FPU_IRQn                    =   0,      /*!< FPU Interrupt                      */
    BT_STAMP_IRQn               =   1,      /*!< BT Playtime Stamp Interrupt        */
    AON_IRQn                    =   2,      /*!< AON Interrupt                      */
    BT_IRQn                     =   3,      /*!< BT All Interrupt                   */
    BTH_IRQn                    =   4,      /*!< BTH All Interrupt                  */
    WIFI_IRQn                   =   5,      /*!< WIFI All Interrupt                 */
    M55C1_IRQn                  =   6,      /*!< The Other M55 Core All Interrupt   */
    DSPC0_IRQn                  =   7,      /*!< DSP Core 0 All Interrupt           */
    DSPC1_IRQn                  =   8,      /*!< DSP Core 1 All Interrupt           */
    WAKEUP_IRQn                 =   9,      /*!< Wakeup Interrupt                   */
    AON_WDT_IRQn                =  10,      /*!< AON Watchdog Timer Interrupt       */
    M55C0_WDT_IRQn              =  11,      /*!< Watchdog Timer Interrupt           */
    DSPC0_WDT_IRQn              =  12,      /*!< Watchdog Timer Interrupt           */
    BTH_WDT_IRQn                =  13,      /*!< Watchdog Timer Interrupt           */
    SENS_WDT_IRQn               =  14,      /*!< Watchdog Timer Interrupt           */
    BTC_WDT_IRQn                =  15,      /*!< Watchdog Timer Interrupt           */
    WIFI_WDT_IRQn               =  16,      /*!< Watchdog Timer Interrupt           */
    USB_IRQn                    =  17,      /*!< USB Interrupt                      */
    USB_PIN_IRQn                =  18,      /*!< USB Pin Interrupt                  */
    SEC_ENG_IRQn                =  19,      /*!< Security Engine Interrupt          */
    SEDMA_IRQn                  =  20,      /*!< Security Engine DMA Interrupt      */
    SHANHAI_NS_IRQn             =  21,      /*!< Shanhai NS Interrupt               */
    SHANHAI_S_IRQn              =  22,      /*!< Shanhai S Interrupt                */
    PMU_IRQn                    =  23,      /*!< PMU Interrupt                      */
    PMU1_IRQn                   =  24,      /*!< PMU1 Interrupt                     */
    SDMMC0_IRQn                 =  25,      /*!< SDMMC0 Interrupt                   */
    EMMC_IRQn                   =  26,      /*!< EMMC Interrupt                     */
    DISPLAY_IRQn                =  27,      /*!< DISPLAY Interrupt                  */
    DSI_IRQn                    =  28,      /*!< DSI Interrupt                      */
    ISDONE2_IRQn                =  29,      /*!< MCU2BTC Done Interrupt             */
    ISDONE3_IRQn                =  30,      /*!< MCU2BTC Done Interrupt             */
    PNG_IRQn                    =  31,      /*!< PNG Interrupt                      */
    IMGDMA_IRQn                 =  32,      /*!< Audio DMA Interrupt                */
    GPU_IRQn                    =  33,      /*!< TRNG Interrupt                     */
    CODEC_IRQn                  =  34,      /*!< CODEC Interrupt                    */
    CODEC_TX_PEAK_IRQn          =  35,      /*!< CODEC TX PEAK Interrupt            */
    XDMA_IRQn                   =  36,      /*!< AXI DMA Interrupt                  */
    DMA0_IRQn                   =  37,      /*!< General Purpose DMA Interrupt      */
    DMA1_IRQn                   =  38,      /*!< General Purpose DMA Interrupt      */
    DMA2_IRQn                   =  39,      /*!< General Purpose DMA Interrupt      */
    PAGE_SPY_IRQn               =  40,      /*!< Page Spy Interrupt                 */
    SPI_WAKEUP_IRQn             =  41,      /*!< SPI Slave Wakeup Interrupt         */
    WIFI_HOST_IRQn              =  42,      /*!< WIFI Host Interrupt                */
    ISDATA2_IRQn                =  43,      /*!< BTC2MCU Data Interrupt             */
    BTPCM_IRQn                  =  44,      /*!< BTPCM Interrupt                    */
    SPDIF0_IRQn                 =  45,      /*!< SPDIF0 Interrupt                   */
    I2S0_IRQn                   =  46,      /*!< I2S0 Interrupt                     */
    I2S1_IRQn                   =  47,      /*!< I2S1 Interrupt                     */
    SPI0_IRQn                   =  48,      /*!< SPI0 Interrupt                     */
    SPILCD_IRQn                 =  49,      /*!< SPILCD Interrupt                   */
    I2C0_IRQn                   =  50,      /*!< I2C0 Interrupt                     */
    I2C1_IRQn                   =  51,      /*!< I2C1 Interrupt                     */
    I2C2_IRQn                   =  52,      /*!< I2C2 Interrupt                     */
    I2C3_IRQn                   =  53,      /*!< I2C3 Interrupt                     */
    I2C4_IRQn                   =  54,      /*!< I2C4 Interrupt                     */
    UART0_IRQn                  =  55,      /*!< UART0 Interrupt                    */
    UART1_IRQn                  =  56,      /*!< UART1 Interrupt                    */
    UART2_IRQn                  =  57,      /*!< UART2 Interrupt                    */
    AON_TIMER00_IRQn            =  58,      /*!< AON Timer00 Interrupt              */
    AON_TIMER01_IRQn            =  59,      /*!< AON Timer01 Interrupt              */
    AON_TIMER10_IRQn            =  60,      /*!< AON Timer10 Interrupt              */
    AON_TIMER11_IRQn            =  61,      /*!< AON Timer11 Interrupt              */
    AON_TIMER20_IRQn            =  62,      /*!< AON Timer20 Interrupt              */
    AON_TIMER21_IRQn            =  63,      /*!< AON Timer21 Interrupt              */
    AON_TIMER30_IRQn            =  64,      /*!< AON Timer30 Interrupt              */
    AON_TIMER31_IRQn            =  65,      /*!< AON Timer31 Interrupt              */
    SYS_TIMER00_IRQn            =  66,      /*!< Timer00 Interrupt                  */
    SYS_TIMER01_IRQn            =  67,      /*!< Timer01 Interrupt                  */
    SYS_TIMER10_IRQn            =  68,      /*!< Timer10 Interrupt                  */
    SYS_TIMER11_IRQn            =  69,      /*!< Timer11 Interrupt                  */
    SYS_TIMER20_IRQn            =  70,      /*!< Timer20 Interrupt                  */
    SYS_TIMER21_IRQn            =  71,      /*!< Timer21 Interrupt                  */
    SYS_TIMER30_IRQn            =  72,      /*!< Timer30 Interrupt                  */
    SYS_TIMER31_IRQn            =  73,      /*!< Timer31 Interrupt                  */
    M55C0_M55C0_DATA_IRQn       =  74,      /*!< M55C0 to itself Indication Interrupt */
    M55C0_M55C0_DONE_IRQn       =  75,      /*!< M55C0 to itself Data Done Interrupt */
    DSPC0_MCU_DATA_IRQn         =  76,      /*!< Intersys DSPC0 to MCU Data Indication Interrupt */
    DSPC0_MCU_DATA1_IRQn        =  77,      /*!< Intersys DSPC0 to MCU Data1 Indication Interrupt */
    DSPC0_MCU_DATA2_IRQn        =  78,      /*!< Intersys DSPC0 to MCU Data2 Indication Interrupt */
    MCU_DSPC0_DONE_IRQn         =  79,      /*!< Intersys MCU to DSPC0 Data Done Interrupt */
    MCU_DSPC0_DONE1_IRQn        =  80,      /*!< Intersys MCU to DSPC0 Data1 Done Interrupt */
    MCU_DSPC0_DONE2_IRQn        =  81,      /*!< Intersys MCU to DSPC0 Data2 Done Interrupt */
    SYS_QSPI_IRQn               =  82,      /*!< QSPI Interrupt                     */
    SYS_JPEG_IRQn               =  83,      /*!< JPEG Interrupt                     */
    RESERVED_84_IRQn            =  84,      /*!< Reserved Interrupt                 */
    RESERVED_85_IRQn            =  85,      /*!< Reserved Interrupt                 */
    RESERVED_86_IRQn            =  86,      /*!< Reserved Interrupt                 */
    RESERVED_87_IRQn            =  87,      /*!< Reserved Interrupt                 */
    CP2MCU_DATA_IRQn            =  88,      /*!< Intersys CP2MCU Data Indication Interrupt */
    CP2MCU_DATA1_IRQn           =  89,      /*!< Intersys CP2MCU Data1 Indication Interrupt */
    CP2MCU_DATA2_IRQn           =  90,      /*!< Intersys CP2MCU Data2 Indication Interrupt */
    MCU2CP_DONE_IRQn            =  91,      /*!< Intersys MCU2CP Data Done Interrupt */
    MCU2CP_DONE1_IRQn           =  92,      /*!< Intersys MCU2CP Data1 Done Interrupt */
    MCU2CP_DONE2_IRQn           =  93,      /*!< Intersys MCU2CP Data2 Done Interrupt */
    SYS2BTH_DONE_IRQn           =  94,      /*!< Intersys SYS2BTH Data Done Interrupt */
    SYS2BTH_DONE1_IRQn          =  95,      /*!< Intersys SYS2BTH Data Done Interrupt */
    BTH2SYS_DATA_IRQn           =  96,      /*!< Intersys BTH2SYS Data Indication Interrupt */
    BTH2SYS_DATA1_IRQn          =  97,      /*!< Intersys BTH2SYS Data1 Indication Interrupt */
    SYS2SENS_DONE_IRQn          =  98,      /*!< Intersys SYS2SENS Data Done Interrupt */
    SYS2SENS_DONE1_IRQn         =  99,      /*!< Intersys SYS2SENS Data1 Done Interrupt */
    SENS2SYS_DATA_IRQn          = 100,      /*!< Intersys SENS2SYS Data Indication Interrupt */
    SENS2SYS_DATA1_IRQn         = 101,      /*!< Intersys SENS2SYS Data1 Indication Interrupt */
    ISDONE_IRQn                 = 102,      /*!< Intersys SYS2BT Data Done Interrupt */
    ISDONE1_IRQn                = 103,      /*!< Intersys SYS2BT Data1 Done Interrupt */
    ISDATA_IRQn                 = 104,      /*!< Intersys BT2SYS Data Indication Interrupt */
    ISDATA1_IRQn                = 105,      /*!< Intersys BT2SYS Data1 Indication Interrupt */
    MCU2WIFI_DONE_IRQn          = 106,      /*!< Intersys MCU2WIFI Data Done Interrupt */
    MCU2WIFI_DONE1_IRQn         = 107,      /*!< Intersys MCU2WIFI Data1 Done Interrupt */
    WIFI2MCU_DATA_IRQn          = 108,      /*!< Intersys WIFI2MCU Data Indication Interrupt */
    WIFI2MCU_DATA1_IRQn         = 109,      /*!< Intersys WIFI2MCU Data1 Indication Interrupt */
    FULLMAP_MMU_PSRAM0_IRQn     = 110,      /*!< Fullmap MMU PSRAM Interrupt        */
    SCI_IRQn                    = 111,      /*!< SCI Interrupt                      */
    M55C1_WDT_IRQn              = 112,      /*!< Watchdog Timer Interrupt           */
    CRC_IRQn                    = 113,      /*!< CRC Interrupt                      */
    CHECKSUM_IRQn               = 114,      /*!< CHECKSUM Interrupt                 */
    I3C0_IRQn                   = 115,      /*!< I3C0 Interrupt                     */
    FULLMAP_MMU_PSRAM1_IRQn     = 116,      /*!< Fullmap MMU PSRAM Interrupt        */
    LM_SENSOR_IRQn              = 117,      /*!< LM Sensor Interrupt                */
    WIFI_TSF_IRQn               = 118,      /*!< WIFI TSF Interrupt                 */
    ISDATA3_IRQn                = 119,      /*!< Intersys BT2SYS Interrupt          */

    USER_IRQn_QTY,
    INVALID_IRQn                = USER_IRQn_QTY,
} IRQn_Type;

#define DSP2MCU_DATA_IRQn       DSPC0_MCU_DATA_IRQn
#define DSP2MCU_DATA1_IRQn      DSPC0_MCU_DATA1_IRQn
#define DSP2MCU_DATA2_IRQn      DSPC0_MCU_DATA2_IRQn
#define MCU2DSP_DONE_IRQn       MCU_DSPC0_DONE_IRQn
#define MCU2DSP_DONE1_IRQn      MCU_DSPC0_DONE1_IRQn

#ifdef CHIP_ROLE_CP
#define TIMER00_IRQn            AON_TIMER10_IRQn
#define TIMER01_IRQn            AON_TIMER11_IRQn
#define TIMER10_IRQn            SYS_TIMER10_IRQn
#define TIMER11_IRQn            SYS_TIMER11_IRQn
#define WDT_IRQn                M55C1_WDT_IRQn
#else
#define TIMER00_IRQn            AON_TIMER00_IRQn
#define TIMER01_IRQn            AON_TIMER01_IRQn
#define TIMER10_IRQn            SYS_TIMER00_IRQn
#define TIMER11_IRQn            SYS_TIMER01_IRQn
#define WDT_IRQn                AON_WDT_IRQn
#endif

#define GPIO_IRQn               AON_IRQn
#define DSP_WDT_IRQn            DSPC0_WDT_IRQn

#define FULLMAP_MMU0_IRQn       FULLMAP_MMU_PSRAM0_IRQn
#define FULLMAP_MMU1_IRQn       FULLMAP_MMU_PSRAM1_IRQn

#endif

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* -------  Start of section using anonymous unions and disabling warnings  ------- */
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


/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __CM55_REV                0x0001U   /* Core revision r0p1 */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __MPU_PRESENT             1U        /* MPU present */
#define __MPU_REGION_NUM          16U       /* MPU region number */
#define __FPU_PRESENT             1U        /* FPU present */
#define __FPU_DP                  1U        /* double precision FPU */
#define __DSP_PRESENT             1U        /* DSP extension present */
#define __SAUREGION_PRESENT       1U        /* SAU regions present */
#define __PMU_PRESENT             1U        /* PMU present */
#define __PMU_NUM_EVENTCNT        8U        /* PMU Event Counters */
#define __ICACHE_PRESENT          1U        /* Instruction Cache present */
#define __DCACHE_PRESENT          1U        /* Data Cache present */

#ifdef ARM_CMNS
#if defined(RTOS) && defined(KERNEL_RTX5)
#define RTE_CMSIS_RTOS2_RTX5_ARMV8M_NS
#endif
#endif

#include "core_cm55.h"                      /* Processor and core peripherals */

#ifndef __ASSEMBLER__
#include "system_ARMCM.h"                   /* System Header */
#endif


/* --------  End of section using anonymous unions and disabling warnings  -------- */
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

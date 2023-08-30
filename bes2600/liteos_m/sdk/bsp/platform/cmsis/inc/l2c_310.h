/**************************************************************************//**
 * @file     l2c_310.h
 * @brief    L2C 310 from CMSIS Cortex-A Core Peripheral Access Layer Header File
 * @version  V1.0.2
 * @date     12. November 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 ARM Limited. All rights reserved.
 * Copyright (c) 2021-2022 BES.
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

#if   defined ( __ICCARM__ )
  #pragma system_include         /* treat file as system include file for MISRA check */
#elif defined (__clang__)
  #pragma clang system_header    /* treat file as system include file */
#endif

#ifndef __L2C_310_H__
#define __L2C_310_H__

#include "cmsis.h"

#ifndef __ARM_ARCH_ISA_ARM

#define RESERVED(N, T) T RESERVED##N;    // placeholder struct members used for "reserved" areas

typedef struct
{
  __IM  uint32_t CACHE_ID;                   /*!< \brief Offset: 0x0000 (R/ ) Cache ID Register               */
  __IM  uint32_t CACHE_TYPE;                 /*!< \brief Offset: 0x0004 (R/ ) Cache Type Register             */
        RESERVED(0[0x3e], uint32_t)
  __IOM uint32_t CONTROL;                    /*!< \brief Offset: 0x0100 (R/W) Control Register                */
  __IOM uint32_t AUX_CNT;                    /*!< \brief Offset: 0x0104 (R/W) Auxiliary Control               */
        RESERVED(1[0x3e], uint32_t)
  __IOM uint32_t EVENT_CONTROL;              /*!< \brief Offset: 0x0200 (R/W) Event Counter Control           */
  __IOM uint32_t EVENT_COUNTER1_CONF;        /*!< \brief Offset: 0x0204 (R/W) Event Counter 1 Configuration   */
  __IOM uint32_t EVENT_COUNTER0_CONF;        /*!< \brief Offset: 0x0208 (R/W) Event Counter 1 Configuration   */
        RESERVED(2[0x2], uint32_t)
  __IOM uint32_t INTERRUPT_MASK;             /*!< \brief Offset: 0x0214 (R/W) Interrupt Mask                  */
  __IM  uint32_t MASKED_INT_STATUS;          /*!< \brief Offset: 0x0218 (R/ ) Masked Interrupt Status         */
  __IM  uint32_t RAW_INT_STATUS;             /*!< \brief Offset: 0x021c (R/ ) Raw Interrupt Status            */
  __OM  uint32_t INTERRUPT_CLEAR;            /*!< \brief Offset: 0x0220 ( /W) Interrupt Clear                 */
        RESERVED(3[0x143], uint32_t)
  __IOM uint32_t CACHE_SYNC;                 /*!< \brief Offset: 0x0730 (R/W) Cache Sync                      */
        RESERVED(4[0xf], uint32_t)
  __IOM uint32_t INV_LINE_PA;                /*!< \brief Offset: 0x0770 (R/W) Invalidate Line By PA           */
        RESERVED(6[2], uint32_t)
  __IOM uint32_t INV_WAY;                    /*!< \brief Offset: 0x077c (R/W) Invalidate by Way               */
        RESERVED(5[0xc], uint32_t)
  __IOM uint32_t CLEAN_LINE_PA;              /*!< \brief Offset: 0x07b0 (R/W) Clean Line by PA                */
        RESERVED(7[1], uint32_t)
  __IOM uint32_t CLEAN_LINE_INDEX_WAY;       /*!< \brief Offset: 0x07b8 (R/W) Clean Line by Index/Way         */
  __IOM uint32_t CLEAN_WAY;                  /*!< \brief Offset: 0x07bc (R/W) Clean by Way                    */
        RESERVED(8[0xc], uint32_t)
  __IOM uint32_t CLEAN_INV_LINE_PA;          /*!< \brief Offset: 0x07f0 (R/W) Clean and Invalidate Line by PA  */
        RESERVED(9[1], uint32_t)
  __IOM uint32_t CLEAN_INV_LINE_INDEX_WAY;   /*!< \brief Offset: 0x07f8 (R/W) Clean and Invalidate Line by Index/Way  */
  __IOM uint32_t CLEAN_INV_WAY;              /*!< \brief Offset: 0x07fc (R/W) Clean and Invalidate by Way     */
        RESERVED(10[0x40], uint32_t)
  __IOM uint32_t DATA_LOCK_0_WAY;            /*!< \brief Offset: 0x0900 (R/W) Data Lockdown 0 by Way          */
  __IOM uint32_t INST_LOCK_0_WAY;            /*!< \brief Offset: 0x0904 (R/W) Instruction Lockdown 0 by Way   */
  __IOM uint32_t DATA_LOCK_1_WAY;            /*!< \brief Offset: 0x0908 (R/W) Data Lockdown 1 by Way          */
  __IOM uint32_t INST_LOCK_1_WAY;            /*!< \brief Offset: 0x090c (R/W) Instruction Lockdown 1 by Way   */
  __IOM uint32_t DATA_LOCK_2_WAY;            /*!< \brief Offset: 0x0910 (R/W) Data Lockdown 2 by Way          */
  __IOM uint32_t INST_LOCK_2_WAY;            /*!< \brief Offset: 0x0914 (R/W) Instruction Lockdown 2 by Way   */
  __IOM uint32_t DATA_LOCK_3_WAY;            /*!< \brief Offset: 0x0918 (R/W) Data Lockdown 3 by Way          */
  __IOM uint32_t INST_LOCK_3_WAY;            /*!< \brief Offset: 0x091c (R/W) Instruction Lockdown 3 by Way   */
  __IOM uint32_t DATA_LOCK_4_WAY;            /*!< \brief Offset: 0x0920 (R/W) Data Lockdown 4 by Way          */
  __IOM uint32_t INST_LOCK_4_WAY;            /*!< \brief Offset: 0x0924 (R/W) Instruction Lockdown 4 by Way   */
  __IOM uint32_t DATA_LOCK_5_WAY;            /*!< \brief Offset: 0x0928 (R/W) Data Lockdown 5 by Way          */
  __IOM uint32_t INST_LOCK_5_WAY;            /*!< \brief Offset: 0x092c (R/W) Instruction Lockdown 5 by Way   */
  __IOM uint32_t DATA_LOCK_6_WAY;            /*!< \brief Offset: 0x0930 (R/W) Data Lockdown 5 by Way          */
  __IOM uint32_t INST_LOCK_6_WAY;            /*!< \brief Offset: 0x0934 (R/W) Instruction Lockdown 5 by Way   */
  __IOM uint32_t DATA_LOCK_7_WAY;            /*!< \brief Offset: 0x0938 (R/W) Data Lockdown 6 by Way          */
  __IOM uint32_t INST_LOCK_7_WAY;            /*!< \brief Offset: 0x093c (R/W) Instruction Lockdown 6 by Way   */
        RESERVED(11[0x4], uint32_t)
  __IOM uint32_t LOCK_LINE_EN;               /*!< \brief Offset: 0x0950 (R/W) Lockdown by Line Enable         */
  __IOM uint32_t UNLOCK_ALL_BY_WAY;          /*!< \brief Offset: 0x0954 (R/W) Unlock All Lines by Way         */
        RESERVED(12[0xaa], uint32_t)
  __IOM uint32_t ADDRESS_FILTER_START;       /*!< \brief Offset: 0x0c00 (R/W) Address Filtering Start         */
  __IOM uint32_t ADDRESS_FILTER_END;         /*!< \brief Offset: 0x0c04 (R/W) Address Filtering End           */
        RESERVED(13[0xce], uint32_t)
  __IOM uint32_t DEBUG_CONTROL;              /*!< \brief Offset: 0x0f40 (R/W) Debug Control Register          */
        RESERVED(14[0x07], uint32_t)
  __IOM uint32_t PREFETCH_CONTROL;           /*!< \brief Offset: 0x0f60 (R/W) Prefetch Control Register       */
        RESERVED(15[0x07], uint32_t)
  __IOM uint32_t POWER_CONTROL;              /*!< \brief Offset: 0x0f80 (R/W) Power Control Register          */
} L2C_310_TypeDef;

/** \brief Cache Sync operation by writing CACHE_SYNC register.
*/
__STATIC_INLINE void L2C_310_Sync(L2C_310_TypeDef *L2C_310)
{
  L2C_310->CACHE_SYNC = 0x0;
  L2C_310->CACHE_SYNC;
  __DSB();
}

/** \brief Read cache controller cache ID from CACHE_ID register.
 * \return L2C_310_TypeDef::CACHE_ID
 */
__STATIC_INLINE int L2C_310_GetID (L2C_310_TypeDef *L2C_310)
{
  return L2C_310->CACHE_ID;
}

/** \brief Read cache controller cache type from CACHE_TYPE register.
*  \return L2C_310_TypeDef::CACHE_TYPE
*/
__STATIC_INLINE int L2C_310_GetType (L2C_310_TypeDef *L2C_310)
{
  return L2C_310->CACHE_TYPE;
}

/** \brief Invalidate all cache by way
*/
__STATIC_INLINE void L2C_310_InvAllByWay (L2C_310_TypeDef *L2C_310)
{
  unsigned int assoc;

  if (L2C_310->AUX_CNT & (1U << 16U)) {
    assoc = 16U;
  } else {
    assoc =  8U;
  }

  L2C_310->INV_WAY = (1U << assoc) - 1U;
  while(L2C_310->INV_WAY & ((1U << assoc) - 1U)); //poll invalidate

  L2C_310_Sync(L2C_310);
}

/** \brief Clean and Invalidate all cache by way
*/
__STATIC_INLINE void L2C_310_CleanInvAllByWay (L2C_310_TypeDef *L2C_310)
{
  unsigned int assoc;

  if (L2C_310->AUX_CNT & (1U << 16U)) {
    assoc = 16U;
  } else {
    assoc =  8U;
  }

  L2C_310->CLEAN_INV_WAY = (1U << assoc) - 1U;
  while(L2C_310->CLEAN_INV_WAY & ((1U << assoc) - 1U)); //poll invalidate

  L2C_310_Sync(L2C_310);
}

__STATIC_INLINE int L2C_310_Enabled(L2C_310_TypeDef *L2C_310)
{
  return L2C_310->CONTROL & 0x01;
}

/** \brief Enable Level 2 Cache
*/
__STATIC_INLINE void L2C_310_Enable(L2C_310_TypeDef *L2C_310)
{
  if (L2C_310_Enabled(L2C_310)) {
    return;
  }
  // Enable double linefill for wrap and incr
  L2C_310->PREFETCH_CONTROL = (1 << 30) | (1 << 23);
  // Enable dynamic clock gating
  L2C_310->POWER_CONTROL = (1 << 1);
  L2C_310_InvAllByWay(L2C_310);
  L2C_310->INTERRUPT_CLEAR = 0x000001FFuL;
  L2C_310->DEBUG_CONTROL = 0;
  L2C_310->DATA_LOCK_0_WAY = 0;
  L2C_310->CONTROL = 0x01;
  L2C_310_Sync(L2C_310);
}

/** \brief Disable Level 2 Cache
*/
__STATIC_INLINE void L2C_310_Disable(L2C_310_TypeDef *L2C_310)
{
  L2C_310->CONTROL = 0x00;
  L2C_310_Sync(L2C_310);
}

/** \brief Invalidate cache by physical address
* \param [in] pa Pointer to data to invalidate cache for.
*/
__STATIC_INLINE void L2C_310_InvPa (L2C_310_TypeDef *L2C_310, void *pa)
{
  L2C_310->INV_LINE_PA = (unsigned int)pa;
  L2C_310_Sync(L2C_310);
}

/** \brief Clean cache by physical address
* \param [in] pa Pointer to data to invalidate cache for.
*/
__STATIC_INLINE void L2C_310_CleanPa (L2C_310_TypeDef *L2C_310, void *pa)
{
  L2C_310->CLEAN_LINE_PA = (unsigned int)pa;
  L2C_310_Sync(L2C_310);
}

/** \brief Clean and invalidate cache by physical address
* \param [in] pa Pointer to data to invalidate cache for.
*/
__STATIC_INLINE void L2C_310_CleanInvPa (L2C_310_TypeDef *L2C_310, void *pa)
{
  L2C_310->CLEAN_INV_LINE_PA = (unsigned int)pa;
  L2C_310_Sync(L2C_310);
}

#endif

/** \brief Invalidate all cache by way
*/
__STATIC_INLINE void L2C_310_CleanAllByWay (L2C_310_TypeDef *L2C_310)
{
  unsigned int assoc;

  if (L2C_310->AUX_CNT & (1U << 16U)) {
    assoc = 16U;
  } else {
    assoc =  8U;
  }

  L2C_310->CLEAN_WAY = (1U << assoc) - 1U;
  while(L2C_310->CLEAN_WAY & ((1U << assoc) - 1U)); //poll invalidate

  L2C_310_Sync(L2C_310);
}

#endif

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
#ifndef __ARMV7A_PMU_H__
#define __ARMV7A_PMU_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * enable the clock counter register
 */
static inline void enable_ccr(void)
{
  /* Set the User Enable register, bit 0 */
  /* Enable all counters in the PNMC control-register */
  asm volatile ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(1));
  /* Enable cycle counter specifically */
  /* bit 31: enable cycle counter */
  /* bits 0-3: enable performance counters 0-3 */
  asm volatile ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x80000000));
}

/**
 * reset the clock counter register to 0
 */
static inline void reset_ccr(void)
{
  uint32_t cc = 0;
  __asm__ volatile ("mcr p15, 0, %0, c9, c13, 0"::"r"(cc));
}

/**
 * read the currrent value of  clock counter register
 */
static inline uint32_t ccnt_read (void)
{
  uint32_t cc = 0;
  __asm__ volatile ("mrc p15, 0, %0, c9, c13, 0":"=r" (cc));
  return cc;
}

#ifdef __cplusplus
}
#endif
#endif /* __ARMV7A_PMU_H__ */

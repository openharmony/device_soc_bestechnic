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
#ifndef __SPEECH_ALGO_H__
#define __SPEECH_ALGO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t speech_algo_init(uint32_t sample_rate, uint32_t frame_len);
int32_t speech_algo_deinit(void);
int32_t speech_algo_process(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __SPEECH_ALGO_H__ */

/***************************************************************************
 *
 * Copyright 2015-2013 BES.
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

#ifndef _APP_FACTORY_WIFI_H_
#define _APP_FACTORY_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

void wifi_rf_test_entry();

/*
*return 1-system on test mdoe
* return 0-system on normal mode
*/
uint8_t factory_get_rf_test_flags(void);

#ifdef __cplusplus
}
#endif

#endif /* _APP_FACTORY_WIFI_H_ */

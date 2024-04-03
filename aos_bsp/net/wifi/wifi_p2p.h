
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

#ifndef WIFI_P2P_H
#define WIFI_P2P_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WIFI_P2P_GO_ID                 = 0,
    WIFI_P2P_GC_ID                 = 1,
    WIFI_P2P_UNKNOWN_ID            = 2,
}WIFI_P2P_ROLE_ID;

typedef enum {
    P2P_DISCONNECTED               = 0,
    P2P_CONNECTED                  = 1,
}WIFI_P2P_CONNECT_STATUS;

bool wpa_is_p2p_name(const char *name, int len);
bool wifi_is_p2p_index();
#ifdef __cplusplus
}
#endif

#endif /* WIFI_DEF_H */

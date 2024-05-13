#ifndef __FACTORY_USER_API_H
#define __FACTORY_USER_API_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "factory_section.h"
#include "hal_trace.h"
#include "norflash_api.h"
#include "hal_norflash.h"
#include "crc32_c.h"
#include "cmsis.h"

enum FLASH_DATA_TYPE {
    FLASH_WIFI_ADDRESS,
    FLASH_WIFI_TX_POWER_CH,
    FLASH_WIFI_TEMP,
    FLASH_BT_TX_POWER,
    FLASH_WIFI_XTAL,
    FLASH_WIFI_POWER_TYPE,
    FLASH_COUNTORY_CODE,
    FLASH_BLE_POWER_CONTROL,
    FLASH_POWER_TABLE,
};

struct wifi_factory_data {
    uint8_t wifi_addr[6];
    uint16_t power;
    int index;
    int temp;
    unsigned short xtal;
    unsigned short power_type;
    uint8_t index1;
    uint32_t div_value;
    uint32_t txPower;
    char counryCode[3];
    unsigned char ble_power_control;
    unsigned char table_index;
};

struct wifi_get_power_cali_t {
    uint16_t cali_chan_mode;
    uint16_t tx_power_ch[4];
    uint16_t tx_power_type;
    uint16_t tx_power_ch_5G[14];
    uint16_t tx_power_flags_5G;
};

#define WIFI_RF_11N_2G4_POWER_CALI_OFFSET 6
#define WIFI_RF_11N_5G_POWER_CALI_OFFSET 13

int factory_section_save_data_to_flash(enum FLASH_DATA_TYPE data_type, struct wifi_factory_data *data);
int factory_section_get_data_from_flash(enum FLASH_DATA_TYPE data_type);

#endif

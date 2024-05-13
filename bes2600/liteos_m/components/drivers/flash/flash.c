/*
 * Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <stdlib.h>
#include "flash.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "hal_trace.h"
#include "hal_norflash.h"
#include "norflash_api.h"
#include "hal_cache.h"
#include "hal_cmu.h"
#include "cmsis.h"
#include "pmu.h"
#include "crc32_c.h"

#define NORFLASH_API_WRAP(x) x
#define TO_FLASH_NC_ADDR(addr)          (((addr) & HAL_NORFLASH_ADDR_MASK) | FLASH_NC_BASE)
#if BES_HAL_DEBUG
#define ENTER_FUNCTION() TRACE(0, "%s enter ->\n", __FUNCTION__)
#define LEAVE_FUNCTION() TRACE(0, "%s <- leave\n", __FUNCTION__)
#define FOOTPRINT() TRACE(0, "%s:%d\n", __FUNCTION__, __LINE__)
#define TRACEME(str, ...) TRACE(0, "%s:%d " str, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define ENTER_FUNCTION()
#define LEAVE_FUNCTION()
#define FOOTPRINT()
#define TRACEME(str, ...)
#endif
#define FAIL_FUNCTION() TRACE(0, "%s:%d fail!\n", __FUNCTION__, __LINE__)

#define FLASH_PAGE_SIZE_IN_BYTES 256
#define OTA_MALLOC_BUF_SIZE FLASH_BLOCK_SIZE_IN_BYTES
#define ONE_BLOCK_SIZE (64 * 1024)
#define BLOCK_MAX_INFO 40
#define RW_MAX_BLOCK 13
#define RW_MAX_PARTITION 3
#define MAX_FLASH_OPTIONS 3
extern hal_logic_partition_t g_halPartitions[];

enum FLASH_OPTIONS {
    FLASH_READ = 0,
    FLASH_WRITE = 1,
    FLASH_ERASE = 2,
};

struct RW_BLOCK_INFO {
    bool flag;
    uint32_t blocknum;
    uint64_t count[MAX_FLASH_OPTIONS];
};

struct HAL_FLASH_BAD_INFO {
    bool flag;
    enum FLASH_OPTIONS option;
    hal_partition_t partition;
    uint32_t offset;
    uint32_t blocknum;
};

struct HAL_FLASH_RW_INFO {
    hal_partition_t partition;
    struct RW_BLOCK_INFO blockInfo[RW_MAX_BLOCK];
};

typedef struct {
    char *bin_name;
    hal_partition_t partition;
} user_writeable_flash_info;

user_writeable_flash_info user_writeable_partitions[] = {
    {"boot2a", HAL_PARTITION_BOOT2A},
    {"boot2b", HAL_PARTITION_BOOT2B},
    {"secure", HAL_PARTITION_TRUSTZONEA},
    {"rtosa", HAL_PARTITION_RTOSA},
    {"rtosb", HAL_PARTITION_RTOSB},
    {"littlefs", HAL_PARTITION_RESOURCE},
    {"log", HAL_PARTITION_LOG},
    {"data", HAL_PARTITION_DATA},
    {"misc", HAL_PARTITION_MISC},
};

struct HAL_FLASH_BAD_INFO g_flashBadInfo[BLOCK_MAX_INFO];
struct HAL_FLASH_BAD_INFO g_normalFlashBadInfo;
static bool g_rwFlashInitFlag = false;
struct HAL_FLASH_RW_INFO g_flashRwInfo[RW_MAX_PARTITION] = {
    {
        .partition = HAL_PARTITION_DATA,
    },
    {
        .partition = HAL_PARTITION_LOG,
    },
    {
        .partition = HAL_PARTITION_RESOURCE,
    }};

static void FlashOptionStructInit(void)
{
    for (size_t i = 0; i < RW_MAX_PARTITION; i++) {
        for (size_t j = 0; j < RW_MAX_BLOCK; j++) {
            g_flashRwInfo[i].blockInfo[j].blocknum = 0;
            g_flashRwInfo[i].blockInfo[j].flag = false;
            for (size_t k = 0; k < MAX_FLASH_OPTIONS; k++) {
                g_flashRwInfo[i].blockInfo[j].count[k] = 0;
            }
        }
    }

    g_rwFlashInitFlag = true;
}

int bes_check_user_write_flash_addr(const uint32_t addr, const uint32_t size)
{
    hal_logic_partition_t *info = NULL;
    int32_t ret = -1;
    int32_t index = 0;

    for (index = 0; index < sizeof(user_writeable_partitions) / sizeof(user_writeable_partitions[0]); index++) {
        info = hal_flash_info_get(user_writeable_partitions[index].partition);
        if ((info->partition_start_addr <= addr) && ((addr + size) <= (info->partition_start_addr + info->partition_length))) {
            ret = 0;
            break;
        }
    }
    return ret;
}
static int32_t SetFlashOptionInfo(hal_partition_t partition, uint32_t size, uint32_t option)
{
    uint32_t blockNum;

    if (((partition != HAL_PARTITION_DATA) && (partition != HAL_PARTITION_LOG) && (partition != HAL_PARTITION_RESOURCE)) || (option >= MAX_FLASH_OPTIONS)) {
        TRACE(0, "%s---%d----%d-----%d\r\n", __FUNCTION__, partition, size, option);
        return -1;
    }

    if (!g_rwFlashInitFlag) {
        FlashOptionStructInit();
    }

    blockNum = (size / ONE_BLOCK_SIZE);

    for (size_t i = 0; i < RW_MAX_PARTITION; i++) {
        if (g_flashRwInfo[i].partition == partition) {
            for (size_t j = 0; j < RW_MAX_BLOCK; j++) {
                if (g_flashRwInfo[i].blockInfo[j].flag) {
                    if (g_flashRwInfo[i].blockInfo[j].blocknum == blockNum)
                        g_flashRwInfo[i].blockInfo[j].count[option]++;
                    else
                        continue;
                } else {
                    g_flashRwInfo[i].blockInfo[j].blocknum = blockNum;
                    g_flashRwInfo[i].blockInfo[j].count[option]++;
                    g_flashRwInfo[i].blockInfo[j].flag = true;
                    break;
                }
            }
        }
    }

    return 0;
}

static int32_t SetFlashBadOptionInfo(hal_partition_t partition, uint32_t size, uint32_t option)
{
    if (((partition != HAL_PARTITION_DATA) && (partition != HAL_PARTITION_LOG) && (partition != HAL_PARTITION_RESOURCE)) || (option >= MAX_FLASH_OPTIONS)) {
        return -1;
    }

    struct HAL_FLASH_BAD_INFO *flashBadInfo = &g_normalFlashBadInfo;
    flashBadInfo->option = option;
    flashBadInfo->partition = partition;

    if ((size % ONE_BLOCK_SIZE) > 0) {
        flashBadInfo->blocknum = (size / ONE_BLOCK_SIZE) + 1;
    } else {
        flashBadInfo->blocknum = (size / ONE_BLOCK_SIZE);
    }

    for (size_t i = 0; i < BLOCK_MAX_INFO; i++) {
        if (g_flashBadInfo[i].flag) {
            continue;
        }

        g_flashBadInfo[i] = g_normalFlashBadInfo;
        g_flashBadInfo[i].flag = true;
    }

    return 0;
}

static struct HAL_FLASH_BAD_INFO *GetFlashBadOptionInfo(void)
{
    return &g_normalFlashBadInfo;
}

static int ota_flash_partition_register(hal_partition_t partition)
{
    int ret = -1;
    hal_logic_partition_t *info = hal_flash_info_get(partition);
    enum HAL_FLASH_ID_T flash_id;
    uint32_t sector_size = 0;
    uint32_t block_size = 0;
    uint32_t page_size = 0;

    info->mod_id = norflash_api_alloc_mod();
    if (info->mod_id < 0) {
        TRACE(0, "%s:%d part %d register alloc fail\n", __FUNCTION__, __LINE__, partition);
        ret = -1;
        return ret;
    }
    flash_id = norflash_api_get_dev_id_by_addr(TO_FLASH_NC_ADDR(info->partition_start_addr));
    block_size = norflash_api_get_block_size(flash_id);
    sector_size = norflash_api_get_sector_size(flash_id);
    page_size = norflash_api_get_page_size(flash_id);

    ret = norflash_api_register(info->mod_id, flash_id, TO_FLASH_NC_ADDR(info->partition_start_addr), info->partition_length,
        block_size, sector_size, page_size, 0x1000, NULL);

    return ret;
}

/**
 * Get the information of the specified flash area
 *
 * @param[in]  partition     The target flash logical partition
 *
 * @return  The buffer to store partition info
 */
hal_logic_partition_t *hal_flash_info_get(hal_partition_t partition)
{
    ASSERT(partition < HAL_PARTITION_MAX, "%s part %d error\n", __FUNCTION__);
    return &g_halPartitions[partition];
}

/**
 * Erase an area on a Flash logical partition
 *
 * @note  Erase on an address will erase all data on a sector that the
 *        address is belonged to, this function does not save data that
 *        beyond the address area but in the affected sector, the data
 *        will be lost.
 *
 * @param[in]  partition  The target flash logical partition which should be erased
 * @param[in]  off_set       Start address of the erased flash area
 * @param[in]  size          Size of the erased flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t hal_flash_erase(hal_partition_t partition, uint32_t off_set, uint32_t size)
{
    uint32_t start_addr;
    hal_logic_partition_t *info = hal_flash_info_get(partition);
    int32_t ret = 0;

    ENTER_FUNCTION();
    if (info->mod_id == 0) {
        ret = ota_flash_partition_register(partition);
        if (ret != NORFLASH_API_OK) {
            TRACE(0, "%s:%d part %d register fail, ret=%d\n", __FUNCTION__, __LINE__, partition, ret);
            ret = -1;
            goto RETURN;
        }
    }

    start_addr = off_set;
    ret = SetFlashOptionInfo(partition, start_addr, FLASH_ERASE);
    if (ret < 0) {
        TRACE(0, "SetFlashOptionInfo FAIL\r\n");
    }

    ret = norflash_api_erase(info->mod_id, TO_FLASH_NC_ADDR(start_addr), size, false);
    if (!ret) {
        goto RETURN;
    } else {
        SetFlashBadOptionInfo(partition, start_addr, FLASH_ERASE);
    }

RETURN:
    LEAVE_FUNCTION();
    return ret;
}

/**
 * Write data to an area on a flash logical partition without erase
 *
 * @param[in]  partition    The target flash logical partition which should be read which should be written
 * @param[in]  off_set         Point to the start address that the data is written to, and
 *                             point to the last unwritten address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  inBuffer        point to the data buffer that will be written to flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t hal_flash_write(hal_partition_t partition, uint32_t *off_set, const void *in_buf, uint32_t in_buf_len)
{
    int32_t ret = 0;
    uint32_t start_addr;
    uint32_t partition_end;
    hal_logic_partition_t *info = hal_flash_info_get(partition);

    ENTER_FUNCTION();
    if (info->mod_id == 0) {
        ret = ota_flash_partition_register(partition);
        if (ret != NORFLASH_API_OK) {
            TRACE(0, "%s:%d part %d register fail, ret=%d\n", __FUNCTION__, __LINE__, partition, ret);
            ret = -1;
            goto RETURN;
        }
    }


    start_addr = *off_set;
    partition_end = info->partition_start_addr + info->partition_length;
    if (start_addr >= partition_end) {
        TRACE(0, "flash over write\r\n");
        ret = -1;
        goto RETURN;
    }

    if ((start_addr + in_buf_len) > partition_end) {
        in_buf_len = partition_end - start_addr;
        TRACE(0, "flash over write, new len is %d\r\n", in_buf_len);
    }

    ret = SetFlashOptionInfo(partition, start_addr, FLASH_WRITE);
    if (ret < 0) {
        TRACE(0, "SetFlashOptionInfo FAIL\r\n");
    }

    ret = norflash_api_write(info->mod_id, TO_FLASH_NC_ADDR(start_addr), in_buf, in_buf_len, false);
    if (!ret) {
        *off_set += in_buf_len;
    } else {
        SetFlashBadOptionInfo(partition, start_addr, FLASH_WRITE);
    }

RETURN:
    LEAVE_FUNCTION();
    return ret;
}

/**
 * Write data to an area on a flash logical partition with erase first
 *
 * @param[in]  partition    The target flash logical partition which should be read which should be written
 * @param[in]  off_set         Point to the start address that the data is written to, and
 *                             point to the last unwritten address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  inBuffer        point to the data buffer that will be written to flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t hal_flash_erase_write(hal_partition_t partition, uint32_t *off_set, const void *in_buf, uint32_t in_buf_len)
{
    int32_t ret = 0;
    uint32_t start_addr;
    uint32_t partition_end;
    hal_logic_partition_t *info = hal_flash_info_get(partition);

    ENTER_FUNCTION();
    if (info->mod_id == 0) {
        ret = ota_flash_partition_register(partition);
        if (ret != NORFLASH_API_OK) {
            TRACE(0, "%s:%d part %d register fail, ret=%d\n", __FUNCTION__, __LINE__, partition, ret);
            ret = -1;
            goto RETURN;
        }
    }


    start_addr = *off_set;
    partition_end = info->partition_start_addr + info->partition_length;

    if (start_addr >= partition_end) {
        TRACE(0, "flash over write\r\n");
        ret = -1;
        goto RETURN;
    }

    if ((start_addr + in_buf_len) > partition_end) {
        in_buf_len = partition_end - start_addr;
        TRACE(0, "flash over write, new len is %d\r\n", in_buf_len);
    }

    ret = SetFlashOptionInfo(partition, start_addr, FLASH_ERASE);
    if (ret < 0) {
        TRACE(0, "SetFlashOptionInfo FAIL\r\n");
    }

    ret = norflash_api_erase(info->mod_id, TO_FLASH_NC_ADDR(start_addr), in_buf_len, false);
    if (ret) {
        TRACE(0, "flash erase fail\r\n");
        ret = -1;
        SetFlashBadOptionInfo(partition, start_addr, FLASH_ERASE);
        goto RETURN;
    }

    ret = SetFlashOptionInfo(partition, start_addr, FLASH_WRITE);
    if (ret < 0) {
        TRACE(0, "SetFlashOptionInfo FAIL\r\n");
    }

    ret = norflash_api_write(info->mod_id, TO_FLASH_NC_ADDR(start_addr), in_buf, in_buf_len, false);
    if (!ret) {
        *off_set += in_buf_len;
    } else {
        SetFlashBadOptionInfo(partition, start_addr, FLASH_WRITE);
    }

RETURN:
    LEAVE_FUNCTION();
    return ret;
}

/**
 * Read data from an area on a Flash to data buffer in RAM
 *
 * @param[in]  partition    The target flash logical partition which should be read
 * @param[in]  off_set         Point to the start address that the data is read, and
 *                             point to the last unread address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  outBuffer       Point to the data buffer that stores the data read from flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t hal_flash_read(hal_partition_t partition, uint32_t *off_set, void *out_buf, uint32_t in_buf_len)
{
    int32_t ret = 0;
    uint32_t start_addr;
    hal_logic_partition_t *info = hal_flash_info_get(partition);

    ENTER_FUNCTION();
    if (info->mod_id == 0) {
        ret = ota_flash_partition_register(partition);
        if (ret != NORFLASH_API_OK) {
            TRACE(0, "%s:%d part %d register fail, ret=%d\n", __FUNCTION__, __LINE__, partition, ret);
            ret = -1;
            goto RETURN;
        }
    }


    start_addr = *off_set;
    ret = SetFlashOptionInfo(partition, start_addr, FLASH_READ);
    if (ret < 0) {
        TRACE(0, "SetFlashOptionInfo FAIL\r\n");
    }
    ret = norflash_api_read(info->mod_id, TO_FLASH_NC_ADDR(start_addr), out_buf, in_buf_len);
    if (!ret) {
        goto RETURN;
    } else {
        SetFlashBadOptionInfo(partition, start_addr, FLASH_READ);
    }

RETURN:
    LEAVE_FUNCTION();
    return ret;
}

/**
 * Set security options on a logical partition
 *
 * @param[in]  partition  The target flash logical partition
 * @param[in]  offset     Point to the start address that the data is read, and
 *                        point to the last unread address after this function is
 *                        returned, so you can call this function serval times without
 *                        update this start address.
 * @param[in]  size       Size of enabled flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t hal_flash_enable_secure(hal_partition_t partition, uint32_t off_set, uint32_t size)
{
    FAIL_FUNCTION();
    return 0;
}

/**
 * Disable security options on a logical partition
 *
 * @param[in]  partition  The target flash logical partition
 * @param[in]  offset     Point to the start address that the data is read, and
 *                        point to the last unread address after this function is
 *                        returned, so you can call this function serval times without
 *                        update this start address.
 * @param[in]  size       Size of disabled flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t hal_flash_dis_secure(hal_partition_t partition, uint32_t off_set, uint32_t size)
{
    FAIL_FUNCTION();
    return 0;
}

/**
 * Convert physical address to logic partition id and offset in partition
 *
 * @param[out]  partition Point to the logic partition id
 * @param[out]  off_set      Point to the offset in logic partition
 * @param[in]   addr         The physical address
 *
 * @return 0 : On success, EIO : If an error occurred with any step
 */
int32_t hal_flash_addr2offset(hal_partition_t *partition, uint32_t *off_set, uint32_t addr)
{
    FAIL_FUNCTION();
    return 0;
}

int ota_flash_read(const hal_partition_t partition, const uint32_t addr, uint8_t *dst, const uint32_t size)
{
    int ret = 0;
    uint32_t flash_offset = 0;
    hal_logic_partition_t *info = hal_flash_info_get(partition);

    if (NULL == dst) {
        ret = -1;
        goto RETURN;
    }

    if (info->mod_id == 0) {
        TRACE(0, "%s:%d part %d not registered\n", __FUNCTION__, __LINE__, partition);
        ret = -1;
        goto RETURN;
    }

    flash_offset = addr + info->partition_start_addr;

    ret = norflash_api_read(info->mod_id, TO_FLASH_NC_ADDR(flash_offset), dst, size);

RETURN:
    return ret;
}

static void ota_feed_watchdog(void)
{
#ifdef __WATCHER_DOG_RESET__
    app_wdt_ping();
#endif
}

hal_partition_t ota_flash_start(void)
{
    int ret = -1;
    hal_partition_t partition = 0;
    hal_logic_partition_t *info = hal_flash_info_get(partition);
    MiscDataInfo bootinfo;

    ret = ota_get_valid_bootinfo_block();
    if (ret != BOOTINFO_ORIGIN) {
        TRACE(0, "%s:%d ota_get_valid_bootinfo_block fail ret=%d\n", __FUNCTION__, __LINE__, ret);
        return ERR_RETURN;
    }

    ret = ota_get_bootinfo(&bootinfo, ret, ota_get_bootinfo_zone_num(ret));
    if (ret) {
        TRACE(0, "error %s %d, ret:%d", __FUNCTION__, __LINE__, ret);
        return ERR_RETURN;
    }

    // app always ops RTOSA and leave bsp doing remap
    partition = HAL_PARTITION_RTOSA;

    if (info->mod_id == 0) {
        ret = ota_flash_partition_register(partition);
        if (ret != NORFLASH_API_OK) {
            TRACE(0, "%s:%d part %d register fail, ret=%d\n", __FUNCTION__, __LINE__, partition, ret);
            return ERR_RETURN;
        }
    }

    return partition;
}

/* addr should be 4K align, size should <= 4K */
int ota_flash_write(const hal_partition_t partition, const uint32_t addr, const uint8_t *src, const uint32_t size)
{
    int ret = -1;
    uint32_t left_len = 0, align_len = 0, fill_len = 0, flash_offset = 0, lengthToBurn = size;
    uint8_t *ptrSource = (uint8_t *)src;
    uint8_t *buf = NULL;
    hal_logic_partition_t *info = hal_flash_info_get(partition);
    uint32_t flash_id = 0;
    uint32_t sector_size = 0;

    if (NULL == ptrSource)
        return ERR_RETURN;

    if (info->mod_id == 0) {
        return ERR_RETURN;
    }

    flash_offset = addr + info->partition_start_addr;
    flash_id = norflash_api_get_dev_id_by_addr(TO_FLASH_NC_ADDR(flash_offset));
    sector_size = norflash_api_get_sector_size(flash_id);

    if ((flash_offset % sector_size != 0) || (size > sector_size)) {
        TRACE(0,"%s:%d sector_size=0x%x, addr=0x%x need align to sector, size=0x%x need <= sector!",
            __FUNCTION__, __LINE__, sector_size, addr, size);
        return ERR_RETURN;
    }

    // check magic
    if ((addr == 0) && (partition == HAL_PARTITION_RTOSA)) {
        ptrSource[0] = 0x1c;
        ptrSource[1] = 0xec;
        ptrSource[2] = 0x57;
        ptrSource[3] = 0xbe;
    }

    if (size < sector_size) {
        buf = malloc(sector_size);
        ASSERT(buf, "%s malloc %d fail\n", __FUNCTION__);
        ret = norflash_api_read(info->mod_id, TO_FLASH_NC_ADDR(flash_offset), buf, sector_size);
        if (ret != NORFLASH_API_OK) {
            TRACE(0,"%s:%d read addr=0x%x:%d fail, ret=%d", __FUNCTION__, __LINE__, flash_offset, sector_size, ret);
            goto end;
        }
        memcpy(buf, ptrSource, size);
    } else {
        buf = ptrSource;
    }

    ota_feed_watchdog();

    ret = norflash_api_erase(info->mod_id, TO_FLASH_NC_ADDR(flash_offset), sector_size, false);
    if (ret != NORFLASH_API_OK) {
        TRACE(0,"%s:%d erase addr=0x%x:%d fail, ret=%d", __FUNCTION__, __LINE__, flash_offset, sector_size, ret);
        goto end;
    }

    ret = norflash_api_write(info->mod_id, TO_FLASH_NC_ADDR(flash_offset), buf, sector_size, false);
    if (ret != NORFLASH_API_OK) {
        TRACE(0,"%s:%d write addr=0x%x:%d fail, ret=%d", __FUNCTION__, __LINE__, flash_offset, sector_size, ret);
        goto end;
    }

end:
    if (size < sector_size) {
        free(buf);
        buf = NULL;
    }

    hal_cache_invalidate_all(HAL_CACHE_ID_I_CACHE);

    return ret;
}

int ota_flash_end(const hal_partition_t partition)
{
    int32_t ret = 0;
    MiscDataInfo bootinfo;

    ret = ota_get_bootinfo(&bootinfo, BOOTINFO_ORIGIN, ota_get_bootinfo_zone_num(BOOTINFO_ORIGIN));
    if (ret) {
        return ret;
    }

#if 1
    bootinfo.curbootArea = 1- bootinfo.curbootArea;
    bootinfo.upgMode = 1- bootinfo.upgMode;
#else
    if ((partition == HAL_PARTITION_BOOT2A) || (partition == HAL_PARTITION_RTOSA)) {
        bootinfo.curbootArea = BOOT_AREA_A;
        bootinfo.upgMode = UPG_MODE_NORMAL;
    } else if ((partition == HAL_PARTITION_BOOT2B) || (partition == HAL_PARTITION_RTOSB)) {
        bootinfo.curbootArea = BOOT_AREA_B;
        bootinfo.upgMode = UPG_MODE_OTA;
    }
#endif

    ret = ota_set_bootinfo_to_zoneAB(&bootinfo);
    if (ret) {
        TRACE(0, "error %s %d, ota_set_bootinfo return %d", __FUNCTION__, __LINE__, ret);
    }

    return ret;
}

/*
 * 获取当前是那个区间：partition
 * 获取具体那个坏块号
 */
int32_t GetFlashBadBlockNum(hal_partition_t partition, uint32_t *blockNum)
{
    struct HAL_FLASH_BAD_INFO *flashbadinfo = NULL;
    if (blockNum == NULL) {
        return -1;
    }

    for (size_t i = 0; i < BLOCK_MAX_INFO; i++) {
        flashbadinfo = &g_flashBadInfo[i];
        if (flashbadinfo->partition == partition) {
            *blockNum = flashbadinfo->blocknum;
            break;
        }
    }

    return 0;
}

/*
 * 获取当前是那个区间：partition
 * 获取区间上偏移（算出具体物理地址）：offset
 * 获取具体那个坏块号：
 */
int32_t GetFlashBadBlock(hal_partition_t *partition, uint32_t *offset, uint32_t *blockNum)
{
    struct HAL_FLASH_BAD_INFO *flashbadinfo = NULL;
    if (partition == NULL || offset == NULL || blockNum == NULL) {
        return -1;
    }

    flashbadinfo = GetFlashBadOptionInfo();
    *partition = flashbadinfo->partition;
    *offset = flashbadinfo->offset;
    *blockNum = flashbadinfo->blocknum;

    return 0;
}

/*
 * 获取集成Flash芯片的厂家ID:factoryID
 * 获取集成Flash芯片的设备ID:deviceID
 */
int32_t GetFlashType(int32_t *factoryID, int32_t *deviceID)
{
    uint8_t flash_id[HAL_NORFLASH_DEVICE_ID_LEN];
    int32_t a, b;

    NORFLASH_API_WRAP(hal_norflash_get_id)(HAL_FLASH_ID_0, flash_id, ARRAY_SIZE(flash_id));

    TRACE(3, "FLASH_ID: %02X-%02X-%02X", flash_id[0], flash_id[1], flash_id[2]);
    *factoryID = flash_id[0];

    a = 0xffff00ff | (flash_id[1] << 8);
    b = 0xffffff00 | flash_id[2];

    *deviceID = a & b;

    return 0;
}

/*
 * 当前是区间：partition
 * 当前是块号：blockNum
 * 读或者写：op
 * 获取该块号的写或者擦的次数：times
 */
int32_t GetFlashRewriteCycle(hal_partition_t partition, uint32_t blockNum, uint32_t op, uint32_t *times)
{
    struct HAL_FLASH_RW_INFO *FlashRwInfo = NULL;

    if (times == NULL || op >= MAX_FLASH_OPTIONS) {
        return -1;
    }

    for (size_t i = 0; i < RW_MAX_PARTITION; i++) {
        FlashRwInfo = &g_flashRwInfo[i];
        if (FlashRwInfo->partition == partition) {
            for (size_t j = 0; j < RW_MAX_BLOCK; j++) {
                if (FlashRwInfo->blockInfo[j].blocknum == blockNum) {
                    *times = FlashRwInfo->blockInfo[j].count[op];
                    break;
                }
            }
        }
    }

    return 0;
}

int32_t GetFlashRewriteCycleCount(hal_partition_t partition, uint32_t op, uint32_t *count)
{
    uint32_t tempTimes = 0;
    uint32_t countTimes = 0;
    uint32_t partitionStartBlockNum;
    uint32_t partitionEndBlockNum;
    hal_logic_partition_t *info = hal_flash_info_get(partition);

    if (count == NULL) {
        TRACE(0, "count is NULL\r\n");
        return -1;
    }

    partitionStartBlockNum = info->partition_start_addr / ONE_BLOCK_SIZE;
    partitionEndBlockNum = (info->partition_start_addr + info->partition_length) / ONE_BLOCK_SIZE;

    for (size_t blockNum = partitionStartBlockNum; blockNum < partitionEndBlockNum; blockNum++) {
        GetFlashRewriteCycle(partition, blockNum, op, &tempTimes);
        countTimes += tempTimes;
    }

    *count = countTimes;

    return 0;
}

/*
 * 当前是区间：partition
 * 当前是块号：blockNum
 * 读或者写：op
 * 获取该块号的写或者擦的次数：blockTimes
 */
int32_t GetFlashStatisticInfo(hal_partition_t partition, uint32_t blockNum, uint32_t op, uint32_t *blockTimes)
{
    struct HAL_FLASH_BAD_INFO *flashBadInfo = NULL;
    uint32_t blockcount = 0;

    if (blockTimes == NULL) {
        TRACE(0, "blockTimes is NULL\r\n");
        return -1;
    }

    for (size_t i = 0; i < BLOCK_MAX_INFO; i++) {
        flashBadInfo = &g_flashBadInfo[i];
        if ((flashBadInfo->partition == partition) && (flashBadInfo->blocknum <= blockNum) && (flashBadInfo->partition == op)) {
            blockcount++;
        }
    }

    *blockTimes = blockcount;

    return 0;
}

#include "eshell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void do_otacmd(int argc, char **argv)
{
    int32_t ret = 0;
    hal_partition_t partition;
    MiscDataInfo bootinfo;
    bootinfo_block block;

    if (argc < 2) {
        TRACE(0, "ota get");
        TRACE(0, "ota switch");
        TRACE(0, "ota destroy");
        return;
    }

    block = ota_get_valid_bootinfo_block();
    ret = ota_get_bootinfo(&bootinfo, block, ota_get_bootinfo_zone_num(block));
    if (ret) {
        TRACE(0, "error %s %d, ret:%d", __FUNCTION__, __LINE__, ret);
        return;
    }

    if (!strncmp(argv[1], "get", strlen(argv[1]))) {
        if (bootinfo.upgMode == UPG_MODE_NORMAL) {
            TRACE(0, "rtosa");
        } else {
            TRACE(0, "rtosb");
        }
    } else {
        partition = ota_flash_start();
        if (partition <= 0) {
            TRACE(0, "%s %d, error ota_flash_start ret = %d", __FUNCTION__, __LINE__, partition);
            return;
        }

        if (!strncmp(argv[1], "switch", strlen(argv[1]))) {
            if (bootinfo.upgMode == UPG_MODE_NORMAL) {
                TRACE(0, "switch to rtosb");
            } else {
                TRACE(0, "switch to rtosa");
            }
        } else if (!strncmp(argv[1], "destroy", strlen(argv[1]))) {
            ota_flash_write(partition, 0x1000, "test", 4);
        } else if (!strncmp(argv[1], "file", strlen(argv[1]))) {
            char *path = argv[2];
            int fd = open(path, O_RDONLY);
            if (fd < 0) {
                TRACE(0, "Failed to open file: %s", path);
                return;
            }
            int len = 4096;
            char *buf = malloc(len);
            if (buf == NULL) {
                TRACE(0, "Failed to malloc %d", len);
                return;
            }
            int readed = 0, total = 0;
            do {
                readed = read(fd, buf, len);
                ota_flash_write(partition, total, buf, readed);
                total += readed;
            } while (readed >= len);
            free(buf);
            buf = NULL;
        } else {
            TRACE(0, "%s %d, error parameter, argv[1]:%s", __FUNCTION__, __LINE__, argv[1]);
            return ;
        }
        ota_flash_end(partition);
    }

    TRACE(0, "%s %s success", argv[0], argv[1]);
}

ESHELL_DEF_COMMAND(ESHELL_CMD_GRP_DSP, "ota", "ota get/switch/destroy/file", do_otacmd);

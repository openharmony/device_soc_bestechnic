/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_BT_AVRCP_CONTROLLER_H
#define OHOS_BT_AVRCP_CONTROLLER_H

#include "ohos_bt_def.h"

typedef enum {
    /** Turns the volume to high. */
    OHOS_AVRCP_KEY_VOLUME_UP = 0x41,
    /** Turns the volume to low. */
    OHOS_AVRCP_KEY_VOLUME_DOWN = 0x42,
    /** Mute */
    OHOS_AVRCP_KEY_MUTE = 0x43,
    /** Play */
    OHOS_AVRCP_KEY_PLAY = 0x44,
    /** Stop */
    OHOS_AVRCP_KEY_STOP = 0x45,
    /** Pause */
    OHOS_AVRCP_KEY_PAUSE = 0x46,
    /** Record */
    OHOS_AVRCP_KEY_RECORD = 0x47,
    /** Moves the position toward the beginning of the medium. */
    OHOS_AVRCP_KEY_REWIND = 0x48,
    /** Fast forward, moves the position away from the beginning of the medium. */
    OHOS_AVRCP_KEY_FAST_FORWARD = 0x49,
    /** Switches the contents, such as music tune or video chapter to the forward one. */
    OHOS_AVRCP_KEY_FORWARD = 0x4B,
    /** Switches the contents, such as music tune or video chapter to the backward one */
    OHOS_AVRCP_KEY_BACKWARD = 0x4C,
    /** Invalid */
    OHOS_AVRCP_KEY_INVALID = 0x7F,
} AvrcpCtKeyOperation;

typedef enum {
    /** Title, song name or content description. */
    OHOS_AVRCP_ATTR_TITLE = 0x01,
    /** Artist(s), performer(s) or group coded per specified character set. */
    OHOS_AVRCP_ATTR_ARTIST_NAME = 0x02,
    /** Album */
    OHOS_AVRCP_ATTR_ALBUM_NAME = 0x03,
    /* The order number of the audio-file on its original recording. */
    OHOS_AVRCP_ATTR_TRACK_NUMBER = 0x04,
    /** The total number of tracks/elements on the original recording */
    OHOS_AVRCP_ATTR_TOTAL_NUMBER_OF_TRACKS = 0x05,
    /** Genre */
    OHOS_AVRCP_ATTR_TRACK_GENRE = 0x06,
    /** The length of the audio file in milliseconds */
    OHOS_AVRCP_ATTR_TRACK_PLAYING_TIME = 0x07
} AvrcpCtElementAttrId;

typedef enum {
    /** Change in playback status of the current track. */
    OHOS_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED = 0x01,
    /** Change of current track. */
    OHOS_AVRCP_EVENT_TRACK_CHANGED = 0x02,
    /** Reached end of a track. */
    OHOS_AVRCP_EVENT_TRACK_REACHED_END = 0x03,
    /** Reached start of a track. */
    OHOS_AVRCP_EVENT_TRACK_REACHED_START = 0x04,
    /** Change in playback position. */
    OHOS_AVRCP_EVENT_PLAYBACK_POS_CHANGED = 0x05,
    /** Change in battery status. */
    OHOS_AVRCP_EVENT_BATT_STATUS_CHANGED = 0x06,
    /** Change in system status. */
    OHOS_AVRCP_EVENT_SYSTEM_STATUS_CHANGED = 0x07,
    /** Change in player application setting. */
    OHOS_AVRCP_EVENT_PLAYER_APPLICATION_SETTING_CHANGED = 0x08,
    /** The content of the Now Playing list has changed. */
    OHOS_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED = 0x09,
    /** The available players have changed. */
    OHOS_AVRCP_EVENT_AVAILABLE_PLAYERS_CHANGED = 0x0A,
    /** The Addressed Player has been changed. */
    OHOS_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED = 0x0B,
    /** The UIDs have changed. */
    OHOS_AVRCP_EVENT_UIDS_CHANGED = 0x0C,
    /** The volume has been changed locally on the TG. */
    OHOS_AVRCP_EVENT_VOLUME_CHANGED = 0x0D,
    /** Reserved for future use. */
    OHOS_AVRCP_EVENT_RESERVED = 0x0E
} AvrcpCtNotificationEventId;

typedef enum {
    OHOS_AVRCP_ABSOLUTE_VOLUME_PERCENTAGE_0 = 0x00,
    OHOS_AVRCP_ABSOLUTE_VOLUME_PERCENTAGE_100 = 0x7F,
    OHOS_AVRCP_ABSOLUTE_VOLUME_INVALID = 0x80,
} AvrcpCtAbsoluteVolume;

typedef enum {
    /** STOPPED */
    OHOS_AVRCP_PLAY_STATUS_STOPPED = 0x00,
    /** PLAYING */
    OHOS_AVRCP_PLAY_STATUS_PLAYING = 0x01,
    /** PAUSED */
    OHOS_AVRCP_PLAY_STATUS_PAUSED = 0x02,
    /** FWD_SEEK */
    OHOS_AVRCP_PLAY_STATUS_FWD_SEEK = 0x03,
    /** REV_SEEK */
    OHOS_AVRCP_PLAY_STATUS_REV_SEEK = 0x04,
    /** ERROR */
    OHOS_AVRCP_PLAY_STATUS_ERROR = 0xFF
} AvrcpCtPlayStatus;

typedef enum {
    /** Battery operation is in normal state. */
    OHOS_AVRCP_BATTERY_STATUS_NORMAL = 0x00,
    /** unable to operate soon. Is provided when the battery level is going down. */
    OHOS_AVRCP_BATTERY_STATUS_WARNING = 0x01,
    /** Cannot operate any more. Is provided when the battery level is going down. */
    OHOS_AVRCP_BATTERY_STATUS_CRITICAL = 0x02,
    /** Plugged to external power supply. */
    OHOS_AVRCP_BATTERY_STATUS_EXTERNAL = 0x03,
    /** when the device is completely charged from the external power supp */
    OHOS_AVRCP_BATTERY_STATUS_FULL_CHARGE = 0x04,
} AvrcpCtBatteryStatus;

typedef enum {
    /** Power on. */
    OHOS_AVRCP_SYSTEM_STATUS_POWER_ON = 0x00,
    /** Power off. */
    OHOS_AVRCP_SYSTEM_STATUS_POWER_OFF = 0x01,
    /** unplugged. */
    OHOS_AVRCP_SYSTEM_STATUS_UNPLUGGED = 0x02
} AvrcpCtSystemStatus;

typedef struct {
    unsigned char attrNum;
    unsigned int *attrIdList;
} AvrcpCtGetElementAttrPara;

typedef struct {
    unsigned char companyNum;
    unsigned int *companyId;
} AvrcpCtSupportCompanyPara;

typedef struct {
    unsigned char eventNum;
    unsigned char *eventId;
} AvrcpCtSupportEventPara;

typedef struct {
    /** Specifies the attribute ID to be written. */
    unsigned int attrId;
    /** Specifies the character set ID to be displayed on CT. */
    unsigned short characterSetId;
    /** Length of the value of the attribute. 0-65535 (0, if no name is provided) */
    unsigned short valueLen;
    const char *value;
} AvrcpCtElementAttrValueCb;

typedef struct {
    unsigned char attrNum;
    AvrcpCtElementAttrValueCb *attrValue;
} AvrcpCtElementAttrParaCb;

typedef struct {
    /** Unique Media Player Id. */
    unsigned short playerId;
    /** UID Counter. */
    unsigned short uidCounter;
} AvrcpCtAddressedPlayerChangedCb;

typedef struct {
    /** The total length of the playing song in milliseconds. */
    unsigned int songLength;
    /** The current position of the playing in milliseconds elapsed. */
    unsigned int songPosition;
    /** Current Status of playing. */
    AvrcpCtPlayStatus playStatus;
} AvrcpCtPlayStatusCb;

typedef union {
    /** {@link OHOS_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED} */
    AvrcpCtPlayStatus playStatus;
    /** {@link OHOS_AVRCP_EVENT_TRACK_CHANGED} */
    unsigned long long identifier;
    /** {@link OHOS_AVRCP_EVENT_PLAYBACK_POS_CHANGED}.
    Current playback position in millisecond.
    If no track currently selected, then return 0xFFFFFFFF in the INTERIM response. */
    unsigned int playbackPosition;
    /** {@link OHOS_AVRCP_EVENT_BATT_STATUS_CHANGED} */
    AvrcpCtBatteryStatus batteryStatus;
    /** {@link OHOS_AVRCP_EVENT_SYSTEM_STATUS_CHANGED } */
    AvrcpCtSystemStatus systemStatus;
    /** {@link OHOS_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED } */
    AvrcpCtAddressedPlayerChangedCb addressedPlayer;
    /** {@link OHOS_AVRCP_EVENT_UIDS_CHANGED } */
    unsigned short uidCounter;
    /** {@link OHOS_AVRCP_EVENT_VOLUME_CHANGED} */
    unsigned char volume;
} AvrcpCtNotificationValueCb;

typedef void (*AvrcpCtConnStateChangedCallback)(const BdAddr *bdAddr, BtProfileConnectState state);

typedef void (*AvrcpCtPressButtonResponseCallback)(const BdAddr *bdAddr, int keyOperation);

typedef void (*AvrcpCtReleaseButtonResponseCallback)(const BdAddr *bdAddr, int keyOperation);

typedef void (*AvrcpCtSupportedCompaniesCallback)(const BdAddr *bdAddr, AvrcpCtSupportCompanyPara *company);

typedef void (*AvrcpCtSupportedEventCallback)(const BdAddr *bdAddr, AvrcpCtSupportEventPara *event);

typedef void (*AvrcpCtGetElementAttributeCallback)(const BdAddr *bdAddr, AvrcpCtElementAttrParaCb *attrCbPara);

typedef void (*AvrcpCtGetPlayStatusCallback)(const BdAddr *bdAddr, AvrcpCtPlayStatusCb *status);

typedef void (*AvrcpCtNotificationCallback)(const BdAddr *bdAddr, AvrcpCtNotificationEventId eventId,
                                            AvrcpCtNotificationValueCb *notification);

typedef struct {
    AvrcpCtConnStateChangedCallback connStateChangedCb;
    AvrcpCtPressButtonResponseCallback pressButtonCb;
    AvrcpCtReleaseButtonResponseCallback releaseButtonCb;
    AvrcpCtSupportedCompaniesCallback supportedCompaniesCb;
    AvrcpCtSupportedEventCallback supportedEventCb;
    AvrcpCtGetElementAttributeCallback elementAttrCb;
    AvrcpCtGetPlayStatusCallback playStatusCb;
    AvrcpCtNotificationCallback notificationCb;
} BtAvrcpCtCallbacks;

int AvrcpCtGetDeviceConnectState(const BdAddr *bdAddr);

int AvrcpCtConnect(const BdAddr *bdAddr);

int AvrcpCtDisconnect(const BdAddr *bdAddr);

int AvrcpCtPressButton(const BdAddr *bdAddr, int keyOperation);

int AvrcpCtReleaseButton(const BdAddr *bdAddr, int keyOperation);

int AvrcpCtGetSupportedCompanies(const BdAddr *bdAddr);

int AvrcpCtGetSupportedEvents(const BdAddr *bdAddr);

int AvrcpCtGetElementAttributes(const BdAddr *bdAddr, const AvrcpCtGetElementAttrPara *param);

int AvrcpCtGetPlayStatus(const BdAddr *bdAddr);

int AvrcpCtRegisterNotification(const BdAddr *bdAddr, unsigned char eventId, unsigned int interval);

int AvrcpCtSetAbsoluteVolume(const BdAddr *bdAddr, unsigned char volume);

int AvrcpCtSendVendorCmd(const BdAddr *bdAddr, bool is_press, unsigned short vendor_data_len, unsigned char *vendor_data);

int AvrcpCtRegisterCallbacks(BtAvrcpCtCallbacks *func);

#endif
/** @} */


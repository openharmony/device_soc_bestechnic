#if 0
#include "private_vendor_api.h"
#include "ohos_bt_gatt_server.h"
#include "ohos_bt_gap.h"
#include "app_a2dp.h"
#include <math.h>
#ifdef IBRT
#include "app_ibrt_customif_cmd.h"
#include "app_tws_ctrl_thread.h"
#include "hal_trace.h"
#include "app_ibrt_if.h"
#include "app_tws_ibrt.h"
#include "app_tws_ibrt_conn_api.h"
#include "ibrt_mgr.h"
#endif
#include "app.h"
#include "app_hfp.h"
#include "nvrecord_env.h"
#include "nvrecord_bt.h"
#include "a2dp_decoder.h"
#include "bt_adaptor_dbg.h"
#include "bluetooth.h"
#include "factory_section.h"
#include "app_bt.h"
#include "apps.h"
#include "app_task.h"
#include "app_bt_stream.h"
#include "ohos_bt_avrcp_controller.h"

#if BLE_AUDIO_ENABLED
#include "app_bap_stream.h"
#include "app_gaf_custom_api.h"
#include "audio_define.h"
#include "audio_sink.h"
#endif
#if !defined(__COEX_MODE_FDD__) || defined(__COEX_MODE_HYBRID__)
#include "app_wifi_bt_time_slot_thread.h"
#endif

#include "alsa_extra.h"
#include "tgt_hardware.h"
#include "string_s.h"

#define MAX_ATTR_HDLR_BUM 10
//static GetBatteryLevel g_BatteryLevelCallback = NULL;

extern void bt_enter_freeman_mode_procedure(void);
extern void GapSetEnterFreemanModeFlag(bool freeman);
extern void GapRegisterUxStateChangeCallback(StereoStateChangedCallback func);
extern void app_register_a2dp_cuton_notify_upper(void (*cb)(void));
extern void beshci_set_linkkey_trace_enable(bool enable);
extern int RegisterBtStackInitDoneCallback(void (*initDoneCb)(void));
extern void app_register_sink_scan_stop_callback(void(*app_bis_sink_scan_stop_cb_t)(void));
extern void app_register_user_log_printf_callback(void(* cb)(const char * fmt, ...));

MediaStreamNotifyCallback MediaStreamNotifyCb = NULL;
StereoReceiveData   ReceiveTwsDataCb = NULL;
static bool (*ux_accept_tws_conn_result_callback)(void) = NULL;
static bool g_uxTwsReconnectAction = false;
uint8_t g_BleAddr[OHOS_BD_ADDR_LEN] = {0};

extern uint8_t app_custom_a2dp_af_control;
void StartLocalMediaPlay(void)
{
#ifdef IBRT
    LOG_I("%s rloe:%d,run:%d stream:%d ", __func__,app_ibrt_conn_get_ui_role(),
         bt_a2dp_is_run(),app_bt_is_a2dp_streaming(BT_DEVICE_ID_1));

    if(app_ibrt_conn_get_ui_role() == TWS_UI_SLAVE)
    {
        app_custom_a2dp_af_control = APP_A2DP_CUSTOM_AF_START;
        if(!bt_a2dp_is_run()){
            if(app_bt_is_a2dp_streaming(BT_DEVICE_ID_1))
                app_bt_af_stream_start();
        }
        return;
    }
#endif
    app_bt_af_stream_start();
}

void StopLocalMediaPlay(void)
{
    LOG_I("%s", __func__);
    if(app_custom_a2dp_af_control == APP_A2DP_CUSTOM_AF_START)
        app_bt_af_stream_stop();
}

void AppGamingModeAudioLatencyConfigure(bool isGamingEnable)
{
    LOG_I("%s gaming_mode=%d", __func__, isGamingEnable);
    uint8_t device_id = BT_DEVICE_ID_1;
    uint8_t streaming_mode = A2DP_PLAYER_STREAMING_MODE_DEFAULT;
    if (isGamingEnable)
        streaming_mode = A2DP_PLAYER_STREAMING_MODE_GAMING;
    else
        streaming_mode = A2DP_PLAYER_STREAMING_MODE_DEFAULT;

    if (streaming_mode != app_audio_a2dp_player_streaming_mode_get(device_id))
    {
        app_audio_a2dp_player_streaming_mode_set(device_id, streaming_mode);
        app_audio_sync_a2dp_player_streaming_mode(device_id);

        //Notes: use AVRCP vendor cmd to implement a2dp audio latency.
        BdAddr mobile_addr = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
        if (app_bt_get_remote_device_address(device_id) != NULL)
        {
            memcpy(mobile_addr.addr, app_bt_get_remote_device_address(device_id)->address, 6);

            //data format: len(1bytes) + data type(3bits) + latency value(5bits, 100ms unit).
            uint8_t vendor_data_buff[1] = {0x00};
            uint16_t vendor_data_len = 0x01;
            vendor_data_buff[0] = (0x01<<5)|(0x02);
            AvrcpCtSendVendorCmd(&mobile_addr, true, vendor_data_len, vendor_data_buff);
        }
    }
}

void app_notify_receive_media_data(void)
{
    if(MediaStreamNotifyCb)
        MediaStreamNotifyCb();
}

#ifdef VAD_IN_BACKGROUND
extern void bt_sco_put_uplink_data(uint8_t*buf , uint16_t len);
#endif
int AppSendHfpPcmStream(PcmStreamInfo *streamInfo)
{
    if (streamInfo == NULL) {
        return -1;
    }
#ifdef VAD_IN_BACKGROUND
    bt_sco_put_uplink_data((uint8_t*)(streamInfo->stream), streamInfo->length);
#endif
    return 0;
}

void SetupStereoHandler(uint8_t ux_role, uint8_t* peer_addr)
{
#ifdef IBRT
    bt_bdaddr_t bd_addr = {{0}};
    if(peer_addr){
        memcpy_s(bd_addr.address,OHOS_BD_ADDR_LEN,peer_addr,OHOS_BD_ADDR_LEN);
    }
    //TODO: tws paring ongoing flag shall be set by UX in advance.
    SetTwsParingOngingFlag(true);
    GapSetEnterFreemanModeFlag(false);
    if (app_bt_ibrt_has_mobile_link_connected())
        GapBtInterfaceEnvCtxGet()->ux_mobile_connected_before_tws_paring = true;
    else
        GapBtInterfaceEnvCtxGet()->ux_mobile_connected_before_tws_paring = false;
    app_ibrt_conn_clear_freeman_enable();
    app_ibrt_if_start_tws_pairing(ux_role, peer_addr);
#endif
}

void SetTwsParingOngingFlag(bool isEnabled)
{
    LOG_D("%s isEnabled=%d", __func__, isEnabled);
    GapBtInterfaceEnvCtxGet()->ux_doing_tws_paring_flag = isEnabled;
}

int SetUpStereo(StereoInfo *data)
{
#ifdef IBRT
    if (data)
    {
        LOG_I("%s role:%d addr:", __func__, data->role);
        DUMP8("%02x ", data->addr.addr, BT_ADDR_OUTPUT_PRINT_NUM);
    }
#if !defined(__COEX_MODE_FDD__) || defined(__COEX_MODE_HYBRID__)
    app_wifi_bt_ts_request(APP_BT_TWS_CONNECTING, NULL, 0);
#endif
    /*if ((GapBtInterfaceEnvCtxGet()->ux_bluetooth_enable == false) || ibrt_mgr_in_calling_state())
    {
        LOG_W("Currently bluetooth is disabled or call is ongoing");
        GapUxStereoStateChangeReport(STEREO_SETUP_FAILED);
        return -1;
    }*/
    if (GapBtInterfaceEnvCtxGet()->ux_doing_tws_paring_flag)
    {
        LOG_W("Currently setup stereo is ongoing, repeated calling");
        return -1;
    }
    GapBtInterfaceEnvCtxGet()->ux_dev_role = data->role;
    memcpy(GapBtInterfaceEnvCtxGet()->ux_peer_addr, data->addr.addr, 6);
    if (GapBtInterfaceEnvCtxGet()->ux_dev_mode == GAP_DEV_MODE_STEREO)
    {
        LOG_W("Already in stereo mode, firstly exit stereo mode!");
        GapBtInterfaceEnvCtxGet()->ux_delay_setupstereo = true;
        GapSetEnterFreemanModeFlag(true);
        app_ibrt_if_event_entry(IBRT_MGR_EV_FREE_MAN_MODE);
        return -1;
    }
    SetupStereoHandler(data->role, data->addr.addr);
#endif
    return 0;
}

void ExitStereo(void)
{
#ifdef IBRT
    bool ibrt_enabled = app_tws_ibrt_tws_link_connected();
    LOG_I("%s tws connect:%d", __func__, ibrt_enabled);

    if (GapBtInterfaceEnvCtxGet()->ux_doing_tws_paring_flag)
    {
        LOG_W("Currently setup stereo is ongoing, not allow to cancel setup stereo");
        return;
    }

    app_ibrt_if_event_entry(IBRT_MGR_EV_EXIT_PAIRING_MODE);
    if(!ibrt_enabled)
    {
        GapSetEnterFreemanModeFlag(true);
        app_ibrt_if_event_entry(IBRT_MGR_EV_FREE_MAN_MODE);
    }
    else
    {
        // need notify peer device to exit stereo mode firstly.
        ibrt_notify_exit_stereo_mode_t exit_stereo_notify_info;
        exit_stereo_notify_info.ux_dev_mode = GAP_DEV_MODE_SINGLE;
        exit_stereo_notify_info.initing_side_running_exit_stereo = true;
        if (app_ibrt_conn_get_ui_role() != TWS_UI_SLAVE)
        {
            exit_stereo_notify_info.initing_side_running_exit_stereo = true;
        }
        else
        {
            // resume slave ble irk
            appm_refresh_ble_irk();
            exit_stereo_notify_info.initing_side_running_exit_stereo = false;
        }
        tws_ctrl_send_cmd(APP_TWS_CMD_EXIT_STEREO_MODE, (uint8_t*)&exit_stereo_notify_info, sizeof(exit_stereo_notify_info));
    }
#endif
}

void SetUxDoingStereoReconnectFlag(bool reconnectFlag)
{
    LOG_I("%s", __func__);
    g_uxTwsReconnectAction = reconnectFlag;
}

void GetUxDoingStereoReconnectFlag(void)
{
    LOG_I("%s", __func__);
    //return g_uxTwsReconnectAction;
}

void StereoReconnect(void)
{
    LOG_I("%s", __func__);
#ifdef IBRT
    if ((GapBtInterfaceEnvCtxGet()->ux_dev_mode == GAP_DEV_MODE_STEREO) && \
            (GapBtInterfaceEnvCtxGet()->ux_doing_tws_paring_flag == false) && \
            (GapBtInterfaceEnvCtxGet()->ux_dev_poweroff_status == false))
    {
        app_ibrt_if_reconnect_once();
        if(ibrt_mgr_get_latest_box_state() != IBRT_IN_BOX_OPEN)
        {
            app_ibrt_if_event_entry(IBRT_MGR_EV_CASE_OPEN);
        }
        else
        {
            app_ibrt_if_enter_pairing_after_tws_connected();
        }
    }
    else
    {
        LOG_W("Currently not in stereo mode!");
    }
#endif
}

int StereoSendData(unsigned char *buf, unsigned int length)
{
    int ret = 0;
    LOG_I("%s", __func__);
#ifdef IBRT
    ret = tws_ctrl_send_cmd(APP_IBRT_CUSTOM_CMD_TEST1, buf, length);
#endif
    return ret;
}

void DevicePowerOffPreHandler()
{
    LOG_I("%s", __func__);
    GapBtInterfaceEnvCtxGet()->ux_dev_poweroff_status = true;
#ifdef IBRT
    if(!app_tws_ibrt_tws_link_connected()){
        SetBtScanMode(OHOS_GAP_SCAN_MODE_NONE,0);
        PairedDeviceInfo devInfo;
        if(GetRemoteDevAddr(&devInfo)){
            LOG_I("%s disconnect remote moblie link", __func__);
            DisconnectRemoteDevice(&devInfo.addr);
        }
    }
    app_ibrt_if_event_entry(IBRT_MGR_EV_CASE_CLOSE);
    osDelay(1000);
#endif
}

static bool user_choose_chnl_enable = false;
static A2DP_AUDIO_CHANNEL_SELECT_E s_current_channel=A2DP_AUDIO_CHANNEL_SELECT_STEREO;
static void user_stereo_set_channel(A2DP_AUDIO_CHANNEL_SELECT_E channel)
{
    s_current_channel=channel;
}
A2DP_AUDIO_CHANNEL_SELECT_E user_stereo_get_channel(void)
{
    return s_current_channel;
}

bool audio_channel_user_config_enable()
{
    return user_choose_chnl_enable;
}

int StereoChannelSwitch(StereoChannelSet channel)
{
    LOG_I("%s channel=%d", __func__, channel);
    user_choose_chnl_enable = true;
    if(LEFT_CHANNEL==channel){
        user_stereo_set_channel(A2DP_AUDIO_CHANNEL_SELECT_LCHNL);
        a2dp_audio_set_channel_select(A2DP_AUDIO_CHANNEL_SELECT_LCHNL);
#if BLE_AUDIO_ENABLED
        app_bis_set_channel_select(BIS_AUDIO_CHANNEL_SELECT_LCHNL);
#endif
    }else if(RIGHT_CHANNEL==channel){
        user_stereo_set_channel(A2DP_AUDIO_CHANNEL_SELECT_RCHNL);
        a2dp_audio_set_channel_select(A2DP_AUDIO_CHANNEL_SELECT_RCHNL);
#if BLE_AUDIO_ENABLED
        app_bis_set_channel_select(BIS_AUDIO_CHANNEL_SELECT_RCHNL);
#endif
    }else{
        user_stereo_set_channel(A2DP_AUDIO_CHANNEL_SELECT_STEREO);
        a2dp_audio_set_channel_select(A2DP_AUDIO_CHANNEL_SELECT_STEREO);
#if BLE_AUDIO_ENABLED
        app_bis_set_channel_select(BIS_AUDIO_CHANNEL_SELECT_LRMERGE);
#endif
    }
    return 0;
}

int StereoMasterSlaveSwitch(BdAddr *addr, StereoRole role)
{
    uint32_t ibrt_role = 0xff;
    LOG_I("%s role=%d", __func__, role);
#ifdef IBRT
    if(role == STEREO_MASTER)
        ibrt_role = IBRT_MASTER;
    else if(role == STEREO_SLAVE)
        ibrt_role = IBRT_SLAVE;
    else
        return -1;

    if(app_ibrt_conn_get_ui_role() == IBRT_UNKNOW)
        return -1;

    if(ibrt_role != app_ibrt_conn_get_ui_role())
        app_ibrt_if_tws_role_switch_request();

    return 0;
#else
    return 0;
#endif
}

BdAddr *GetExternalDisplayAddress(void)
{
    static BdAddr local_bt_addr;
    bt_bdaddr_t local_addr;
    factory_section_original_btaddr_get((uint8_t *)local_addr.address);
    memcpy_s(local_bt_addr.addr,OHOS_BD_ADDR_LEN,local_addr.address, OHOS_BD_ADDR_LEN);
    return (BdAddr*)(&local_bt_addr);
}

void app_tws_rec_cmd_handler(uint8_t *buf,uint32_t len)
{
    if(ReceiveTwsDataCb){
        ReceiveTwsDataCb(buf,len);
    }
}

int RegisterMediaStreamNotifyCallback(MediaStreamNotifyCallback cb)
{
    MediaStreamNotifyCb = cb;
    app_a2dp_notify_receive_stream_callback_register(app_notify_receive_media_data);
    return 0;
}

int RegisterStereoDateNotifyCallback(StereoReceiveData func)
{
#ifdef IBRT
    ReceiveTwsDataCb = func;
    app_ibrt_custom_cmd_rx_register(app_tws_rec_cmd_handler);
#endif
    return 0;
}

int RegisterStereoNotifyCallback(StereoStateChangedCallback func)
{
#ifdef IBRT
    GapRegisterUxStateChangeCallback(func);
#endif
    return 0;
}

int RegisterEnDecryptCallback(HksEnDecryptLinkKey encryptCb, HksEnDecryptLinkKey decryptCb)
{
#ifdef LINK_KEY_ENCRYPT_BY_CUSTOMER
    AES256_GCM_encrypt_register(encryptCb);
    AES256_GCM_decrypt_register(decryptCb);
#endif
    return 0;
}

int RegisterBisEnDecryptCallback(HksEnDecryptLinkKey encryptCb, HksEnDecryptLinkKey decryptCb)
{
#ifdef LINK_KEY_ENCRYPT_BY_CUSTOMER
    aes256_bis_encrypt_register(encryptCb);
    aes256_bis_decrypt_register(decryptCb);
#endif
    return 0;
}

static uint8_t sinkPlayStatusRecord = STEREO_STOP_PLAY;
ExecLeAudioSyncResult appSinkSyncCb = NULL;
LeAudioRole locla_le_audio_role = LE_AUDIO_UNKNOWN;
LeAudioSetRoleCallback le_audio_set_role_cb = NULL;

void UxSetStereoRole(StereoRole role)
{
#if BLE_AUDIO_ENABLED
    app_customer_set_ibrt_role(role);
#endif
}

void SetLeAudioRole(LeAudioRole role)
{
    locla_le_audio_role = role;
    if (le_audio_set_role_cb)
        le_audio_set_role_cb(0);
}

LeAudioRole GetLeAudioRole(void)
{
    return locla_le_audio_role;
}

void RegiterLeAduioMusicShareCallback(LeAudioSetRoleCallback setRoleCb)
{
    le_audio_set_role_cb = setRoleCb;
}

void ExitLeAudioMusicShareMode(void)
{
    locla_le_audio_role = LE_AUDIO_UNKNOWN;
}

void app_bis_sink_sync_callback(uint8_t status)
{
#if BLE_AUDIO_ENABLED
    if (status == BIS_SINK_SYNC_DISCONNECT)
    {
        sinkPlayStatusRecord = STEREO_STOP_PLAY;
    }

    if (appSinkSyncCb)
        appSinkSyncCb(status);
#endif
}

void AppNotifyStereoPlayStatus(StereoPlayMode playMode, StereoPlayStatus playStatus)
{
#if BLE_AUDIO_ENABLED
    static uint8_t playStatusRecord = STEREO_STOP_PLAY;
    if (locla_le_audio_role == LE_AUDIO_SLAVE)
    {
        LOG_I("%s role error:%d", __func__, locla_le_audio_role);
        return;
    }
    LOG_I("%s current:%d new:%d", __func__, playStatusRecord, playStatus);
    if (playStatusRecord != playStatus)
    {
        playStatusRecord = playStatus;
        app_set_src_cmd(playStatus);
        if (playStatus == STEREO_START_PLAY)
        {
            switch (playMode)
            {
                case WIFI_TO_BIS:
                    audio_sink_global_sink_type_set(AUDIO_SINK_LE_AUDIO);
                    app_bis_set_src_pcm_input_type(BIS_SRC_PCM_INPUT_WIFI);
                    app_bap_bc_src_start(0);
                break;
                case A2DP_TO_BIS:
                    app_bis_set_src_pcm_input_type(BIS_SRC_PCM_INPUT_BT);
                    app_bap_bc_src_start(0);
                break;
                default:
                break;
            }
        }
        else if (playStatus == STEREO_STOP_PLAY)
        {
            switch (playMode)
            {
                case WIFI_TO_BIS:
                    audio_sink_global_sink_type_set(AUDIO_SINK_ALSA);
                    app_bap_bc_src_stop(0);
                break;
                case A2DP_TO_BIS:
                    app_bap_bc_src_stop(0);
                break;
                default:
                break;
            }
        }
    }
    else
    {
        app_src_cmd_result(BIS_CMD_SUCCESS);
        LOG_I("%s playstatus error:%d", __func__, playStatus);
    }
#endif
}

void AppNotifyStereoSinkPlayStatus(StereoPlayStatus playStatus)
{
#if BLE_AUDIO_ENABLED
    if (locla_le_audio_role == LE_AUDIO_MASTER)
    {
        LOG_I("%s role error:%d", __func__, locla_le_audio_role);
        return;
    }
    LOG_I("%s current:%d new:%d", __func__, sinkPlayStatusRecord, playStatus);
    if (sinkPlayStatusRecord != playStatus)
    {
        sinkPlayStatusRecord = playStatus;
        app_set_sink_cmd(playStatus);
        if (playStatus == STEREO_START_PLAY)
        {
            app_bap_bc_scan_start(0);
        }
        else if (playStatus == STEREO_STOP_PLAY)
        {
            app_bap_bc_scan_stop();
            app_bis_sink_stop();
        }
    }
    else
    {
        app_sink_cmd_result(BIS_CMD_SUCCESS);
        LOG_I("%s playstatus error:%d", __func__, playStatus);
    }
#endif
}

int RegisterDataReportCallback(BtReportAudioCatonEventToApp reportEventCb)
{
    app_register_a2dp_cuton_notify_upper(reportEventCb);
    return 0;
}

void RegisterLeAudioCmdCallback(ExecLeAudioCmdStatusResult cmdResultCb)
{
#if BLE_AUDIO_ENABLED
    app_register_src_sink_cmd_callback((app_cmd_cb_t)cmdResultCb);
#endif
}

void RegisterLeAudioSyncCallback(ExecLeAudioSyncResult syncResultCb)
{
#if BLE_AUDIO_ENABLED
    app_register_sink_sync_callback(app_bis_sink_sync_callback);
    appSinkSyncCb = syncResultCb;
#endif
}

int StartHfpAudioPlay(void)
{
    app_hfp_audio_start();
    return 0;
}

int StopHfpAudioPlay(void)
{
    app_hfp_audio_stop();
    return 0;
}

int SetHfpAudioVolume(float volume)
{
    uint32_t local_vol=0;
    if(volume > 1.0f)
    {
        volume = 1.0f;
    }
    if(volume < 0.0f)
    {
        volume = 0.0f;
    }
    double volume_double = (double)(volume * 100.0f);
    double floor_volume = floor(volume_double);
    double ceil_volume = ceil(volume_double);
    uint32_t u32vol = (uint32_t)floor_volume;
    volume = (float)floor_volume;
    if ((volume_double - floor_volume) > (double)0.5)
    {
        u32vol += 1;
        volume = (float)ceil_volume;
    }
#ifdef TGT_VOLUME_LEVEL_USER_MAX
    if(u32vol == 0)
    {
        local_vol = 0;
        LOG_I("%s vol = 0, mute vol_dac = 0",__func__);
    }
    else
    {
        uint32_t step = tgt_user_volume_step_get();
        uint8_t vol_tbl = 0;
#ifdef TGT_VOLUME_LEVEL_TBL_2
        vol_tbl = alsa_volume_get_tbl();
        if (vol_tbl == ALSA_VOLUME_TBL_USER_CALL_APP)
        {
            step = tgt_user_volume_2_step_get();
        }
#endif
#ifndef TGT_VOLUME_LEVEL_SUPPORT_100
        uint32_t step_index = 0;
        for (step_index = 1; step_index<step;step_index++)
        {
            if(u32vol >= alsa_float_volume_mapping_array[step_index-1] && u32vol < alsa_float_volume_mapping_array[step_index])
            {
                break;
            }
        }
        if(u32vol == alsa_float_volume_mapping_array[step-1])
        {
            step_index = step;
        }
        local_vol = step_index;
#else
        local_vol = u32vol;
#endif
#ifdef TGT_VOLUME_LEVEL_TBL_2
        if (vol_tbl == ALSA_VOLUME_TBL_USER_CALL_APP)
        {
            local_vol += tgt_user_volume_2_base_get();
        }
#endif
        LOG_I("%s vol_tbl %d step %d, vol_32 =%d local_vol(step_index) =%d",__func__, vol_tbl, step,u32vol,local_vol);
    }
#else
    local_vol = u32vol/6;
    LOG_I("%s default_step vol_32 = %d vol_dac = %d",__func__,u32vol,local_vol);
#endif
#ifdef ALSA_AUDIO_PROCESS_RENDER_EN
    alsa_audio_process_render_set_vol(volume);
#endif
    hfp_update_local_volume(BT_DEVICE_ID_1,local_vol);
    return 0;
}

int SetA2dpMediaVolume(float volume)
{
    uint32_t local_vol = 0;
#ifdef BT_USE_UX_VOLUME
    uint8_t abs_volume = 0;
#endif
    if(volume > 1.0f)
    {
        volume = 1.0f;
    }
    if(volume < 0.0f)
    {
        volume = 0.0f;
    }
#ifdef BT_USE_UX_VOLUME
    abs_volume = (uint8_t)volume * MAX_A2DP_VOL;
#endif
    double volume_double = (double)(volume * 100.0f);
    double floor_volume = floor(volume_double);
    double ceil_volume = ceil(volume_double);
    uint32_t u32vol = (uint32_t)floor_volume;
    volume = (float)floor_volume;
    if ((volume_double - floor_volume) > (double)0.5)
    {
        u32vol += 1;
        volume = (float)ceil_volume;
    }
#ifdef TGT_VOLUME_LEVEL_USER_MAX
    if(u32vol == 0)
    {
        local_vol = 0;
        LOG_I("%s vol = 0, mute vol_dac = 0",__func__);
    }
    else
    {
        uint32_t step = tgt_user_volume_step_get();
        uint8_t vol_tbl = 0;
#ifdef TGT_VOLUME_LEVEL_TBL_2
        vol_tbl = alsa_volume_get_tbl();
        if (vol_tbl == ALSA_VOLUME_TBL_USER_CALL_APP)
        {
            step = tgt_user_volume_2_step_get();
        }
#endif
#ifndef TGT_VOLUME_LEVEL_SUPPORT_100
        uint32_t step_index = 0;
        for (step_index = 1; step_index<step;step_index++)
        {
            if(u32vol >= alsa_float_volume_mapping_array[step_index-1] && u32vol < alsa_float_volume_mapping_array[step_index])
            {
                break;
            }
        }
        if(u32vol == alsa_float_volume_mapping_array[step-1])
        {
            step_index = step;
        }
        local_vol = step_index;
#else
        local_vol = u32vol;
#endif
#ifdef TGT_VOLUME_LEVEL_TBL_2
        if (vol_tbl == ALSA_VOLUME_TBL_USER_CALL_APP)
        {
            local_vol += tgt_user_volume_2_base_get();
        }
#endif
        LOG_I("%s vol_tbl %d step %d, vol_32 =%d local_vol(step_index) =%d",__func__, vol_tbl, step,u32vol,local_vol);
    }
#else
    local_vol = u32vol/6;
    LOG_I("%s default_step vol_32 = %d vol_dac = %d",__func__,u32vol,local_vol);
#endif
#ifdef BT_USE_UX_VOLUME
    btif_ux_a2dpabsvolume_set(abs_volume);
    btif_ux_a2dplocalvol_set(local_vol);
#endif
#ifdef ALSA_AUDIO_PROCESS_RENDER_EN
    alsa_audio_process_render_set_vol(volume);
#endif
    a2dp_update_local_volume(BT_DEVICE_ID_1,local_vol);
    return 0;
}

int SetLeAudioMediaVolume(float volume)
{
    uint32_t local_vol=0;
    if(volume > 1.0f)
    {
        volume = 1.0f;
    }
    if(volume < 0.0f)
    {
        volume = 0.0f;
    }
    double volume_double = (double)(volume * 100.0f);
    double floor_volume = floor(volume_double);
    double ceil_volume = ceil(volume_double);
    uint32_t u32vol = (uint32_t)floor_volume;
    volume = (float)floor_volume;
    if ((volume_double - floor_volume) > (double)0.5)
    {
        u32vol += 1;
        volume = (float)ceil_volume;
    }
#ifdef TGT_VOLUME_LEVEL_USER_MAX
    if(u32vol == 0)
    {
        local_vol = 0;
        LOG_I("%s vol = 0, mute vol_dac = 0",__func__);
    }
    else
    {
        uint32_t step = tgt_user_volume_step_get();
        uint8_t vol_tbl = 0;
#ifdef TGT_VOLUME_LEVEL_TBL_2
        vol_tbl = alsa_volume_get_tbl();
        if (vol_tbl == ALSA_VOLUME_TBL_USER_CALL_APP)
        {
            step = tgt_user_volume_2_step_get();
        }
#endif
#ifndef TGT_VOLUME_LEVEL_SUPPORT_100
        uint32_t step_index = 0;
        for (step_index = 1; step_index<step;step_index++)
        {
            if(u32vol >= alsa_float_volume_mapping_array[step_index-1] && u32vol < alsa_float_volume_mapping_array[step_index])
            {
                break;
            }
        }
        if(u32vol == alsa_float_volume_mapping_array[step-1])
        {
            step_index = step;
        }
        local_vol = step_index;
#else
        local_vol = u32vol;
#endif
#ifdef TGT_VOLUME_LEVEL_TBL_2
        if (vol_tbl == ALSA_VOLUME_TBL_USER_CALL_APP)
        {
            local_vol += tgt_user_volume_2_base_get();
        }
#endif
        LOG_I("%s vol_tbl %d step %d, vol_32 =%d local_vol(step_index) =%d",__func__, vol_tbl, step,u32vol,local_vol);
    }
#else
    local_vol = u32vol/6;
    LOG_I("%s default_step vol_32 = %d vol_dac = %d",__func__,u32vol,local_vol);
#endif
#ifdef ALSA_AUDIO_PROCESS_RENDER_EN
    alsa_audio_process_render_set_vol(volume);
#endif
#if BLE_AUDIO_ENABLED
    app_bis_set_local_volume(local_vol);
#endif
    return 0;
}

BdAddr *GetRpaAddress(void)
{
    BdAddr bleRpaAddr;
    memcpy(bleRpaAddr.addr, app_get_ble_rpa_address(), OHOS_BD_ADDR_LEN);
    return NULL;
}

void RegisterTwsAcceptConnectCallback(bool (*ArbitrationFunc)(void))
{
    ux_accept_tws_conn_result_callback = ArbitrationFunc;
}

bool GetUxIsEnableAcceptTwsConnectReq(void)
{
    bool ret = false;
    if (ux_accept_tws_conn_result_callback != NULL)
        ret = ux_accept_tws_conn_result_callback();
    return ret;
}

#endif

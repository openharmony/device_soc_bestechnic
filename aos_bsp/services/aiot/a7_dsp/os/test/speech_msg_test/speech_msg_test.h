#ifdef SPEECH_MSG_TEST_CASE
#ifndef __SPEECH_MSG_TEST_H__
#define __SPEECH_MSG_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ASR_AUDIO__SAMPLE_RATE          1600
#define ASR_AUDIO_CHANNEL		1
#define VOIP_AUDIO_SAMPLE_RATE          1600
#define VOIP_AUDIO_CHANNEL		1

typedef enum {
    // A -> M33
    WAKEUP_EVENT = 0,
    ASR_AUDIO_EVENT,
    HFP_AUDIO_EVENT,
    VOIP_AUDIO_OPUS_UPLOAD,	/* Upload voip call */
    VOIP_AUDIO_PCM_DOWNLOAD,	/* Download voip call */
    TALK_AUDIO_OPUS,
    TALK_AUDIO_PCM,
    WAKEUP_UPLOAD_META_INFO_EVENT,
    WAKEUP_UPLOAD_AUDIO_EVENT,
    WAKEUP_UPLAD_BELOW_THRESHOLD,	/* 未达到门限的唤醒音频上传 */
    PRE_WAKEUP_EVENT,

    // M33->A
    MUTE_EVENT = 50,
    UNMUTE_EVENT,
    START_CAPTURE,
    STOP_CAPTURE,
    VOIP_START,
    VOIP_DECODE,
    VOIP_STOP,
    HFP_START,
    HFP_STOP,
    TALK_START,
    TALK_DECODE,
    TALK_STOP,
    GET_WAKEUP_META_INFO_EVENT,
    GET_AUDIO_INFO_EVENT,
    PLACEMENT_DIRECTION,

    // MODE A核START TEST MODE
    A7_TEST_RX_START = 70,
    A7_TEST_RX_STOP,
    A7_TEST_TX_START,
    A7_TEST_TX_STOP,

    CORE_MESSAGE_MAX_ID
} SpeechMessageID;

typedef struct {
    SpeechMessageID type;
    int value;
    int size;
    char data[0];
} SpeechMessage;

typedef enum {
    A7_WAKEUP_MODE,
    A7_TALK_MODE,
    A7_VOIP_MODE,
    A7_HFP_MODE,
    A7_MAX_MODE,
} CurrentMode;

typedef struct {
    /* data */
    size_t offset;
    int wakeupszie;
} WakeupAduioInfo;

uint32_t speech_msg_get_signal(int signal);
int speech_msg_signal_send(char * buf, uint32_t len, uint32_t signal);
void speech_msg_test_init(void);
void speech_msg_test_tx_init(void);
void speech_msg_set_tx_interval(uint32_t interval);
void speech_msg_test_rx_init(void);
void speech_msg_tx_status(uint8_t enable);
void speech_msg_tx_start(uint32_t interval);
void speech_msg_tx_stop(void);
// a7 command
void speech_msg_a7_tx_start(uint32_t interval);
void speech_msg_a7_tx_stop(void);
void speech_msg_a7_rx_start(void);
void speech_msg_a7_rx_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* __SPEECH_MSG_H__ */
#endif //SPEECH_MSG_ENABLE
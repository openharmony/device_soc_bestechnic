
/*******************************************************************************************************************************/
/*********************************************bt controller symbol**************************************************************/

#ifndef  __BT_CONTROLLER_SYMBOL_H__
#define  __BT_CONTROLLER_SYMBOL_H__


#define HCI_FC_ENV_ADDR                                               0XC0008048
#define LD_ACL_ENV_ADDR                                               0XC000069C
#define BT_UTIL_BUF_ENV_ADDR                                          0XC0000864
#define BLE_UTIL_BUF_ENV_ADDR                                         0XC0006D64
#define LD_BES_BT_ENV_ADDR                                            0XC000170C
#define DBG_STATE_ADDR                                                0XC0005D5C
#define LC_STATE_ADDR                                                 0XC0000B6C
#define LD_SCO_ENV_ADDR                                               0XC00006B4
#define RX_MONITOR_ADDR                                               0XC00065C8
#define LC_ENV_ADDR                                                   0XC0000B5C
#define LM_NB_SYNC_ACTIVE_ADDR                                        0XC00003CE
#define LM_ENV_ADDR                                                   0XC00001AC
#define LM_KEY_ENV_ADDR                                               0XC0000304
#define HCI_ENV_ADDR                                                  0XC0007F78
#define LC_SCO_ENV_ADDR                                               0XC00003B0
#define LLM_ENV_ADDR                                                  0XC0007154
#define LD_ENV_ADDR                                                   0XC0000C40
#define RWIP_ENV_ADDR                                                 0XC0005AF4
#define BLE_RX_MONITOR_ADDR                                           0XC000657A
#define LLC_ENV_ADDR                                                  0XC0007A98
#define RWIP_RF_ADDR                                                  0XC0005A5C
#define LD_ACL_METRICS_ADDR                                           0XC00003E8
#define RF_RX_HWGAIN_TBL_ADDR                                         0XC00043F6
#define RF_HWAGC_RSSI_CORRECT_TBL_ADDR                                0XC000436E
#define RF_RX_GAIN_FIXED_TBL_ADDR                                     0XC000656E
#define HCI_DBG_EBQ_TEST_MODE_ADDR                                    0XC0005E30
#define HOST_REF_CLK_ADDR                                             0XC0005A10
#define DBG_TRC_TL_ENV_ADDR                                           0XC0005EF4
#define DBG_TRC_MEM_ENV_ADDR                                          0XC0005F00
#define DBG_BT_COMMON_SETTING_ADDR                                    0XC0005DC0
#define DBG_BT_SCHE_SETTING_ADDR                                      0XC0005E60
#define DBG_BT_IBRT_SETTING_ADDR                                      0XC0005CCE
#define DBG_BT_HW_FEAT_SETTING_ADDR                                   0XC0005D8C
#define HCI_DBG_SET_SW_RSSI_ADDR                                      0XC0005E88
#define LP_CLK_ADDR                                                   0XC0005AF0
#define RWIP_PROG_DELAY_ADDR                                          0XC0005AE8
#define DATA_BACKUP_CNT_ADDR                                          0XC0004060
#define DATA_BACKUP_ADDR_PTR_ADDR                                     0XC0004064
#define DATA_BACKUP_VAL_PTR_ADDR                                      0XC0004068
#define SCH_MULTI_IBRT_ADJUST_ENV_ADDR                                0XC00083E8
#define RF_HWAGC_RSSI_CORRECT_TBL_ADDR                                0XC000436E
#define RF_RX_GAIN_THS_TBL_LE_ADDR                                    0XC00043D8
#define REPLACE_MOBILE_ADDR_ADDR                                      0XC0000B98
#define REPLACE_ADDR_VALID_ADDR                                       0XC00000B8
#define PCM_NEED_START_FLAG_ADDR                                      0XC00000AC
#define I2V_VAL_ADDR                                                  0XC000009C
#define I2V_THD_ADDR                                                  0XC0000046
#define RT_SLEEP_FLAG_CLEAR_ADDR                                      0XC0005A44
#define RF_RX_GAIN_THS_TBL_BT_3M_ADDR                                 0XC0006550
#define TESTMODE_3M_FLAG_ADDR                                         0XC0005A34
#define TESTMODE_3M_EN_ADDR                                           0XC0005A33
#define TESTMODE_1M_FLAG_ADDR                                         0XC0005A32
#define TESTMODE_1M_EN_ADDR                                           0XC0005A31
#define NORMAL_IQTAB_ADDR                                             0XC0006598
#define NORMAL_IQTAB_EN_ADDR                                          0XC0005A2C
#define POWER_ADJUST_EN_ADDR                                          0XC0005A30
#define LD_IBRT_ENV_ADDR                                              0XC0001720
#define LLM_LOCAL_LE_FEATS_ADDR                                       0XC0004470
#define ISOOHCI_ENV_ADDR                                              0XC0008424
#define RF_RX_GAIN_THS_TBL_BT_ADDR                                    0XC00043BA
#define LLD_CON_ENV_ADDR                                              0XC0007E98
#define RT_SLEEP_FLAG_CLEAR_ADDR                                      0XC0005A44
#define I2V_VAL_ADDR                                                  0XC000009C
#define RT_SLEEP_FLAG_CLEAR_ADDR                                      0XC0005A44
#define RF_RX_GAIN_THS_TBL_BT_3M_ADDR                                 0XC0006550
#define TESTMODE_3M_FLAG_ADDR                                         0XC0005A34
#define NORMAL_IQTAB_ADDR                                             0XC0006598
#define POWER_ADJUST_EN_ADDR                                          0XC0005A30
#define LLD_ISO_ENV_ADDR                                              0XC0007EC4
#define HCI_DBG_SET_TRANSQ_FLAG_ADDR                                  0XC0005D40

//commit 6cf5998520b12d029989f3892bf2b4197f0f7962
//Author: donghuiguo <donghuiguo@bestechnic.com>
//Date:   Thu Aug 24 11:08:28 2023 +0800
//    Fix le channel map error about 3e disconnection.
//    
//    Change-Id: I64107282e9a39d782820c3a85928ccd7bdb57ff1

#ifndef  BT_CONTROLLER_COMMIT_ID
#define  BT_CONTROLLER_COMMIT_ID                    "commit 6cf5998520b12d029989f3892bf2b4197f0f7962"
#endif
#ifndef  BT_CONTROLLER_COMMIT_AUTHOR
#define  BT_CONTROLLER_COMMIT_AUTHOR                "Author: donghuiguo <donghuiguo@bestechnic.com>"
#endif
#ifndef  BT_CONTROLLER_COMMIT_DATE
#define  BT_CONTROLLER_COMMIT_DATE                  "Date:   Thu Aug 24 11:08:28 2023 +0800"
#endif
#ifndef  BT_CONTROLLER_COMMIT_MESSAGE
#define  BT_CONTROLLER_COMMIT_MESSAGE               "    Fix le channel map error about 3e disconnection.          Change-Id: I64107282e9a39d782820c3a85928ccd7bdb57ff1 "
#endif

#endif

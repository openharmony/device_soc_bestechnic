#!/bin/bash

##### change A7_PROJECT_PARAM to genie if need to open ali algorithm #####
A7_PROJECT_PARAM=dummy
MCU_PROJECT_PARAM=$A7_PROJECT_PARAM
MIC_SAMPLERATE_PARAM=16000
MIC_PERIOD_MS_PARAM=64
AQE_KWS_PARAM=0
ALI_KWS_PARAM=0
WEBRTC_VAD_PARAM=0
VAD_RNN_PARAM=0
MSMART_KWS_PARAM=0
if [ "$AQE_KWS_PARAM$ALI_KWS_PARAM$WEBRTC_VAD_PARAM$VAD_RNN_PARAM" != "0000" ]; then
   MCU_PROJECT_PARAM=kws
   if [ $AQE_KWS_PARAM == 1 ]; then
      A7_PROJECT_PARAM=kws
      MIC_PERIOD_MS_PARAM=16
   else
      MIC_PERIOD_MS_PARAM=32
   fi
elif [ $MSMART_KWS_PARAM == "1" ]; then
    A7_PROJECT_PARAM=listen
    MCU_PROJECT_PARAM=listen
    MIC_SAMPLERATE_PARAM=16000
    MIC_PERIOD_MS_PARAM=64
fi

COMPRESS_DSP_CODE=1

WIFI_FW_LZMA_PARAM=0

COMPRESS_SRAM_CODE=0
COMPRESS_PSRAM_CODE=0

# if open disp, either A7 or m33 in use.
A7_DISP_PARAM=0
M33_DISP_PARAM=0

####### each project need to config following toolchain path #########
TOOLCHAIN_10_2_1="/home/sh10/gcc/gcc-arm-none-eabi-10-2020-q4-major/bin"
TOOLCHAIN_5_4_1="/home/toolchain/ali_5.4.1/gcc-arm-none-eabi/Linux64/bin"

####### FLASH TYPE & SECURE_TYPE #########
SECURE_TYPE=1
OTA_CODE_OFFSET=0x18000
BOOT_INFO_OFFSET=0x10000
FLA_SIZE=0x1000000
TRACE_BAUD_RATE=1500000
CP_XIP=0
CSI_DSI_LOOP_V=0
A7_DSP_V=1
PSRAM_XCCELA_MODE=0
RF_TX_CONTROL_IO_PARAM=16
PSRAMUHS_AP325608A=0
PSRAMULS_MODE=0
ALT_BOOT_FLASH=0
PSRAM_APS3208K=0
ODT_CALIB=0
for arg in $*
do
   if [ $arg == "cpxip=1" ];then
      CP_XIP=1
   fi
   if [ $arg == "CSI_DSI_LOOP=1" ];then
      CSI_DSI_LOOP_V=1
      A7_DSP_V=0
   fi
   if [ $arg == "AX4D=1" ];then
      PSRAM_XCCELA_MODE=0
      PSRAMUHS_AP325608A=0
      PSRAMULS_MODE=0
      ODT_CALIB=1
   fi
   if [ $arg == "AX4C=1" ];then
      PSRAMULS_MODE=1
   fi
   if [ $arg == "GX6D=1" ];then
      PSRAM_XCCELA_MODE=1
      RF_TX_CONTROL_IO_PARAM=100
      SDMMC_DET_IO=2
      FLA_SIZE=0x2000000
      ODT_CALIB=1
   fi
   if [ $arg == "NX6D=1" ];then
      PSRAM_XCCELA_MODE=1
      RF_TX_CONTROL_IO_PARAM=100
   fi
   if [ $arg == "AX4F=1" ];then
      PSRAMUHS_AP325608A=1
      ALT_BOOT_FLASH=1
      RF_TX_CONTROL_IO_PARAM=100
      ODT_CALIB=1
   fi
   if [ $arg == "NX4F=1" ];then
      PSRAMUHS_AP325608A=1
   fi
   if [ $arg == "GX4F=1" ];then
      PSRAM_XCCELA_MODE=1
      RF_TX_CONTROL_IO_PARAM=100
      PSRAMUHS_AP325608A=1
      FLA_SIZE=0x2000000
   fi
   if [ $arg == "8X3CAB=1" ];then
      PSRAM_APS3208K=1
      PSRAMULS_MODE=1
      FLA_SIZE=0x800000
   fi
   if [ $arg == "8X3CAE=1" ];then
      PSRAM_HYPERBUS_MODE=1
      PSRAMULS_MODE=1
      FLA_SIZE=0x800000
   fi
   if [ $arg == "WIFI_RELEASE_ORIGIN=1" ];then
      WIFI_RELEASE_ORIGIN=1
   fi
done
echo "SECURE_TYPE=$SECURE_TYPE"

FS_SIZE="0"
# config psram fs
PSRAMFS_OPT=" PSRAM_XCCELA_MODE=$PSRAM_XCCELA_MODE PSRAMULS_MODE=$PSRAMULS_MODE ODT_CALIB=$ODT_CALIB PSRAMUHS_AP325608A=$PSRAMUHS_AP325608A PSRAM_APS3208K=$PSRAM_APS3208K \
              PSRAMFS_SIZE=${FS_SIZE} MCU_SLAVE_FS_EN=0 SDMMC_DET_GPIO=$SDMMC_DET_IO PSRAM_HYPERBUS_MODE=$PSRAM_HYPERBUS_MODE"

####### each project need to config project #########
if [ "$MCU_PROJECT_PARAM" == "kws" ]; then
   A7_PROJECT_PARAM=genie
   A7_DSP_BIN_NAME=best2600w_a7
else
   A7_DSP_BIN_NAME=best2003_a7
fi

COMPRESS_DSP_CODE_NAME=$A7_DSP_BIN_NAME'_text2'

M33_BIN_NAME=best2003_aiot
COMPRESSED_BIN_NAME_SRAM=$M33_BIN_NAME'_sram'
COMPRESSED_BIN_NAME_FSRAM=$M33_BIN_NAME'_fasram'
COMPRESSED_BIN_NAME_CPSRAM=$M33_BIN_NAME'_text_cpsram'
COMPRESSED_BIN_NAME_CPSRAM_DATA=$M33_BIN_NAME'_data_cpsram'
COMPRESSED_BIN_NAME_PSRAM=$M33_BIN_NAME'_psram'

BUILDOPT_BOOT="ALT_BOOT_FLASH=$ALT_BOOT_FLASH PSRAM_XCCELA_MODE=$PSRAM_XCCELA_MODE PSRAMULS_MODE=$PSRAMULS_MODE PSRAMUHS_AP325608A=$PSRAMUHS_AP325608A"

make_boot1="make T=ota_boot_aiot BUILD_BOOT2=1 FLASH_SIZE=$FLA_SIZE OTA_CODE_OFFSET=0 TRACE_BAUD_RATE=$BAUD_RATE SECURE_BOOT=$SECURE_TYPE TRACE_BAUD_RATE=$TRACE_BAUD_RATE \
            BOOTINFO_OFFSET=$BOOT_INFO_OFFSET REMAP_SUPPORT=1 RTOS_A_CODE_OFFSET=0x18000 RTOS_B_CODE_OFFSET=0xC18000 $BUILDOPT_BOO -j8"

BUILDOPT_M33="CHIP=best2003 A7_DSP_ENABLE=$A7_DSP_V DSP_AUDIO_TASK=1 DSP_CODEC_SUPPORT=0 SECURE_BOOT=$SECURE_TYPE \
              A7_SRAM=0 PSRAM_ENABLE=1 PSRAMUHS_ENABLE=1 SPEECH_MSG_TEST_CASE=1 TRACE_BAUD_RATE=$TRACE_BAUD_RATE \
              MIC_SAMPLERATE=$MIC_SAMPLERATE_PARAM MIC_PERIOD_MS=$MIC_PERIOD_MS_PARAM \
              A7_DISPLAY=$A7_DISP_PARAM M33_DISPLAY=$M33_DISP_PARAM CONFIG_DISPLAY_A064=0 \
              OPENAMP_RPTUN=0 \
              FLASH_SIZE=$FLA_SIZE ${PSRAMFS_OPT} LIB_BIN_IN_SRC_DIR=1 OPT_LEVEL=2 -j8"

BUILD_BT_CONFIG="BLE_STACK_NEW_DESIGN=1 \
BLE_AUDIO_DOLPHIN_COMPATIBLE_SUPPORT=0 \
BLE_AUDIO_STARLORD_COMPATIBLE_SUPPORT=0 \
BT_RAMRUN_NEW=0 \
BLE=1 \
BLE_AUDIO_ENABLED=0 \
AOB_MOBILE_ENABLED=0 \
APP_TRACE_RX_ENABLE=0 \
IGNORE_POWER_ON_KEY_DURING_BOOT_UP=0 \
IS_AUTOPOWEROFF_ENABLED=0 \
POWER_ON_ENTER_TWS_PAIRING_ENABLED=0 \
FREE_TWS_PAIRING_ENABLED=0 \
BT_HID_HEAD_TRACKER_PROTOCOL=0 \
BT_HID_DEVICE=0 \
BT_DIP_SUPPORT=0 \
BT_MAP_SUPPORT=0 \
BT_PBAP_SUPPORT=0 \
BT_OPP_SUPPORT=0 \
BT_PAN_SUPPORT=0 \
GATT_OVER_BR_EDR=0 \
CTKD_ENABLE=0 \
IS_CTKD_OVER_BR_EDR_ENABLED=0 \
HOST_GEN_ECDH_KEY=0 \
AI_VOICE=0 \
AMA_VOICE=0 \
TOTA_v2=0 \
TEST_OVER_THE_AIR_v2=0 \
THROUGH_PUT=0 \
IS_HCI_ISO_PACKET_TRACE_ENABLED=1 \
APP_UI_TEST_MODE_ENABLE=0 \
BLE_AUDIO_STEREO_CHAN_OVER_CIS_CNT=0 \
"

# for fit build increate
BUILD_SDK=0
BUILD_GEN_LIB=0
for arg in $*
do
   if [ $arg == "SDK=1" ];then
      BUILD_SDK=1
      BUILDOPT_M33=$BUILDOPT_M33" FORCE_TO_USE_LIB=1 "
      echo "find SDK = 1"
   fi
   if [ $arg == "GEN_LIB=1" ];then
      BUILD_GEN_LIB=1
      echo "find GEN_LIB = 1"
   fi
done

if [ $COMPRESS_DSP_CODE == 1 ];then
   BUILDOPT_M33=$BUILDOPT_M33" COMPRESS_DSP_CODE=$COMPRESS_DSP_CODE COMPRESS_DSP_CODE_NAME=$COMPRESS_DSP_CODE_NAME "
fi

make_cmcp=""

if [ "$MCU_PROJECT_PARAM" == "genie" ]; then
   make_a7="make T=$A7_DSP_BIN_NAME \
   A7_UART=4 \
   $BUILDOPT_M33 A7_PROJECT=$A7_PROJECT_PARAM \
   MMC_DISK_FATFS_IN_A7=0 KERNEL=RHINO HW_MODULE=0 \
   MCU_DSP_SEPERATE=1 A7_ALI_LIB=1 A7_CMSIS_TEST_CASE=0 LITTLEFS_ENABLE=0 A7_LITTLEFS_TEST=0 LFS_ON_RAM=0"
else
   make_a7="make T=$A7_DSP_BIN_NAME \
   A7_UART=2 \
   $BUILDOPT_M33 A7_PROJECT=$A7_PROJECT_PARAM \
   MMC_DISK_FATFS_IN_A7=0 \
   KERNEL=LITEOS_A A7_CMSIS_TEST_CASE=0 LITTLEFS_ENABLE=0 A7_LITTLEFS_TEST=0 LFS_ON_RAM=0"
fi

make_best2600w="make T=$M33_BIN_NAME MODULE_KERNEL_STUB=1 OHOS_SUPPORT=1 \
                LWIP_SUPPORT=1 NET_LWIP_SUPPORT=1 LWIP_ETHERNETIF=1 NET_LWIP_EXTERN=2 AP_MODE_SUPPORT=1 WIFI_IPERF_SUPPORT=1 \
                $BUILD_BT_CONFIG INTERSYS_DEBUG=1 WIFI_APP_SUPPORT=1 JTAG_ENABLE=1 \
                OTA_CODE_OFFSET=$OTA_CODE_OFFSET FLASH_REMAP=1 OTA_REMAP_OFFSET=0xC00000 OTA_ENABLE=1 OTA_BOOT_INFO_OFFSET=$BOOT_INFO_OFFSET \
                CSI_DSI_LOOP=$CSI_DSI_LOOP_V TRACE_BUF_SIZE=32768 \
                RF_TX_CONTROL_IO=$RF_TX_CONTROL_IO_PARAM \
                NET_SUPPORT=1 CHIP_OCD=0 \
                WIFI_RF_TEST_MODULE=1 WIFI_TEST=0 WIFI_NONSIGNALING_MODE=0 AIOT_FACTORY_EN=1 \
                WIFI_FW_LZMA=$WIFI_FW_LZMA_PARAM \
                COMPRESS_SRAM_CODE=$COMPRESS_SRAM_CODE \
                COMPRESS_PSRAM_CODE=$COMPRESS_PSRAM_CODE \
                ALT_BOOT_FLASH=$ALT_BOOT_FLASH $BUILDOPT_M33 DSP_BIN_NAME=$A7_DSP_BIN_NAME A7_PROJECT=$MCU_PROJECT_PARAM DUAL_BAND=1"

################################ functions list begin #########################
BUILD_INFO_LIST=()
BUILD_INDEX=0
GLOBAL_OPTS=""
RELEASE_SDK=0

SPACE=" "
EQUEL="="

# Set variables according to real environment
# on ubuntu
dev_env=`uname | sed 'y/LINUX/linux/'`
if [ "x$dev_env" == "xlinux" ]; then
    JOBS=`grep ^processor /proc/cpuinfo|wc -l`
else
    error "Unsupported Build System : $dev_env"
fi

error () {
    set +u
    echo "$0: error: $@" >&2
    exit 1
    set -u
}

build_cmd_echo="\n"
tmp_path=$PATH
function build_result()
{
   eval cmd="\${$1}"
   cmd=`echo $cmd`

   toolchain_path=`which arm-none-eabi-gcc`
   toolchain_info="\n=== build with toolchain [$toolchain_path] ===\n"
   PATH=$tmp_path
   if [ "x$2" != "x1" ];then
      echo -e "$toolchain_info"
      echo -e "\033[41m[build err]\033[0m ${cmd}\n"
      pwd_cmd=`pwd`
      project_root="$pwd_cmd"
      echo -e "\033[41m[build log]\033[0m $project_root/${3}\n"
      if [ "x${BUILD_PIECE}" != "xtrue"  ] ;then
         exit 1
      fi
      error 1
   fi
   build_cmd_echo+=$toolchain_info
   build_cmd_echo+="${cmd}\n\n"
}

#pre-handle option of every target,for now GEN_LIB and SDK
function pre_handle_global_opt()
{
    for((i=2; i<=$#; i++))
    do
        GLOBAL_OPTS+="$SPACE${!i}"
    done
    printf "pre_handle_global_opt $GLOBAL_OPTS\n"
}

#pre-handle options of target
function pre_handle_opt()
{
   eval cmd="\${$1}"
   cmd=`echo $cmd`
   # printf "\n[$1] $cmd\n"
   temp_cmd=""

   for input_opt in $*
   do
      #strip first para
      if [ "x${1}" == "x${input_opt}" ] ;then
         continue
      fi
      # parse key value by =
      input_opt_key=${input_opt%%=*}

      flag_match=0
      for cmd_opt in $cmd
      do
         # parse key value by =
         cmd_opt_key=${cmd_opt%%=*}

         # cover same parameter by check key
         if [ "x${input_opt_key}" == "x${cmd_opt_key}" ] ;then
            printf "[$1][opt] ${cmd_opt} covered by ${input_opt}\n"
            temp_cmd+="$input_opt$SPACE"
            flag_match=1
            continue
         fi

         # keep current parameter not matched
         #temp_cmd+="$cmd_opt$SPACE"
      done

      # handle no matched parameters
      if [ "x${flag_match}" == "x0" ] ;then
         printf "[$1][opt] target add ${input_opt}\n"
         temp_cmd+="$input_opt$SPACE"
      fi
      # printf "[temp_cmd] ${temp_cmd} \n"
      cmd=$temp_cmd
   done
   # printf "[cmd] ${cmd} \n"
   BUILD_INFO_LIST[BUILD_INDEX]="$1,${cmd}"
   printf "[GEN] ${BUILD_INFO_LIST[BUILD_INDEX]}\n"
   let BUILD_INDEX=BUILD_INDEX+1
}

function pre_handle_build_enable()
{
   for input_opt in $*
   do
      OLD_IFS="$IFS"
      IFS="="
      arry=(${input_opt})
      IFS="$OLD_IFS"
      if [ "x${arry[0]}" == "xBUILD_SE" ] ;then
         BUILD_SE=${arry[1]}
      fi
      if [ "x${arry[0]}" == "xBUILD_MINI" ] ;then
         BUILD_MINI=${arry[1]}
      fi
      if [ "x${arry[0]}" == "xbranch" ] ;then
         rel_branch="_${arry[1]}"
      fi
      if [ "x${arry[0]}" == "xBUILD_PIECE" ] ;then
         BUILD_PIECE="${arry[1]}"
      fi
   done
}

function pre_handle_relase()
{
   # printf "pre_handle_relase\n "
   RELEASE_SDK=1
}

function build_cmd()
{
   eval cmd="\${$1}"
   build_target=`echo $cmd`
   # printf "$build_target \n"

   build_format=" "
   if [ "x${BUILD_PIECE}" != "xtrue"  ] ;then
      for ((index=0; index<${#buid_m33_arry[@]}; index++))
      do
         if [ "x$1" == "x${buid_m33_arry[index]}" ] ;then
            build_format=" lib "
         fi
      done
   fi
   for ((index=0; index<$BUILD_INDEX; index++))
   do
      kv_cmd=${BUILD_INFO_LIST[index]}
      OLD_IFS="$IFS"
      IFS=","
      arr_cmd=(${kv_cmd})
      IFS="$OLD_IFS"
      if [ "x${arr_cmd[0]}" == "x$1" ] ;then
         build_target+="$SPACE${arr_cmd[1]}"
         break;
      fi
   done
   if [ "x${BUILD_INDEX}" == "x0"  ] ;then
      build_target="$build_target"
   fi

   build_target="$build_target$GLOBAL_OPTS${build_format}"

   BUILD_INFO_LIST[index]="$1,${build_target}"
   ### config toolchain ###
   if [ "x$2" == "x" ] ;then
      if [ -d $M33_TOOLCHAIN ] ;then
         export PATH=$M33_TOOLCHAIN:$PATH
      fi
   else
      eval toolchain="\${$2}"
      # printf "\ntoolchain : $toolchain \n"
      if [ -d $toolchain ]; then
         export PATH=$toolchain:$PATH
      fi
   fi
   log_path="out/log/bsp_build_$1.log"
   rm -rf $log_path
   mkdir -p out/log
   ${build_target} 2>&1 | tee $log_path
#   cat $log_path | awk 'END {print}' | grep "Error"  > /dev/null
   tail -n 5 $log_path | grep "Error" > /dev/null
   rc=$?
   # echo "rc = $rc"
   echo "${build_target}" >> $log_path
   build_result build_target $rc $log_path
}

function print_all_cmds()
{
   echo -e "\n~~~ build success! show all build command list ~~~\n"
   echo -e "${build_cmd_echo}" 2>&1 | tee out/log/bsp_build_cmd.log
   echo -e "~~~ build success! show all build command over  ~~~\n"
}

function release_sdk()
{
   if [ "x$RELEASE_SDK" == "x1" ]; then
      release_cmd="tools/relsw_$1 $1 WORKING $2 $WIFI_RELEASE_ORIGIN"
      echo -e "[release_sdk] $release_cmd\n"
      $release_cmd
   fi
}

usage ()
{
cat<<EOF
Usage:

[       -c,     -a,     -m,    -s,       -l,        -x,         -y,         -z,     -h,        -r ]
[  --cmcp=,  --a7=, --m33=, --se=, --lmini=, --xboot1=, --yboot2a=, --zboot2b=, --help, --release ]

EOF
exit 1
}
################################ functions list end   #########################


# total_param=$@
GETOPT_ARGS=`getopt -o c:a:m:s:l:x:y:z:d:hr -al cmcp:,a7:,m33:,se:,lmini:,xboot1:,yboot2a:,zboot2b:,defineEx:,help,release -- "$@"`

eval set -- "$GETOPT_ARGS"

while [ -n "$1" ]
do
#    printf "1:$1 2:$2 total_param:$total_param\n"
    case "$1" in
        -c|--cmcp)      pre_handle_opt make_cmcp $2;              shift 2;;
        -a|--a7)        pre_handle_opt make_a7  $2;               shift 2;;
        -m|--m33)       pre_handle_opt make_best2600w $2;         shift 2;;
        -s|--se)        pre_handle_opt make_best2600w_se $2;      shift 2;;
        -l|--lmini)     pre_handle_opt make_best2600w_mini $2;    shift 2;;
        -x|--xboot1)    pre_handle_opt make_boot1 $2;             shift 2;;
        -y|--yboot2a)   pre_handle_opt make_boot2a $2;            shift 2;;
        -z|--zboot2b)   pre_handle_opt make_boot2b $2;            shift 2;;
        -d|--defineEx)  pre_handle_build_enable $2;               shift 2;;
        -h|--help)      usage;                                    shift 1;;
        -r|--realse)    pre_handle_relase;                        shift 1;;
        --)             pre_handle_global_opt $@;break;                  ;;
        *) break ;;
    esac
done

if [ "x${BUILD_PIECE}" == "xtrue"  ] ;then
    cp out/$M33_BIN_NAME/services/aiot/a7_dsp/prj/$MCU_PROJECT_PARAM/built-in.a out/$M33_BIN_NAME/services/aiot/a7_dsp/prj/built-in.a
    ar -M <tools/best2003_liteos.mri
    mv libbest2003_aiot.a out/$M33_BIN_NAME/libbest2003_aiot.a
    cp -rf out/$M33_BIN_NAME/libbest2003_aiot.a out/$M33_BIN_NAME/libbspbest2003_aiot.a
    exit 0
else
   # for fit build increate
   BUILD_SDK=0
   for arg in $*
   do
      if [ $arg == "SDK=1" ];then
         BUILD_SDK=1
         echo "find SDK = 1"
      fi
   done
fi

if [ "x${BUILD_PIECE}" == "xtrue"  ] ;then
   for ((index=0; index<${#special_buid_src2lib_dir[@]}; index++))
   do
      pack_lib_dir=${special_buid_src2lib_dir[index]}
      if [ -d out/best2003_aiot/${pack_lib_dir} ] ;then
         echo "del out/best2003_aiot/$pack_lib_dir"
         rm -rf out/best2003_aiot/$pack_lib_dir
      fi
      rm -rf out/best2003_aiot/bsp.mri
      rm -rf out/best2003_aiot/libbspbest2003_aiot.a
   done
fi

if [ "x${BUILD_PIECE}" != "xtrue"  ] ;then
   rm -rf out/cmcp
   rm -rf out/${A7_DSP_BIN_NAME}
   rm -rf out/best2003_aiot

# make ota boot
#   if [ "x$BUILD_SDK" != "x1" ]; then
     sed -i "/SECURE_BOOT_VER == 1/i\#undef SECURE_BOOT" utils/boot_struct/boot_struct.c
     build_cmd make_boot1;
     sed -i "/#undef SECURE_BOOT/d" utils/boot_struct/boot_struct.c
#   fi

if [ "x${A7_DSP_V}" == "x1" ] ;then
    # make_a7
    if [ "$MCU_PROJECT_PARAM" == "kws" ]; then
        make_a7="make T=best2600w_a7 SDK=1 VOICE_DETECTOR_EN=1 LIB_BIN_IN_SRC_DIR=1 A7_UART=4 KERNEL=RHINO A7_ALI_LIB=1 CHIP=best2003 A7_DSP_ENABLE=1 DSP_AUDIO_TASK=1 DSP_CODEC_SUPPORT=0 DSP_USE_PSRAM=0 PSRAMDSP_SIZE=0 A7_SRAM=0 PSRAMUHS_ENABLE=1 PSRAM_ENABLE=1 SPEECH_MSG_ENABLE=1 MIC_SAMPLERATE=16000 MIC_PERIOD_MS=32 MCU_DSP=0 SPEECH_MSG_TEST_CASE=0 SPEECH_MSG_TEST_RX=0 SPEECH_MSG_TEST_TX=0 SPEECH_MSG_TEST_TXRX_LOOP=0 HW_MODULE=0 A7_HEARTBEAT_TIMEOUT=15000 SLEEP_STATS_TRACE=0 A7_PROJECT=genie PSRAMDSP_BASE=0x34000000 KERNEL=RHINO A7_CMSIS_TEST_CASE=0 -j8 MCU_DSP_SEPERATE=1"
        build_cmd make_a7 TOOLCHAIN_5_4_1
    elif [ "$MCU_PROJECT_PARAM" == "listen" ]; then
        build_cmd make_a7 TOOLCHAIN_5_4_1
    elif [ "$MCU_PROJECT_PARAM" == "genie" ]; then
        build_cmd make_a7 TOOLCHAIN_5_4_1
    else
        build_cmd make_a7 TOOLCHAIN_10_2_1
    fi

   # compress dsp bin
      if [ $COMPRESS_DSP_CODE == 1 ];then
         # strip text/data
         export PATH=$TOOLCHAIN_10_2_1:$PATH
         arm-none-eabi-objcopy -j .text -j .data -O binary out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.elf out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin
         cp out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin.org
         tools/lzma e out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin.lzma
         #mv out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin.lzma out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin
         tools/a7_lzma_partition.sh out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin.lzma out/$A7_DSP_BIN_NAME/$COMPRESS_DSP_CODE_NAME.bin
      fi
   cp out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin.bk
   tools/lzma e out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin.lzma
   mv out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin.lzma out/$A7_DSP_BIN_NAME/$A7_DSP_BIN_NAME.bin

   # mklittlefs
   FS_BIN_NAME="littlefs"
   let FS_BIN_SIZE=${FS_SIZE}
   ./tools/mklittlefs -c apps/bes_test/fs -d 5 -b 4096 -s $FS_BIN_SIZE out/$A7_DSP_BIN_NAME/$FS_BIN_NAME.bin
   # compress fs bin
   tools/lzma e out/$A7_DSP_BIN_NAME/$FS_BIN_NAME.bin out/$A7_DSP_BIN_NAME/$FS_BIN_NAME.bin.lzma
   mv out/$A7_DSP_BIN_NAME/$FS_BIN_NAME.bin.lzma out/$A7_DSP_BIN_NAME/$FS_BIN_NAME.bin
fi

if [ "x$BUILD_SDK" == "x1" ]; then
   # use prebuild cp bin
   echo "=============mkdir out for cmcp==============="
   mkdir -p out/cmcp/
   cp prebuild/cmcp.bin out/cmcp/cmcp.bin
   ls out/cmcp/
   echo "=============mkdir out for cmcp==============="
else
   # build cp
   if [ "$MCU_PROJECT_PARAM" == "kws" ]; then
      make_cmcp=$make_cmcp" CODEC_VAD_CFG_BUF_SIZE=0x8000 "
   fi
   build_cmd make_cmcp TOOLCHAIN_10_2_1
   # compress cp bin
if [ "x$CP_XIP" == "x0" ]; then
   tools/lzma e out/cmcp/cmcp.bin out/cmcp/cmcp.bin.lzma
   mv out/cmcp/cmcp.bin.lzma out/cmcp/cmcp.bin
fi
fi

# compress prebuild bins
# bt ramrun
cp platform/drivers/bt/best2003/best2003_ramrun_fw.bin out/prebuild/.
tools/lzma e out/prebuild/best2003_ramrun_fw.bin out/prebuild/best2003_ramrun_fw.lzma
mv out/prebuild/best2003_ramrun_fw.lzma out/prebuild/best2003_ramrun_fw.bin

# make_best2600w TrustZone App
   if [ "x$BUILD_SE" == "xtrue" ]; then
      build_cmd make_best2600w_se
   fi

# make_best2600w TrustZone App
   if [[ "x$BUILD_SE" == "xtrue" ]] && [[ "x$BUILD_MINI" == "xtrue" ]]; then
      build_cmd make_best2600w_mini_se
   fi
fi
# make_best2600w_mini
if [ "x$BUILD_MINI" == "xtrue" ]; then
   pre_build_into_lib_cmd make_best2600w_mini
   build_cmd make_best2600w_mini
fi

if [ "$WIFI_FW_LZMA_PARAM" == "1" ]; then
   for file in `ls platform/drivers/wifi/best2003/rom/*.bin`
   do
      tools/lzma e $file $file.lzma
   done
fi

# make_best2600w
if [ "$MCU_PROJECT_PARAM" == "kws" ]; then
   make_best2600w=$make_best2600w" MCU_DSP_SEPERATE=1 AQE_KWS=$AQE_KWS_PARAM ALI_KWS=$ALI_KWS_PARAM WEBRTC_VAD=$WEBRTC_VAD_PARAM VAD_RNN=$VAD_RNN_PARAM VOICE_DETECTOR_EN=1 DEBUG_SLEEP_USER=1 WAKEUP_SRC_STATS=1 CODEC_VAD_CFG_BUF_SIZE=0x8000 "
   make_best2600w=$make_best2600w" CP_BOOT=0 WIFI_ON_CP=0 BT_RAMRUN=0 WIFI_NO_IPC=1 WIFI_POWERSAVE=1 LWIP_NO_TIMER=0 SLEEP_STATS_TRACE=1 DMA_DEBUG=1"
fi

LOCAL_BUILD=1
# full function for bsp test
if [ "$BUILD_GEN_LIB$BUILD_SDK" == "00" ];then
   #make_best2600w=$make_best2600w" LPLAYER_EN=1 AAC_CODEC_SUPPORT=1 NET_LWIP_EXTERN=0 NET_MBEDTLS_SUPPORT=1 "
   make_best2600w=$make_best2600w" NET_LWIP_EXTERN=0 NET_MBEDTLS_SUPPORT=1 "
else
   LOCAL_BUILD=0
   make_best2600w=$make_best2600w" lib "
fi

# compress m33 bin partly
if [ "$COMPRESS_SRAM_CODE$COMPRESS_PSRAM_CODE" != "00" ];then
   make_best2600w=$make_best2600w" COMPRESSED_BIN_NAME_SRAM=$COMPRESSED_BIN_NAME_SRAM \
      COMPRESSED_BIN_NAME_FSRAM=$COMPRESSED_BIN_NAME_FSRAM \
      COMPRESSED_BIN_NAME_CPSRAM=$COMPRESSED_BIN_NAME_CPSRAM \
      COMPRESSED_BIN_NAME_CPSRAM_DATA=$COMPRESSED_BIN_NAME_CPSRAM_DATA \
      COMPRESSED_BIN_NAME_PSRAM=$COMPRESSED_BIN_NAME_PSRAM"
   # stub bin for 1st build
   mkdir out/$M33_BIN_NAME
   touch out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin
   touch out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin
   touch out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin
   touch out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin
   touch out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin
   # 1st build
   build_cmd make_best2600w TOOLCHAIN_10_2_1

   # gen lzma bin
   if [ "$COMPRESS_SRAM_CODE" == "1" ];then
      arm-none-eabi-objcopy -j .sram_text -j .sram_data -O binary out/$M33_BIN_NAME/$M33_BIN_NAME.elf out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin
      cp out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin.bk
      tools/lzma e out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin.lzma
      mv out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin.lzma out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_SRAM.bin

      arm-none-eabi-objcopy -j .fast_text_sram -O binary out/$M33_BIN_NAME/$M33_BIN_NAME.elf out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin
      cp out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin.bk
      tools/lzma e out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin.lzma
      mv out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin.lzma out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_FSRAM.bin

      arm-none-eabi-objcopy -j .cp_text_sram -O binary out/$M33_BIN_NAME/$M33_BIN_NAME.elf out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin
      cp out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin.bk
      tools/lzma e out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin.lzma
      mv out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin.lzma out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM.bin

      arm-none-eabi-objcopy -j .cp_data_sram -O binary out/$M33_BIN_NAME/$M33_BIN_NAME.elf out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin
      cp out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin.bk
      tools/lzma e out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin.lzma
      mv out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin.lzma out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_CPSRAM_DATA.bin
   fi
   if [ "$COMPRESS_PSRAM_CODE" == "1" ];then
      arm-none-eabi-objcopy -j .psram_text -j .psram_data -O binary out/$M33_BIN_NAME/$M33_BIN_NAME.elf out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin
      cp out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin.bk
      tools/lzma e out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin.lzma
      mv out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin.lzma out/$M33_BIN_NAME/$COMPRESSED_BIN_NAME_PSRAM.bin
   fi

   # pack bin into image
   rm -rf out/${M33_BIN_NAME}/platform/main/compress/compress_data.o
   build_cmd make_best2600w TOOLCHAIN_10_2_1
else
   build_cmd make_best2600w TOOLCHAIN_10_2_1
fi

# release script
if [ "$BUILD_GEN_LIB" == "1" ];then
   # release script
   release_sdk $M33_BIN_NAME best2003_liteos
fi

# print all make cmds
print_all_cmds

# pack lib
cp out/$M33_BIN_NAME/services/aiot/a7_dsp/prj/$MCU_PROJECT_PARAM/built-in.a out/$M33_BIN_NAME/services/aiot/a7_dsp/prj/built-in.a
ar -M <tools/best2003_liteos.mri
mv libbest2003_aiot.a out/$M33_BIN_NAME/libbest2003_aiot.a
cp -rf out/$M33_BIN_NAME/libbest2003_aiot.a out/$M33_BIN_NAME/libbspbest2003_aiot.a

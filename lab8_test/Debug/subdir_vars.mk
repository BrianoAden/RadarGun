################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../lab8_msps003f3.syscfg 

C_SRCS += \
./ti_msp_dl_config.c \
/Applications/ti/mspm0_sdk_2_03_00_07/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_msps003fx_ticlang.c \
../main.c 

GEN_CMDS += \
./device_linker.cmd 

GEN_FILES += \
./device_linker.cmd \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./ti_msp_dl_config.d \
./startup_msps003fx_ticlang.d \
./main.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./ti_msp_dl_config.o \
./startup_msps003fx_ticlang.o \
./main.o 

GEN_MISC_FILES += \
./device.cmd.genlibs \
./ti_msp_dl_config.h \
./Event.dot 

OBJS__QUOTED += \
"ti_msp_dl_config.o" \
"startup_msps003fx_ticlang.o" \
"main.o" 

GEN_MISC_FILES__QUOTED += \
"device.cmd.genlibs" \
"ti_msp_dl_config.h" \
"Event.dot" 

C_DEPS__QUOTED += \
"ti_msp_dl_config.d" \
"startup_msps003fx_ticlang.d" \
"main.d" 

GEN_FILES__QUOTED += \
"device_linker.cmd" \
"device.opt" \
"ti_msp_dl_config.c" 

SYSCFG_SRCS__QUOTED += \
"../lab8_msps003f3.syscfg" 

C_SRCS__QUOTED += \
"./ti_msp_dl_config.c" \
"/Applications/ti/mspm0_sdk_2_03_00_07/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_msps003fx_ticlang.c" \
"../main.c" 



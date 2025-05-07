################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-2109964143: ../lab8_msps003f3.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/Users/adenbriano/ti/sysconfig_1.22.0/sysconfig_cli.sh" --script "/Users/adenbriano/Downloads/lab8_test/lab8_msps003f3.syscfg" -o "." -s "/Applications/ti/mspm0_sdk_2_03_00_07/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-2109964143 ../lab8_msps003f3.syscfg
device.opt: build-2109964143
device.cmd.genlibs: build-2109964143
ti_msp_dl_config.c: build-2109964143
ti_msp_dl_config.h: build-2109964143
Event.dot: build-2109964143

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/Users/adenbriano/Downloads/lab8_test" -I"/Users/adenbriano/Downloads/lab8_test/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_msps003fx_ticlang.o: /Applications/ti/mspm0_sdk_2_03_00_07/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_msps003fx_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/Users/adenbriano/Downloads/lab8_test" -I"/Users/adenbriano/Downloads/lab8_test/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/Users/adenbriano/Downloads/lab8_test" -I"/Users/adenbriano/Downloads/lab8_test/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "math.h"

#define DELAY (16000000)
int t = 0;
uint32_t del_div = 0;
float speed_threshold = 0;
volatile bool gCheckADC;
volatile uint16_t gAdcResult;
volatile int16_t gADCOffset;
volatile uint32_t ticks = 0;
volatile uint32_t flag = 0;
volatile float distance = 0;
volatile float time = 0;
volatile float last_distance;
volatile float now_s;
volatile float last_echo_s;

void check_val(){
    DL_ADC12_startConversion(ADC12_0_INST);

    while (false == gCheckADC) {
        __WFE();
    }

    gAdcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

//    /* Apply calibrated ADC offset - workaround for ADC_ERR_06 */
//    int16_t adcRaw = (int16_t) gAdcResult + gADCOffset;
//    if (adcRaw < 0) {
//        adcRaw = 0;
//    }
//    if (adcRaw > 4095) {
//        adcRaw = 4095;
//    }
//    gAdcResult = (uint16_t) adcRaw;

    del_div = gAdcResult >> 2; // (Divide by 4)
    if (del_div > 1000)
        del_div = 1000;

    gCheckADC = false;
    DL_ADC12_enableConversions(ADC12_0_INST);
    //speed_threshold = (del_div/111.8468146);
    speed_threshold = (del_div/1000);
}

//deldiv = 1000 => speed_threshold = 8.94 m/s
//deldiv = 1000 => speed_threshold = 2 m/s
//divide by 111.8468146

int main(void)
{

    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    gADCOffset =
        DL_ADC12_getADCOffsetCalibration(ADC12_0_ADCMEM_0_REF_VOLTAGE_V);


    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_INT_IRQN);
    gCheckADC = false;
    DL_ADC12_enableConversions(ADC12_0_INST);

    DL_Timer_setCaptureCompareValue(PWM_0_INST,8192, 1); //Red off
    DL_Timer_setCaptureCompareValue(PWM_0_INST,8192, 2); //Blue off

    while (1) {
        check_val();

        // adjust PWM of LEDs by turning POT

        DL_Timer_setLoadValue(PWM_0_INST, 8192);
        DL_Timer_setCaptureCompareValue(PWM_0_INST, 4096, 0);

        //DL_Timer_setLoadValue(PWM_0_INST, 32768/(del_div/50));
        //DL_Timer_setCaptureCompareValue(PWM_0_INST, (32768/(del_div/50))/2, 2);
        //DL_Timer_setCaptureCompareValue(PWM_0_INST, (32768/(del_div/50))/2, 3);

        //DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_25);  // LED ON

//        if(distance > 0.5 && distance < 4){
//            DL_Timer_setCaptureCompareValue(PWM_0_INST,8192, 1);
//        }else{
//            DL_Timer_setCaptureCompareValue(PWM_0_INST,16384, 1);
//        }

        if (del_div>500) {
            DL_SYSCTL_enableBeeperOutput();
        }
        else {
            DL_SYSCTL_disableBeeperOutput();
        }
    }

}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}

void GPIOA_IRQHandler(void)
{
    uint32_t status = DL_GPIO_getEnabledInterruptStatus(GPIOA, DL_GPIO_PIN_22); //check for interrupt
    DL_GPIO_clearInterruptStatus(GPIOA, DL_GPIO_PIN_22); //clear interrupt

    if (status) {
        if (DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_22)) { //rising edge
            DL_Timer_setCounterValueAfterEnable(TIMER_0_INST, 0);
            DL_Timer_startCounter(TIMER_0_INST);
        } else { //falling edge
            ticks = DL_Timer_getTimerCount(TIMER_0_INST);
            now_s = ticks*(0.00001067f);
            distance = (now_s*345)/2;
            if (last_echo_s != 0){
                float dx = distance - last_distance;
                float dt_s = (DL_Timer_getTimerCount(TIMER_1_INST)*(1e-6));
                float speed_ms = dx / dt_s;
                speed_ms = fabsf(speed_ms);
                DL_Timer_setCaptureCompareValue(PWM_0_INST,4096, 2);
                if (speed_ms > speed_threshold){
                    DL_Timer_setCaptureCompareValue(PWM_0_INST,4096, 1);
                    DL_Timer_setCaptureCompareValue(PWM_0_INST,4096, 2);
                    DL_SYSCTL_enableBeeperOutput();
                } else {
//                    DL_Timer_setCaptureCompareValue(PWM_0_INST,8192, 1);
//                    DL_Timer_setCaptureCompareValue(PWM_0_INST,8192, 2);
//                    DL_SYSCTL_disableBeeperOutput();
                }
            }
            DL_Timer_setCounterValueAfterEnable(TIMER_1_INST, 0);
            DL_Timer_stopCounter(TIMER_0_INST);
            DL_Timer_startCounter(TIMER_1_INST);
            last_echo_s = now_s;
            last_distance = distance;
        }
    }
}

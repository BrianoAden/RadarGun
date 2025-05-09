/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPS003FX
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPS003FX
#define CONFIG_MSPM0C1103

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     24000000



/* Defines for BEEPER */
#define GPIO_BEEPER_OUT_PORT                                               GPIOA
#define GPIO_BEEPER_OUT_PIN                                       DL_GPIO_PIN_26
#define GPIO_BEEPER_IOMUX_OUT                                    (IOMUX_PINCM27)
#define GPIO_BEEPER_IOMUX_OUT_FUNC                IOMUX_PINCM27_PF_SYSCTL_BEEPER


/* Defines for PWM_0 */
#define PWM_0_INST                                                        TIMG14
#define PWM_0_INST_IRQHandler                                  TIMG14_IRQHandler
#define PWM_0_INST_INT_IRQN                                    (TIMG14_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                                32768
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_23
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM24)
#define GPIO_PWM_0_C0_IOMUX_FUNC                    IOMUX_PINCM24_PF_TIMG14_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                         DL_GPIO_PIN_25
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM26)
#define GPIO_PWM_0_C1_IOMUX_FUNC                    IOMUX_PINCM26_PF_TIMG14_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX
/* GPIO defines for channel 2 */
#define GPIO_PWM_0_C2_PORT                                                 GPIOA
#define GPIO_PWM_0_C2_PIN                                         DL_GPIO_PIN_24
#define GPIO_PWM_0_C2_IOMUX                                      (IOMUX_PINCM25)
#define GPIO_PWM_0_C2_IOMUX_FUNC                    IOMUX_PINCM25_PF_TIMG14_CCP2
#define GPIO_PWM_0_C2_IDX                                    DL_TIMER_CC_2_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                         (46874U)
/* Defines for TIMER_1 */
#define TIMER_1_INST                                                     (TIMG8)
#define TIMER_1_INST_IRQHandler                                 TIMG8_IRQHandler
#define TIMER_1_INST_INT_IRQN                                   (TIMG8_INT_IRQn)
#define TIMER_1_INST_LOAD_VALUE                                             (0U)




/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE_V                                       3.3
#define GPIO_ADC12_0_C0_PORT                                               GPIOA
#define GPIO_ADC12_0_C0_PIN                                       DL_GPIO_PIN_27



/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for ECHO: GPIOA.22 with pinCMx 23 on package pin 17 */
// pins affected by this interrupt request:["ECHO"]
#define GPIO_GRP_0_INT_IRQN                                     (GPIOA_INT_IRQn)
#define GPIO_GRP_0_INT_IIDX                     (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_GRP_0_ECHO_IIDX                                (DL_GPIO_IIDX_DIO22)
#define GPIO_GRP_0_ECHO_PIN                                     (DL_GPIO_PIN_22)
#define GPIO_GRP_0_ECHO_IOMUX                                    (IOMUX_PINCM23)
#define GPIOA_EVENT_PUBLISHER_1_CHANNEL                                      (1)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_BEEPER_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_1_init(void);
void SYSCFG_DL_ADC12_0_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

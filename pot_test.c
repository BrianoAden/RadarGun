#include <stdint.h>
#include <ti/devices/msp/msp.h>        // CMSIS + device startup
#include <ti/driverlib/driverlib.h>   // DriverLib core
#include <ti/driverlib/dl_adc12.h>    // DL‐ADC12 API
#include "ti_msp_dl_config.h"         // SysConfig init

#define LED_PIN     (1u << 25)          // PA25 for LED test
#define POT_PIN     (1u << 27)          // PA27 = ADC A0
#define POT_MEM     DL_ADC12_MEM_IDX_0  // ADC12 MEM0 ↔ A0

// 1) Drop‐in: force PA27 into analog mode and turn OFF its digital buffer
static void initPotAnalog(void) {
    // 1) Mux PA27 into true analog mode (PC_ANALOG = 0)
    IOMUX->SECCFG.PINCM[IOMUX_PINCM27] = 0;
                              // clears the PC bits so the pin is analog-only :contentReference[oaicite:0]{index=0}

    // 2) Disable the digital input buffer on PA27
    //    (clear the bit in the DINEN register; there's no separate "clear" register)
    //GPIOA->DINEN31_0 &= ~(1u << 27);
                              // DINEN31_0 bit cleared → digital input disabled :contentReference[oaicite:1]{index=1}
}


// 2) Setup PA25 as a regular push‐pull GPIO output for the LED
static void initLedPin(void) {
    // Power up/reset GPIOA
    GPIOA->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W
                         | GPIO_RSTCTL_RESETSTKYCLR_CLR
                         | GPIO_RSTCTL_RESETASSERT_ASSERT);
    GPIOA->GPRCM.PWREN  = (GPIO_PWREN_KEY_UNLOCK_W
                         | GPIO_PWREN_ENABLE_ENABLE);
    delay_cycles(16);

    // Mux PA25 for GPIO output
    IOMUX->SECCFG.PINCM[IOMUX_PINCM25] = (IOMUX_PINCM_PC_CONNECTED | 0x1);

    // Configure PA25 as output, start low
    GPIOA->DOUTCLR31_0 = LED_PIN;
    GPIOA->DOESET31_0  = LED_PIN;
    GPIOA->DOE31_0    |= LED_PIN;
    delay_cycles(16);
}

int main(void) {
    // — 1) Core + SysConfig peripheral init —
    SYSCFG_DL_init();
    SYSCFG_DL_GPIO_init();
    SYSCFG_DL_ADC12_0_init();

    // — 2) Override PA27 to true analog input —
    initPotAnalog();

    // — 3) ADC12 single‐shot on MEM0 (PA27/A0) —
    DL_ADC12_reset(   ADC12_0_INST);
    DL_ADC12_enablePower(ADC12_0_INST);
    DL_ADC12_initSingleSample(
        ADC12_0_INST,
        DL_ADC12_REPEAT_MODE_DISABLED,
        DL_ADC12_SAMPLING_SOURCE_MANUAL,
        DL_ADC12_TRIG_SRC_SOFTWARE,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED
    );
    DL_ADC12_configConversionMem(
        ADC12_0_INST,
        POT_MEM,
        DL_ADC12_INPUT_CHAN_0,           // channel A0
        DL_ADC12_REFERENCE_VOLTAGE_VDDA, // Vref = VDDA (3.3 V)
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
        DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED,
        DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED
    );
    DL_ADC12_enableConversions(ADC12_0_INST);

    // — 4) Set up LED pin for our visual test —
    initLedPin();

    // — 5) Main loop: light LED when pot > mid-scale —
    while (1) {
        // trigger a conversion
        DL_ADC12_startConversion(ADC12_0_INST);
        while (DL_ADC12_isConversionStarted(ADC12_0_INST));

        // read the raw 12-bit value (0…4095)
        uint16_t pot = DL_ADC12_getMemResult(ADC12_0_INST, POT_MEM);

        // threshold at 2048
        if (pot > 2048) {
            GPIOA->DOUT31_0 = LED_PIN;   // LED on
        } else {
            GPIOA->DOUT31_0 = 0;         // LED off
        }
    }
}

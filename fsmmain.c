#include "ti_msp_dl_config.h"
#include <stdbool.h>
#include <stdint.h>

/* ultrasonic pins on PA26 (trigger) and PA27 (echo) */
#define TRIG_PIN   (1U << 26)
#define ECHO_PIN   (1U << 27)

/* LEDs on PA25 (red) and PA24 (blue) */
#define RED_LED_PIN   (1U << 25)
#define BLUE_LED_PIN  (1U << 24)

/* delay constant for your loops */
#define DELAY_MS     5

volatile uint32_t msTicks = 0;
volatile bool    gCheckADC;
volatile uint16_t gAdcResult;
volatile int16_t  gADCOffset;
float del_div;

/*--------------------------------------------------
  SysTick for millisecond counter
--------------------------------------------------*/
void initSysTick(void) {
    SysTick->LOAD = (24000000/1000) - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
                  | SysTick_CTRL_TICKINT_Msk
                  | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
    msTicks++;
}

static inline void delay_ms(uint32_t ms) {
    uint32_t start = msTicks;
    while ((msTicks - start) < ms) {
        __WFI();
    }
}

/*--------------------------------------------------
  Bare‑metal GPIO init (trigger, echo, LEDs)
--------------------------------------------------*/
void InitializeGPIO(void) {
    /* Reset & enable GPIOA (your existing code) */
    GPIOA->GPRCM.RSTCTL  = GPIO_RSTCTL_KEY_UNLOCK_W
                         | GPIO_RSTCTL_RESETSTKYCLR_CLR
                         | GPIO_RSTCTL_RESETASSERT_ASSERT;
    GPIOA->GPRCM.PWREN   = GPIO_PWREN_KEY_UNLOCK_W
                         | GPIO_PWREN_ENABLE_ENABLE;
    delay_cycles(16);

    /* Route PA26 → DIO26, PA27 → DIO27 */
    IOMUX->SECCFG.PINCM[IOMUX_PINCM27] = IOMUX_PINCM27_PF_GPIOA_DIO26;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM28] = IOMUX_PINCM28_PF_GPIOA_DIO27;

    /* LEDs */
    IOMUX->SECCFG.PINCM[IOMUX_PINCM26] = IOMUX_PINCM26_PF_GPIOA_DIO25;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM25] = IOMUX_PINCM25_PF_GPIOA_DIO24;

    /* Enable outputs for trigger, echo pin unused as input */
    GPIOA->DOESET31_0   = (TRIG_PIN | RED_LED_PIN | BLUE_LED_PIN);
    /* LEDs off, trigger low */
    GPIOA->DOUTCLR31_0  = (TRIG_PIN | RED_LED_PIN | BLUE_LED_PIN);

    delay_cycles(16);
}

/*--------------------------------------------------
  LED control
--------------------------------------------------*/
static inline void Red_LED_On(void)   { GPIOA->DOUTSET31_0 = RED_LED_PIN; }
static inline void Red_LED_Off(void)  { GPIOA->DOUTCLR31_0 = RED_LED_PIN; }
static inline void Blue_LED_On(void)  { GPIOA->DOUTSET31_0 = BLUE_LED_PIN;}
static inline void Blue_LED_Off(void) { GPIOA->DOUTCLR31_0 = BLUE_LED_PIN;}

/*--------------------------------------------------
  Distance measurement (bare‑metal trigger/echo)
--------------------------------------------------*/
float measureDist(void) {
    /* Trigger pulse */
    GPIOA->DOUTCLR31_0 = TRIG_PIN;
    delay_ms(1);
    GPIOA->DOUTSET31_0 = TRIG_PIN;
    delay_ms(10);
    GPIOA->DOUTCLR31_0 = TRIG_PIN;

    /* Simple busy‑wait echo read (replace with timer capture if needed) */
    uint32_t start = msTicks;
    while (!(GPIOA->DIN31_0 & ECHO_PIN));
    uint32_t t0 = msTicks - start;
    while (GPIOA->DIN31_0 & ECHO_PIN);
    uint32_t t1 = msTicks - start;

    float duration = (float)(t1 - t0);
    /* distance in cm: (duration_ms * 34000 cm/s)/2 */
    return (duration * 34.0f) / 2.0f;
}

/*--------------------------------------------------
  ADC check (unchanged)
--------------------------------------------------*/
void check_val(void) {
    DL_ADC12_startConversion(ADC12_0_INST);
    while (!gCheckADC) __WFE();
    gAdcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
    float del_div = gAdcResult >> 2;
    if (del_div > 1000) del_div = 1000;
    gCheckADC = false;
    DL_ADC12_enableConversions(ADC12_0_INST);
}

void ADC12_0_INST_IRQHandler(void) {
    if (DL_ADC12_getPendingInterrupt(ADC12_0_INST)
        == DL_ADC12_IIDX_MEM0_RESULT_LOADED) {
        gCheckADC = true;
    }
}

/*--------------------------------------------------
  Main FSM
--------------------------------------------------*/
enum current_state_enum {
    SirenOff = 0,
    Calc     = 1,
    SirenOn  = 2,
};

int main(void) {
    SYSCFG_DL_init();
    initSysTick();
    InitializeGPIO();

    gADCOffset = DL_ADC12_getADCOffsetCalibration(ADC12_0_ADCMEM_0_REF_VOLTAGE_V);
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    gCheckADC = false;
    DL_ADC12_enableConversions(ADC12_0_INST);

    enum current_state_enum next_state = SirenOff;
    int current_led = 0;
    float distance1 = 0, distance2 = 0, speed = 0;
    float time_ms = DELAY_MS;
    uint32_t threshold = 0;

    while (1) {
        switch (next_state) {
            case SirenOff:
                distance1 = measureDist();
                Red_LED_Off();
                Blue_LED_Off();
                //enableBuzzer(0);
                if (distance1 > 0) next_state = Calc;
                break;

            case Calc:
                distance2 = measureDist();
                speed = (distance2 - distance1) / (time_ms / 1000.0f);
                check_val();
                threshold = del_div / 10;
                next_state = (speed > threshold) ? SirenOn : SirenOff;
                distance1 = 0;
                break;

            case SirenOn:
                distance1 = measureDist();
                if (current_led) {
                    Red_LED_Off();
                    Blue_LED_On();
                } else {
                    Red_LED_On();
                    Blue_LED_Off();
                }
                current_led = !current_led;
                //setTone(BUZZER_TONE_LOAD);
                //enableBuzzer(1);
                if (distance1 > 0) next_state = Calc;
                break;
        }

        delay_ms(time_ms);
    }
}

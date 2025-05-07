#include "ti_msp_dl_config.h"
#include <stdbool.h>
#include <stdint.h>
#include <math.h>


/* delay constant for your loops */
#define DELAY_MS     50000

volatile uint32_t msTicks = 0;
volatile bool    gCheckADC;
volatile uint16_t gAdcResult;
volatile int16_t  gADCOffset;
float del_div;
int timerTicked = 0;
float total_ms_passed = 0;
int I2C_TX_PACKET_SIZE = 5;
uint8_t gTxPacket[5] = {'M', 'S', 'P', 'M', '0'};

volatile bool gCheckEdgeCaptureTime;

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
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

    int start = 0;
    while(start < ms){
        if(timerTicked == 1){
            start++;
        }
    }
}

/*--------------------------------------------------
  Bare‑metal GPIO init (trigger, echo, LEDs)
--------------------------------------------------*/

/*--------------------------------------------------
  LED control
--------------------------------------------------*/
static inline void Red_LED_On(void)   { DL_GPIO_writePins(GPIOA,GPIO_GRP_0_RED_LED_PIN); }
static inline void Red_LED_Off(void)  { DL_GPIO_clearPins(GPIOA,GPIO_GRP_0_RED_LED_PIN); }
static inline void Blue_LED_On(void)  { DL_GPIO_writePins(GPIOA,GPIO_GRP_3_BLUE_LED_PIN);}
static inline void Blue_LED_Off(void) { DL_GPIO_clearPins(GPIOA,GPIO_GRP_3_BLUE_LED_PIN);}

/*--------------------------------------------------
  Distance measurement (bare‑metal trigger/echo)
--------------------------------------------------*/
float measureDist(void) {
    /* Trigger pulse */
    DL_GPIO_clearPins(GPIOA,GPIO_GRP_1_TRIG_PIN_PIN);
    delay_ms(2);
    DL_GPIO_writePins(GPIOA,GPIO_GRP_1_TRIG_PIN_PIN);
    delay_ms(10);
    DL_GPIO_clearPins(GPIOA,GPIO_GRP_1_TRIG_PIN_PIN);

    /* Simple busy wait echo read (replace with timer capture if needed) */
    __attribute__((unused)) volatile static uint32_t start;
    gCheckEdgeCaptureTime = false;
    while (false == gCheckEdgeCaptureTime) {
                        __WFE();
     }
    start = 100 - (DL_Timer_getCaptureCompareValue(CAPTURE_0_INST, DL_TIMER_CC_0_INDEX));
    __attribute__((unused)) volatile static uint32_t t0;
    __attribute__((unused)) volatile static uint32_t t1;
    while ((GPIOA->DIN31_0 & GPIO_GRP_2_ECHO_PIN_PIN))
    {
        gCheckEdgeCaptureTime = false;
            while (false == gCheckEdgeCaptureTime) {
                                __WFE();
             }
            t0 = 100 - (DL_Timer_getCaptureCompareValue(CAPTURE_0_INST, DL_TIMER_CC_0_INDEX));
    }
    while (!(GPIOA->DIN31_0 & GPIO_GRP_2_ECHO_PIN_PIN)){

        gCheckEdgeCaptureTime = false;
            while (false == gCheckEdgeCaptureTime) {
                                __WFE();
             }
            t1 = 100 - (DL_Timer_getCaptureCompareValue(CAPTURE_0_INST, DL_TIMER_CC_0_INDEX));
    }

    float duration = fabs((float)(t1 - t0));
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

    DL_SYSCTL_disableBeeperOutput();
    gADCOffset = DL_ADC12_getADCOffsetCalibration(ADC12_0_ADCMEM_0_REF_VOLTAGE_V);
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    gCheckADC = false;
    DL_ADC12_enableConversions(ADC12_0_INST);
    DL_TimerG_startCounter(TIMER_0_INST);

    NVIC_EnableIRQ(CAPTURE_0_INST_INT_IRQN);
    DL_TimerG_startCounter(CAPTURE_0_INST);

    enum current_state_enum next_state = SirenOff;
    int current_led = 0;
    float distance1 = 0, distance2 = 0, speed = 0;
    float time_ms = 1000000;
    uint32_t threshold = 0;
    while (1) {
        switch (next_state) {
            case SirenOff:
                distance1 = measureDist();
                //enableBuzzer(0);
                if (distance1 > 0) {
                    next_state = SirenOn;
                }
                else{
                    next_state = SirenOff;
                }

                break;

            case Calc:
                distance2 = measureDist();
                Red_LED_On();
                speed = (distance2 - distance1) / (time_ms / 1000.0f);
                check_val();
                threshold = del_div / 10;
                next_state = (speed > threshold) ? SirenOn : SirenOff;
                distance1 = 0;
                break;

            case SirenOn:
                distance1 = measureDist();
                if (current_led == 1) {
                    Blue_LED_On();
                } else {
                    Red_LED_On();
                }
                current_led = !current_led;
                //setTone(BUZZER_TONE_LOAD);
                //enableBuzzer(1);
                //if (distance1 > 0) next_state = SirenOn;
                break;
        }

        delay_ms(time_ms);
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    // This wakes up the processor!

    switch (TIMG14->CPU_INT.IIDX)
    {
    case GPTIMER_CPU_INT_IIDX_STAT_Z: // Counted down to zero event.
        timerTicked = 1; // set a flag so we can know what woke us up.
        total_ms_passed = total_ms_passed + 1;
        break;
    default:
        break;
    }
}

void CAPTURE_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_0_INST)) {
        case DL_TIMERG_IIDX_CC0_DN:
            gCheckEdgeCaptureTime = true;
            break;
        default:
            break;
    }
}


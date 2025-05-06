#include <ti/devices/msp/msp.h>
#include <ti_msp_dl_config.h>

#define PIN  (0x1u << 26)

void InitializeGPIO(void) {
    GPIOA->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETSTKYCLR_CLR | GPIO_RSTCTL_RESETASSERT_ASSERT);
    GPIOA->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE);

    delay_cycles(16); // delay to enable GPIO to turn on and reset

    IOMUX->SECCFG.PINCM[(IOMUX_PINCM27)] = (IOMUX_PINCM_PC_CONNECTED | ((uint32_t) 0x00000001));

    GPIOA->DOUTCLR31_0 = PIN;
    GPIOA->DOESET31_0 = PIN;

    GPIOA->DOE31_0 |= PIN;

    delay_cycles(16); // delay to enable GPIO to turn on and reset
}

int main(void){
    InitializeGPIO();
    while (1){
        GPIOA->DOUT31_0 = PIN;
        delay_cycles(55000);
        GPIOA->DOUT31_0 = 0x0;
        delay_cycles(55000);
    }
}

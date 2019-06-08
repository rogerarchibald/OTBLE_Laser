/*

Custom header file to work with OTBLE_GPIO.c for controlling I/O on OpenTrap

*/


#ifndef OTBLE_GPIO
#define OTBLE_GPIO
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "stdint.h"


#define intPin NRF_GPIO_PIN_MAP(1,14) //this is the interrupt pin from teh rangefinder saying that a conversion is complete.
#define vlShutDwn NRF_GPIO_PIN_MAP(1,13)  //likely will not use this shutdown pin...will software shut-down.
#define powerON NRF_GPIO_PIN_MAP(0,28)
#define TP1 NRF_GPIO_PIN_MAP(0,31)
#define SOL_SNS NRF_GPIO_PIN_MAP(0,30) //pin to be configured as input...will be hi if no solenoid is present, otherwise will be low.
#define HP_LED NRF_GPIO_PIN_MAP(1,12)
#define BST_DRV NRF_GPIO_PIN_MAP(0,26)


//function prototypes:
void initIO(void);
void setRGB(uint32_t newColor);
void togTP1(void);
void setTP1(void);
void clearTP1(void);
void powerLED(uint8_t lightVal);
void fireSolenoid(void);
void PWM_INIT(void);

enum Colors{
red = (1 << 22),
green = (1 << 23),
blue = (1 << 24),
purple = (red | blue),
orange = (red | green),
white = (red | green | blue),
};







#endif

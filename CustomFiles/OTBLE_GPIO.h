/*

Custom header file to work with OTBLE_GPIO.c for controlling I/O on OpenTrap

*/


#ifndef OTBLE_GPIO
#define OTBLE_GPIO
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "stdint.h"


#define intPin NRF_GPIO_PIN_MAP(1,14)
#define vlShutDwn NRF_GPIO_PIN_MAP(1,13)


//function prototypes:
void initRGB(void);
void setRGB(uint32_t newColor);


enum Colors{
red = (1 << 22),
green = (1 << 23),
blue = (1 << 24),
purple = (red | blue),
orange = (red | green),
white = (red | green | blue),
};







#endif

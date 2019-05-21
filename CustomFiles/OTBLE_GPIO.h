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
#define TP2 NRF_GPIO_PIN_MAP(0,28)
#define TP1 NRF_GPIO_PIN_MAP(0,31)


//function prototypes:
void initIO(void);
void setRGB(uint32_t newColor);
void togTP1(void);
void togTP2(void);
void setTP1(void);
void setTP2(void);
void clearTP1(void);
void clearTP2(void);


enum Colors{
red = (1 << 22),
green = (1 << 23),
blue = (1 << 24),
purple = (red | blue),
orange = (red | green),
white = (red | green | blue),
};







#endif

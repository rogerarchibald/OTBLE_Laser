/*
Will run the GPIO from here, including running LED's and test point/interrupt from laser rangefinder.

*/






#include "OTBLE_GPIO.h"






//initialize the RGB LED as outputs
void initRGB(void){
    nrf_gpio_range_cfg_output(22,24); //config the RGB LED as outputs
setRGB(green);
   nrf_gpio_pin_set(vlShutDwn);
    nrf_gpio_cfg_output(vlShutDwn);
      
nrf_gpio_cfg(intPin,NRF_GPIO_PIN_DIR_INPUT , NRF_GPIO_PIN_INPUT_CONNECT , NRF_GPIO_PIN_PULLUP , NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE );

   }


//will enum colors in OTBLE_GPIO.h to write to the port
  void setRGB(uint32_t newColor){

  nrf_gpio_port_out_set(NRF_P0,(1 << 22)|(1<< 23)|(1<<24)); //first turn off all LED's
  nrf_gpio_port_out_clear(NRF_P0,newColor); //turn on the LED's as defined in newColor

  }
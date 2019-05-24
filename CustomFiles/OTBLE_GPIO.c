/*
Will run the GPIO from here, including running LED's and test point/interrupt from laser rangefinder.

*/






#include "OTBLE_GPIO.h"
#include "OpenTrap.h"
#include "nrf_drv_gpiote.h"





//initialize the RGB LED as outputs
void initIO(void){
    ret_code_t err_code;
    nrf_gpio_range_cfg_output(22,24); //config the RGB LED as outputs
    nrf_gpio_cfg_output(TP1);
    nrf_gpio_cfg_output(TP2);
   nrf_gpio_pin_set(vlShutDwn);
    nrf_gpio_cfg_output(vlShutDwn);
      //starting setting up IOC for interrupt pin (intPin defined in OTBLE_GPIO.h)
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);


    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(intPin, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(intPin, true);

   }


//will enum colors in OTBLE_GPIO.h to write to the port
  void setRGB(uint32_t newColor){

  nrf_gpio_port_out_set(NRF_P0,(1 << 22)|(1<< 23)|(1<<24)); //first turn off all LED's
  nrf_gpio_port_out_clear(NRF_P0,newColor); //turn on the LED's as defined in newColor

  }



//Test Point Function Implementations:
void togTP1(void){
nrf_gpio_pin_toggle(TP1);
}

void togTP2(void){
nrf_gpio_pin_toggle(TP2);
}

void setTP1(void){
nrf_gpio_pin_set(TP1);
}

void setTP2(void){
nrf_gpio_pin_set(TP2);
}

void clearTP1(void){
nrf_gpio_pin_clear(TP1);
}

void clearTP2(void){
nrf_gpio_pin_clear(TP2);
}
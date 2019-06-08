/*
Will run the GPIO from here, including running LED's and test point/interrupt from laser rangefinder.
Will also run the PWM for driving the solenoid/boost from here just since this is a sensible place for it.

*/






#include "OTBLE_GPIO.h"
#include "OpenTrap.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_pwm.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


/*following I/O are defined in OTBLE_GPIO.h:
intPin NRF_GPIO_PIN_MAP(1,14) //this is the interrupt pin from teh rangefinder saying that a conversion is complete.
vlShutDwn NRF_GPIO_PIN_MAP(1,13)  //likely will not use this shutdown pin...will software shut-down.
powerON NRF_GPIO_PIN_MAP(0,28)
TP1 NRF_GPIO_PIN_MAP(0,31)
SOL_SNS NRF_GPIO_PIN_MAP(0,30) //pin to be configured as input...will be hi if no solenoid is present, otherwise will be low.
HP_LED NRF_GPIO_PIN_MAP(1,12)
BST_DRV NRF_GPIO_PIN_MAP(0,26)
*/


//variable declarations:
static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);  //instance of 






//initialize the RGB LED as outputs
void initIO(void){
    ret_code_t err_code;
    nrf_gpio_range_cfg_output(22,24); //config the RGB LED as outputs
    nrf_gpio_cfg_output(TP1);
    nrf_gpio_cfg_output(powerON); 
    nrf_gpio_pin_set(powerON);  //this will keep the chip powered up until a reset or until it's pulled low after advertising timeout
   nrf_gpio_pin_set(vlShutDwn);
    nrf_gpio_cfg_output(vlShutDwn);
//set up HP LED:
      nrf_gpio_pin_clear(HP_LED);
      nrf_gpio_cfg_output(HP_LED);
//set up pin that will read whether or not there's a solenoid online:
      nrf_gpio_cfg_input(SOL_SNS,NRF_GPIO_PIN_PULLUP);


      //starting setting up IOC for interrupt pin (intPin defined in OTBLE_GPIO.h)
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(intPin, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(intPin, true);

//check sol_sns after setting up other stuff...Insta-check is coming back low, suspect it takes a few uS for the pull-up to actually pull up
      if(!(nrf_gpio_pin_read(SOL_SNS))){
      set_solenoid_detected();
      }

   } //end of initIO






//below array/sequence are for the solenoid drive
static uint16_t /*const*/ seq0_values[] =
{
    80,
    80,
    80,
    80,
    80,
    80,
    80
};

nrf_pwm_sequence_t const seq0 =
{
    .values.p_common = seq0_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq0_values),
    .repeats         = 4000,
    .end_delay       = 1
};
//end of PWM config stuffs















   //Initialize PWM for the Boost drive for solenoid
void PWM_INIT(void){
    uint32_t err_code;
    nrf_drv_pwm_config_t const config0 =
    {
       .output_pins =
        {
            BST_DRV,                              // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .base_clock   = NRF_PWM_CLK_16MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 160,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    
    err_code = nrf_drv_pwm_init(&m_pwm0, &config0, NULL);
    if (err_code != NRF_SUCCESS)
    {
        // Initialization failed. Take recovery action.
        NRF_LOG_INFO("PWM0_INIT_FAILED!");
    }

    }
       
    

    //function to actuate the solenoid.  called from openTrap.c
void fireSolenoid(void){
//if there's no solenoid detected, make the light purple to indicate we're off
if(nrf_gpio_pin_read(SOL_SNS)){
setRGB(purple);
}else{
        setRGB(blue);
        (void)nrf_drv_pwm_simple_playback(&m_pwm0, &seq0, 1, NRF_DRV_PWM_FLAG_STOP);
         NRF_LOG_INFO("PWM_RUNNING!");
}
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


void setTP1(void){
nrf_gpio_pin_set(TP1);
}



void clearTP1(void){
nrf_gpio_pin_clear(TP1);
}



//function to control power LED on board:
void powerLED(uint8_t lightVal){
if(lightVal){
nrf_gpio_pin_set(HP_LED);
}else{
nrf_gpio_pin_clear(HP_LED);
}
}
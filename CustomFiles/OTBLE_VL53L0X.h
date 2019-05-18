/*

Custom header file to work with OTBLE_GPIO.c for controlling I/O on OpenTrap

*/


#ifndef OTBLE_VL53L0x
#define OTBLE_VL53L0x
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "stdint.h"

//starting to bring in vl53 library stuffs
//Need to include this API once I get the rest of hte BLE stuff happy
//#include "vl53l0x_api.h"
//#include "vl53l0x_platform.h"
//#include "vl53l0x_i2c_platform.h"









void initVL53(void);  //iniitialization function
//TODO: un-comment this once the vl53 stuff is incoroporated//void print_pal_error(VL53L0X_Error Status);
bool isMeasurementReady(void);
void startMeasuring(void);
void stopMeasuring(void);
void getRangeClearFlag(void);







#endif

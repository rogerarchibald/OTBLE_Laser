/*
Here will call my custom functions that interact with the VL53L0X API's.


like vl53l0x_i2c_platform.c, this is largely based on LukeL99’s GitHub  page: https://github.com/LukeL99/nrf51-vl53l0x-driver

*/






#include "OTBLE_VL53L0X.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

 uint32_t err_code;
VL53L0X_Dev_t MyDevice;
VL53L0X_Dev_t *pMyDevice = &MyDevice;


    VL53L0X_Version_t                   Version;
    VL53L0X_Version_t                  *pVersion   = &Version;
    uint8_t deviceRevisionMajor;
    uint8_t deviceRevisionMinor;

    uint8_t vhvCalibrationValue;
    uint8_t phaseCalibrationValue;
   

    VL53L0X_RangingMeasurementData_t rangingData;


//function to dump an error to the debugger.
void print_pal_error(VL53L0X_Error Status){
	if(Status != 0){
		char buf[VL53L0X_MAX_STRING_LENGTH];
		VL53L0X_GetPalErrorString(Status, buf);
		NRF_LOG_DEBUG("API Status: %i : %s\n", (uint32_t)Status, *buf);
	}
}
//for use when polling as opposed to interrupts, will check if there's data ready
bool isMeasurementReady(void){
	uint8_t deviceReady;
	VL53L0X_GetMeasurementDataReady(pMyDevice, &deviceReady);
	return (bool)deviceReady;
}

//function that can be called from my app to start measurments being taken.
void startMeasuring(void){
uint8_t holderVar = 0;
                NRF_LOG_INFO("mode starts as: %d\r\n", holderVar);
    	VL53L0X_StartMeasurement(pMyDevice);
        VL53L0X_GetDeviceMode(pMyDevice, &holderVar);
         NRF_LOG_INFO("mode ends as: %d\r\n", holderVar);

        }
//function that can be called from my app to stop measurments being taken.
void stopMeasuring(void){
        VL53L0X_StopMeasurement(pMyDevice);
        }




void getRangeClearFlag(void){
    	VL53L0X_GetRangingMeasurementData(pMyDevice, &rangingData);
    	VL53L0X_ClearInterruptMask(pMyDevice, 0);
}




void initVL53(void){
                      pMyDevice->comms_speed_khz = 400;
                      pMyDevice->I2cDevAddr = 0x52;


    err_code = VL53L0X_comms_initialise(pMyDevice->comms_speed_khz);

		if(VL53L0X_ERROR_NONE == err_code){

			err_code = VL53L0X_DataInit(pMyDevice);
			if(VL53L0X_ERROR_NONE != err_code) NRF_LOG_ERROR("DataInit: %d\r\n", err_code);

			uint16_t osc_calibrate_val=0;
			err_code = VL53L0X_RdWord(&MyDevice, VL53L0X_REG_OSC_CALIBRATE_VAL,&osc_calibrate_val);
			NRF_LOG_DEBUG("%i\n",osc_calibrate_val);

			err_code = VL53L0X_StaticInit(pMyDevice);
			if(VL53L0X_ERROR_NONE != err_code) NRF_LOG_ERROR("StaticInit: %d\r\n", err_code);

			uint32_t refSpadCount;
			uint8_t isApertureSpads;

			VL53L0X_PerformRefSpadManagement(pMyDevice, &refSpadCount, &isApertureSpads);
			if(VL53L0X_ERROR_NONE != err_code) {
				NRF_LOG_ERROR("SpadCal: %d\r\n", err_code)
			} else {
				NRF_LOG_DEBUG("refSpadCount: %d\r\nisApertureSpads: %d\r\n", refSpadCount,isApertureSpads)
			}

			err_code = VL53L0X_PerformRefCalibration(pMyDevice, &vhvCalibrationValue, &phaseCalibrationValue);
			print_pal_error(err_code);
			NRF_LOG_DEBUG("Calibration Values:\r\n VHV:%d phaseCal:%d\r\n", vhvCalibrationValue, phaseCalibrationValue);

			err_code = VL53L0X_GetVersion(pVersion);
			print_pal_error(err_code);
			NRF_LOG_INFO("VL53L0X API Version: %d.%d.%d (revision %d)\r\n", pVersion->major, pVersion->minor ,pVersion->build, pVersion->revision);

			err_code = VL53L0X_GetProductRevision(pMyDevice, &deviceRevisionMajor, &deviceRevisionMinor);

			if(VL53L0X_ERROR_NONE == err_code){
				NRF_LOG_INFO("VL53L0X product revision: %d.%d\r\n", deviceRevisionMajor, deviceRevisionMinor);
			} else {
				NRF_LOG_ERROR("Version Retrieval Failed.  ");
				print_pal_error(err_code);
			}

     
                VL53L0X_SetInterMeasurementPeriodMilliSeconds(pMyDevice, 250);    //set 1/4 second between samples  
                VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING); //setting this up for continuous timed ranging with inter-measurment period as defined above.

                 err_code = 0;
              //following are setting up for hi-accuracy as per user manual...may look at changing the timing budget for power considerations moving forward

		if (err_code == VL53L0X_ERROR_NONE) {
			err_code = VL53L0X_SetLimitCheckValue(pMyDevice,
					VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
					(FixPoint1616_t) (0.25 * 65536));
		}

		if (err_code == VL53L0X_ERROR_NONE) {
			err_code = VL53L0X_SetLimitCheckValue(pMyDevice,
					VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
					(FixPoint1616_t) (18 * 65536));
		}
                //setting a timing budget of 60mS (may revisit this after testing in all ambient light conditions)

		if (err_code == VL53L0X_ERROR_NONE) {
			err_code = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,
					60000);
		}
 


    } else {
    	NRF_LOG_ERROR("Data Init Failed\r\n");
    }

}





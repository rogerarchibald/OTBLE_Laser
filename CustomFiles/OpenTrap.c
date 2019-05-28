/*

Starting to build the OpenTrap firmware using the BLE-Characteristics Tutorial as a base-point.  Will add my 3 characteristics and firmware to run them

*/



#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "OpenTrap.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "OTBLE_GPIO.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

uint8_t initCntrlVal = 0; //this value will start as 0.  Bit 3 will be set in the event that a solenoid is detected as part of GPIO Init.

//function to receive data from the Central to OpenTrap so I can check received bytes and act accordingly:
void rx_from_central(ble_ot_t * p_ot, const ble_evt_t * p_ble_evt){
	static uint8_t lastlightstat = 0;
	uint8_t lightswitch;
	uint8_t cmd;
	const ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
	//if the handle associated with the write is my control handle:
	
if (p_evt_write->handle == p_ot->cntrl_handles.value_handle)
{
cmd = p_evt_write->data[0];
	NRF_LOG_INFO("received: %d", cmd);
	lightswitch = (cmd & 0x80);
	if (lightswitch != lastlightstat){
	powerLED(lightswitch);
	}
	lastlightstat = lightswitch;	//MSBit of the command is the light switch
//check the LSBit of command, this will be used to activate the PWM for solenoid	
        if(cmd & 0x01){
        if (lastlightstat){ //if we're about to drive the solenoid, turn off the LED for power consumption
        lastlightstat = 0; //set this off so that it will trigger on next change
        powerLED(0);
        }
        fireSolenoid();

        }
}
}





/*
 *
 * This will be the distance characteristic that's read several times/second
 *
 */
static uint32_t dist_char_add(ble_ot_t * p_opentrap_service)
{
    // OUR_JOB: Step 2.A, Add a custom characteristic UUID
    uint32_t err_code;
		ble_uuid_t	char_uuid;
		ble_uuid128_t	base_uuid = BLE_UUID_OPENTRAP_BASE_UUID;
		char_uuid.uuid	=	BLE_UUID_DIST_CHARACTERISTC_UUID;
		err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
		APP_ERROR_CHECK(err_code);
	
    // OUR_JOB: Step 2.F Add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
		char_md.char_props.read = 1;
		char_md.char_props.write = 0;
	
    
    // OUR_JOB: Step 3.A, Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
		char_md.p_cccd_md = &cccd_md;
		char_md.char_props.notify = 1;
	
	
	
	
    // OUR_JOB: Step 2.B, Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));  
		attr_md.vloc = BLE_GATTS_VLOC_STACK;

    
    // OUR_JOB: Step 2.G, Set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
		BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    
    // OUR_JOB: Step 2.C, Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
		attr_char_value.p_uuid = &char_uuid;
		attr_char_value.p_attr_md = &attr_md;
    
    // OUR_JOB: Step 2.H, Set characteristic length in number of bytes
		attr_char_value.max_len = 2;
		attr_char_value.init_len = 2;
		uint8_t value[2] = {0x23,0x32};
		attr_char_value.p_value = value;

    // OUR_JOB: Step 2.E, Add our new characteristic to the service
		err_code = sd_ble_gatts_characteristic_add(p_opentrap_service->service_handle, &char_md, &attr_char_value, &p_opentrap_service->dist_handles);
		APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

/*
 *
 * This will be the battery characteristic that's read once/second
 *
 */


static uint32_t cntrl_char_add(ble_ot_t * p_opentrap_service)
{
    // OUR_JOB: Step 2.A, Add a custom characteristic UUID
    uint32_t err_code;
		ble_uuid_t	char_uuid;
		ble_uuid128_t	base_uuid = BLE_UUID_OPENTRAP_BASE_UUID;
		char_uuid.uuid	=	BLE_UUID_CNTRL_CHARACTERISTC_UUID;
		err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
		APP_ERROR_CHECK(err_code);
	
    // OUR_JOB: Step 2.F Add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
		char_md.char_props.read = 1;
		char_md.char_props.write = 1;
	
	
    // OUR_JOB: Step 2.B, Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));  
		attr_md.vloc = BLE_GATTS_VLOC_STACK;

    
    // OUR_JOB: Step 2.G, Set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    // OUR_JOB: Step 2.C, Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
		attr_char_value.p_uuid = &char_uuid;
		attr_char_value.p_attr_md = &attr_md;
    
    // OUR_JOB: Step 2.H, Set characteristic length in number of bytes
		attr_char_value.max_len = 1;
		attr_char_value.init_len = 1;
		uint8_t value[1] = {initCntrlVal};
		attr_char_value.p_value = value;

    // OUR_JOB: Step 2.E, Add our new characteristic to the service
		err_code = sd_ble_gatts_characteristic_add(p_opentrap_service->service_handle, &char_md, &attr_char_value, &p_opentrap_service->cntrl_handles);
		APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}


/*
 *
 * This will be the battery characteristic that's read once/second
 *
 */
static uint32_t batt_char_add(ble_ot_t * p_opentrap_service)
{
    // OUR_JOB: Step 2.A, Add a custom characteristic UUID
    uint32_t err_code;
		ble_uuid_t	char_uuid;
		ble_uuid128_t	base_uuid = BLE_UUID_OPENTRAP_BASE_UUID;
		char_uuid.uuid	=	BLE_UUID_BATT_CHARACTERISTC_UUID;
		err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
		APP_ERROR_CHECK(err_code);
	
    // OUR_JOB: Step 2.F Add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
		char_md.char_props.read = 1;
		char_md.char_props.write = 0;
	
    
    // OUR_JOB: Step 3.A, Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
		char_md.p_cccd_md = &cccd_md;
		char_md.char_props.notify = 1;
	
	
	
	
    // OUR_JOB: Step 2.B, Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));  
		attr_md.vloc = BLE_GATTS_VLOC_STACK;

    
    // OUR_JOB: Step 2.G, Set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
		BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    
    // OUR_JOB: Step 2.C, Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
		attr_char_value.p_uuid = &char_uuid;
		attr_char_value.p_attr_md = &attr_md;
    
    // OUR_JOB: Step 2.H, Set characteristic length in number of bytes
		attr_char_value.max_len = 2;
		attr_char_value.init_len = 2;
		uint8_t value[2] = {0x23,0x32};
		attr_char_value.p_value = value;

    // OUR_JOB: Step 2.E, Add our new characteristic to the service
		err_code = sd_ble_gatts_characteristic_add(p_opentrap_service->service_handle, &char_md, &attr_char_value, &p_opentrap_service->batt_handles);
		APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}








/**@brief Function for initiating our new service.
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
void opentrap_service_init(ble_ot_t * p_opentrap_service)
{
    uint32_t   err_code; // Variable to hold return codes from library and softdevice functions
    p_opentrap_service->conn_handle             = BLE_CONN_HANDLE_INVALID;

    // FROM_SERVICE_TUTORIAL: Declare 16-bit service and 128-bit base UUIDs and add them to the BLE stack
    ble_uuid_t        service_uuid;
    ble_uuid128_t     base_uuid = BLE_UUID_OPENTRAP_BASE_UUID;
    service_uuid.uuid = BLE_UUID_OPENTRAP_SERVICE_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);    
     APP_ERROR_CHECK(err_code);
    
    // OUR_JOB: Step 3.B, Set our service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
		p_opentrap_service->conn_handle = BLE_CONN_HANDLE_INVALID;

    // FROM_SERVICE_TUTORIAL: Add our service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_opentrap_service->service_handle);
    
    APP_ERROR_CHECK(err_code);
    
    // OUR_JOB: Call the function our_char_add() to add our new characteristic to the service. 
    dist_char_add(p_opentrap_service);
		batt_char_add(p_opentrap_service);
		cntrl_char_add(p_opentrap_service);
}


//will be called as part of GPIO Init if no solenoid
void set_solenoid_detected(void){
initCntrlVal |= 0x08;
}



//The characteristic update will be called whenever one of the App Timer rollovers calls it.
//It'll come with a pointer to the service, a pointer to the actual value to be updated,
//and the 'whichChar' argument that will tell us whether to update the battery value or the distance value

void opentrap_characteristic_update(ble_ot_t *p_opentrap_service, int16_t *newchar_value, uint8_t whichChar){
	if(p_opentrap_service->conn_handle != BLE_CONN_HANDLE_INVALID)
	{
	if(whichChar == 0){
	    // OUR_JOB: Step 3.E, Update characteristic value
	
		uint16_t	len = 2;
		ble_gatts_hvx_params_t hvx_params;
		memset(&hvx_params, 0, sizeof(hvx_params));
		
		hvx_params.handle = p_opentrap_service->batt_handles.value_handle;
		hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len = &len;
		hvx_params.p_data = (uint8_t*)newchar_value;
		
		sd_ble_gatts_hvx(p_opentrap_service->conn_handle, &hvx_params);
	

}else if (whichChar == 1){
	
    //when whichChar = 1 it means the distance is the characteristic I need to update.
	
                
		uint16_t	len = 2;
		ble_gatts_hvx_params_t hvx_params;
		memset(&hvx_params, 0, sizeof(hvx_params));
		
		hvx_params.handle = p_opentrap_service->dist_handles.value_handle;
		hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len = &len;
		hvx_params.p_data = (uint8_t*)newchar_value;
		
		sd_ble_gatts_hvx(p_opentrap_service->conn_handle, &hvx_params);
	
}
	}
}



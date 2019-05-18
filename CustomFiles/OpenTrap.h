
#ifndef OPENTRAP_SVC_H__
#define OPENTRAP_SVC_H__

#include <stdint.h>
//#include "Timers_OTBLE.h"
#include "ble.h"
#include "ble_srv_common.h"

// FROM_SERVICE_TUTORIAL: Defining 16-bit service and 128-bit base UUIDs
#define BLE_UUID_OPENTRAP_BASE_UUID              {{0x53, 0xC1, 0x83, 0x54, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x58, 0x92, 0x00, 0x00, 0x55, 0xAA}} // 128-bit base UUID
#define BLE_UUID_OPENTRAP_SERVICE_UUID                0xF00D // Just a random, but recognizable value

// ALREADY_DONE_FOR_YOU: Defining 16-bit characteristic UUID
#define BLE_UUID_DIST_CHARACTERISTC_UUID          0xD1D1 // Just a random, but recognizable value
#define BLE_UUID_BATT_CHARACTERISTC_UUID          0xBABA // Just a random, but recognizable value
#define BLE_UUID_CNTRL_CHARACTERISTC_UUID         0xBEEF // Just a random, but recognizable value

// This structure contains various status information for our service. 
// The name is based on the naming convention used in Nordics SDKs. 
// 'ble indicates that it is a Bluetooth Low Energy relevant structure and 
// os is short for Our Service). 
typedef struct
{
    uint16_t                    conn_handle;    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t                    service_handle; /**< Handle of Our Service (as provided by the BLE stack). */
    // OUR_JOB: Step 2.D, Add handles for the characteristic attributes to our struct
		ble_gatts_char_handles_t		dist_handles;	//This handle will be used to pass the distance information from the ultrasonic transducer to the centra
		ble_gatts_char_handles_t		batt_handles; //will periodically sample battery voltage and use this handle to convey that to the remote
		ble_gatts_char_handles_t		cntrl_handles;
}ble_ot_t;

/**@brief Function for handling BLE Stack events related to our service and characteristic.
 *
 * @details Handles all events from the BLE stack of interest to Our Service.
 *
 * @param[in]   p_our_service       Our Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.\
 * 
 * Will be called when a ble write event occurs.   
 */
void rx_from_central(ble_ot_t * p_ot, const ble_evt_t * p_ble_evt);

void set_solenoid_detected(void); //will be called as part of GPIO init if solenoid is detected.


/**@brief Function for initializing our new service.
 *
 * @param[in]   p_our_service       Pointer to Our Service structure.
 */
void opentrap_service_init(ble_ot_t * p_opentrap_service);

/**@brief Function for updating and sending new characteristic values
 *
 * @details The application calls this function whenever our timer_timeout_handler triggers
 *
 * @param[in]   p_our_service                     Our Service structure.
 * @param[in]   characteristic_value     New characteristic value.
 *
 * To simplify things and avoid having two different characteristic update handles, added 
 * the 'whichChar' argument to the function to determine which characteristic needs to be updated.
 * Keeping it simple, 0 == battery update, 1 == distance update.
 */

void opentrap_characteristic_update(ble_ot_t *p_opentrap_service, int16_t *newchar_value, uint8_t whichChar);

#endif  /* _ OPENTRAP_SVC_H__ */

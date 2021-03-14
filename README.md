# OTBLE_Laser
NRF52840 FW for controlling OpenTrap BLE.  This was built using Segger Embedded Studio with SDK version: nRF5_SDK_15.2.0_9412b96


BLE Functionality:

OpenTrap employs a custom BLE service where the board acts as a peripheral to be hosted by a Central.  The service base UUID is:

AA55F00D9258EFDE1523785F5483C153


The service employs 3 characteristics:

	-1 Distance: This is a 16-bit value containing the distance reported by the vl53l0x rangefinder.  Distance is the raw value reported in mm, it is a read/notify characteristic which will update the Central when the detected range has changed by more than +/- 8mm.  The Distance Characteristic UUID is:

AA55FD1D19258EFDE1523785F5483C153

	-2 Battery: This is a 16-bit value containing an ADC reading result from a resistor divider connected to the battery input.  Battery voltage is divided by a factor of ~14 and is compared against the ADC’s 0.6V internal reference.  The ADC result is a read/notify which will update the Central when the detected ADC result has changed by more than +/- 5 ADC counts.  The Battery Characteristic UUID is:

AA55BABA9258EFDE1523785F5483C153

	-3 Status/Control: This is an 8-bit bitfield with status/control information for the Trap.  It is a read/write characteristic.  3 of the 8 bits are currently implemented:
		-Bit3 of the Control value will be set on startup if a solenoid is detected.  On the trap hardware, the SOL_SNS pin will be pulled low by the solenoid coil if present; otherwise it will be pulled high via internal pull-up resistors.  The board’s awareness of solenoid presence is important to ensure that the board doesn’t try to drive an unregulated boost converter without a load present.   This bit will also advise the user if the solenoid isn’t present.  

		-Bits 7 and 0 control the power LED and the solenoid respectively.  When Bit 7 is set, the LED is turned on.  When bit 0 is set, the solenoid will be actuated to close the trap door.  This is done by driving the on-board discrete boost converter at 100kHz, 50% duty cycle for a brief period.  

The Status/Control Characteristic UUID is:

AA55BEEF9258EFDE1523785F5483C153

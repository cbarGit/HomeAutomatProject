//-----------------------------------------------------------------------------------------------
// Main Configuration
// - Comment to delete from the sketch the relative section
//-----------------------------------------------------------------------------------------------

/* boards */
//#define BOARD_DFROBOT_ETHERNET_SHIELD_W5200 // WARNING: If you use W5200, ensure you use the Ethernet W5200 library
#define BOARD_ETHERNET_SHIELD_W5100 // WARNING: If you use W5100, ensure you use the Ethernet W5100 library; On Arduino UNO you loose pins 10, 11, 12, and 13
//#define BOARD_SEEEDSTUDIO_RELAY_SHIELD_V10 
//#define BOARD_DFROBOT_RELAY_SHIELD_V21

/* peripherals */
//#define PERIPHERAL_RN_WIFLY

/* main functionalities */
#define FUNCTION_WEBSERVER
//#define FUNCTION_SDCARD
//#define FUNCTION_MEMORY
//#define FUNCTION_SERIAL
//#define FUNCTION_SERIAL_DEBUG

/* select the PIN for the active DIGITAL sensors */
//#define SENSOR_DIGITAL_DFROBOT_DHT22_PIN 5

/* select the PIN for the active ANALOG sensors */
//#define SENSOR_ANALOG_DFROBOT_DHT11_PIN A0
//#define SENSOR_ANALOG_DFROBOT_MQ2_PIN A0
//#define SENSOR_ANALOG_DFROBOT_TEMT_6000_PIN A0
//#define SENSOR_ANALOG_DFROBOT_AMBIENT_LIGHT_507010_PIN A1
//#define SENSOR_ANALOG_DFROBOT_18B20_TEMPERATURE_507010_PIN 6

/* RELAY ACTUATORS */
//#define ACTUATOR_RELAY_SAINSMART_5V_8RELAYS
//#define ACTUATOR_RELAY_SAINSMART_5V_4RELAYS

/* select the INTERRUPT */
//#define INTERRUPT_SO 3 // IRQ 0 = Digital 2, IRQ 1 = Digital 3
/* select the PULSE */
#define PULSE_SO 

/* select the WATCHDOG */
#define WATCHDOG

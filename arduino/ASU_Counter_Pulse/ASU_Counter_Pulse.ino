/*
SUPPORTED HARDWARE

MAIN BOARDS
- Arduino UNO http://arduino.cc/en/Main/arduinoBoardUno
- Arduino MEGA 2560 http://arduino.cc/en/Main/ArduinoBoardMega2560
- Arduino YUN http://arduino.cc/en/Products/ArduinoYUN

PERIPHERALS
- Sparkfun RN-XV Wifly https://www.sparkfun.com/products/10822

SHIELDS
- Ethernet Shield http://arduino.cc/en/Main/ArduinoEthernetShield
- Ethernet Shield v1.0 DFRobot (http://www.dfrobot.com/index.php?route=product/product&product_id=1028)
- I/O Expansion Shield v7.1 DFRobot (http://www.dfrobot.com/index.php?route=product/product&product_id=1009)

DIGITAL SENSORS
- Digital Temperature and Humidity sensor DFRobot DHT11 (to be mounted on analogic port!!!) 
- Digital Temperature and Humidity sensor DFRobot DHT22

ANALOG SENSORS
- Analog Light sensor DFRobot TEMT 6000
- Analog Light sensor DFRobot Dallas
- Analog Gas sensor DFRobot MQ2
- Analog Temperature sensor DFRobot 18B20

RELAY ACTUATORS
- SainSmart 8 Relay 5V module http://www.sainsmart.com/8-channel-dc-5v-relay-module-for-arduino-pic-arm-dsp-avr-msp430-ttl-logic.html
- SainSmart 4 Relay 5V module 
- SainSmart 2 Relay 5V module 

OTHER
- Power Meter SO+/SO- pulses

*/



//-----------------------------------------------------------------------------------------------
// Arduino main libraries
//-----------------------------------------------------------------------------------------------
#include <MemoryFree.h>
#if ARDUINO < 100
#include <WProgram.h>
#include <pins_arduino.h>
#else
#include <Arduino.h>
#endif

// Configuration file
#include "config.h"

//-----------------------------------------------------------------------------------------------
// SD Card
//-----------------------------------------------------------------------------------------------
#ifdef FUNCTION_SDCARD

#include <SD.h>

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
#define SDCARD_PIN 4

#endif

//-----------------------------------------------------------------------------------------------
// Watch Dog
//-----------------------------------------------------------------------------------------------
#ifdef WATCHDOG
#include <avr/io.h>
#include <avr/wdt.h>

void setupWatchdog()
{
  // 8 secs
  wdt_enable(WDTO_8S);
}  

void resetWatchdog()
{
  // reset watchdog
  wdt_reset();
}  

#endif 

//-----------------------------------------------------------------------------------------------
// SAINSMART RELAYS ACTUATOR
//-----------------------------------------------------------------------------------------------

#ifdef ACTUATOR_RELAY_SAINSMART_5V_8RELAYS
 #define ACTUATOR_RELAY
 #define RELAY_ON LOW
 #define RELAY_OFF HIGH
 const unsigned char relayPin[8] = {5,6,7,8,A2,A5,A4,A3}; 
#endif

#ifdef ACTUATOR_RELAY_SAINSMART_5V_4RELAYS
 #define ACTUATOR_RELAY
 #define RELAY_ON LOW
 #define RELAY_OFF HIGH
 const unsigned char relayPin[4] = {2,3,4,5}; 
#endif

#ifdef ACTUATOR_RELAY_SAINSMART_5V_2RELAYS
 #define ACTUATOR_RELAY
 #define RELAY_ON LOW
 #define RELAY_OFF HIGH
 const unsigned char relayPin[2] = {2,3}; 
#endif

//-----------------------------------------------------------------------------------------------
// RN-XV Wifly
//-----------------------------------------------------------------------------------------------

#ifdef PERIPHERAL_RN_WIFLY

#include <WiFlyHQ.h>
#include <SoftwareSerial.h>

// Used to communicate with the WiFly module
const int ss_rx = 0;
const int ss_tx = 1;
const int ss_baud_rate = 9600;
SoftwareSerial wifiSerial(ss_rx,ss_tx);

/* Change these to match your WiFi network */
const char mySSID[] = "";
const char myPassword[] = "";

// WiFly
WiFly wifly;

// Wifly Watchdog
unsigned long wifly_watchdog_timer_DEADLINE_in_ms;
const unsigned long wifly_watchdog_timer_RESET_in_ms = 5*60*1000; // max 5 minutes

#define WIFLY_DEVICEID "Wifly-WebServer"

void updateWiflyWatchdog()
{
  wifly_watchdog_timer_DEADLINE_in_ms = millis() + wifly_watchdog_timer_RESET_in_ms;
}

void WiFlySetup()
{
    // Abilitando il codice in calce, si può monitorare la scheda su porta COM tramite il cavo USB
    wifiSerial.begin(ss_baud_rate);
    
    if (!wifly.begin(&wifiSerial, &Serial)) 
    {
    	wifly.terminal();
    }
    
    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) 
    {
	/* Setup the WiFly to connect to a wifi network */
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();
	wifly.save();
	if (!wifly.join()) wifly.terminal();
    }

    // Turn off UPD broadcast
    wifly.setBroadcastInterval(0);	
    
    wifly.setDeviceID(WIFLY_DEVICEID);

    if (wifly.isConnected()) 
    {
	wifly.close();
    }

    wifly.setProtocol(WIFLY_PROTOCOL_TCP);
    if (wifly.getPort() != 80) 
    {
        wifly.setPort(80);
	/* local port does not take effect until the WiFly has rebooted (2.32) */
	wifly.save();
	wifly.reboot();
	delay(3000);
    } 
   
    // watchdog
    updateWiflyWatchdog();
} 

void checkWiflyWatchdog()
{
  if (millis() > wifly_watchdog_timer_DEADLINE_in_ms)
  {
    // watchdog deadline failed!
    WiFlySetup();
  }
  else
  {    
    if ( (wifly_watchdog_timer_DEADLINE_in_ms - millis()) > wifly_watchdog_timer_RESET_in_ms )
    {
      // millis rolled out but watchdog deadline failed!
      WiFlySetup();
    }
  }  
} 

#endif

//-----------------------------------------------------------------------------------------------
// Relay Shield
//-----------------------------------------------------------------------------------------------

#ifdef BOARD_SEEEDSTUDIO_RELAY_SHIELD_V10
 #define ACTUATOR_RELAY
  #define RELAY_ON HIGH
  #define RELAY_OFF LOW
 const unsigned char relayPin[4] = {2,3,4,5};
#endif 

#ifdef BOARD_DFROBOT_RELAY_SHIELD_V1
 #define ACTUATOR_RELAY
  #define RELAY_ON HIGH
  #define RELAY_OFF LOW
 const unsigned char relayPin[4] = {2,7,8,10};
#endif 

//-----------------------------------------------------------------------------------------------
// Ethernet Shield
// WARNING: If you use W5200, ensure you use the Ethernet W5200 library
// WARNING: If you use W5100, ensure you use the normal Arduino Ethernet library
//-----------------------------------------------------------------------------------------------

#ifdef BOARD_DFROBOT_ETHERNET_SHIELD_W5200
#define ETHERNET
#endif

#ifdef BOARD_ETHERNET_SHIELD_W5100
#define ETHERNET
#endif

#ifdef ETHERNET

#include <SPI.h>
#include <Ethernet.h>

// mac address
static byte mac_address[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x12, 0x52 };
//static byte mac_address[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// ip address
IPAddress ip_address(192,168,1,10);

String getMacString()
{
  char char_mac[18]; // includes the terminating byte
  sprintf(char_mac, "%02x:%02x:%02x:%02x:%02x:%02x", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5] );  
  String str_mac(char_mac);
  return str_mac;  
}

String getIpString()
{
  char char_ip[16]; // includes the terminating byte
  IPAddress currentIpAddress = Ethernet.localIP();  
  sprintf(char_ip, "%d.%d.%d.%d", currentIpAddress[0], currentIpAddress[1], currentIpAddress[2], currentIpAddress[3]);  
  String str_ip(char_ip);  
  return str_ip;
}  

#endif


#ifdef BOARD_DFROBOT_ETHERNET_SHIELD_W5200

//define the pins for the W5200
#define W5200_SS 10
#define W5200_nPWDN 9
#define W5200_nRST 8
#define W5200_nINT 3

void ethernetSetup()
{
  // Next setting parameter is very important for W5200
  // If you don't have reliable reset setting, W5200 maybe will not work!!!!!  //   
  pinMode(W5200_SS,OUTPUT);  //Define the interfave :Dreamer MEGA X2 PORT  Gadgeteer PIN 6 use SS
  pinMode(W5200_nRST,OUTPUT);
  pinMode(W5200_nPWDN,OUTPUT);
  pinMode(W5200_nINT,INPUT);  
  digitalWrite(W5200_nPWDN,LOW);  //enable power
  
  digitalWrite(W5200_nRST,LOW);  //Reset W5200
  delay(10);
  digitalWrite(W5200_nRST,HIGH);  
  delay(200);       // wait W5200 work
   
    // initialize the ethernet device not using DHCP:
    Ethernet.begin(mac_address, ip_address);
  //}
}

#endif

#ifdef BOARD_ETHERNET_SHIELD_W5100

void ethernetSetup()
{
    // set PIN 4 and PIN 10 to be high
    pinMode(4,OUTPUT);
    digitalWrite(4,HIGH);
    pinMode(10,OUTPUT);
    digitalWrite(10,HIGH);     
    
    // initialize the ethernet device not using DHCP:
    //Ethernet.begin(mac_address, ip_address);
    Ethernet.begin(mac_address, ip_address);
}

#endif

//-----------------------------------------------------------------------------------------------
// Web Server
//-----------------------------------------------------------------------------------------------

#ifdef FUNCTION_WEBSERVER

 #ifdef ETHERNET  
  EthernetServer webserver(80);
 #endif
 
 const int max_bufHttpRequest_length = 100;
 char bufHttpRequest[max_bufHttpRequest_length];

#endif 

//-----------------------------------------------------------------------------------------------
// DHT 11 Temperature and Humidity Sensor
// (http://www.dfrobot.com/index.php?route=product/product&product_id=174)
//-----------------------------------------------------------------------------------------------

#ifdef SENSOR_ANALOG_DFROBOT_DHT11_PIN

#define DHT11_PIN SENSOR_ANALOG_DFROBOT_DHT11_PIN

byte temp_integer = 0;    
byte temp_decimal = 0;
byte humid_integer = 0;
byte humid_decimal = 0;

byte get_dht11_data()
{
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++)
  {
    while(!(PINC & _BV(DHT11_PIN)))
    {};  // wait  forever until analog input port is '1'   (NOTICE: PINC reads all the analog input ports 
    //and  _BV(X) is the macro operation which pull up positon 'X'to '1' and the rest positions to '0'. it is equivalent to 1<<X.) 
    delayMicroseconds(30);
    if(PINC & _BV(DHT11_PIN))  //if analog input port 0 is still '1' after 30 us
      result |=(1<<(7-i));     //this position is 1
    while((PINC & _BV(DHT11_PIN)));  // wait '1' finish
    }
    return result;
}

// synch with the DHT11 sensor
// update the temp and humidity values
// returns true if checksum is fine, returns false otherwise
boolean update_dht11_data()
{
  // temperature/humidity sensor setup
  DDRC |= _BV(DHT11_PIN);   //let analog port DHT11 be an output port 
  PORTC |= _BV(DHT11_PIN);  //let the initial value of this port be '1'
  
  byte dht11_dat[5];    
  byte dht11_in;
  byte i;// start condition
	 
  PORTC &= ~_BV(DHT11_PIN);    // 1. pull-down the DHT11 pin for 18ms
  delay(18);
  
  PORTC |= _BV(DHT11_PIN);     // 2. pull-up the DHT11 pin for 40micros
  DDRC &= ~_BV(DHT11_PIN);     //let the DHT11 analog port be input port 
  delayMicroseconds(40);      
  
  dht11_in = PINC & _BV(DHT11_PIN);  // read only the DHT11 input port
  if(dht11_in)
  {
    delay(1000);
    return false;
  }
  
  delayMicroseconds(80);
  dht11_in = PINC & _BV(DHT11_PIN); //  
  if(!dht11_in)
  {    
    return false;
  }
  
  delayMicroseconds(80);// now ready for data reception
  for (i=0; i<5; i++)
  {  dht11_dat[i] = get_dht11_data(); }  //recieved 40 bits data. Details are described in datasheet
  
  DDRC |= _BV(DHT11_PIN);      //let analog port 0 be output port after all the data have been received
  PORTC |= _BV(DHT11_PIN);     //let the  value of this port be '1' after all the data have been received
  byte dht11_check_sum = dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3];// check check_sum
  if(dht11_dat[4]!= dht11_check_sum)
  {
    return false;
  }
  temp_integer = dht11_dat[2];    
  temp_decimal = dht11_dat[3];
  humid_integer = dht11_dat[0];
  humid_decimal = dht11_dat[1];
  return true;
}

float get_humidity()
{
  update_dht11_data();
  return (float) (humid_integer + (humid_decimal / 100)); 
}

#endif

//-----------------------------------------------------------------------------------------------
// DHT 22 Temperature and Humidity Sensor
// (http://www.dfrobot.com/index.php?route=product/product&product_id=1102)
//-----------------------------------------------------------------------------------------------

#ifdef SENSOR_DIGITAL_DFROBOT_DHT22_PIN

#include <dht.h>

dht DHT;

float get_temperature()
{  
  DHT.read(SENSOR_DIGITAL_DFROBOT_DHT22_PIN);    // READ DATA
  return DHT.temperature;
}

float get_humidity()
{
  DHT.read(SENSOR_DIGITAL_DFROBOT_DHT22_PIN);    // READ DATA
  return DHT.humidity;
}

#endif

//-----------------------------------------------------------------------------------------------
// DFRobot Analog Ambient Light Sensor TEMT 6000
// (http://www.dfrobot.com/index.php?route=product/product&product_id=274)
//-----------------------------------------------------------------------------------------------

#ifdef SENSOR_ANALOG_DFROBOT_TEMT_6000_PIN

int get_light()
{
  return analogRead(SENSOR_ANALOG_DFROBOT_TEMT_6000_PIN);
}

#endif

//-----------------------------------------------------------------------------------------------
// DFRobot Analog Ambient Light Sensor
// (http://www.dfrobot.com/index.php?route=product/product&product_id=1004)
//-----------------------------------------------------------------------------------------------

#ifdef SENSOR_ANALOG_DFROBOT_AMBIENT_LIGHT_507010_PIN

int get_light()
{
  return (1023 - analogRead(SENSOR_ANALOG_DFROBOT_AMBIENT_LIGHT_507010_PIN));          
}

#endif

//-----------------------------------------------------------------------------------------------
// DFRobot MQ2 Analog Gas Sensor
//
// Sensitive for Methane, Butane, LPG, smoke.
// This sensor is sensitive for flamable and combustible gasses.
// The heater uses 5V.
// The MQ-2 at seeed: http://www.seeedstudio.com/wiki/Grove_-_Gas_Sensor%28MQ2%29
// Search for datasheet: http://duckduckgo.com/?q=%22mq-2%22+gas+sensor+filetype%3Apdf
//-----------------------------------------------------------------------------------------------

#ifdef SENSOR_ANALOG_DFROBOT_MQ2_PIN

float get_gas()
{
  float vol;
  int sensorValue = analogRead(SENSOR_ANALOG_DFROBOT_MQ2_PIN);
  vol=(float)sensorValue/1024*5.0;
  return vol;
}

#endif

//-----------------------------------------------------------------------------------------------
// DFRobot Digital Temperature Sensor
// (http://www.dfrobot.com/wiki/index.php/18B20_Temerature_Sensor_%28Arduino_Compatible%29_%28SKU:_DFR0024%29)
//-----------------------------------------------------------------------------------------------

#ifdef SENSOR_ANALOG_DFROBOT_18B20_TEMPERATURE_507010_PIN

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
#include <OneWire.h>
OneWire oneWire(SENSOR_ANALOG_DFROBOT_18B20_TEMPERATURE_507010_PIN);

// Pass our oneWire reference to Dallas Temperature. 
#include <DallasTemperature.h>
DallasTemperature temp_sensor(&oneWire);

float get_temperature()
{
      // Send the command to get temperatures
    temp_sensor.requestTemperatures();
    
    return temp_sensor.getTempCByIndex(0);    
}

#endif


#ifdef PULSE_SO
volatile unsigned long counter = 0;   

void onPulse(){
  (int)counter++;
}
// Setup
void pulseMeterSetup()
{  
   // agganciamo l'interrupt al pin PULSE_SO, chiamiamo la funzione pulse_so sul fronte di discesa.
   //attachInterrupt(digitalPinToInterrupt(PULSE_SO), onPulse, FALLING); // interrupt pulse counter so, FALLING: HIGH --> LOW
   attachInterrupt(digitalPinToInterrupt(3), onPulse, FALLING); // interrupt pulse counter so, FALLING: HIGH --> LOW

}
#endif

//-----------------------------------------------------------------------------------------------
// Power Meter
//-----------------------------------------------------------------------------------------------

#ifdef INTERRUPT_SO

//Pulse Counter
volatile boolean flagTimer;
volatile unsigned long diff;   
volatile unsigned long start;
volatile unsigned long stop;
const unsigned long num_pulses_per_kwh = 1000;
const double ws_per_pulse = (double)3600.0*(1000.0/num_pulses_per_kwh);
int pValue = 1; //pulse for every wh (could change with a different counter)
double power;
double kwh;

void power()
{

  if (flagTimer == true){
    start = micros();
    flagTimer = false;
  }else{
    stop = micros();
    flagTimer = true; // update variable timer_status
    if(stop > start) // control micro() reset
      diff = stop - start; // calculating the time interval pulse  
  }


  //Debugging

  Serial.println((power),10);
  Serial.println((diff),10);

}
//////////////////////////////////////////////////////////////////////////////

void checkPower(){
  if(( micros() - start ) > 80000000 && flagTimer == true){
    diff = 0.0;
  }
  if (diff == 0.0) 
    power = 0.0;
  else
    power = (ws_per_pulse * 1000000)/(double)diff;
}

// Setup
void powerMeterSetup()
{  
   // agganciamo l'interrupt al pin INTERUPT_SO, chiamiamo la funzione pulse_so sul fronte di discesa.
   //attachInterrupt(digitalPinToInterrupt(INTERUPT_SO), pulse_so, FALLING); // interrupt pulse counter so, FALLING: HIGH --> LOW
   attachInterrupt(digitalPinToInterrupt(INTERRUPT_SO), power(), FALLING); // interrupt pulse counter so, FALLING: HIGH --> LOW
   flagTimer = true;
}

#endif

//-----------------------------------------------------------------------------------------------
// Free Memory
//-----------------------------------------------------------------------------------------------

#ifdef FUNCTION_MEMORY

#include <MemoryFree.h>

int get_memory()
{
  return freeMemory();
}

#endif

//-----------------------------------------------------------------------------------------------
// Serial communication
//-----------------------------------------------------------------------------------------------

#ifdef FUNCTION_SERIAL

 const int serial_baud_rate = 9600;
 
 #ifdef FUNCTION_SERIAL_DEBUG

   // milliseconds between the Arduino reset and the last debug execution
   unsigned long last_serial_debug_ms = 0; 
   const int serial_debug_period_ms = 1000; // in milliseconds

 #endif

#endif

//-----------------------------------------------------------------------------------------------
// RELAY FUNCTIONS
//-----------------------------------------------------------------------------------------------

#ifdef ACTUATOR_RELAY

void relaySetup()
{
  for(int i = 0; i < sizeof(relayPin); i++)
  {
    pinMode(relayPin[i],OUTPUT);
    digitalWrite(relayPin[i],RELAY_OFF);    
  }
} 

void switchRelayON(int relayPin)
{
  digitalWrite(relayPin,RELAY_ON); 
}

void switchRelayOFF(int relayPin)
{
  digitalWrite(relayPin,RELAY_OFF);  
}

void switchRelay(int relayPin)
{
  int status = digitalRead(relayPin);
  if (status == RELAY_ON)
   digitalWrite(relayPin,RELAY_OFF);
  else
   digitalWrite(relayPin,RELAY_ON); 
}

void toggleRelay(int relayPin, int toggleDelay)
{
  digitalWrite(relayPin,RELAY_ON); 
  delay(toggleDelay);
  digitalWrite(relayPin,RELAY_OFF); 
}

#endif

//-----------------------------------------------------------------------------------------------
// SETUP
//-----------------------------------------------------------------------------------------------

#ifdef FUNCTION_SERIAL 
void serialSetup()
{
    Serial.begin(serial_baud_rate); 
    while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
    } 
}
#endif

#ifdef ETHERNET
 #ifdef FUNCTION_WEBSERVER
void webServerSetup()
{
  //avvia il web server
  webserver.begin();
}
 #endif
#endif

void setup()
{
  #ifdef FUNCTION_SERIAL
   serialSetup();
  #endif
  
  #ifdef ACTUATOR_RELAY
   relaySetup();
  #endif 
  
  #ifdef INTERRUPT_SO
   powerMeterSetup();
  #endif

  #ifdef PULSE_SO
   pulseMeterSetup();
  #endif
  
  #ifdef PERIPHERAL_RN_WIFLY
   WiFlySetup(); 
  #endif
  
  #ifdef ETHERNET  
   ethernetSetup();    
   
   #ifdef FUNCTION_WEBSERVER
    webServerSetup();
   #endif 
  #endif 
  
  #ifdef WATCHDOG
   setupWatchdog();
  #endif
}

//-----------------------------------------------------------------------------------------------
// JSON FUNCTIONS
//-----------------------------------------------------------------------------------------------

String getJson()
{
    String json = "{ ";
    
    boolean add_comma = false;
   
    char strbuf[32]; 
 
    #ifdef ACTUATOR_RELAY
      for(int i = 0; i < sizeof(relayPin); i++)
      {
        if (add_comma) json += ", "; else add_comma = true;
        json += " \"RELAY";
        json += (i+1);
        json += "\": \"";
        json += (digitalRead(relayPin[i]) == RELAY_ON);
        json += "\"";
      }   
    #endif

    #ifdef SENSOR_ANALOG_DFROBOT_18B20_TEMPERATURE_507010_PIN
      // temperature 
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"temperature\": \"";
      json += dtostrf(get_temperature(),2,2,strbuf);
      json += "\"";
    #endif
        
    #ifdef SENSOR_ANALOG_DFROBOT_DHT11_PIN
      // DHT11 humidity
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"humidity\": \"";
      json += dtostrf(get_humidity(),2,2,strbuf);    
      json += "\"";
    #endif
        
    #ifdef SENSOR_DIGITAL_DFROBOT_DHT22_PIN
      // DHT22 humidity
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"humidity\": \"";
      json += dtostrf(get_humidity(),2,2,strbuf);
      json += "\"";
    #endif    
    
    #ifdef SENSOR_DIGITAL_DFROBOT_DHT22_PIN
      // DHT22 temperature
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"temperature\": \"";
      json += dtostrf(get_temperature(),2,2,strbuf);
      json += "\"";
    #endif
    
    #ifdef SENSOR_ANALOG_DFROBOT_AMBIENT_LIGHT_507010_PIN
      // light
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"light\": \"";
      json += get_light();
      json += "\"";
    #endif
    
    #ifdef SENSOR_ANALOG_DFROBOT_TEMT_6000_PIN
      // light
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"light\": \"";
      json += get_light();
      json += "\"";
    #endif

    #ifdef SENSOR_ANALOG_DFROBOT_MQ2_PIN  
      // gas
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"gas\": \"";
      json += dtostrf(get_gas(),2,2,strbuf);
      json += "\"";
    #endif
    
    #ifdef POWER_SO
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"power\": \"";
      json += dtostrf(power,2,2,strbuf);
      json += "\"";  
    #endif

    #ifdef PULSE_SO
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"pulses\": \"";
      //json += dtostrf(counter,2,0,strbuf);
      json += String(counter);
      json += "\"";  
    #endif
    
    #ifdef FUNCTION_MEMORY
      // memory  
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"memory\": \"";
      json += get_memory();
      json += "\"";
    #endif
    
    #ifdef ETHERNET
      // mac
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"mac\": \"";      
      json += getMacString();
      json += "\"";  
      
      // ip
      if (add_comma) json += ", "; else add_comma = true;
      json += " \"ip\": \"";      
      json += getIpString();
      json += "\"";      
    #endif
    
    json += " }";
    
    return json;    
}

//-----------------------------------------------------------------------------------------------
// SERIAL COMMUNICATION FUNCTIONS
//-----------------------------------------------------------------------------------------------

void printSensorStatus()
{        
   Serial.println(getJson());
}

void printStatus()
{ 
  printSensorStatus();
}

//-----------------------------------------------------------------------------------------------
// LOOP FUNCTIONS
//-----------------------------------------------------------------------------------------------

#ifdef FUNCTION_SERIAL

#ifdef FUNCTION_SERIAL_DEBUG

void serialDebug()
{  
  unsigned long now_ms = millis();
  
  if (now_ms < last_serial_debug_ms)
  {
    // overflow
    last_serial_debug_ms = 4294967295 - last_serial_debug_ms; 
  }
  
  if ((now_ms - last_serial_debug_ms) >= serial_debug_period_ms)
  {    
    last_serial_debug_ms = now_ms;
    printStatus();
  }  
}

#endif

void executeSerialCommand()
{
  if(Serial.available()>0) 
  {
   char c = Serial.read();
   if( c == 'j' ) 
   { 
      printStatus();
   }
   #ifdef ACTUATOR_RELAY
   else if( c >= '1' && c <= ('1'+sizeof(relayPin)-1) )
   {
     switchRelay(relayPin[c-'1']);           
   }   
   #endif
   else 
   {  
      Serial.println(F("Help"));
      Serial.println(F("'j' show json"));
      #ifdef ACTUATOR_RELAY
       Serial.print(F("'1' to '"));
       Serial.print(sizeof(relayPin));
       Serial.println(F("' to switch relays"));
      #endif
   }
  }
}
#endif


#ifdef FUNCTION_WEBSERVER

void handleHttpRequest()
{
  #ifdef ACTUATOR_RELAY  
  String get;
  char get_buf[21];
  for(int i = 0; i < sizeof(relayPin); i++)
  {
    get = "GET /?RELAY";
    get += (i+1);
    get += "=ON";
    get.toCharArray(get_buf, get.length()+1);
    if (strncmp(bufHttpRequest, get_buf, get.length()) == 0) 
    {		
        switchRelayON(relayPin[i]);
    } 
    else
    {
      get = "GET /?RELAY";
      get += (i+1);
      get += "=OFF";
      get.toCharArray(get_buf, get.length()+1);
      if (strncmp(bufHttpRequest, get_buf, get.length()) == 0) 
      {		
        switchRelayOFF(relayPin[i]);
      }
      else 
      {	
        get = "GET /?RELAY";
        get += (i+1);
        get += "=TOGGLE";
        get.toCharArray(get_buf, get.length()+1);
      	if (strncmp(bufHttpRequest, get_buf, get.length()) == 0) 
        {
          toggleRelay(relayPin[i],800);
        }          
      } 
    }
  } 
  if (strncmp_P(bufHttpRequest, PSTR("GET /?ALL=ON"), 12) == 0) 
  { 
      for(int i = 0; i < sizeof(relayPin); i++)
      {
        switchRelayON(relayPin[i]); 
      }
  } 
  else if (strncmp_P(bufHttpRequest, PSTR("GET /?ALL=OFF"), 13) == 0) 
  { 
      for(int i = 0; i < sizeof(relayPin); i++)
      {
        switchRelayOFF(relayPin[i]);        
      }
  } 
  #endif
}

#endif

#ifdef ETHERNET
 #ifdef FUNCTION_WEBSERVER

void handleClientRequestEthernet()
{
  // listen for incoming clients
  EthernetClient client = webserver.available();
  
  if (client) 
  {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    // read char
    int bufIndex = 0;
    
    while (client.connected()) 
    {
      if (client.available() > 0) 
      {        
        char c = client.read();
        if (bufIndex < max_bufHttpRequest_length)
         bufHttpRequest[bufIndex] = c;
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) 
        {
          // handle the Http request
          handleHttpRequest();
          
          // send a standard http response header
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: application/json"));
          client.println();

          // output the value of each analog input pin
          client.println(getJson());
          
          break;
        }
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        bufIndex++;        
      }
      else
      {
        bufIndex = 0;
      }  
    }
    
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
  }
}
 #endif
#endif

#ifdef PERIPHERAL_WIFLY
 #ifdef FUNCTION_WEBSERVER
 
void handleClientRequestWifly()
{
  if (wifly.available() > 0) 
    {   
        /* See if there is a request */
	if (wifly.gets(bufHttpRequest, sizeof(bufHttpRequest))) 
        {            
            // handle the http request
            handleHttpRequest();
            
            // discard rest of input
            wifly.flushRx();
            
            // write the new status
            /* Send the header directly with print */
            wifly.println(F("HTTP/1.1 200 OK"));
            wifly.println(F("Content-Type: application/json"));
            wifly.println();
            
            wifly.print(getJson());
            wifly.println();
            delay(10);
            wifly.close();
            
            // reset watchdog
            updateWiflyWatchdog();
        }
    }
}

 #endif
#endif

void loop()
{
  #ifdef WATCHDOG
   wdt_reset();
  #endif

  #ifdef ETHERNET
   #ifdef FUNCTION_WEBSERVER
    handleClientRequestEthernet();
   #endif
  #endif
  
  #ifdef PERIPHERAL_WIFLY
   #ifdef FUNCTION_WEBSERVER    
    handleClientRequestWifly();
    checkWiflyWatchdog();    
   #endif
  #endif
  
  #ifdef INTERRUPT_SO
   checkPower();
  #endif 
  
  #ifdef FUNCTION_SERIAL
   executeSerialCommand();
   #ifdef FUNCTION_SERIAL_DEBUG
    serialDebug();
   #endif
  #endif 

}

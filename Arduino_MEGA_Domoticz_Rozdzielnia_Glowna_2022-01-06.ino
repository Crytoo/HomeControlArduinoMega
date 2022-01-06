#define versions "1.1.1"
#define date "2021-03-22"
#define ipAddress "192.168.1.221"

#include <Wire.h>               // Required for I2C communication
#include <PCF8574.h>            // Required for PCF8574
#include <SPI.h>                // For communication with the ethernet module
#include <Ethernet.h>           // Module Ethernet
#include <OneWire.h>            // For communication with a Dallas thermometer and other
#include <DallasTemperature.h>  // Dallas thermometer library 
#include <Adafruit_BME280.h>    // Temperature, pressure and humidity sensor

//__________________________________________________________SETTINGS FOR DS18B20
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DeviceAddress DS18B20Sensor0 = { 0x28, 0xAA, 0xBA, 0xDE, 0x52, 0x14, 0x1, 0xD6 };//DiningRoom
DeviceAddress DS18B20Sensor1 = { 0x28, 0xAA, 0x21, 0x63, 0x53, 0x14, 0x1, 0xB6 };//Bedroom
DeviceAddress DS18B20Sensor2 = { 0x28, 0xAA, 0xF9, 0xE8, 0x52, 0x14, 0x1, 0x30 };//Pantry
DeviceAddress DS18B20Sensor3 = { 0x28, 0xAA, 0x35, 0xF4, 0x52, 0x14, 0x1, 0x19 };//Kitchen
DeviceAddress DS18B20Sensor4 = { 0x28, 0xFF, 0x68, 0x5A, 0xB4, 0x16, 0x5, 0xFF };//Vestibule
DeviceAddress DS18B20Sensor5 = { 0x28, 0xFF, 0x59, 0xB7, 0xC0, 0x16, 0x4, 0x4D };//Suffit
DeviceAddress DS18B20Sensor6 = { 0x28, 0xAA, 0x35, 0xF4, 0x52, 0x14, 0x1, 0x19 };//OutdoorEnter
DeviceAddress DS18B20Sensor7 = { 0x28, 0xFF, 0xEF, 0xC7, 0xC0, 0x16, 0x4, 0xAA };//Free

unsigned long termometryTime = 60000;
unsigned long previousMillis = 0;
//__________________________________________________________SETTINGS FOR BME280
Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1013.25)  //Constant declaration for the BME280 sensor
//__________________________________________________________SWITCH MAPPING AND OTHER ARDUINO MEGA INPUTS
#define In03 3//DoorTechnicalRoom
#define In04 4//DoorVestibule
#define In05 5//WindowBedroom
#define In06 6//WindowKitchen
#define In07 7//DoorTerrace
#define Out08 8//BoilerSocket
#define Out09 9//SuffitSocket
//#define In10 10//Ethernet SD Card Ethernet Shield
//#define In11 11//Resereved
//#define In12 12//Resereved
//#define In13 13//Resereved
#define In14 14//BlindSwitchBedroom
#define In15 15//BlindSwitchKitchen
#define In16 16//BlindSwitchDiningRoomLeft
#define In17 17//BlindSwitchDiningRoomRight
#define In18 18//KitchenLED
#define In19 19//KitchenPlafon
#define In22 22//DiningRoomGirandole
#define In23 23//DiningRoomLED
#define In24 24//DiningRoomHalogensWindowStairs
#define In25 25//DiningRoomhalogenyKitchen
#define In26 26//Stairs
#define In27 27//Vestibule
#define In28 28//Pantry
#define In29 29//SoffitBack
#define In30 30//SoffitFront
#define In31 31//SoffitEntertrance
#define In32 32//OutdoorFront
#define In33 33//Terrace
#define Out34 34//KitchenLED
#define Out35 35//KitchenPlafon
#define Out36 36//DiningRoomGirandole
#define Out37 37//DiningRoomLED
#define Out38 38//DiningRoomHalogensWindowsStairs
#define Out39 39//DiningRoomHalogensKitchenWallpaper
#define Out40 40//Stairs
#define Out41 41//Vestibule
#define Out42 42//BlindUpBedroom
#define Out43 43//BlindDownBedroom
#define Out44 44//BlindUpKitchen
#define Out45 45//BlindDownKitchen
#define Out46 46//BlindUpDiningRoomLeft
#define Out47 47//BlindDownDiningRoomLeft
#define Out48 48//BlindUpDiningRoomRight
#define Out49 49//BlindDownDiningRoomRight
//__________________________________________________________MAPPING EXPANDERS
//__________________________________________________________EXPANDER 1
#define OutExp01_PIN0 0//Pantry
#define OutExp01_PIN1 1//SoffitBack
#define OutExp01_PIN2 2//SoffitFront
#define OutExp01_PIN3 3//SoffitEntertrace
#define OutExp01_PIN4 4//OutdoorFront
#define OutExp01_PIN5 5//Terrace
#define OutExp01_PIN6 6//Driveway
#define OutExp01_PIN7 7//Gateway
//__________________________________________________________EXPANDER 2
#define OutExp02_PIN0 0//
#define OutExp02_PIN1 1//
#define OutExp02_PIN2 2//
#define OutExp02_PIN3 3//
#define OutExp02_PIN4 4//
#define OutExp02_PIN5 5//
#define OutExp02_PIN6 6//
#define OutExp02_PIN7 7//

PCF8574 expander01;
PCF8574 expander02;
//PCF8574 expander03;
//PCF8574 expander04;
//PCF8574 expander05;
//PCF8574 expander06;
//PCF8574 expander07;
//PCF8574 expander08;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
IPAddress ip(192,168,1,221); // ARDUINO IP W SIECI
IPAddress gateway(192,168,1,20); // internet access via router
IPAddress subnet(255,255,255,0); //subnet mask
IPAddress myserver(192,168,1,220); // DOMOTICZ IP
EthernetServer server(84); //PORT ARDUINO
EthernetClient client;
//__________________________________________________________SETTING VARIABLE
String readString; 

int Blind4243 = 0;//Bedroom
int Blind4445 = 0;//Kitchen
int Blind4647 = 0;//DiningRoomLeft
int Blind4849 = 0;//DiningRoomRight

//Show(1) or not(0) in serial port message form function GET, LOG or both
int SerialPrintGet = 0; 
int SerialPrintLog = 0;
unsigned long currentSwitches[24];
unsigned long previousSwitches[24];
unsigned long timeSwitches=1000;
int DustSensor[0];//DustSensor Initialization - [number of connected sensors]
String LocationTemp[8] = {"DiningRoom","Bedroom","Pantry","Kitchen","Vestibule","Suffit","DwórWejście",""};
int ReedSensor[5];//ReedSensor Initialization - [number of connected sensors]
String LocationReed[5] = {"DoorTechnicalRoom","DoorVestibule","WindowBedroom","WindowKitchen","DoorTerrace"};
//##########################################################FUNCTION SETUP################################################
void setup(){
  Serial.begin(9600); 
//__________________________________________________________SETTINGS ETHERNET SHIELD W5100 
  Ethernet.begin(mac, ip, subnet, gateway); 
  server.begin();
//__________________________________________________________PCF8574 INITALIZING
  expander01.begin(0x27);
  expander02.begin(0x26);
//  expander03.begin(0x25);
//  expander04.begin(0x24);
//  expander05.begin(0x23);
//  expander06.begin(0x25);
//  expander07.begin(0x24);
//  expander08.begin(0x20);
//__________________________________________________________EXPANDER 1
  expander01.pinMode(OutExp01_PIN0, OUTPUT);//Pantry
  expander01.pinMode(OutExp01_PIN1, OUTPUT);//SoffitBack
  expander01.pinMode(OutExp01_PIN2, OUTPUT);//SoffitFront
  expander01.pinMode(OutExp01_PIN3, OUTPUT);//SoffitEntertrace
  expander01.pinMode(OutExp01_PIN4, OUTPUT);//OutdoorFront
  expander01.pinMode(OutExp01_PIN5, OUTPUT);//Terrace
  expander01.pinMode(OutExp01_PIN6, OUTPUT);//Driveway
  expander01.pinMode(OutExp01_PIN7, OUTPUT);//Gateway
//__________________________________________________________EXPANDER 2
  expander02.pinMode(OutExp01_PIN0, OUTPUT);//LightBedroomGirandole
  expander02.pinMode(OutExp01_PIN1, OUTPUT);//LightBedroomHalogens
  expander02.pinMode(OutExp01_PIN2, OUTPUT);//LightBedroomLED
  expander02.pinMode(OutExp01_PIN3, OUTPUT);//LightBathroom
  expander02.pinMode(OutExp01_PIN4, OUTPUT);//LightCoriddal
  expander02.pinMode(OutExp01_PIN5, OUTPUT);//LightTechnicalRoom
  expander02.pinMode(OutExp01_PIN6, OUTPUT);//LightUnderStairs
  expander02.pinMode(OutExp01_PIN7, OUTPUT);//LightOutdoorBack
//__________________________________________________________PINS ARDUINO MEGA ROZDZIELNIA GLOWNA
  pinMode(In03, INPUT_PULLUP);//ReedTechnicalRoomDoor
  pinMode(In04, INPUT_PULLUP);//ReedVestibuleDoor
  pinMode(In05, INPUT_PULLUP);//ReedBedroomWindow
  pinMode(In06, INPUT_PULLUP);//ReedKitchenWindow
  pinMode(In07, INPUT_PULLUP);//ReedTaraceDoor
  pinMode(Out08, OUTPUT);//SocketBoiler
  pinMode(Out09, OUTPUT);//SocketSuffit
  pinMode(In14, INPUT_PULLUP);//SwitchBlindBedroom
  pinMode(In15, INPUT_PULLUP);//SwitchBlindKitchen
  pinMode(In16, INPUT_PULLUP);//SwitchBlindDiningRoomLeft
  pinMode(In17, INPUT_PULLUP);//SwitchBlindDiningRoomRight
  pinMode(In18, INPUT_PULLUP);//SwitchLightKitchenLED
  pinMode(In19, INPUT_PULLUP);//SwitchLightKitchenPlafon
  pinMode(In22, INPUT_PULLUP);//SwitchLightDiningRoomGirandole
  pinMode(In23, INPUT_PULLUP);//SwitchLightDiningRoomLED
  pinMode(In24, INPUT_PULLUP);//SwitchLightDiningRoomHalogensWindowStairs
  pinMode(In25, INPUT_PULLUP);//SwitchLightDiningRoomHalogensKitchenWallpaper
  pinMode(In26, INPUT_PULLUP);//SwitchLightStairs
  pinMode(In27, INPUT_PULLUP);//SwitchLightVestibule
  pinMode(In28, INPUT_PULLUP);//SwitchPantry
  pinMode(In29, INPUT_PULLUP);//SwitchSoffitBack
  pinMode(In30, INPUT_PULLUP);//SwitchSoffitFront
  pinMode(In31, INPUT_PULLUP);//SwitchSoffitEntertrance
  pinMode(In32, INPUT_PULLUP);//SwitchOutDoorFront
  pinMode(In33, INPUT_PULLUP);//SwitchTarace
  pinMode(Out34, OUTPUT);//LightKitchenLED
  pinMode(Out35, OUTPUT);//LightKitchenPlafon
  pinMode(Out36, OUTPUT);//LightDiningRoomGirandole
  pinMode(Out37, OUTPUT);//LightDiningRoomLED
  pinMode(Out38, OUTPUT);//LightDiningRoomHalogensWindowStairs
  pinMode(Out39, OUTPUT);//LightDiningRoomHalogensKitchenWallpaper
  pinMode(Out40, OUTPUT);//LightVestibule
  pinMode(Out41, OUTPUT);//LightStaris
  pinMode(Out42, OUTPUT);//BlindUpBedroom
  pinMode(Out43, OUTPUT);//BlindDownBedroom
  pinMode(Out44, OUTPUT);//BlindUpKitchen
  pinMode(Out45, OUTPUT);//BlindDownkitchen
  pinMode(Out46, OUTPUT);//BlindUpDiningRoomLeft
  pinMode(Out47, OUTPUT);//BlindDownDiningRoomLeft
  pinMode(Out48, OUTPUT);//BlindUpDiningRoomRight
  pinMode(Out49, OUTPUT);//BlindDownDiningRoomRight
//__________________________________________________________Ustawienie stanów niskich na PINach
  digitalWrite(Out08, HIGH);//LightKitchenLED
  digitalWrite(Out09, HIGH);//LightKitchenPlafon
  digitalWrite(Out34, HIGH);//LightKitchenLED
  digitalWrite(Out35, HIGH);//LightKitchenPlafon
  digitalWrite(Out36, HIGH);//LightDiningRoomGirandole
  digitalWrite(Out37, HIGH);//LightDiningRoomLED
  digitalWrite(Out38, HIGH);//LightDiningRoomHalogensWindowStairs
  digitalWrite(Out39, HIGH);//LightDiningRoomHalogensKitchenWallpaper
  digitalWrite(Out40, HIGH);//LightStaris
  digitalWrite(Out41, HIGH);//LightVestibule
  digitalWrite(Out42, HIGH);//BlindUpBedroom
  digitalWrite(Out43, HIGH);//BlindDownBedroom
  digitalWrite(Out44, HIGH);//BlindUpKitchen
  digitalWrite(Out45, HIGH);//BlindDownkitchen
  digitalWrite(Out46, HIGH);//BlindUpDiningRoomLeft
  digitalWrite(Out47, HIGH);//BlindDownDiningRoomLeft
  digitalWrite(Out48, HIGH);//BlindUpDiningRoomRight
  digitalWrite(Out49, HIGH);//BlindDownDiningRoomRight

  expander01.digitalWrite(OutExp01_PIN0,HIGH);//LightPantry
  expander01.digitalWrite(OutExp01_PIN1,HIGH);//LightSoffitBack
  expander01.digitalWrite(OutExp01_PIN2,HIGH);//LightSoffitFront
  expander01.digitalWrite(OutExp01_PIN3,HIGH);//LightSoffitEntertrace
  expander01.digitalWrite(OutExp01_PIN4,HIGH);//LightOutdoorFront
  expander01.digitalWrite(OutExp01_PIN5,HIGH);//LightTerrace
  expander01.digitalWrite(OutExp01_PIN6,HIGH);//Driveway
  expander01.digitalWrite(OutExp01_PIN7,HIGH);//Gateway

  expander02.digitalWrite(OutExp02_PIN0,HIGH);//LightBedroomGirandole
  expander02.digitalWrite(OutExp02_PIN1,HIGH);//LightBedroomHalogens
  expander02.digitalWrite(OutExp02_PIN2,HIGH);//LightBedroomLED
  expander02.digitalWrite(OutExp02_PIN3,HIGH);//LightBathroomGround
  expander02.digitalWrite(OutExp02_PIN4,HIGH);//LightCorridalGround
  expander02.digitalWrite(OutExp02_PIN5,HIGH);//LightTechnicalRoom
  expander02.digitalWrite(OutExp02_PIN6,HIGH);//LightUnderStairs
  expander02.digitalWrite(OutExp02_PIN7,HIGH);//LightOutsiteRear
//__________________________________________________________LOG DOMOTICZ and SERIAL PORT
  Serial.println("I wait 30 seconds for an ethernet connection.");
  for(int i=30;i>-1;i--){delay(1000);Serial.print(i);Serial.print(" ");Serial.write(13);}
  Serial.println("");
  String ver = versions;
  Serial.print("Server/Client version: ");
  Serial.print(ver);
  Serial.print(" Arduino Mega Rozdzielnia IP: ");
  Serial.println(ipAddress);
  String Log="Arduino+Mega+Rozdzienia+IP:+";
  Log+=ipAddress;
  Log+="+Online";
  sendLOG(Log, SerialPrintLog);
  Version(165);//Information to Domoticz about the software version, date of upload and IP Arduino
  sensors.requestTemperatures();
  sensors.begin();
}
//##########################################################PĘTLA LOOP#####################################################
void loop(){
//________________________________________________Updating data from thermometers, pressure, humidity and other sensors.
//  Reed(3,155,0,LocationReed[0]);  
//  Reed(4,156,0,LocationReed[1]);  
//  Reed(5,157,0,LocationReed[2]);  
//  Reed(6,158,0,LocationReed[3]);  
//  Reed(7,159,0,LocationReed[4]);  
  unsigned long currentMillis = millis();//This code will be executed every specified time
  if (currentMillis - previousMillis > termometryTime) {
    sensors.requestTemperatures();
    previousMillis = currentMillis; //Assign the current time to the previousMillis variable
    Termometry(DS18B20Sensor0,31,11, 0.0, LocationTemp[0]);//Jadalnia
    Termometry(DS18B20Sensor0,49,11, 0.0, LocationTemp[1]);//Sypialnia
    Termometry(DS18B20Sensor0,50,11, 0.0, LocationTemp[2]);//Spiżarnia
    Termometry(DS18B20Sensor0,50,11, 0.0, LocationTemp[3]);//Kuchnia
    Termometry(DS18B20Sensor0,50,11, 0.0, LocationTemp[4]);//Vestibule
    Termometry(DS18B20Sensor0,50,11, 0.0, LocationTemp[5]);//Podbitka
    Termometry(DS18B20Sensor0,50,11, 0.0, LocationTemp[6]);//DwórWejście
    Termometry(DS18B20Sensor0,50,11, 0.0, LocationTemp[7]);//Wolne
    //BME280(51,1);//Spiżarnia
  }
// check for serial input
//________________________________________________SPRAWDZANIE POŁĄCZENIA Z SERWEREM
  EthernetClient client = server.available();
  int connectLoop =0;
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100) {
          readString += c; 
        } 
        //if HTTP request has ended
        if (c == '\n') {
          Serial.println(readString); //print to serial monitor for debuging 
          //now output HTML data header
        if(readString.indexOf('?') >=0) { //don't send new page
          client.println("HTTP/1.1 204 pybomatik");
          client.println();
          // client.println(); 
        } else {
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Dziala</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>Sve dobro!</BODY></HTML>");
          client.stop();
        }
        delay(1);
        ParsujPolecenia();
        }
      } 
      connectLoop++;
      if(connectLoop > 10000) {
        Serial.println();
        Serial.println(F("Timeout"));
        client.stop();
      }
    }
  }
  CheckPhisicalSwitch();
  delay(1);
}
//________________________________________________SPRAWDZENIE NACIŚNIĘCIA PRZEŁĄCZNIKÓW FIZYCZNYCH
void CheckPhisicalSwitch(){
  // ODBIERANIE SYGNAŁÓW WCIŚNIĘCIA Switch PRZEZ ARDUINO I WYSYŁANIE DO DOMOTICZA
  //RolleyShutterBedroom
  if (digitalRead(In14) == LOW) {
    currentSwitches[0] = millis();
    delay(50);
    if (digitalRead(In14) == LOW) {
      if (currentSwitches[0] - previousSwitches[0] > timeSwitches) {
        Blind4243 = !Blind4243;
        if ((Blind4243 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=145&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[0] = currentSwitches[0];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=145&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[0] = currentSwitches[0];
          delay(10);
        }
      }
    }
  }
  //RolleyShutterKitchen
  if (digitalRead(In15) == LOW) {
    currentSwitches[1] = millis();
    delay(50);
    if (digitalRead(In15) == LOW) {
      if (currentSwitches[1] - previousSwitches[1] > timeSwitches) {
        Blind4445 = !Blind4445;
        if ((Blind4445 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=148&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[1] = currentSwitches[1];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=148&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[1] = currentSwitches[1];
          delay(10);
        }
      }
    }
  }
  //RolleyShutterDiningroomLeft
  if (digitalRead(In16) == LOW) {
    currentSwitches[2] = millis();
    delay(50);
    if (digitalRead(In16) == LOW) {
      if (currentSwitches[2] - previousSwitches[2] > timeSwitches) {
        Blind4647 = !Blind4647;
        if ((Blind4647 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=151&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[2] = currentSwitches[2];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=151&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[2] = currentSwitches[2];
          delay(10);
        }
      }
    }
  }
  //RolleyShutterDiningroomRight
  if (digitalRead(In17) == LOW) {
    currentSwitches[3] = millis();
    delay(50);
    if (digitalRead(In17) == LOW) {
      if (currentSwitches[3] - previousSwitches[3] > timeSwitches) {
        Blind4849 = !Blind4849;
        if ((Blind4849 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=154&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[3] = currentSwitches[3];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=154&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[3] = currentSwitches[3];
          delay(10);
        }
      }
    }
  }  if (digitalRead(In18)==LOW) { //SwitchKitchenLED
    currentSwitches[4] = millis();delay(50);
    if (digitalRead(In18)==LOW) {
      if (currentSwitches[4] - previousSwitches[4] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=99&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=99&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[4] = currentSwitches[4];delay(10);
      }
    }
  }
  if (digitalRead(In19) == LOW) {  //SwitchKitchenPlafon
    currentSwitches[5] = millis();delay(50);
    if (digitalRead(In19) == LOW) {
      if (currentSwitches[5] - previousSwitches[5] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=100&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=100&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[5] = currentSwitches[5];delay(10);
      }
    }
  }
  if (digitalRead(In22) == LOW) {//SwitchDiningRoomGirandole
    currentSwitches[6] = millis();delay(50);
    if (digitalRead(In22) == LOW) {
      if (currentSwitches[6] - previousSwitches[6] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=101&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=101&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[6] = currentSwitches[6];delay(10);
      }
    }
  }
  if (digitalRead(In23) == LOW) {//SwitchDiningRoomLED
    currentSwitches[7] = millis();delay(50);
    if (digitalRead(In23) == LOW) {
      if (currentSwitches[7] - previousSwitches[7] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=102&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=102&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[7] = currentSwitches[7];delay(10);
      }
    }
  }
  if (digitalRead(In24) == LOW) {//SwitchDiningRoomHalogensWindowSteirs
    currentSwitches[8] = millis();delay(50);
    if (digitalRead(In24) == LOW) {
      if (currentSwitches[8] - previousSwitches[8] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=103&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=103&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[8] = currentSwitches[8];delay(10);
      }
    }
  }
  if (digitalRead(In25) == LOW) {//SwitchDiningRoomHalogensKitchenWalper
    currentSwitches[9] = millis();delay(50);
    if (digitalRead(In25) == LOW) {
      if (currentSwitches[9] - previousSwitches[9] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=104&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=104&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[9] = currentSwitches[9];delay(50);
      }
    }
  }
  if (digitalRead(In26) == LOW) {//Stairs
    currentSwitches[10] = millis();delay(50);
    if (digitalRead(In26) == LOW) {
      if (currentSwitches[10] - previousSwitches[10] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=105&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=105&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[10] = currentSwitches[10];delay(50);
      }
    }
  }
  if (digitalRead(In27) == LOW) {//Vestibule
    currentSwitches[11] = millis();delay(50);
    if (digitalRead(In27) == LOW) {
      if (currentSwitches[11] - previousSwitches[11] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=106&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=106&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[11] = currentSwitches[11];delay(50);
      }
    }
  }
  if (digitalRead(In28) == LOW) {//Pantry
    currentSwitches[12] = millis();delay(50);
    if (digitalRead(In28) == LOW) {
      if (currentSwitches[12] - previousSwitches[12] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=107&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=107&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[12] = currentSwitches[12];delay(50);
      }
    }
  }
  if (digitalRead(In29) == LOW) {//SoffitBack
    currentSwitches[13] = millis();delay(50);
    if (digitalRead(In29) == LOW) {
      if (currentSwitches[13] - previousSwitches[13] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=108&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=108&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[13] = currentSwitches[13];delay(50);
      }
    }
  }
  if (digitalRead(In30) == LOW) {//SoffitFront
    currentSwitches[14] = millis();delay(50);
    if (digitalRead(In30) == LOW) {
      if (currentSwitches[14] - previousSwitches[14] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=109&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=109&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[14] = currentSwitches[14];delay(50);
      }
    }
  }
  if (digitalRead(In31) == LOW) {//SoffitEntertrance
    currentSwitches[15] = millis();delay(50);
    if (digitalRead(In31) == LOW) {
      if (currentSwitches[15] - previousSwitches[15] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=110&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=110&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[15] = currentSwitches[15];delay(50);
      }
    }
  }
  if (digitalRead(In32) == LOW) {//OutDoorFront
    currentSwitches[16] = millis();delay(50);
    if (digitalRead(In32) == LOW) {
      if (currentSwitches[16] - previousSwitches[11] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=111&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=111&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[16] = currentSwitches[16];delay(50);
      }
    }
  }
  if (digitalRead(In33) == LOW) {//Tarace
    currentSwitches[17] = millis();delay(50);
    if (digitalRead(In33) == LOW) {
      if (currentSwitches[17] - previousSwitches[17] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=112&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=112&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[17] = currentSwitches[17];delay(50);
      }
    }
  }
/*  if (digitalRead(In08) == LOW) {//Driveway
    currentSwitches[18] = millis();delay(50);
    if (digitalRead(In08) == LOW) {
      if (currentSwitches[18] - previousSwitches[18] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=106&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=106&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[18] = currentSwitches[18];delay(50);
      }
    }
  }
  if (digitalRead(In09) == LOW) {//Gateway
    currentSwitches[19] = millis();delay(50);
    if (digitalRead(In09) == LOW) {
      if (currentSwitches[19] - previousSwitches[19] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=106&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=106&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[19] = currentSwitches[19];delay(50);
      }
    }
  }*/
}
//________________________________________________ODBIERANIE POLECEN Z DOMOTICZA 2019-12-15
void ParsujPolecenia(){
  if(readString.indexOf("Out08on") >0){//checks for on Out34 KitchenLED
    TurnOnLight(Out08,0); 
    Serial.println("+ TurnOn_ArduinoMega Out08");
  }
  if(readString.indexOf("Out08off") >0){
    TurnOffLight(Out08,0); 
    Serial.println("- TurnOff_ArduinoMega Out08");
  }
  if(readString.indexOf("Out09on") >0){//checks for on Out34 KitchenLED
    TurnOnLight(Out09,0); 
    Serial.println("+ TurnOn_ArduinoMega Out09");
  }
  if(readString.indexOf("Out09off") >0){
    TurnOffLight(Out09,0); 
    Serial.println("- TurnOff_ArduinoMega Out09");
  }
  if(readString.indexOf("Out34on") >0){//checks for on Out34 KitchenLED
    TurnOnLight(Out34,0); 
    Serial.println("+ TurnOn_ArduinoMega Out34");
  }
  if(readString.indexOf("Out34off") >0){
    TurnOffLight(Out34,0); 
    Serial.println("- TurnOff_ArduinoMega Out34");
  }
  if(readString.indexOf("Out35on") >0){//checks for on Out35 KitchenPlafon
    TurnOnLight(Out35,0); 
    Serial.println("+ TurnOn_ArduinoMega Out35");
  }
  if(readString.indexOf("Out35off") >0){
    TurnOffLight(Out35,0); 
    Serial.println("- TurnOff_ArduinoMega Out35");
  }
  if(readString.indexOf("Out36on") >0){//checks for on Out36 DiningRoomGirandole
    TurnOnLight(Out36,0); 
    Serial.println("+ TurnOn_ArduinoMega Out36");
  }
  if(readString.indexOf("Out36off") >0){
    TurnOffLight(Out36,0); 
    Serial.println("- TurnOff_ArduinoMega Out36");
  }
  
  if(readString.indexOf("Out37on") >0){//checks for on Out37 DiningRoomLED
    TurnOnLight(Out37,0); 
    Serial.println("+ TurnOn_ArduinoMega Out37");
  }
  if(readString.indexOf("Out37off") >0){
    TurnOffLight(Out37,0); 
    Serial.println("- TurnOff_ArduinoMega Out37");
  }
  
  if(readString.indexOf("Out38on") >0){//checks for on Out38 DiningRoomHalogensWindowStairs
    TurnOnLight(Out38,0); 
    Serial.println("+ TurnOn_ArduinoMega Out38");
  }
  if(readString.indexOf("Out38off") >0){
    TurnOffLight(Out38,0); 
    Serial.println("- TurnOff_ArduinoMega Out38");
  }
  
  if(readString.indexOf("Out39on") >0){//checks for on Out39 DiningRoomHalogensKitchenWalper
    TurnOnLight(Out39,0); 
    Serial.println("+ TurnOn_ArduinoMega Out39");
  }
  if(readString.indexOf("Out39off") >0){
    TurnOffLight(Out39,0); 
    Serial.println("- TurnOff_ArduinoMega Out39");
  }
  
  if(readString.indexOf("Out40on") >0){//checks for on Out40 Stairs
    TurnOnLight(Out40,0); 
    Serial.println("+ TurnOn_ArduinoMega Out40");
  }
  if(readString.indexOf("Out40off") >0){
    TurnOffLight(Out40,0); 
    Serial.println("- TurnOff_ArduinoMega Out40");
  }
  
  if(readString.indexOf("Out41on") >0){//checks for on Out41 Vestibule
    TurnOnLight(Out41,0); 
    Serial.println("+ TurnOn_ArduinoMega Out41");
  }
  if(readString.indexOf("Out41off") >0){
    TurnOffLight(Out41,0); 
    Serial.println("- TurnOff_ArduinoMega Out41");
  }
  //________________________________________________________Blinds
  if(readString.indexOf("BlindsMega42on") >0){//checks for on Out42 BedroomUp
    TurnOnLight(Out42,0); 
    Serial.println("+ Turn On Arduino Mega Out42");
  }
  if(readString.indexOf("BlindsMega42off") >0){
    TurnOffLight(Out42,0); 
    Serial.println("- Turn Off Arduino Mega Out42");
  }
  
  if(readString.indexOf("BlindsMega43on") >0){//checks for on Out43 BedroomDown
    TurnOnLight(Out43,0); 
    Serial.println("+ Turn On Arduino Mega Out43");
  }
  if(readString.indexOf("BlindsMega43off") >0){
    TurnOffLight(Out43,0); 
    Serial.println("- Turn Off Arduino Mega Out43");
  }

  if(readString.indexOf("BlindsMega44on") >0){//checks for on Out44 KitchenUp
    TurnOnLight(Out44,0); 
    Serial.println("+ Turn On Arduino Mega Out44");
  }
  if(readString.indexOf("BlindsMega44off") >0){
    TurnOffLight(Out44,0); 
    Serial.println("- Turn Off Arduino Mega Out44");
  }
  
  if(readString.indexOf("BlindsMega45on") >0){//checks for on Out45 KitchenDown
    TurnOnLight(Out45,0); 
    Serial.println("+ Turn On Arduino Mega Out45");
  }
  if(readString.indexOf("BlindsMega45off") >0){
    TurnOffLight(Out45,0); 
    Serial.println("- Turn Off Arduino Mega Out45");
  }
  
  if(readString.indexOf("BlindsMega46on") >0){//checks for on Out46 DiningRoomLeftUp
    TurnOnLight(Out46,0); 
    Serial.println("+ Turn On Arduino Mega Out46");
  }
  if(readString.indexOf("BlindsMega46off") >0){
    TurnOffLight(Out46,0); 
    Serial.println("- Turn Off Arduino Mega Out46");
  }
  
  if(readString.indexOf("BlindsMega47on") >0){//checks for on Out47 DiningRoomLeftDown
    TurnOnLight(Out47,0); 
    Serial.println("+ Turn On Arduino Mega Out47");
  }
  if(readString.indexOf("BlindsMega47off") >0){
    TurnOffLight(Out47,0); 
    Serial.println("- Turn Off Arduino Mega Out47");
  }
  
  if(readString.indexOf("BlindsMega48on") >0){//checks for on Out48 DiningRoomLeftUp
    TurnOnLight(Out48,0); 
    Serial.println("+ Turn On Arduino Mega Out48");
  }
  if(readString.indexOf("BlindsMega48off") >0){
    TurnOffLight(Out48,0); 
    Serial.println("- Turn Off Arduino MegaOut48");
  }
  
  if(readString.indexOf("BlindsMega49on") >0){//checks for on Out49 DiningRoomLeftDown
    TurnOnLight(Out49,0); 
    Serial.println("+ Turn On Arduino Mega Out49");
  }
  if(readString.indexOf("BlindsMega49off") >0){
    TurnOffLight(Out49,0); 
    Serial.println("- Turn Off Arduino Mega Out49");
  }
  //________________________________________________________Expander    
  if(readString.indexOf("OutExp01_PIN0on") >0){//checks for on Exp01_PIN0 Pantry 
    TurnOnLight(OutExp01_PIN0,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN0!!");
  }
  if(readString.indexOf("OutExp01_PIN0off") >0){
    TurnOffLight(OutExp01_PIN0,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN0");
  }

  if(readString.indexOf("OutExp01_PIN1on") >0){//checks for on Exp01_PIN1 SoffitBack
    TurnOnLight(OutExp01_PIN1,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN1");
  }
  if(readString.indexOf("OutExp01_PIN1off") >0){
    TurnOffLight(OutExp01_PIN1,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN1");
  }
  
  if(readString.indexOf("OutExp01_PIN2on") >0){//checks for on Exp01_PIN2 SoffitFront
    TurnOnLight(OutExp01_PIN2,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN2");
  }
  if(readString.indexOf("OutExp01_PIN2off") >0){
    TurnOffLight(OutExp01_PIN2,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN2");
  }
  
  if(readString.indexOf("OutExp01_PIN3on") >0){//checks for on Exp01_PIN3 SoffitEntertrace
    TurnOnLight(OutExp01_PIN3,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN3");
  }
  if(readString.indexOf("OutExp01_PIN3off") >0){
    TurnOffLight(OutExp01_PIN3,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN3");
  }
  
  if(readString.indexOf("OutExp01_PIN4on") >0){//checks for on Exp01_PIN4 OutdoorFront
    TurnOnLight(OutExp01_PIN4,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN4");
  }
  if(readString.indexOf("OutExp01_PIN4off") >0){
    TurnOffLight(OutExp01_PIN4,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN4");
  }
  
  if(readString.indexOf("OutExp01_PIN5on") >0){//checks for on Exp01_PIN5 Terrace
    TurnOnLight(OutExp01_PIN5,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN5");
  }
  if(readString.indexOf("OutExp01_PIN5off") >0){
    TurnOffLight(OutExp01_PIN5,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN5");
  }
  
  if(readString.indexOf("OutExp01_PIN6on") >0){//checks for on Exp01_PIN6 Driveway
    TurnOnLight(OutExp01_PIN6,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN6");
  }
  if(readString.indexOf("OutExp01_PIN6off") >0){
    TurnOffLight(OutExp01_PIN6,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN6");
  }
  
  if(readString.indexOf("OutExp01_PIN7on") >0){//checks for on Exp01_PIN7 Gateway
    TurnOnLight(OutExp01_PIN7,1); 
    Serial.println("+ TurnOn_ArduinoMega Light W TESTEXT01_PIN7");
  }
  if(readString.indexOf("OutExp01_PIN7off") >0){
    TurnOffLight(OutExp01_PIN7,1); 
    Serial.println("- TurnOff_ArduinoMega Light W TESTEXT01_PIN7");
  }   
  if(readString.indexOf("version") >0){//checks for Version
    Version(165);//idx=165
    Serial.println("Wysłano dane o wersji oprogramowania");
  }   
  readString=""; //clearing string for next read
}
//________________________________________________SENDING GET DATA TO THE DOMOTICZ 2019-12-15
void sendGET(String url,int SerialPrintGet){ 
  if (client.connect(myserver, 8080)) {
    if(SerialPrintGet==1){
      Serial.print("Wyslano url: ");
      Serial.println(url);
    }
    client.print("GET ");
    client.print(url); 
    client.println(" HTTP/1.0");
    client.println();
  } else {
    Serial.println("Connection failed UNO");
    Serial.println();
    }
  client.stop();
}
//________________________________________________SENDING LOG DATA TO THE DOMOTICZ 2019-12-15
void sendLOG(String url,int SerialPrintLog){
  String logtxt="";
  if (client.connect(myserver, 8080)) {
    if(SerialPrintLog==1){Serial.print("wysylam LOG:  ");}
    client.print("GET ");
    logtxt="/json.htm?type=command&param=addlogmessage&message=";
    logtxt+=url;
    client.print(logtxt); 
    client.println(" HTTP/1.0");
    client.println();
    if(SerialPrintLog==1){Serial.println(url);}
  } else {
    Serial.println("connection failed");
    Serial.println();
  }
  client.stop();
  if(SerialPrintLog==1){Serial.println("Wyslano log.");}
}
//________________________________________________TURN ON LIGHT MEGA AND EXPANDER'S
void TurnOnLight(int pinnr,int expanderNumber) {
  String logtxt="";
  logtxt="Ustawiam+stan+NISKI+na+PINie+nr:+";
  logtxt+=pinnr;
  if(expanderNumber>0 && expanderNumber <9){
    logtxt+="+na+Expander:+";
    logtxt+=expanderNumber;
  } else {
    logtxt+="+Arduino+Mega+IP:+192.168.1.221";   
  }
  switch(expanderNumber){
    case 0:
    digitalWrite(pinnr, HIGH); delay(2);
    digitalWrite(pinnr, LOW);
    break;
    case 1:
    expander01.digitalWrite(pinnr, HIGH); delay(2);
    expander01.digitalWrite(pinnr, LOW);
    break;
    case 2:
    expander02.digitalWrite(pinnr, HIGH); delay(2);
    expander02.digitalWrite(pinnr, LOW);
    break;
/*    case 3:
    expander03.digitalWrite(pinnr, HIGH); delay(2);
    expander03.digitalWrite(pinnr, LOW);
    break;
    case 4:
    expander04.digitalWrite(pinnr, HIGH); delay(2);
    expander04.digitalWrite(pinnr, LOW);
    break;
    case 5:
    expander05.digitalWrite(pinnr, HIGH); delay(2);
    expander05.digitalWrite(pinnr, LOW);
    break;
    case 6:
    expander06.digitalWrite(pinnr, HIGH); delay(2);
    expander06.digitalWrite(pinnr, LOW);
    break;
    case 7:
    expander07.digitalWrite(pinnr, HIGH); delay(2);
    expander07.digitalWrite(pinnr, LOW);
    break;
    case 8:
    expander08.digitalWrite(pinnr, HIGH); delay(2);
    expander08.digitalWrite(pinnr, LOW);
    break;*/
    default:
    logtxt="Nieznane_Urządzenie";
    break;
  }
  sendLOG(logtxt,SerialPrintLog); 
}
//________________________________________________TURN OFF LIGHT MEGA AND EXPANDER'S
void TurnOffLight(int pinnr,int expanderNumber) {
  String logtxt="";
  logtxt="Ustawiam+stan+WYSOKI+na+PINie+nr:+";
  logtxt+=pinnr;
  if(expanderNumber>0 && expanderNumber <9){
    logtxt+="+na+Expander:+";
    logtxt+=expanderNumber;
  } else {
    logtxt+="+Arduino+Mega+IP:+192.168.1.221";   
  }
  switch(expanderNumber){
    case 0:
    digitalWrite(pinnr, LOW); delay(2);
    digitalWrite(pinnr, HIGH);
    break;
    case 1:
    expander01.digitalWrite(pinnr, LOW); delay(2);
    expander01.digitalWrite(pinnr, HIGH);
    break;
    case 2:
    expander02.digitalWrite(pinnr, LOW); delay(2);
    expander02.digitalWrite(pinnr, HIGH);
    break;
/*    case 3:
    expander03.digitalWrite(pinnr, LOW); delay(2);
    expander03.digitalWrite(pinnr, HIGH);
    break;
    case 4:
    expander04.digitalWrite(pinnr, LOW); delay(2);
    expander04.digitalWrite(pinnr, HIGH);
    break;
    case 5:
    expander05.digitalWrite(pinnr, LOW); delay(2);
    expander05.digitalWrite(pinnr, HIGH);
    break;
    case 6:
    expander06.digitalWrite(pinnr, LOW); delay(2);
    expander06.digitalWrite(pinnr, HIGH);
    break;
    case 7:
    expander07.digitalWrite(pinnr, LOW); delay(2);
    expander07.digitalWrite(pinnr, HIGH);
    break;
    case 8:
    expander08.digitalWrite(pinnr, LOW); delay(2);
    expander08.digitalWrite(pinnr, HIGH);
    break;*/
    default:
    logtxt="Nieznane_Urządzenie";
    break;
  }
  sendLOG(logtxt,SerialPrintLog); 
}
//__________________________________________________________READING DATA FROM SENSORS DS18B20 2020-01-02
void Termometry(DeviceAddress deviceAddress, int idx, int TEMPERATURE_PRECISION, float correction, String location) {
  sensors.setResolution(deviceAddress, TEMPERATURE_PRECISION);
  float temp = sensors.getTempC(deviceAddress);
  temp = temp - correction;
  if(temp<=0){
    
  } else {
    String url = "/json.htm?type=command&param=udevice&idx=";
    if (SerialPrintGet == 1) {
      Serial.print("Temperature = ");
      Serial.print(temp);
      Serial.print(" - ");
      Serial.println(location);
    }
    url += idx;
    url += "&nvalue=0&svalue=";
    url += temp;
    sendGET(url, SerialPrintGet);
    delay(10);
  }
}
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
//__________________________________________________________READING DATA FROM SENSOR BME280 2020-01-03
void BME280(int idx,int SerialPrint = 0) {
  bme.begin(0x76);  
  float Temperatura = bme.readTemperature();
  float PressureAbsolute = bme.readPressure() / 100.0F;
  float Humidity = bme.readHumidity();
  double h = 8000*((1+(0.004*Temperatura))/PressureAbsolute);
  double i = 1+h;
  double j = i/PressureAbsolute;
  double k = 8000*j;
  double PressureRelative = PressureAbsolute+(175/h);
  if(SerialPrint==1){
    Serial.print("Temperature = ");
    Serial.print(Temperatura);
    Serial.println(" *C");
    Serial.print("Pressure Absolute = ");
    Serial.print(PressureAbsolute);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    Serial.print("Humidity = ");
    Serial.print(Humidity);
    Serial.println(" %"); 
    Serial.print("Pressure Relative = ");
    Serial.print(PressureRelative);
    Serial.println(" hPa");
  }
  String url="/json.htm?type=command&param=udevice&idx=";
  url+=idx;url+="&nvalue=0&svalue=";url+=Temperatura;url+=";";url+=Humidity;url+=";";
  //int HUM_STAT = HumidityStats(Humidity);
  url+=HumidityStats(Humidity);
  url+=";";url+=PressureRelative;url+=";";url+=0;//BAR_FORCAST
  sendGET(url,SerialPrintGet);
}
int HumidityStats(int Humidity){
  if(Humidity<40){return 2;}
  if(Humidity>=65){return 3;}
  if(Humidity>=48&&Humidity<55){return 1;}
  if(Humidity>=40&&Humidity<48){return 0;}
  if(Humidity>=55&&Humidity<65){return 0;}
}
//__________________________________________________________READING DATA FROM PHOTORESISTOR 2020-02-26
void Photoresistor(int PIN, int idx, int SensorNumber, String location) {
  String url;
  if (digitalRead(PIN) == 1 && DustSensor[SensorNumber] == 0) {
    url = "/json.htm?type=command&param=switchlight&idx=";
    url += idx;
    url += "&switchcmd=Off";
    sendGET(url, SerialPrintGet);
    if (SerialPrintLog == 1) {
      Serial.print(digitalRead(PIN));
      Serial.print(":Ciemno - ");
      Serial.println(location);
    }
    DustSensor[SensorNumber] = 1;
  }
  if (digitalRead(PIN) == 0 && DustSensor[SensorNumber] == 1) {
    url = "/json.htm?type=command&param=switchlight&idx=";
    url += idx;
    url += "&switchcmd=On";
    sendGET(url, SerialPrintGet);
    if (SerialPrintLog == 1) {
      Serial.print(digitalRead(PIN));
      Serial.print(":Jasno - ");
      Serial.println(location);
    }
    DustSensor[SensorNumber] = 0;
  }
}
//__________________________________________________________READING DATA FROM REED SENSOR 2020-03-15
void Reed(int PIN, int idx, int SensorNumber, String location){
  String url;
  if (digitalRead(PIN) == 1 && ReedSensor[SensorNumber] == 0) {
    url = "/json.htm?type=command&param=switchlight&idx=";url += idx;url += "&switchcmd=On";
    sendGET(url, SerialPrintGet);
    if (SerialPrintLog == 1) {Serial.print(digitalRead(PIN));Serial.print(":Drzwi Otwarte - ");Serial.println(location);}
    ReedSensor[SensorNumber] = 1;
  }
  if (digitalRead(PIN) == 0 && ReedSensor[SensorNumber] == 1) {
    url = "/json.htm?type=command&param=switchlight&idx=";url += idx;url += "&switchcmd=Off";
    sendGET(url, SerialPrintGet);
    if (SerialPrintLog == 1) {Serial.print(digitalRead(PIN));Serial.print(":Drzwi zamknięte - ");Serial.println(location);}
    ReedSensor[SensorNumber] = 0;
  }
}
//__________________________________________________________SENDING VERSION, IP and UPLOAD DATE TO DOMOTICZ 2020-03-29
void Version(int idx){
  String url;
  url = "/json.htm?type=command&param=udevice&idx=";
  url+=idx;
  url+="&nvalue=0&svalue=Wersja+oprogramowania:+";
  url+=versions;
  url+="+Adres+IP:+";
  url+=ipAddress;
  url+="+Data+wgrania:+";
  url+=date;
  sendGET(url, SerialPrintGet);
   if (SerialPrintGet == 1) {Serial.println(url);}
}

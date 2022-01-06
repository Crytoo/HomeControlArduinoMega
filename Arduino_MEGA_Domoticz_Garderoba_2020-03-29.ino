#define versions "1.1.0"            //Sketch version
#define date "2020-03-29"           //Upload date
#define ipAddress "192.168.1.223"   //IpAdress only for information not set

#include <Wire.h>               // Required for I2C communication
//#include <PCF8574.h>            // Required for PCF8574
#include <SPI.h>                // For communication with the ethernet module
#include <Ethernet.h>           // Mole Ethernet
#include <OneWire.h>            // For communication with a Dallas thermometer and other
#include <DallasTemperature.h>  // Dallas thermometer library 
#include <Adafruit_BME280.h>    // Czujnik temperatury, ciśnienia, wilgotności

//__________________________________________________________SETTINGS FOR DS18B20
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DeviceAddress DS18B20Sensor0 = { 0x28, 0xFF, 0x62, 0xB2, 0x85, 0x16, 0x4, 0x3C };//Błażej
DeviceAddress DS18B20Sensor1 = { 0x28, 0xFF, 0xFD, 0x37, 0x86, 0x16, 0x5, 0x6  };//Bartosz
DeviceAddress DS18B20Sensor2 = { 0x28, 0xFF, 0x8C, 0x37, 0x86, 0x16, 0x5, 0xA6 };//Salon
DeviceAddress DS18B20Sensor3 = { 0x28, 0xFF, 0x46, 0xC8, 0x6B, 0x18, 0x1, 0xB8 };//Łazienka
DeviceAddress DS18B20Sensor4 = { 0x28, 0xFF, 0x6C, 0x9B, 0x86, 0x16, 0x5, 0xC7 };//Strych
DeviceAddress DS18B20Sensor5 = { 0x28, 0xFF, 0x68, 0x5A, 0xB4, 0x16, 0x5, 0xFF };//Zasilanie
DeviceAddress DS18B20Sensor6 = { 0x28, 0xFF, 0x59, 0xB7, 0xC0, 0x16, 0x4, 0x4D };//Powrót
DeviceAddress DS18B20Sensor7 = { 0x28, 0xFF, 0xEF, 0xC7, 0xC0, 0x16, 0x4, 0x36 };//Pompka
unsigned long termometryTime = 60000;
unsigned long previousMillis = 0;
//__________________________________________________________SETTINGS FOR BME280
Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1013.25)  //Deklaracja stałej dla czujnika BME280
//__________________________________________________________MAPOWANIE SwitchóW (PRZEŁĄCZNIKÓW) I INNE INPUTY ARDUINO MEGA
#define In03 3//DustSensorBłażej
#define In04 4//DustSensorBartosz
#define In05 5//DustSensorSalon
#define In06 6//DustSensorBathroom1stFloor
#define In07 7//DustSensorWarderobe
//#define In08 8//
//#define In09 9//
//#define In10 10//
//#define In11 11//
//#define In12 12//
//#define In13 13//
#define In14 14//BlindSwitchBlazej
#define In15 15//BlindSwitchBartosz
#define In16 16//BlindSwitchSalonLeft
#define In17 17//BlindSwitchSalonRight
#define In18 18//SwitchBłażej
#define In19 19//SwitchBartosz
#define In22 22//SwitchBathroom1stFloor
#define In23 23//Switchwarderoba
#define In24 24//SwitchCorridor1stFloor
#define In25 25//SwitchAttic
#define In26 26//SwitchSalonDoor
#define In27 27//SwitchSalonWindow
#define Out28 28//HeatingBłażej
#define Out29 29//HeatingBartosz
#define Out30 30//HeatingSalon1
#define Out31 31//HeatingSalon2
#define Out32 32//HeatingBathroom1stFloor
#define Out33 33//HeatingPump
#define Out34 34//Błażej
#define Out35 35//Bartosz
#define Out36 36//SalonDoor
#define Out37 37//SalonWindow
#define Out38 38//Bathroom1stFloor
#define Out39 39//Wardrobe
#define Out40 40//Attic
#define Out41 41//Corridor1stFloor
#define Out42 42//BlindUpBłażej
#define Out43 43//BlindDownBłażej
#define Out44 44//BlindUpBartosz
#define Out45 45//BlindDownBartosz
#define Out46 46//BlindUpSalonLeft
#define Out47 47//BlindDownSalonLeft
#define Out48 48//BlindUpSalonRight
#define Out49 49//BlindDownSalonRight 
//########################################################################################
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE }; //physical mac address
IPAddress ip(192, 168, 1, 223); // ARDUINO IP W SIECI
IPAddress gateway(192, 168, 1, 20); // internet access via router
IPAddress subnet(255, 255, 255, 0); //subnet mask
IPAddress myserver(192, 168, 1, 220); // DOMOTICZ IP
EthernetServer server(84); //PORT ARDUINO
EthernetClient client;
//########################################################################################
String readString;
//Number of connected buttons time settings
unsigned long currentSwitches[12];
unsigned long previousSwitches[12];
unsigned long timeSwitches = 1000;
//Status of connected buttons
int Blind4243 = 0;//Blazej
int Blind4445 = 0;//Bartosz
int Blind4647 = 0;//SalonLeft
int Blind4849 = 0;//SalonRight
//Show(true) or not(false) in serial port message form function GET, LOG or both
bool SerialPrintGet = false;
bool SerialPrintLog = false;
int DustSensor[5];//DustSensor Initialization - [number of connected sensors]
String Location[10] = {"Błażej","Bartosz","Salon","Łazienka","Garderoba","Strych","KorytarzykGóra","Zasilanie","Powrót","Pompka"};
int ReedSensor[5];//ReedSensor Initialization - [number of connected sensors]
String LocationReed[5] = {"1","2","3","4","5"};
//##########################################################FUNCTION SETUP##########################################################
void setup() {
  Serial.begin(9600);
  //__________________________________________________________SETTINGS ETHERNET SHIELD W5100
  Ethernet.begin(mac, ip, subnet, gateway);
  server.begin();
  //__________________________________________________________PIN'S ARDUINO MEGA ROZDZIELNIA GARDEROBA
  pinMode(In03, INPUT);//DustSensorBłażej
  pinMode(In04, INPUT);//DustSensorBartosz
  pinMode(In05, INPUT);//DustSensorSalon
  pinMode(In06, INPUT);//DustSensorBathroomUp
  pinMode(In07, INPUT);//DustSensorWarderobe
  //  pinMode(In08, INPUT);//DustSensorAttic
  //  pinMode(In09, INPUT);//DustSensorCorridorUp
  pinMode(In14, INPUT_PULLUP);//BlindSwitchBłażej
  pinMode(In15, INPUT_PULLUP);//BlindSwitchBartosz
  pinMode(In16, INPUT_PULLUP);//BlindSwitchSalonLeft
  pinMode(In17, INPUT_PULLUP);//BlindSwitchSalonRight
  pinMode(In18, INPUT_PULLUP);//SwitchLightBłażej
  pinMode(In19, INPUT_PULLUP);//SwitchLightBartosz
  pinMode(In22, INPUT_PULLUP);//SwitchLightBathroom1stFloor
  pinMode(In23, INPUT_PULLUP);//SwitchLightWarderoba
  pinMode(In24, INPUT_PULLUP);//SwitchLightAttic
  pinMode(In25, INPUT_PULLUP);//SwitchLightCorridor1stFloor
  pinMode(In26, INPUT_PULLUP);//SwitchLightSalonDoor
  pinMode(In27, INPUT_PULLUP);//SwitchLightSalonWindow
  pinMode(Out28, OUTPUT);//HeatingBłażej
  pinMode(Out29, OUTPUT);//HeatingBartosz
  pinMode(Out30, OUTPUT);//HeatingSalon1
  pinMode(Out31, OUTPUT);//HeatingSalon2
  pinMode(Out32, OUTPUT);//HeatingBathroom1stFloor
  pinMode(Out33, OUTPUT);//Pump
  pinMode(Out34, OUTPUT);//Błażej
  pinMode(Out35, OUTPUT);//Bartosz
  pinMode(Out36, OUTPUT);//SalonDoor
  pinMode(Out37, OUTPUT);//SalonWindow
  pinMode(Out38, OUTPUT);//BathroomUp
  pinMode(Out39, OUTPUT);//Wardrobe
  pinMode(Out40, OUTPUT);//Corridor1stFloor
  pinMode(Out41, OUTPUT);//Attic
  pinMode(Out42, OUTPUT);//BlindUpBłażej
  pinMode(Out43, OUTPUT);//BlindDownBłażej
  pinMode(Out44, OUTPUT);//BlindUpBartosz
  pinMode(Out45, OUTPUT);//BlindDownBartosz
  pinMode(Out46, OUTPUT);//BlindUpSalonLeft
  pinMode(Out47, OUTPUT);//BlindDownSalonLeft
  pinMode(Out48, OUTPUT);//BlindUpSalonRight
  pinMode(Out49, OUTPUT);//BlindDownSalonRight
  //__________________________________________________________Ustawienie stanów na PINach wyjściowych
  digitalWrite(Out28, HIGH);//HeatingBłażej
  digitalWrite(Out29, HIGH);//HeatingBartosz
  digitalWrite(Out30, HIGH);//Heating
  digitalWrite(Out31, HIGH);//Heating
  digitalWrite(Out32, HIGH);//Heating
  digitalWrite(Out33, HIGH);//Pump
  digitalWrite(Out34, HIGH);//Błażej
  digitalWrite(Out35, HIGH);//Bartosz
  digitalWrite(Out36, HIGH);//SalonDoor
  digitalWrite(Out37, HIGH);//SalonWindow
  digitalWrite(Out38, HIGH);//BathroomUp
  digitalWrite(Out39, HIGH);//Wardrobe
  digitalWrite(Out40, HIGH);//Attic
  digitalWrite(Out41, HIGH);//CorridorUp
  digitalWrite(Out42, HIGH);//BlindUpBłażej
  digitalWrite(Out43, HIGH);//BlindDownBłażej
  digitalWrite(Out44, HIGH);//BlindUpBartosz
  digitalWrite(Out45, HIGH);//BlindDownBartosz
  digitalWrite(Out46, HIGH);//BlindUpSalonLeft
  digitalWrite(Out47, HIGH);//BlindDownSalonLeft
  digitalWrite(Out48, HIGH);//BlindUpSalonRight
  digitalWrite(Out49, HIGH);//BlindDownSalonRight
  //__________________________________________________________LOG DOMOTICZ and SERIAL PORT
  Serial.println("Czekam 30 sekund na połączenie ethernet.");
  for(int i=30;i>-1;i--){delay(1000);Serial.print(i);Serial.print(" ");Serial.write(13);}
  Serial.println("");
  String ver = versions;
  if(SerialPrintLog==true){
  Serial.print("Server/Client version: ");
  Serial.print(ver);
  Serial.print(" Arduino Mega Garderoba IP: ");
  Serial.println(ipAddress);}
  String Log="Arduino+Mega+Garderoba+IP:+";
  Log+=ipAddress;
  Log+="+Online";
  sendLOG(Log, SerialPrintLog);
  Log="Server/Client+version:+";
  Log+=ver;
  Version(167);//Informacja do Domoticza o wersji oprogramownia, dacie wgrania i IP Arduino
  sendLOG(Log, SerialPrintLog);
  sensors.begin();
}
//##########################################################PĘTLA LOOP##########################################################
void loop() {
  //__________________________________________________________Updating data from connected sensors
  //Photoresistor(In03, 11, 0, Location[0]);//(PIN,idx,sensorNumber,sensorLocation)DustSensorBlażej
  //Photoresistor(In04, 12, 1, Location[1]);//(PIN,idx,sensorNumber,sensorLocation)DustSensorBartosz
  //Photoresistor(In05, 13, 2, Location[2]);//(PIN,idx,sensorNumber,sensorLocation)DustSensorSalon
  //Photoresistor(In06, 14, 3, Location[3]);//(PIN,idx,sensorNumber,sensorLocation)DustSensorSalon
  unsigned long currentMillis = millis();//This code will be executed every specified time (In this case 60000 ms = 1 minute)
  if (currentMillis - previousMillis > termometryTime) {
    previousMillis = currentMillis; //Assign the current time to the previousMillis variable
    sensors.requestTemperatures();
    Termometry(DS18B20Sensor0, 28, 11, 0.0, Location[0]);//Blażej
    Termometry(DS18B20Sensor1, 27, 11, 0.5, Location[1]);//Bartosz
    Termometry(DS18B20Sensor2, 29, 11, 0.0, Location[2]);//Salon
    Termometry(DS18B20Sensor3, 30, 11, 0.0, Location[3]);//Łazienka
    Termometry(DS18B20Sensor4, 36, 11, 0.0, Location[5]);//Strych
    Termometry(DS18B20Sensor5, 38, 11, 0.0, Location[7]);//Zasilanie
    Termometry(DS18B20Sensor6, 39, 11, 0.0, Location[8]);//Powrót
    Termometry(DS18B20Sensor7, 40, 11, 0.0, Location[9]);//Pompka
    BME280(51,SerialPrintGet);
  }
  //__________________________________________________________SPRAWDZANIE POŁĄCZENIA Z SERWEREM
  EthernetClient client = server.available();
  int connectLoop = 0;
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
        //if HTTP request has ended
        if (c == '\n') {
          Serial.println(readString); //print to serial monitor for debuging
          //now output HTML data header
          if (readString.indexOf('?') >= 0) { //don't send new page
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
          ParseCommands();
        }
      }
      connectLoop++;
      if (connectLoop > 10000) {
        Serial.println();
        Serial.println(F("Timeout"));
        client.stop();
      }
    }
  }
  CheckPhisicalSwitch();
  delay(1);
}
//__________________________________________________________SPRAWDZENIE NACIŚNIĘCIA PRZEŁĄCZNIKÓW FIZYCZNYCH
void CheckPhisicalSwitch() {
  //RolleyShutterBlazej
  if (digitalRead(In14) == LOW) {
    currentSwitches[0] = millis();
    delay(50);
    if (digitalRead(In14) == LOW) {
      if (currentSwitches[0] - previousSwitches[0] > timeSwitches) {
        Blind4243 = !Blind4243;
        if ((Blind4243 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=133&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[0] = currentSwitches[0];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=133&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[0] = currentSwitches[0];
          delay(10);
        }
      }
    }
  }
  //RolleyShutterBartosz
  if (digitalRead(In15) == LOW) {
    currentSwitches[1] = millis();
    delay(50);
    if (digitalRead(In15) == LOW) {
      if (currentSwitches[1] - previousSwitches[1] > timeSwitches) {
        Blind4445 = !Blind4445;
        if ((Blind4445 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=136&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[1] = currentSwitches[1];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=136&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[1] = currentSwitches[1];
          delay(10);
        }
      }
    }
  }
  //RolleyShutterSalonLeft
  if (digitalRead(In16) == LOW) {
    currentSwitches[2] = millis();
    delay(50);
    if (digitalRead(In16) == LOW) {
      if (currentSwitches[2] - previousSwitches[2] > timeSwitches) {
        Blind4647 = !Blind4647;
        if ((Blind4647 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=139&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[2] = currentSwitches[2];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=139&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[2] = currentSwitches[2];
          delay(10);
        }
      }
    }
  }
  //RolleyShutterSalonRight
  if (digitalRead(In17) == LOW) {
    currentSwitches[3] = millis();
    delay(50);
    if (digitalRead(In17) == LOW) {
      if (currentSwitches[3] - previousSwitches[3] > timeSwitches) {
        Blind4849 = !Blind4849;
        if ((Blind4849 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=142&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[3] = currentSwitches[3];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=142&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[3] = currentSwitches[3];
          delay(10);
        }
      }
    }
  }
  if (digitalRead(In18) == LOW) {//SwitchBłażej
    currentSwitches[4] = millis();delay(50);
    if (digitalRead(In18) == LOW) {
      if (currentSwitches[4] - previousSwitches[4] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=91&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=91&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[4] = currentSwitches[4];delay(10);
      }
    }
  }
  if (digitalRead(In19) == LOW) {  //SwitchBartosz
    currentSwitches[5] = millis();delay(50);
    if (digitalRead(In19) == LOW) {
      if (currentSwitches[5] - previousSwitches[5] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=92&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=92&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[5] = currentSwitches[5];delay(10);
      }
    }
  }
  if (digitalRead(In22) == LOW) {//SwitchBathroom1stFloor
    currentSwitches[8] = millis();delay(50);
    if (digitalRead(In22) == LOW) {
      if (currentSwitches[8] - previousSwitches[8] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=95&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=95&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[8] = currentSwitches[8];delay(10);
      }
    }
  }
  if (digitalRead(In23) == LOW) {//SwitchWarderobe
    currentSwitches[9] = millis();delay(50);
    if (digitalRead(In23) == LOW) {
      if (currentSwitches[9] - previousSwitches[9] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=96&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=96&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[9] = currentSwitches[9];delay(50);
      }
    }
  }
  if (digitalRead(In24) == LOW) {//Corridor1stFloor
    currentSwitches[10] = millis();delay(50);
    if (digitalRead(In24) == LOW) {
      if (currentSwitches[10] - previousSwitches[10] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=97&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=97&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[10] = currentSwitches[10];delay(50);
      }
    }
  }
  if (digitalRead(In25) == LOW) {//Attic
    currentSwitches[11] = millis();delay(50);
    if (digitalRead(In25) == LOW) {
      if (currentSwitches[11] - previousSwitches[11] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=98&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=98&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[11] = currentSwitches[11];delay(50);
      }
    }
  }
  if (digitalRead(In26) == LOW) {//SwitchSalonDoor
    currentSwitches[6] = millis();delay(50);
    if (digitalRead(In26) == LOW) {
      if (currentSwitches[6] - previousSwitches[6] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=93&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=93&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[6] = currentSwitches[6];delay(10);
      }
    }
  }
  if (digitalRead(In27) == LOW) {//SwitchSalonWindow
    currentSwitches[7] = millis();delay(50);
    if (digitalRead(In27) == LOW) {
      if (currentSwitches[7] - previousSwitches[7] > timeSwitches) {
        Serial.println("json.htm?type=command&param=switchlight&idx=94&switchcmd=Toggle");
        sendGET("/json.htm?type=command&param=switchlight&idx=94&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[7] = currentSwitches[7];delay(10);
      }
    }
  }
}
//__________________________________________________________ODBIERANIE POLECEN Z DOMOTICZA
void ParseCommands() {
  //________________________________________________________Heating
  if (readString.indexOf("Out28on") > 0) { //checks for on Out28 Błażej
    TurnOnLight(Out28, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out28off") > 0) { //checks for off
    TurnOffLight(Out28, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out29on") > 0) { //checks for on Out29 Bartosz
    TurnOnLight(Out29, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out29off") > 0) { //checks for off
    TurnOffLight(Out29, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out30on") > 0) { //checks for on Out30 Salon 1
    TurnOnLight(Out30, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out30off") > 0) { //checks for off
    TurnOffLight(Out30, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out31on") > 0) { //checks for on Out31 Salon 2
    TurnOnLight(Out31, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out31off") > 0) { //checks for off
    TurnOffLight(Out31, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out32on") > 0) { //checks for on Out32 Bathroom1stFloor
    TurnOnLight(Out32, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out32off") > 0) { //checks for off
    TurnOffLight(Out32, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out33on") > 0) { //checks for on Out33 Pomp
    TurnOnLight(Out33, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out33off") > 0) { //checks for off
    TurnOffLight(Out33, 0, SerialPrintLog, Location[0]);
  }
  //________________________________________________________Lights
  if (readString.indexOf("Out34on") > 0) { //checks for on Out34 Błażej
    TurnOnLight(Out34, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out34off") > 0) { //checks for off
    TurnOffLight(Out34, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out35on") > 0) { //checks for on Out35 Bartosz
    TurnOnLight(Out35, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out35off") > 0) { //checks for off
    TurnOffLight(Out35, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out36on") > 0) { //checks for on Out36 SAlon Door
    TurnOnLight(Out36, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out36off") > 0) { //checks for off
    TurnOffLight(Out36, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out37on") > 0) { //checks for on Out37 Salon Window
    TurnOnLight(Out37, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out37off") > 0) { //checks for off
    TurnOffLight(Out37, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out38on") > 0) { //checks for on Out38 BathroomUp
    TurnOnLight(Out38, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out38off") > 0) { //checks for off
    TurnOffLight(Out38, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out39on") > 0) { //checks for on Out39 Warderoba
    TurnOnLight(Out39, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out39off") > 0) { //checks for off
    TurnOffLight(Out39, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out40on") > 0) { //checks for on Out40 CorridorUp
    TurnOnLight(Out40, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out40off") > 0) { //checks for off
    TurnOffLight(Out40, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("Out41on") > 0) { //checks for on Out41 Attic
    TurnOnLight(Out41, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("Out41off") > 0) { //checks for off
    TurnOffLight(Out41, 0, SerialPrintLog, Location[0]);
  }
  //________________________________________________________Blinds
  if (readString.indexOf("BlindsMega42on") > 0) { //checks for on Out42 BłażejUp
    TurnOnLight(Out42, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega42off") > 0) { //checks for off
    TurnOffLight(Out42, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("BlindsMega43on") > 0) { //checks for on Out43 BlażejDown
    TurnOnLight(Out43, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega43off") > 0) { //checks for off
    TurnOffLight(Out43, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("BlindsMega44on") > 0) { //checks for on Out44 BartoszUp
    TurnOnLight(Out44, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega44off") > 0) { //checks for off
    TurnOffLight(Out44, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("BlindsMega45on") > 0) { //checks for on Out45 BartoszDown
    TurnOnLight(Out45, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega45off") > 0) { //checks for off
    TurnOffLight(Out45, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("BlindsMega46on") > 0) { //checks for on Out46 SalonLeftUp
    TurnOnLight(Out46, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega46off") > 0) { //checks for off
    TurnOffLight(Out46, 0, SerialPrintLog, Location[0]);
    Blind4647 = 1;
  }

  if (readString.indexOf("BlindsMega47on") > 0) { //checks for on Out47 SalonLeftDown
    TurnOnLight(Out47, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega47off") > 0) { //checks for off
    TurnOffLight(Out47, 0, SerialPrintLog, Location[0]);
    Blind4647 = 0;
  }

  if (readString.indexOf("BlindsMega48on") > 0) { //checks for on Out48 SalonRightUp
    TurnOnLight(Out48, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega48off") > 0) { //checks for off
    TurnOffLight(Out48, 0, SerialPrintLog, Location[0]);
  }

  if (readString.indexOf("BlindsMega49on") > 0) { //checks for on Out49 SalonRightDown
    TurnOnLight(Out49, 0, SerialPrintLog, Location[0]);
  }
  if (readString.indexOf("BlindsMega49off") > 0) { //checks for off
    TurnOffLight(Out49, 0, SerialPrintLog, Location[0]);
  }
  if(readString.indexOf("version") >0){//checks for Version
    Version(167);//idx=167
    Serial.println("Wysłano dane o wersji oprogramowania");
  }
  //clearing string for next read
  readString = "";
}
//__________________________________________________________SENDING GET DATA TO THE DOMOTICZ
void sendGET(String url, int SerialPrintGet) {
  if (client.connect(myserver, 8080)) {
    if (SerialPrintGet == 1) {
      Serial.print("Wyslano url: ");
      Serial.println(url);
    }
    client.print("GET ");
    client.print(url);
    client.println(" HTTP/1.0");
    client.println();
  } else {
    Serial.println("Connection failed GET MEGA Garderoba");
    Serial.println();
  }
  client.stop();
}
//__________________________________________________________SENDING LOG DATA TO THE DOMOTICZ
void sendLOG(String url, int SerialPrintLog) {
  String logtxt = "";
  if (client.connect(myserver, 8080)) {
    if (SerialPrintLog == 1) {
      Serial.print("wysylam LOG:  ");
    }
    client.print("GET ");
    logtxt = "/json.htm?type=command&param=addlogmessage&message=";
    logtxt += url;
    client.print(logtxt);
    client.println(" HTTP/1.0");
    client.println();
    if (SerialPrintLog == 1) {
      Serial.println(url);
    }
  } else {
    Serial.println("connection failed LOG MEGA Garderoba");
    Serial.println();
  }
  client.stop();
  if (SerialPrintLog == 1) {
    Serial.println("Wyslano log.");
  }
}
//__________________________________________________________TURN ON LIGHT
void TurnOnLight(int pinnr, int expanderNumber, bool SerialPrintLog, String Location) {
  String logtxt = "";
  logtxt = "Ustawiam+stan+NISKI+na+PINie+nr:+";
  logtxt += pinnr;
  logtxt += "+Arduino+Mega+IP:+";
  logtxt += ipAddress;
  digitalWrite(pinnr, LOW);
  if(SerialPrintLog==true){Serial.print("+ Turn On Arduino Mega ");Serial.println(pinnr);}
  sendLOG(logtxt, SerialPrintLog);
}
//__________________________________________________________TURN OFF LIGHT
void TurnOffLight(int pinnr, int expanderNumber, bool SerialPrintLog, String Location) {
  String logtxt = "";
  logtxt = "Ustawiam+stan+WYSOKI+na+PINie+nr:+";
  logtxt += pinnr;
  logtxt += "+Arduino+Mega+IP:+";
  logtxt += ipAddress;
  digitalWrite(pinnr, HIGH);
  if(SerialPrintLog==true){Serial.print("- Turn Off Arduino Mega ");Serial.println(pinnr);}
  sendLOG(logtxt, SerialPrintLog);
}
//__________________________________________________________READING DATA FROM SENSORS DS18B20 2020-01-02
void Termometry(DeviceAddress deviceAddress, int idx, int TEMPERATURE_PRECISION, float correction, String location) {
  //if (!sensors.getAddress(deviceAddress, 0)) Serial.println("Unable to find address for Device 0");  printAddress(deviceAddress);
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
void BME280(int idx,int SerialPrint) {
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
  if((Humidity>=40&&Humidity<48)or(Humidity>=55&&Humidity<65)){return 0;}
}
//__________________________________________________________ANALOG KEYBORD READ 2020-03-15
int getAnalogButton(int AnalogPin){
  if(analogRead(AnalogPin)<500 and analogRead(AnalogPin)>400){
    currentSwitches[20]=millis();
    delay(50);
    if(analogRead(AnalogPin)<500 and analogRead(AnalogPin)>0){
      if(currentSwitches[20] - previousSwitches[20] > timeSwitches){
        previousSwitches[20]=currentSwitches[20];
        Serial.println("0");
        return 0;
      }
    }
  }  if(analogRead(AnalogPin)<800 and analogRead(AnalogPin)>700){
    currentSwitches[21]=millis();
    delay(50);
    if(analogRead(AnalogPin)<800 and analogRead(AnalogPin)>700){
      if(currentSwitches[21] - previousSwitches[21] > timeSwitches){
        previousSwitches[21]=currentSwitches[21];
        Serial.println("5");
        return 5;
      }
    }
  }  if(analogRead(AnalogPin)<700 and analogRead(AnalogPin)>600){
    currentSwitches[22]=millis();
    delay(50);
    if(analogRead(AnalogPin)<700 and analogRead(AnalogPin)>600){
      if(currentSwitches[22] - previousSwitches[22] > timeSwitches){
        previousSwitches[22]=currentSwitches[22];
        Serial.println("8");
        return 8;
      }
    }
  }  if(analogRead(AnalogPin)<600 and analogRead(AnalogPin)>500){
    currentSwitches[23]=millis();
    delay(50);
    if(analogRead(AnalogPin)<600 and analogRead(AnalogPin)>500){
      if(currentSwitches[23] - previousSwitches[23] > timeSwitches){
        previousSwitches[23]=currentSwitches[23];
        Serial.println("2");
        return 2;
      }
    }
  }
  return -1;
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

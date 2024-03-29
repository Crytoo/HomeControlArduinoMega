#define versions "1.1.1"            //Sketch version
#define date "2022-01-07"           //Upload date
#define ipAddress "192.168.1.222"   //IpAdress only for information not settings

#include <Wire.h>               // Required for I2C communication
//#include <PCF8574.h>            // Required for PCF8574
#include <SPI.h>                // Do komunikacji z modułem ethernet
#include <Ethernet.h>           // Moduł Ethernet
#include <OneWire.h>            // Do komunikacji z termometrem dallas
#include <DallasTemperature.h>  // Biblioteka termometrów Dallas 
#include <Adafruit_BME280.h>    // Temperature, pressure and humidity sensor

//__________________________________________________________SETTINGS FOR DS18B20
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. For 8 sensord resistor 3,3k is OK
DallasTemperature sensors(&oneWire);
DeviceAddress DS18B20Sensor0 = { 0x28, 0x5, 0xB0, 0x79, 0x97, 0x4, 0x3, 0xA };//Bufor
DeviceAddress DS18B20Sensor1 = { 0x28, 0xAA, 0xBA, 0xDE, 0x52, 0x14, 0x1, 0xD6 };//RecuInEarth
DeviceAddress DS18B20Sensor2 = { 0x28, 0xAA, 0x21, 0x63, 0x53, 0x14, 0x1, 0xB6 };//RecuGoHome
DeviceAddress DS18B20Sensor3 = { 0x28, 0xAA, 0xF9, 0xE8, 0x52, 0x14, 0x1, 0x30 };//RecuOutside
DeviceAddress DS18B20Sensor4 = { 0x28, 0xAA, 0x35, 0xF4, 0x52, 0x14, 0x1, 0x19 };//RecuBackHome
DeviceAddress DS18B20Sensor5 = { 0x28, 0xAA, 0x35, 0xF4, 0x52, 0x14, 0x1, 0x19 };//TechnicalRoom  dodać czujnik zmienić adres
DeviceAddress DS18B20Sensor6 = { 0x28, 0xAA, 0xBA, 0xDE, 0x52, 0x14, 0x1, 0xD6 };//Bedroom        dodać czujnik zmienić adres
DeviceAddress DS18B20Sensor7 = { 0x28, 0xAA, 0xBA, 0xDE, 0x52, 0x14, 0x1, 0xD6 };//Bathroom       dodać czujnik zmienić adres
                                                                                                                                                                                                                                                                                                                                                                                                                       
unsigned long termometryTime = 60000;
unsigned long previousMillis = 0;
//__________________________________________________________SETTINGS FOR BME280
Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1013.25)  //Constant declaration for the BME280 sensor
//__________________________________________________________SWITCH MAPPING AND OTHER ARDUINO MEGA INPUTS
//#define In03 3//Free
//#define In04 4//Free
//#define In05 5//Free
//#define In06 6//Free
//#define In07 7//Free
//#define In08 8//Free
//#define In09 9//Free
//#define In10 10//Ethernet SD Card Ethernet Shield
//#define In11 11//Resereved
//#define In12 12//Resereved
//#define In13 13//Resereved
#define In14 14//BedroomGirandole
#define In15 15//BedroomHalogens
#define In16 16//BedroomLED
#define In17 17//TechnicalRoom
#define In18 18//OutsideRear
#define In19 19//BedroomRoleyShuter
#define In22 22//
#define In23 23//
//#define In24 24//
//#define In25 25//
//#define In26 26//
//#define In27 27//
#define Out28 28//BedroomGirandole
#define Out29 29//BedroomHalogens
#define Out30 30//BedroomLED
#define Out31 31//BathroomGorund
#define Out32 32//CorridorGround
#define Out33 33//UnderStairs
#define Out34 34//TechnicalRoom
#define Out35 35//OutsiedRear
#define Out36 36//BuforHeat1
#define Out37 37//BuforHeat2
//#define Out38 38//
//#define Out39 39//
//#define Out40 40//
//#define Out41 41//
//#define Out42 42//
//#define Out43 43//
//#define Out44 44//
//#define Out45 45//
//#define Out46 46//
//#define Out47 47//
//#define Out48 48//
//#define Out49 49//

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF }; //physical mac address
IPAddress ip(192,168,1,222); // ARDUINO IP W SIECI
IPAddress gateway(192,168,1,20); // internet access via router
IPAddress subnet(255,255,255,0); //subnet mask
IPAddress myserver(192,168,1,220); // DOMOTICZ IP
EthernetServer server(84); //PORT ARDUINO
EthernetClient client;
//__________________________________________________________SETTING VARIABLE
String readString;

int Blind4243 = 0;//Bedroom
//Show(1) or not(0) in serial port message form function GET, LOG or both
int SerialPrintGet = 0; 
int SerialPrintLog = 0;
unsigned long currentSwitches[24];
unsigned long previousSwitches[24];
unsigned long timeSwitches = 2000;
int DustSensor[0];//DustSensor Initialization - [number of connected sensors]
String LocationTemp[8] = {"Bufor","RecuFromGWC","RecuInHome","RecuBackHome","RecuOutside","TechnicalRoom","Bedroom","Bathroom"};
int ReedSensor[0];//ReedSensor Initialization - [number of connected sensors]
String LocationReed[1] = {""};
//##########################################################FUNCTION SETUP#####################################################
void setup(){
  Serial.begin(9600); 
//________________________________________________SETTINGS ETHERNET SHIELD W5100 
  Ethernet.begin(mac, ip, subnet, gateway); 
  server.begin();
//________________________________________________PIN'S ARDUINO MEGA ROZDZIELNIA TECHNICZNE
//  pinMode(In03, INPUT);//
//  pinMode(In04, INPUT);//
//  pinMode(In05, INPUT);//
//  pinMode(In06, INPUT);//
//  pinMode(In07, INPUT);//
//  pinMode(In08, INPUT);//
//  pinMode(In09, INPUT);//
  pinMode(In14, INPUT_PULLUP);//BedroomGirandole
  pinMode(In15, INPUT_PULLUP);//BedroomHalogens
  pinMode(In16, INPUT_PULLUP);//BedroomLED
  pinMode(In17, INPUT_PULLUP);//TechnicalRoom
  pinMode(In18, INPUT_PULLUP);//SoffitBack
  pinMode(In19, INPUT_PULLUP);//BedroomROleyShuter
  pinMode(In22, INPUT_PULLUP);//BathroomGround
  pinMode(In23, INPUT_PULLUP);//CorridorGround
//  pinMode(In24, INPUT_PULLUP);//
//  pinMode(In25, INPUT_PULLUP);//
//  pinMode(In26, INPUT_PULLUP);//
//  pinMode(In27, INPUT_PULLUP);//
  pinMode(Out28, OUTPUT);//BedroomGirandole
  pinMode(Out29, OUTPUT);//BedroomHalogens
  pinMode(Out30, OUTPUT);//BedroomLED
  pinMode(Out31, OUTPUT);//BathroomGround
  pinMode(Out32, OUTPUT);//CorridorGround
  pinMode(Out33, OUTPUT);//UnderStairs
  pinMode(Out34, OUTPUT);//TechnicalRoom
  pinMode(Out35, OUTPUT);//OutdoorRear
  pinMode(Out36, OUTPUT);//BuforHeat1
  pinMode(Out37, OUTPUT);//BuforHeat2
//  pinMode(Out38, OUTPUT);//
//  pinMode(Out39, OUTPUT);//
//  pinMode(Out40, OUTPUT);//
//  pinMode(Out41, OUTPUT);//
//  pinMode(Out42, OUTPUT);//
//  pinMode(Out43, OUTPUT);//
//  pinMode(Out44, OUTPUT);//
//  pinMode(Out45, OUTPUT);//
//  pinMode(Out46, OUTPUT);//
//  pinMode(Out47, OUTPUT);//
//  pinMode(Out48, OUTPUT);//
//  pinMode(Out49, OUTPUT);//
//________________________________________________Ustawienie stanów wysokich na PINach
  digitalWrite(Out28, HIGH);
  digitalWrite(Out29, HIGH);
  digitalWrite(Out30, HIGH);
  digitalWrite(Out31, HIGH);
  digitalWrite(Out32, HIGH);
  digitalWrite(Out33, HIGH);
  digitalWrite(Out34, HIGH);
  digitalWrite(Out35, HIGH);
  digitalWrite(Out36, HIGH);
  digitalWrite(Out37, HIGH);
  //______________________________________________LOG DOMOTICZ and SERIAL PORT
  Serial.println("I wait 30 seconds for an ethernet connection.");
  for(int i=30;i>-1;i--){delay(1000);Serial.print(i);Serial.print(" ");Serial.write(13);}
  Serial.println("");
  String ver = versions;
  Serial.print("Server/Client version: ");
  Serial.print(ver);
  Serial.print(" Arduino Mega Techniczne IP: ");
  Serial.println(ipAddress);
  String Log="Arduino+Mega+Techniczne+IP:+";
  Log+=ipAddress;
  Log+="+Online";
  sendLOG(Log, SerialPrintLog);
  Version(166);//Information to Domoticz about the software version, date of upload and IP Arduino
  sendLOG(Log, SerialPrintLog);
  sensors.requestTemperatures();
  sensors.begin();
}
//##########################################################PĘTLA LOOP#####################################################
void loop(){
//________________________________________________Aktualizadja danych z termometrów i czujników ciśnienia i wilgotności
  unsigned long currentMillis = millis();//This code will be executed every specified time
  if (currentMillis - previousMillis > termometryTime) {
    previousMillis = currentMillis; //Assign the current time to the previousMillis variable
    sensors.requestTemperatures();
    Termometry(DS18B20Sensor0,37,11, 0.0, LocationTemp[0]);//Bufor
    Termometry(DS18B20Sensor1,47,11, 0.0, LocationTemp[1]);//RecuCzerpnia
    Termometry(DS18B20Sensor2,48,11, 0.0, LocationTemp[2]);//RecuNawiew
    Termometry(DS18B20Sensor3,49,11, 0.0, LocationTemp[3]);//RecuWywiew
    Termometry(DS18B20Sensor4,50,11, 0.0, LocationTemp[4]);//RecuWyrzut
//  Termometry(DS18B20Sensor5,33,11, 0.0, LocationTemp[5]);//TechnicalRoom
//  Termometry(DS18B20Sensor6,32,11, 0.0, LocationTemp[6]);//Bedroom
//  Termometry(DS18B20Sensor7,34,11, 0.0, LocationTemp[7]);//Bathroom*/
//    BME280(51,SerialPrintGet);
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
        //store characters to string 
        readString += c; 
        //Serial.print(c);
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
//________________________________________________CHECKING TOUCH THE PHISICAL SWITCHES
void CheckPhisicalSwitch(){
  // ODBIERANIE SYGNAŁÓW WCIŚNIĘCIA Switch PRZEZ ARDUINO I WYSYŁANIE DO DOMOTICZA
  //__________AnalogPINs
/*  if (getAnalogButton(0)==8) { //delay(200);//Switch Bathroom
    sendGET("/json.htm?type=command&param=switchlight&idx=116&switchcmd=Toggle",SerialPrintGet); delay(50);
  }
  if (getAnalogButton(0)==8) { //delay(200);//Switch CorridorBasment
    sendGET("/json.htm?type=command&param=switchlight&idx=117&switchcmd=Toggle",SerialPrintGet); delay(50);
  }
  if (getAnalogButton(0)==8) { //delay(200);//Switch Understairs
    sendGET("/json.htm?type=command&param=switchlight&idx=118&switchcmd=Toggle",SerialPrintGet); delay(50);
  }
  if (getAnalogButton(0)==8) { //delay(200);//Switch DiningRoomHalogensWindow
    sendGET("/json.htm?type=command&param=switchlight&idx=103&switchcmd=Toggle",SerialPrintGet); delay(50); 
  } */
  //__________DigitalPINs
  //SwitchBedroomGirandole
  if (digitalRead(In14) == LOW) {
    currentSwitches[4] = millis();
    delay(50);
    if (digitalRead(In14) == LOW) {
      if (currentSwitches[4] - previousSwitches[4] > timeSwitches) {
        sendGET("/json.htm?type=command&param=switchlight&idx=113&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[4] = currentSwitches[4];
        delay(10);
      }
    }
  }
  //SwitchBedroomHalogens
  if (digitalRead(In15) == LOW) {
    currentSwitches[5] = millis();
    delay(50);
    if (digitalRead(In15) == LOW) {
      if (currentSwitches[5] - previousSwitches[5] > timeSwitches) {
        sendGET("/json.htm?type=command&param=switchlight&idx=114&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[5] = currentSwitches[5];
        delay(10);
      }
    }
  }
  //SwitchBedroomLED
  if (digitalRead(In16) == LOW) {
    currentSwitches[6] = millis();
    delay(50);
    if (digitalRead(In16) == LOW) {
      if (currentSwitches[6] - previousSwitches[6] > timeSwitches) {
        sendGET("/json.htm?type=command&param=switchlight&idx=115&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[6] = currentSwitches[6];
        delay(10);
      }
    }
  }
  //SwitchTechnicalRoom
  if (digitalRead(In17) == LOW) {
    currentSwitches[7] = millis();
    delay(50);
    if (digitalRead(In17) == LOW) {
      if (currentSwitches[7] - previousSwitches[7] > timeSwitches) {
        sendGET("/json.htm?type=command&param=switchlight&idx=120&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[7] = currentSwitches[7];
        delay(10);
      }
    }
  }
  //SwitchSoffitBack
  if (digitalRead(In18) == LOW) {
    currentSwitches[8] = millis();
    delay(50);
    if (digitalRead(In18) == LOW) {
      if (currentSwitches[8] - previousSwitches[8] > timeSwitches) {
        sendGET("/json.htm?type=command&param=switchlight&idx=108&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[8] = currentSwitches[8];
        delay(10);
      }
    }
  }
  //RolleyShutterBedroom
  if (digitalRead(In19) == LOW) {
    currentSwitches[11] = millis();
    delay(50);
    if (digitalRead(In19) == LOW) {
      if (currentSwitches[11] - previousSwitches[11] > timeSwitches) {
        Blind4243 = !Blind4243;
        if ((Blind4243 == true)) {
          sendGET("/json.htm?type=command&param=switchlight&idx=145&switchcmd=On", SerialPrintGet); delay(10);
          previousSwitches[11] = currentSwitches[11];
          delay(10);
        } else {
          sendGET("/json.htm?type=command&param=switchlight&idx=145&switchcmd=Off", SerialPrintGet); delay(10);
          previousSwitches[11] = currentSwitches[11];
          delay(10);
        }
      }
    }
  }  //SwitchBathroomGround
  if (digitalRead(In22) == LOW) {
    currentSwitches[9] = millis();
    delay(50);
    if (digitalRead(In22) == LOW) {
      if (currentSwitches[9] - previousSwitches[9] > timeSwitches) {
        sendGET("/json.htm?type=command&param=switchlight&idx=116&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[9] = currentSwitches[9];
        delay(10);
      }
    }
  }
  //SwitchCoridalGround
  if (digitalRead(In23) == LOW) {
    currentSwitches[10] = millis();
    delay(50);
    if (digitalRead(In23) == LOW) {
      if (currentSwitches[10] - previousSwitches[10] > timeSwitches) {
        sendGET("/json.htm?type=command&param=switchlight&idx=117&switchcmd=Toggle", SerialPrintGet);
        previousSwitches[10] = currentSwitches[10];
        delay(10);
      }
    }
  }
}
//_________________________________________________RECEIVING DATA FROM DOMOTICZ
void ParsujPolecenia(){
  if(readString.indexOf("Out28on") >0){//checks for on Out28 BedroomGirandole
    TurnOnLight(Out28,0); 
    Serial.println("+ TurnOn_ArduinoMega Out28");
  }
  if(readString.indexOf("Out28off") >0){
    TurnOffLight(Out28,0); 
    Serial.println("- TurnOff_ArduinoMega Out28");
  }
  
  if(readString.indexOf("Out29on") >0){//checks for on Out29 BedroomHalogens
    TurnOnLight(Out29,0); 
    Serial.println("+ TurnOn_ArduinoMega Out29");
  }
  if(readString.indexOf("Out29off") >0){
    TurnOffLight(Out29,0); 
    Serial.println("- TurnOff_ArduinoMega Out29");
  }
  
  if(readString.indexOf("Out30on") >0){//checks for on Out30 BedroomLED
    TurnOnLight(Out30,0); 
    Serial.println("+ TurnOn_ArduinoMega Out30");
  }
  if(readString.indexOf("Out30off") >0){
    TurnOffLight(Out30,0); 
    Serial.println("- TurnOff_ArduinoMega Out30");
  }
  
  if(readString.indexOf("Out31on") >0){//checks for on Out31 BathroomGround
    TurnOnLight(Out31,0); 
    Serial.println("+ TurnOn_ArduinoMega Out31");
  }
  if(readString.indexOf("Out31off") >0){
    TurnOffLight(Out31,0); 
    Serial.println("- TurnOff_ArduinoMega Out31");
  }
  
  if(readString.indexOf("Out32on") >0){//checks for on Out32 CorridorGround
    TurnOnLight(Out32,0); 
    Serial.println("+ TurnOn_ArduinoMega Out32");
  }
  if(readString.indexOf("Out32off") >0){
    TurnOffLight(Out32,0); 
    Serial.println("- TurnOff_ArduinoMega Out32");
  }

  if(readString.indexOf("Out33on") >0){//checks for on Out33 UnderStairs
    TurnOnLight(Out33,0); 
    Serial.println("+ TurnOn_ArduinoMega Out33");
  }
  if(readString.indexOf("Out33off") >0){
    TurnOffLight(Out33,0); 
    Serial.println("- TurnOff_ArduinoMega Out33");
  }
  
  if(readString.indexOf("Out34on") >0){//checks for on Out34 TechnicalRoom
    TurnOnLight(Out34,0); 
    Serial.println("+ TurnOn_ArduinoMega Out34");
  }
  if(readString.indexOf("Out34off") >0){
    TurnOffLight(Out34,0); 
    Serial.println("- TurnOff_ArduinoMega Out34");
  }

  if(readString.indexOf("Out35on") >0){//checks for on Out35 OutsideRear
    TurnOnLight(Out35,0); 
    Serial.println("+ TurnOn_ArduinoMega Out35");
  }
  if(readString.indexOf("Out35off") >0){
    TurnOffLight(Out35,0); 
    Serial.println("- TurnOff_ArduinoMega Out35");
  }

  if(readString.indexOf("Out36on") >0){//checks for on Heating01
    TurnOnLight(Out36,0); 
    Serial.println("+ TurnOn_ArduinoMega Out36");
  }
  if(readString.indexOf("Out36off") >0){
    TurnOffLight(Out36,0); 
    Serial.println("- TurnOff_ArduinoMega Out36");
  }

  if(readString.indexOf("Out37on") >0){//checks for on Heating02
    TurnOnLight(Out37,0); 
    Serial.println("+ TurnOn_ArduinoMega Out37");
  }
  if(readString.indexOf("Out37off") >0){
    TurnOffLight(Out37,0); 
    Serial.println("- TurnOff_ArduinoMega Out37");
  }
  if(readString.indexOf("version") >0){//checks for Version
    Version(166);//idx=166
    Serial.println("Wysłano dane o wersji oprogramowania");
  } 
  readString="";//clearing string for next read
}
//________________________________________________SENDING GET DATA TO THE DOMOTICZ
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
    Serial.println("Connection failed MEGA Techniczne GET");
    Serial.println();
    }
  client.stop();
}
//________________________________________________SENDING LOG DATA TO THE DOMOTICZ
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
    Serial.println("connection failed MEGA Techniczne LOG");
    Serial.println();
  }
  client.stop();
  if(SerialPrintLog==1){Serial.println("Wyslano log.");}
}
//________________________________________________TURN ON LIGHT
void TurnOnLight(int pinnr, int expanderNumber) {
  String logtxt = "";
  logtxt = "Ustawiam+stan+NISKI+na+PINie+nr:+";
  logtxt += pinnr;
  logtxt += "+Arduino+Mega+IP:+192.168.1.222";
  digitalWrite(pinnr, HIGH); delay(2);
  digitalWrite(pinnr, LOW);
  sendLOG(logtxt, SerialPrintLog);
}
//_________________________________________________TURN OFF LIGHT
void TurnOffLight(int pinnr, int expanderNumber) {
  String logtxt = "";
  logtxt = "Ustawiam+stan+WYSOKI+na+PINie+nr:+";
  logtxt += pinnr;
  logtxt += "+Arduino+Mega+IP:+192.168.1.222";
  digitalWrite(pinnr, LOW); delay(2);
  digitalWrite(pinnr, HIGH);
  sendLOG(logtxt, SerialPrintLog);
}
//_________________________________________________READING DATA FROM SENSORS DS18B20 2020-01-02
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
//_________________________________________________READING DATA FROM SENSOR BME280 2020-01-03
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
  if(Humidity>=40&&Humidity<48){return 0;}
  if(Humidity>=55&&Humidity<65){return 0;}
}
//_________________________________________________ANALOG KEYBORD READ 2020-03-15
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
//________________________________________________READING DATA FROM PHOTORESISTOR 2020-02-26
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
//________________________________________________READING DATA FROM REED SENSOR 2020-03-15
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
//________________________________________________SENDING VERSION, IP and UPLOAD DATE TO DOMOTICZ 2020-03-29
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

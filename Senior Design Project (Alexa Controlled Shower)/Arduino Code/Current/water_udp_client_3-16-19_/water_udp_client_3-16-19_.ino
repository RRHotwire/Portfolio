
//UDP water control client test

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DallasTemperature.h>
#include <Ticker.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads1115(0x48);
const char *ssid = "udp_comm";
const char *pass = "password"; 

#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);


unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress waterClientIP(192,168,4,4);
IPAddress subnet(255, 255, 255, 0);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
ESP8266WebServer server(80);

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];   //Where we get the UDP data

char result[UDP_TX_PACKET_MAX_SIZE];
bool breakCommand = true;

//Pins for Nodemcu (ESP8266)
int enableCold = D0; //ENA 
int in1_cold = D1;   //IN1
int in2_cold = D2;   //IN2

float targetTemp;
//Ball valve for hot water
int enableHot = D8; //ENB
int in1_hot = D6; //IN3
int in2_hot = D7; //IN4

#define turnTime 800 //determines how long a valve turns
#define maxTurnTime 5000  //Assuming 5 seconds to close the valve fully


float cold_adc1, cold_adc2, hot_adc1, hot_adc2;
float cold_vin1 ,cold_vin2, hot_vin1, hot_vin2;

#define OPEN 2
#define PARTIAL 1
#define CLOSED 0
String statusString [] = {"CLOSED", "PARTIAL", "OPEN"};

int hotStatus, coldStatus;


void turnOffHot(){
  digitalWrite(in1_hot, LOW);
  digitalWrite(in2_hot, LOW);
}
void turnOffCold(){
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, LOW);

}
void turnOffValves(){
  turnOffHot();
  turnOffCold();
}
void enableValvePins(){
  pinMode(enableCold, OUTPUT);
  pinMode(enableHot, OUTPUT);
  pinMode(in1_cold, OUTPUT);
  pinMode(in2_cold, OUTPUT);
  pinMode(in1_hot, OUTPUT);
  pinMode(in2_hot, OUTPUT);
}

void fullCloseValves(){
  digitalWrite(in1_cold, HIGH);
  digitalWrite(in2_cold, LOW);
  analogWrite(enableCold, 256);
  
  digitalWrite(in1_hot, LOW);
  digitalWrite(in2_hot, HIGH);
  analogWrite(enableHot, 256);
  delay(maxTurnTime);
}

void OpenCold(){
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, HIGH);
  analogWrite(enableCold, 256);
}
void CloseCold(){
  digitalWrite(in1_cold, HIGH);
  digitalWrite(in2_cold, LOW);
  analogWrite(enableCold, 256);
}
void OpenHot(){
  digitalWrite(in1_hot, HIGH);
  digitalWrite(in2_hot, LOW);
  analogWrite(enableHot, 256);
}
void CloseHot(){
  digitalWrite(in1_hot, LOW);
  digitalWrite(in2_hot, HIGH);
  analogWrite(enableHot, 256);
}

float currentTemp = 0;

void sendMsg(String msg){
  Serial.println("Sending message: " + msg);
  udp.beginPacket(ServerIP, 2000);
  udp.print(msg + "");  
  udp.endPacket();
}
void sendTempUpdate(){
  //Serial.println("Sending temp update");
  String temp = String(currentTemp);
  udp.beginPacket(ServerIP, 2000);
  udp.print("tempReading="+ temp);  
  udp.endPacket();
}

String msgRcv = "";
boolean isFloat(String tString) {
  String tBuf;
  boolean decPt = false;
  
  if(tString.charAt(0) == '+' || tString.charAt(0) == '-') tBuf = &tString[1];
  else tBuf = tString;  

  for(int x=0;x<tBuf.length();x++)
  {
    if(tBuf.charAt(x) == '.') {
      if(decPt) return false;
      else decPt = true;  
    }    
    else if(tBuf.charAt(x) < '0' || tBuf.charAt(x) > '9') return false;
  }
  return true;
}

double currentValveTimer = 0;

String valveHandler = "";

void recvMessageHandler(String msg){
  Serial.println("Recieved... ");
     if(msg == "TOW"){
      Serial.println("Command: Closing Valves");
      sendMsg("Water Client recieved command: Turn off Water");
      breakCommand = true;
      fullCloseValves();
      valveHandler = "";
     }
     else if(msg =="W"){
        Serial.println("Command: Warmer Water");
        sendMsg("Water Client recieved command: Warmer");
        breakCommand = false;
        currentValveTimer = millis();
        valveHandler = "Warmer";
     }
     else if(msg=="C"){
      Serial.println("Command: Colder Water");
      sendMsg("Water Client recieved command: Colder");
      breakCommand = false;
      currentValveTimer = millis();
      valveHandler = "Colder";
     }
     else if(isFloat(msg)) //set temperature target for 
     {
      Serial.println("Command: Setting temperature to " + msg + " F");
      sendMsg("Water Client recieved command: Set temperature to " + msg);
      valveHandler = "Auto";
      targetTemp= msg.toFloat();
     }
   
}

void setup() {
  
  Wire.begin(D3, D4);
  ads1115.begin();
  ads1115.setGain(GAIN_TWO); 
  Serial.begin(115200);
  WiFi.begin(ssid, pass);   //Connect to access point
    WiFi.mode(WIFI_STA);
    WiFi.config(waterClientIP, ServerIP, subnet);
  
    Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    
    //Start UDP
    Serial.println("Starting UDP");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
  enableValvePins();
  fullCloseValves();
}
double tempTimer = 0;
double prevTime = 0;
int interval = 1000;
String command = "";


double autoTimer = 0;

float tempCompare = 0;

bool optimize = true;
bool checkTemp = true;
bool waterFlows = false;

void getTemp(){
     sensors.requestTemperatures();
     currentTemp = sensors.getTempFByIndex(0);
}
void autoTemp(){
  if(hotStatus == CLOSED && coldStatus == CLOSED && waterFlows == false){
    getTemp();
    delay(500);
    tempCompare = currentTemp;
    OpenHot();
    OpenCold();
    delay(200);
    turnOffCold();turnOffHot();
    getTemp();
    delay(1000);
    if((tempCompare < currentTemp*1.05) && (tempCompare > currentTemp*.95)){
          Serial.println("There is no water flowing!");
          delay(100);
          valveHandler = "";
          sendMsg("Water Client: No water available");
          waterFlows = false;
          delay(100);
          fullCloseValves();
        }
        else
        waterFlows = true;

  }
  else{    
  
  if(currentTemp != targetTemp){
    if(checkTemp == true){        
        
      
     if(currentTemp < targetTemp && hotStatus != OPEN){
        OpenHot();
        delay(100);
        getTemp();
        turnOffHot();

      }
      else if(currentTemp < targetTemp && coldStatus != CLOSED){
        CloseCold();
        delay(100);
        getTemp();
        turnOffCold();

      }
      else if(currentTemp > targetTemp && coldStatus != OPEN){
        OpenCold();
        delay(100);
        turnOffCold();
        getTemp();
      }
      else if(currentTemp > targetTemp && hotStatus != CLOSED){
        CloseHot();
        delay(100);
        turnOffHot();
        getTemp();
        
      }
      else{
        
      }
  }
  /*else if(optimize == true){ //get optimal water flow
    if(hotStatus != OPEN && coldStatus != OPEN){
      OpenHot();
      OpenCold();
      delay(10);
      checkTemp = true;
      turnOffCold();
      turnOffHot();
    }  */
  }
  delay(500);
  turnOffCold();turnOffHot();
  }
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  double currentTime = millis();
  //A0 open cold
  //A1 closed cold
  cold_adc1 = ads1115.readADC_SingleEnded(0); // high if open
  cold_adc2 = ads1115.readADC_SingleEnded(1); // high if closed
  cold_vin1 = (cold_adc1 * 0.1875)/1000;
  cold_vin2 = (cold_adc2 * 0.1875)/1000;
  
  if(cold_vin1 <= 7.0 && cold_vin1 >= 0.5){
    coldStatus = OPEN;
  }
  else if(cold_vin2 <= 7.0 && cold_vin2 >= 0.5){
    coldStatus = CLOSED;
  }
  else{
    coldStatus = PARTIAL;
  }
  //A2 open hot
  //A3 closed hot
  hot_adc1 = ads1115.readADC_SingleEnded(2); // high if open
  hot_adc2 = ads1115.readADC_SingleEnded(3); // high if closed
  
  hot_vin1 = (hot_adc1 * 0.1875)/1000;
  hot_vin2 = (hot_adc2 * 0.1875)/1000;
  
  if(hot_vin1 <= 7.0 && hot_vin1 >= 0.5){
    hotStatus = OPEN;
  }
  else if(hot_vin2 <= 7.0 && hot_vin2 >= 0.5){
    hotStatus = CLOSED;
  }
  else {
    hotStatus = PARTIAL;
  }
  
  if(currentTime - prevTime >= 500){
        Serial.println("Hot Valve Status: " + statusString[hotStatus]);

    Serial.println("hot_vin1: " + String(hot_vin1));
    Serial.println("hot_vin2: " + String(hot_vin2));
        Serial.println("Cold Valve Status: " + statusString[coldStatus]);

    Serial.println("cold_vin1: " + String(cold_vin1));
    Serial.println("cold_vin2: " + String(cold_vin2));
    if(coldStatus != CLOSED || hotStatus != CLOSED){
    getTemp();
    //Serial.print("Temperature is: "); 
    //Serial.print(currentTemp);
    sendTempUpdate();     
    }
    else{
      currentTemp = -99;
      sendTempUpdate();
    }
  }

  Serial.println("");
  delay(100);
  int cb = udp.parsePacket();
    if (!cb) 
    {
      //If serial data is recived send it to UDP
      if(Serial.available()>0)
        {
        udp.beginPacket(ServerIP, 2000);  //Send Data to Master unit
        //Send UDP requests are to port 2000
        
        char a[1];
        a[0]=char(Serial.read()); //Serial Byte Read
        udp.write(a,1); //Send one byte to ESP8266 
        udp.endPacket();
        }
    }
    else {
      // We've received a UDP packet, send it to serial
      for(int i = 0; i<UDP_TX_PACKET_MAX_SIZE; i++) packetBuffer[i] = 0;
      String recvMessage;
      udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); // read the packet into the buffer, we are reading only one byte
      recvMessage=packetBuffer;
      recvMessageHandler(recvMessage);
      delay(20);
    }
    delay(100);
    if(valveHandler == "Colder")
   {
    if(coldStatus != OPEN){
      if((currentTime - currentValveTimer) >= turnTime){
        valveHandler = "";
        turnOffCold();
      }
      else{
      Serial.println("Opening cold");
        OpenCold();
      }
    }
    else if(hotStatus != CLOSED){
      if((currentTime - currentValveTimer) >= turnTime){
        valveHandler = "";
        turnOffHot();
      }
      else{
      Serial.println("Closing hot");
        CloseHot();
      }
    }
    else{
      valveHandler = "";
    }
   }
   
   else if(valveHandler == "Warmer")
   {
    if(hotStatus != OPEN){
      if((currentTime - currentValveTimer) >= turnTime){
        valveHandler = "";
        turnOffHot();
      }
      else{
        Serial.println("Opening hot");
        OpenHot();
      }
    }
    else if(coldStatus != CLOSED){
      if((currentTime - currentValveTimer) >= turnTime){
        valveHandler = "";
        turnOffCold();
      }
      else{
      Serial.println("Closing cold");
        CloseCold();
      }
    }
    else{
      valveHandler = "";
    }
     
   }
   else if(valveHandler == "Auto"){
    Serial.println("Turning automatic waterflow");
    autoTemp();
   }
   
    
}

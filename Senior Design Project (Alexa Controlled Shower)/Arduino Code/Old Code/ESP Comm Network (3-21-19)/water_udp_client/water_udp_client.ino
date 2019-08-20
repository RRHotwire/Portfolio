//UDP water control client

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
const char *ssid = "udp_comm";
const char *pass = "password"; 

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
int enableHot = D5; //ENB
int in1_hot = D6; //IN3
int in2_hot = D7; //IN4

/*Pins for Arduino Uno
 * 
//Ball valve for cold water
int enableCold = 10; //ENA 
int in1_cold = 9;   //IN1
int in2_cold = 8;   //IN2

float targetTemp;
//Ball valve for hot water
int enableHot = 5; //ENB
int in1_hot = 7; //IN3
int in2_hot = 6; //IN4
*/

//
int coldTime = 0;
int hotTime = 0;

#define turnTime 500 //determines how long a valve turns
#define maxTurnTime 5000  //Assuming 5 seconds to close the valve fully
const int ThermistorPin = A0; //Analog pin on ESP 8266 (nodemcu)
//#define ThermistorPin 0   //Using Analog Pin 0 on the arduino uno to read the sensor value

//======================================================================
//                Incoming Message Functions
//======================================================================
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
//======================================================================
//                Thermal Reading Functions
//======================================================================
float Vin=3.3;     // [V]        
float Rt=5000;    // Resistor t [ohm]
float R0=5000;    // value of rct in T0 [ohm]
float T0=298.15;   // use T0 in Kelvin [K]
float Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0
// use the datasheet to get this data.
float T1=273.15;      // [K] in datasheet 0Âº C
float T2=373.15;      // [K] in datasheet 100Â° C
float RT1=16330;   // [ohms]  resistence in T1
float RT2=339.9;    // [ohms]   resistence in T2
float beta=0.0;    // initial parameters [K]
float Rinf=0.0;    // initial parameters [ohm]   
float TempK=0.0;   // variable output
float TempC=0.0;   // variable output
float TempF=0.0;

float currentTemp = 0;
Ticker tempTicker;

void checkTemp(){
  
  Vout=Vin*((float)(analogRead(ThermistorPin))/1024.0); // calc for ntc
  Rout=(Rt*Vout/(Vin-Vout));
  //Serial.println(Rout);
  TempK=(beta/log(Rout/Rinf)); // calc for temperature
  //Serial.println(TempK);
  TempC=TempK-273.15;
  TempF=(TempC *9/5) + 32;
  currentTemp = TempF;
}
void sendMsg(String msg){
  Serial.println("Sending message: " + msg);
  udp.beginPacket(ServerIP, 2000);
  udp.print(msg + "");  
  udp.endPacket();
}
void sendTempUpdate(){
  checkTemp();
  Serial.println("Sending temp update");
  String temp = String(currentTemp);
  udp.beginPacket(ServerIP, 2000);
  udp.print("tempReading="+ temp);  
  udp.endPacket();
}
//======================================================================
//                Setup
//======================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println();
    beta=(log(RT1/RT2))/((1/T1)-(1/T2));
    Rinf=R0*exp(-beta/T0);
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

//****************************
  tempTicker.attach(.25, sendTempUpdate);
  enableValvePins();

}
//======================================================================
//                Valve Control Functions
//======================================================================


void autoTempReach(){
  if(!breakCommand)
  if(currentTemp != targetTemp){
    if(currentTemp < targetTemp){
      if(hotTime < maxTurnTime/2 && currentTemp != targetTemp){
        OpenHot();
        }
      else{
        if(coldTime > maxTurnTime/2 && currentTemp != targetTemp){
          CloseCold();
          } 
        }
    }
    
    else if(currentTemp > targetTemp){
      if(hotTime > maxTurnTime/2 && currentTemp != targetTemp){
        CloseHot();
        }
      else{
        if(coldTime < maxTurnTime/2 && currentTemp != targetTemp){
          OpenCold();
          } 
        }
    } 
  }
}
void turnOffValves(){
  turnOffHot();
  turnOffCold();
}
void turnOffHot(){
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, LOW);  
}
void turnOffCold(){
   digitalWrite(in1_hot, LOW);
  digitalWrite(in2_hot, LOW);
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
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, HIGH);
  analogWrite(enableCold, 200);
  
  digitalWrite(in1_hot, HIGH);
  digitalWrite(in2_hot, LOW);
  analogWrite(enableHot, 200);

  delay(maxTurnTime);
}

void OpenCold(){
  if(coldTime < maxTurnTime){
  digitalWrite(in1_cold, HIGH);
  digitalWrite(in2_cold, LOW);
  analogWrite(enableCold, 200);
  delay(turnTime);
  coldTime += turnTime; 
  }
}
void CloseCold(){
  if(coldTime > 0){
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, HIGH);
  analogWrite(enableCold, 200);
  delay(turnTime);
  coldTime -= turnTime; 
  }
}
void OpenHot(){
  if(hotTime < maxTurnTime){
  digitalWrite(in1_hot, HIGH);
  digitalWrite(in2_hot, LOW);
  analogWrite(enableHot, 200);
  delay(turnTime);
  hotTime += turnTime;
  }
  
}
void CloseHot(){
  if(hotTime > 0){
  digitalWrite(in1_hot, LOW);
  digitalWrite(in2_hot, HIGH);
  analogWrite(enableHot, 200);
  delay(turnTime);
  hotTime -= turnTime; 
  }
}
//======================================================================
//                UDP Message Functions
//======================================================================
void msgSender(String msg){
  udp.beginPacket(ServerIP, 2000);
  udp.print(msg);
  udp.endPacket();
}

void recvMessageHandler(String msg){
  Serial.println("Recieved... ");
   if(msg =="Open Cold"){
    Serial.println("Command: Opening Cold Water");
    sendMsg("Water Client recieved command: Open Cold Valve");
    breakCommand = true;
    OpenCold();
    turnOffCold();
   }
   else if(msg == "Close Cold"){
    Serial.println("Command: Closing Cold Water");
    sendMsg("Water Client recieved command: Close Cold Valve");
    breakCommand = true;
    CloseCold();
    }
   else if(msg == "Open Hot"){
    Serial.println("Command: Opening Hot Water");
    breakCommand = true;
    OpenHot();
    sendMsg("Water Client recieved command: Open Hot Valve");
   }
   else if(msg == "Close Hot"){
    Serial.println("Command: Closing Hot Water");
    breakCommand = true;
    CloseHot();
    sendMsg("Water Client recieved command: Close Hot Valve");
   }
   else if(msg == "Turn off Water"){
    Serial.println("Command: Closing Valves");
    sendMsg("Water Client recieved command: Turn off Water");
    breakCommand = true;
    fullCloseValves();
    sendMsg("Water Client recieved command: Turn off Water");
   }

   else if(isFloat(msg)) //set temperature target for 
   {
    Serial.println("Command: Setting temperature to " + msg + " F");
    breakCommand = false;
    targetTemp= msg.toFloat();
    sendMsg("Water Client recieved command: Set temperature to " + msg);
   }
   
}
//======================================================================
//                MAIN LOOP
//======================================================================
void loop()
{
    
    autoTempReach();
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
}

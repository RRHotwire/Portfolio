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

char result[15];
bool breakCommand = false;
//Ball valve for cold water
int enableCold = 10; //ENA 
int in1_cold = 9;   //IN1
int in2_cold = 8;   //IN2

float targetTemp;
//Ball valve for hot water
int enableHot = 5; //ENB
int in1_hot = 7; //IN3
int in2_hot = 6; //IN4

//
int coldTime = 0;
int hotTime = 0;

#define turnTime 100 //determines how long a valve turns
#define maxTurnTime 5000  //Assuming 5 seconds to close the valve fully
#define ThermistorPin 0   //Using Analog Pin 0 on the arduino to read the sensor value

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

void checkMsg(){
  if(msgRcv=="Open Cold"){
    breakCommand = true;
    Serial.println("Opening cold water");
  }
  else if(msgRcv=="Close Cold"){
    breakCommand = true;
    Serial.println("Closing cold water");
  }
  else if(msgRcv=="Open Hot"){
    breakCommand = true;
    Serial.println("Opening hot water");
  }
  else if(msgRcv=="Close Hot"){
    breakCommand = true;
    Serial.println("Closing hot water");
  }
  else if(isFloat(msgRcv)){
    breakCommand = false;
    targetTemp= msgRcv.toFloat();
    autoTempReach();
    Serial.println("Setting temperature to: " + msgRcv + " F");
  }
  else
    Serial.println(msgRcv);         // print the character
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

  TempK=(beta/log(Rout/Rinf)); // calc for temperature
  TempC=TempK-273.15;
  TempF=(TempC *9/5) + 32;
  currentTemp = TempF;
}
void sendMsg(String msg){
  udp.beginPacket(ServerIP, 2000);
  udp.print(msg);  
  udp.endPacket();
}
void sendTempUpdate(){
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
    Serial.println("Opening Cold Water");
    sendMsg("Water Client recieved command: Open Cold Valve");
    breakCommand = true;
    OpenCold();
    turnOffCold();
   }
   else if(msg == "Close Cold"){
    Serial.println("Closing Cold Water");
    sendMsg("Water Client recieved command: Close Cold Valve");

    breakCommand = true;
    CloseCold();
    turnOffCold();
    }
   else if(msg == "Open Hot"){
    Serial.println("Opening Hot Water");
    sendMsg("Water Client recieved command: Open Hot Valve");

    breakCommand = true;
    OpenHot();
    turnOffHot();
   }
   else if(msg == "Close Hot"){
    Serial.println("Closing Hot Water");
    sendMsg("Water Client recieved command: Close Hot Valve");
    breakCommand = true;
    CloseHot();
    turnOffHot();
   }
   else if(msg == "Turn off Water"){
    Serial.println("Closing Valves");
    sendMsg("Water Client recieved command: Turn off Water");
    breakCommand = true;
    fullCloseValves();
    turnOffValves();
   }

   else if(isFloat(msg)) //set temperature target for 
   {
    Serial.println("Setting temperature to: " + msgRcv + " F");
    breakCommand = false;
    targetTemp= msgRcv.toFloat();
    sendMsg("Water Client recieved command: Set temperature to " + msgRcv);
   }
   
}
//======================================================================
//                MAIN LOOP
//======================================================================
String recvMessage;
void loop()
{
    checkTemp();
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
      udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); // read the packet into the buffer, we are reading only one byte
      recvMessage=packetBuffer;
      recvMessageHandler(recvMessage);
      delay(20);
    }
}

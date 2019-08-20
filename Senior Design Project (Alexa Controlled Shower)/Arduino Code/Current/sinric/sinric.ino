
//esp udp server 
#include <ESP8266WiFi.h>
#include "debug.h"           
#include "WifiConnection.h"    
#include "Wemulator.h"          
#include "WemoCallbackHandler.h" 
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Wire.h>

#define WIRE_SDA D1
#define WIRE_SCL D2


const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;

int16_t slave_address = I2C_SLAVE; // may be overridden using scan()

const char *ssid_wifi = "Pixel_5448";
const char *pass_wifi = "12345678"; 

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress waterClientIP(192,168,4,4);
IPAddress trackClientIP(192,168,4,3);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
ESP8266WebServer server(80);

WifiConnection * wifi;
Wemulator* shower;

bool cold_commandReceived = false;
bool warm_commandReceived = false;
bool hot_commandReceived = false;

bool headF_commandReceived = false;
bool headB_commandReceived = false;


bool forward_commandReceived = false;
bool center_commandReceived = false;
bool backward_commandReceived = false;

bool colder_commandReceived = false;
bool warmer_commandReceived = false;
bool water_commandReceived = false;


void receiveEvent(unsigned int howMany) {
    Serial.print("Received "); Serial.print(howMany); Serial.println(" bytes...");
  String data = "";
  while( Wire.available()){
    data += (char)Wire.read();
  }
  Serial.println("Message received: " + data);
}


void sendMessage(String msg) {
  
  Serial.print("[MASTER] Sending message: ");
  Serial.print(msg);  
  Wire.beginTransmission(I2C_SLAVE); // transmit to device 0x08
  //Sending Side


    Wire.print(msg);

  Wire.endTransmission();    // stop transmitting
  Serial.println("");
  
}


void handleRoot() { 
  
  //server.send(200, "text/html", MAIN_page);
}


//=======================================================================
//                Setup
//=======================================================================

void setup()
{
     Serial.begin(115200);
    Serial.println("");
      Wire.pins(WIRE_SDA, WIRE_SCL);
  Wire.begin(I2C_MASTER); // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent);
  
      server.on("/", handleRoot);
    //WiFi.softAP(ssid, pass);    //Create Access point
    
    //server.begin();
    //Start UDP
    Serial.println("Starting UDP");
    delayMicroseconds(1000); 
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
    
   // server.on("/readADC", handleADC);
    
    server.on("/", handleRoot);
  server.on("/Forward", [](){
    Serial.println("Sending command: Forward");
    delayMicroseconds(1000); 
//    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("F"); //Send one byte to ESP8266
    udp.endPacket();
    delayMicroseconds(1000);
  });

    server.on("/Center", [](){
    Serial.println("Sending command: Center");
    delayMicroseconds(1000); 
   // server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("C"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);  
  });
  server.on("/Warmer", [](){
    Serial.println("Sending command: Warm the Water");
    delayMicroseconds(1000); 
 //   server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("W"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);    
  });
  server.on("/Colder", [](){
    Serial.println("Sending command: Cool the Water");
    delayMicroseconds(1000); 
 //   server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("C"); //Send one byte to ESP8266 
    udp.endPacket();   
    delayMicroseconds(1000);  
  });

//  server.on("/save", handleSave);

  server.on("/Backward", [](){
    Serial.println("Sending command: Backward");
    delayMicroseconds(1000); 
 //   server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("B"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });
  
  server.on("/Head_Forward", [](){
    Serial.println("Sending command: Moving Head Forward");
    delayMicroseconds(1000); 
 //   server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("HF"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });

  server.on("/Turn_off_water", [](){
    Serial.println("Sending command: Turning Off Water");
    delayMicroseconds(1000); 
  //  server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("TOW"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });
  
  server.on("/Head_Backward", [](){
    Serial.println("Sending command: Moving Head Backward");
    delayMicroseconds(1000); 
//    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("HB"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });
  wifi = new WifiConnection(ssid_wifi,pass_wifi);
  wifi->begin();

  
    shower = new Wemulator(); 

  if (wifi->connect())
    { 

        /* Phrases and devices associated to it.
         * 
         */
        shower->begin(); 
        //start the wemo emulator (it runs as a series of webservers) 
        //Serial.println("adding devices");

        //device name, a port number that is different from the rest, and a bool variable to recieve the signal that the word was recieved by Alexa
        shower->addDevice("Cold", 80, new WemoCallbackHandler(&cold_commandReceived));
        delay(1);
        shower->addDevice("Warm", 81, new WemoCallbackHandler(&warm_commandReceived));
        delay(1);

        shower->addDevice("Colder", 89, new WemoCallbackHandler(&colder_commandReceived));
        delay(1);
        shower->addDevice("Warmer", 90, new WemoCallbackHandler(&warmer_commandReceived));
        delay(1);
        shower->addDevice("Hot", 91, new WemoCallbackHandler(&hot_commandReceived));
        delay(1);
        
        
        shower->addDevice("Forward", 82, new WemoCallbackHandler(&forward_commandReceived));
        delay(1);
        shower->addDevice("Backward", 83, new WemoCallbackHandler(&backward_commandReceived));
        delay(1);
        shower->addDevice("Center", 84, new WemoCallbackHandler(&center_commandReceived));
        delay(1);

        
        shower->addDevice("Water", 85, new WemoCallbackHandler(&water_commandReceived));
        delay(1);
        shower->addDevice("Off", 86, new WemoCallbackHandler(&water_commandReceived));
        delay(1);
        
        shower->addDevice("Head Forward", 87, new WemoCallbackHandler(&headF_commandReceived));
        delay(1);
        shower->addDevice("Head Backward", 88, new WemoCallbackHandler(&headB_commandReceived));
        delay(1);
        
        
        //Serial.println("finished adding");

    }
    delay(1);
 
}
//======================================================================
//                MAIN LOOP
//======================================================================


void loop()
{
   // if (wifi->isConnected)
    //{
        //Serial.println("Connected");
        //Serial.println("");
        
        shower->listen(); 
        //delay(1000);
    //}
    //delay(1);

    //Checks for confirmation of signal of Alexa from from shower->listen, then writes via I2C to the server.
    if(colder_commandReceived){
      colder_commandReceived = false;
      Serial.println("Sending command: Cool the Water");
      sendMessage("C"); //Send one byte to ESP8266 
      delayMicroseconds(1000); 
    }
    else if(warmer_commandReceived){
      warmer_commandReceived=false;
      Serial.println("Sending command: Warm the Water");
      delayMicroseconds(1000); 
      sendMessage("W"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
    else if(warm_commandReceived){
      warm_commandReceived=false;
      Serial.println("Sending command: Set target temperature to warm");
      sendMessage("warm"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
    else if(cold_commandReceived){
      cold_commandReceived=false;
      Serial.println("Sending command: Set target temperature to cold");
      sendMessage("cold"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
    else if(hot_commandReceived){
      hot_commandReceived=false;
      Serial.println("Sending command: Set target temperature to hot");
      sendMessage("hot"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
    else if(forward_commandReceived){
      forward_commandReceived=false;
      Serial.println("Sending command: Forward");
      sendMessage("Forward"); //Send one byte to ESP8266
      delayMicroseconds(1000);
    }
    else if(center_commandReceived){
      center_commandReceived=false;
      Serial.println("Sending command: Center");
      sendMessage("Center"); //Send one byte to ESP8266 
      delayMicroseconds(1000); 
    }
    else if(backward_commandReceived){
      backward_commandReceived=false;
      Serial.println("Sending command: Backward");
      sendMessage("Backward"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
    else if(water_commandReceived){
      water_commandReceived=false;
      Serial.println("Sending command: Turning Off Water");
      sendMessage("TOW"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
    else if(headF_commandReceived){
      headF_commandReceived=false;
      Serial.println("Sending command: Moving Head Forward");
      sendMessage("HF"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
    else if(headB_commandReceived){
      headB_commandReceived=false;
      Serial.println("Sending command: Moving Head Forward");
      sendMessage("HB"); //Send one byte to ESP8266 
      delayMicroseconds(1000);
    }
}

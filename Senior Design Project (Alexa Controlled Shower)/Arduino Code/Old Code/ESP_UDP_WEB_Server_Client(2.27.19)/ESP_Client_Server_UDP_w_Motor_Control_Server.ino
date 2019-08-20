#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
const char *ssid = "udp_comm";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress ClientIP(192,168,4,3);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
ESP8266WebServer server(80);

char packetBuffer[9];   //Where we get the UDP data
String webPage = "<html> <head> <style> button {   background-color: #000000;  padding: 12px 28px;   width: 100%;  height: 33%;  border-radius: 8px;   border: 0px;  font-size: 20px;  color: white;   } h1 {  text-align: center;   }</style> </head> <body> <h1>ALS Shower Control</h1>  <p><button onmousedown=location.href=\"/Forward\">Forward</button></p> <p><button onmousedown=location.href=\"/Backward\">Backward</button></p><p><button onmousedown=location.href=\"/Center\">Center</button></p> </body> </html>";
void handleRoot () {
server.send(200, "text/html", webPage);}

//=======================================================================
//                Setup
//=======================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println();
      server.on("/", handleRoot);
    WiFi.softAP(ssid, pass);    //Create Access point
    server.begin();
    //Start UDP
    Serial.println("Starting UDP");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());

    server.on("/", handleRoot);
  server.on("/Forward", [](){
    server.send(200, "text/html", webPage);
    udp.beginPacket(ClientIP, 2000);
    udp.print("Forward"); //Send one byte to ESP8266
    udp.endPacket();  
  });

    server.on("/Center", [](){
    server.send(200, "text/html", webPage);
    udp.beginPacket(ClientIP, 2000);
    udp.print("Center"); //Send one byte to ESP8266 
    udp.endPacket();   
    
  });
  
  server.on("/Backward", [](){
    server.send(200, "text/html", webPage);
    udp.beginPacket(ClientIP, 2000);
    udp.print("Backward"); //Send one byte to ESP8266 
    udp.endPacket(); 
  });
}
//======================================================================
//                MAIN LOOP
//======================================================================
void loop()
{
//    WiFi client = server.available();
    server.handleClient();


    int cb = udp.parsePacket();
    if (!cb) 
    {
      //If serial data is recived send it to UDP
      if(Serial.available()>0)
        {
        udp.beginPacket(ClientIP, 2000);
        //Send UDP requests are to port 2000
        
        char a[1];
        a[0]=char(Serial.read()); //Serial Byte Read
        udp.write(a,1); //Send one byte to ESP8266 
        udp.endPacket();
        }
    }
    else {
      // We've received a UDP packet, send it to serial
      udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); // read the packet into the buffer, we are reading only one byte
      Serial.print(packetBuffer);
      delay(20);
    }
}

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
const char *ssid = "udp_comm";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress ClientIP(192,168,4,2);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
ESP8266WebServer server(80);

char packetBuffer[9];   //Where we get the UDP data
String data = "test";
String webPage = "<html> <head> <style> button {   background-color: #000000;  padding: 12px 28px;   width: 100%;  height: 33%;  border-radius: 8px;   border: 0px;  font-size: 20px;  color: white;   } h1 {  text-align: center;   }</style> </head> <body> <h1>ALS Shower Control</h1>  <p><button onmousedown=location.href=\"/Forward\">Forward</button></p> <p><button onmousedown=location.href=\"/Backward\">Backward</button></p><p><button onmousedown=location.href=\"/Center\">Center</button></p> </body> </html>";
void handleRoot () {
server.send(200, "text/html", webPage);}
int i = 0;
int pos = 0;
int steps = 0; 
const int stepping = 0; 
const int dir = 2;

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
    if (pos < 4)
   {
   Go_Forward(3100);
   pos++;
   }
   else 
   {
   Go_Forward(0); 
   }
  });

    server.on("/Center", [](){
    server.send(200, "text/html", webPage);

   if (pos == 0)
   {
    Go_Forward(6200);
    pos = 2;
   }
   else if (pos == 1) 
   {
   Go_Forward(3100); 
   pos = 2; 
   }

  else if (pos == 3) 
   {
   Go_Backward(3100); 
   pos = 2; 
   }
    
else if (pos == 4) 
   {
   Go_Backward(6200); 
   pos = 2; 
   }        
  }

  
  );
  
  server.on("/Backward", [](){
    server.send(200, "text/html", webPage);
    if (pos > 0)
    {
   Go_Backward(3100);
   pos--;
    }
    else {
    Go_Backward(0);  }
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
      udp.read(packetBuffer, 15); // read the packet into the buffer, we are reading only one byte
      Serial.print(packetBuffer);
      delay(20);
    }
}
//======================================================================
void Go_Forward (int steps)
{
digitalWrite(dir, HIGH); //Rotate stepper motor in clock wise direction //change to LOW when on slider

          for( i=1;i<=steps;i++){
          digitalWrite(stepping, HIGH);
     
          digitalWrite(stepping, LOW);
          delay(1);}  
}

void Go_Backward(int steps)
{
digitalWrite(dir, LOW); //Rotate stepper motor in anti clock wise direction ////change to HIGH when on slider

          for( i=1;i<=steps;i++){
          digitalWrite(stepping, HIGH);
        
          digitalWrite(stepping, LOW);
          delay(1);}
}
void Restart (int pos)
{
  int currentPosition = pos;

  if (currentPosition == 0)
  {
    
  }

  else if (currentPosition == 1)
  {
    Go_Backward(3100);
    pos = 0;
  }

  else if (currentPosition == 2)
  {
    Go_Backward(6200);
    pos = 0;
  }

  else if (currentPosition == 3)
  {
    Go_Backward(9300);
    pos = 0;
  }

  else if (currentPosition == 4)
  {
    Go_Backward(12400);
    pos = 0;
  }
}

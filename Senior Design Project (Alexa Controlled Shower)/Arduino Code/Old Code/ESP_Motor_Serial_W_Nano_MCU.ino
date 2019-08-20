#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
SoftwareSerial NodeMCU(14,12);

const char* ssid = "ALS"; 
const char* password = "James";

ESP8266WebServer server(80);
int i = 0;
int pos = 0;
int steps = 0; 
//const int enable = 16; /*D0*/
const int stepping = 0; 
const int dir = 2; 
const int m3 = 5; /*D1*/
const int m2 = 4; /*D2*/
const int m1 = 14; /*D5*/
//const int StepperSpeed = 1;
//const int sliderWidth = 12400; //4' slider
//int direct = 1;
//int sliderWidthMulti = 0;
String webPage = "<html> <head> <style> button {   background-color: #000000;  padding: 12px 28px;   width: 100%;  height: 33%;  border-radius: 8px;   border: 0px;  font-size: 20px;  color: white;   } h1 {  text-align: center;   }</style> </head> <body> <h1>ALS Shower Control</h1>  <p><button onmousedown=location.href=\"/Forward\">Forward</button></p> <p><button onmousedown=location.href=\"/Backward\">Backward</button></p><p><button onmousedown=location.href=\"/Center\">Center</button></p> </body> </html>";
String Error_1 = "Has reached end\n\n";
String Error_2 = "Has reached beginning\n\n";
void handleRoot() {
  server.send(200, "text/html", webPage);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t x=0; x<server.args(); x++){
    message += " " + server.argName(x) + ": " + server.arg(x) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
 Serial.begin(115200);
 NodeMCU.begin(4800);
  pinMode(stepping, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(14,INPUT);
  pinMode(12,OUTPUT);
  

  WiFi.softAP(ssid, password);
  
  
  
  IPAddress apip = WiFi.softAPIP();
  Serial.println("apip");

  
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
   NodeMCU.write(HIGH);
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
  });
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
  
  

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient(); 
   NodeMCU.write(HIGH);
}

void Go_Forward (int steps)
{
digitalWrite(dir, HIGH); //Rotate stepper motor in clock wise direction
// if (pos < 4)
// {
          for( i=1;i<=steps;i++){
          digitalWrite(stepping, HIGH);
        //  delay(10);
          digitalWrite(stepping, LOW);
          delay(1);} 
         // pos++;
 //}
// //else 
// {
//  server.send(404, "text/plain", Error_1);
// } 
// return 0;  
}

void Go_Backward(int steps)
{
digitalWrite(dir, LOW); //Rotate stepper motor in anti clock wise direction
//if (pos > 0)
//{
          for( i=1;i<=steps;i++){
          digitalWrite(stepping, HIGH);
         // delay(10);
          digitalWrite(stepping, LOW);
          delay(1);}
         // pos --;
//}
//else 
//{
// server.send(404, "text/plain", Error_2);  
//}
////return 0;   
}

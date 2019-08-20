#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
/* Set these to your desired credentials. */
const char *ssid = "ALS Shower Project";
const char *password = "GROUPG";

ESP8266WebServer server(80);

int i = 0;
int pos = 0;
int steps = 0; 
const int stepping = 0; 
const int dir = 2; 


String webPage = "<html> <head> <style> button {   background-color: #000000;  padding: 12px 28px;   width: 100%;  height: 33%;  border-radius: 8px;   border: 0px;  font-size: 20px;  color: white;   } h1 {  text-align: center;   }</style> </head> <body> <h1>ALS Shower Control</h1>  <p><button onmousedown=location.href=\"/Forward\">Forward</button></p> <p><button onmousedown=location.href=\"/Backward\">Backward</button></p><p><button onmousedown=location.href=\"/Center\">Center</button></p> </body> </html>";
void handleRoot () {
 server.send(200, "text/html", webPage);
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
	server.begin();
	Serial.println("HTTP server started");
  pinMode(stepping, OUTPUT);
  pinMode(dir, OUTPUT);

//****************************
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
}

  

void Go_Forward (int steps)
{
digitalWrite(dir, HIGH); //Rotate stepper motor in clock wise direction

          for( i=1;i<=steps;i++){
          digitalWrite(stepping, HIGH);
     
          digitalWrite(stepping, LOW);
          delay(1);}  
}

void Go_Backward(int steps)
{
digitalWrite(dir, LOW); //Rotate stepper motor in anti clock wise direction

          for( i=1;i<=steps;i++){
          digitalWrite(stepping, HIGH);
        
          digitalWrite(stepping, LOW);
          delay(1);}
}
void loop() {
	server.handleClient();
}

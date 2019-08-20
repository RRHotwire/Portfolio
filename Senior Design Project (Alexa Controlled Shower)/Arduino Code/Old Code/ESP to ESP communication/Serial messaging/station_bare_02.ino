/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/station-class.rst
 *  Works with: accesspoint_bare_01.ino
 */

#include <ESP8266WiFi.h>

byte ledPin = 2;
char ssid[] = "test";           // SSID of your AP
char pass[] = "password";         // password of your AP

IPAddress server(192,168,4,15);     // IP address of the AP
WiFiClient client;


void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);           // connects to the WiFi AP
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println("station_bare_01.ino");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
}
char string[256];
void loop() {
  client.connect(server, 80);
  //client.setTimeout(500);

  //Serial.println("connection");
  /*
  Serial.println("********************************");
  Serial.print("Byte sent to the AP: ");
  Serial.println(client.print("Anyo\r"));
  String answer = client.readStringUntil('\r');
  Serial.println("From the AP: " + answer);
  client.flush();

  client.stop();
  */
  
  if(Serial.available()>0)
        { 
        Serial.println("data sending start");
        String a;
        a = Serial.readString() + "\r"; //Serial Byte Read
        a.toCharArray(string,256);
        client.write(string, sizeof(string)); //Send one byte to ESP8266 
        Serial.println("data sending end");


        }
    
    else {
      if(client.connected()){
        //if(client.available())
      //Serial.println("reading start");
      Serial.print(client.readStringUntil('\r'));
      //Serial.println("reading end");

      }
    }
    client.flush();
    //client.stop();
}

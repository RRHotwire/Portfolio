/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */ 


#include <ESP8266WiFi.h>

WiFiServer server(80);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);
byte ledPin = 2;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("test", "password");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  server.setNoDelay(1);

  Serial.println();
  Serial.println("accesspoint_bare_01.ino");
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());
}
char string[256];
void loop() {
  WiFiClient client = server.available();
    //client.setTimeout(500);

  if (!client) {return;}
  /*
  String request = client.readStringUntil('\r');
  Serial.println("********************************");
  Serial.println("From the station: " + request);
  client.flush();
  Serial.print("Byte sent to the station: ");
  Serial.println(client.println(request + "ca" + "\r"));
  */
  
    else if(Serial.available()>0)
    {
      Serial.println("data sending start");
      String a;
      a = Serial.readString()+ "\r";
      a.toCharArray(string, 256);
      client.write(string, sizeof(string));
      Serial.println("data sending end");
    }
    else{
      if(client.connected()){
      //Serial.println("reading start");
      //if(client.available())
      Serial.print(client.readStringUntil('\r'));
      //Serial.println("reading end");
      }
    }
  
   client.flush();
  

}

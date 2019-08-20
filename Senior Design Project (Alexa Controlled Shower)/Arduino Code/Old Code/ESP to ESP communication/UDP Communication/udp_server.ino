
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "udp_comm";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress ClientIP(192,168,4,2);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
WiFiServer server(80);

char packetBuffer[9];   //Where we get the UDP data
String data = "test";

//=======================================================================
//                Setup
//=======================================================================
void setup()
{
    Serial.begin(9600);
    Serial.println();
    
    WiFi.softAP(ssid, pass);    //Create Access point
    server.begin();
    //Start UDP
    Serial.println("Starting UDP");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
}
//======================================================================
//                MAIN LOOP
//======================================================================
void loop()
{
    WiFi client = server.available();
    
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
      udp.read(packetBuffer, 1); // read the packet into the buffer, we are reading only one byte
      Serial.print(packetBuffer);
      delay(20);
    }
}
//======================================================================

/*
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
char packetBuffer[255];
unsigned int localPort = 9999;
const char *ssid = "Your SSID";  
const char *password = "Your password";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Udp.begin(localPort);
  }

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len-1] = 0;
    Serial.print("Recibido(IP/Size/Data): ");
    Serial.print(Udp.remoteIP());Serial.print(" / ");
    Serial.print(packetSize);Serial.print(" / ");
    Serial.println(packetBuffer);

    Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
    Udp.write("recived: ");
    Udp.write(packetBuffer);
    Udp.write("\r\n");
    Udp.endPacket();
     }
}

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
const char *ssid = "Your SSID";  //
const char *password = "Your Password";

char packetBuffer[255];
unsigned int localPort = 9999;
IPAddress ipServidor(192, 168, 4, 1);
IPAddress ipCliente(192, 168, 4, 10);
IPAddress Subnet(255, 255, 255, 0);
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA); // para que solo sea STA y no genere la IP 192.168.4.1
  WiFi.config(ipCliente, ipServidor, Subnet);
  Udp.begin(localPort);
}

void loop() {
unsigned long Tiempo_Envio = millis();
//ENVIO
    Udp.beginPacket(ipServidor,9999);
    Udp.write("Millis: ");
    char buf[20];   unsigned long testID = millis();    sprintf(buf, "%lu", testID); 
    Udp.write(buf);
    Udp.write("\r\n");
    Udp.endPacket();
    Serial.print("enviando: ");Serial.println(buf);
delay(10); // para que le de tiempo a recibir la respuesta al envio anterior
 
//RECEPCION
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len-1] = 0;
    Serial.print("RECIBIDO(IP/Port/Size/Datos): ");
    Serial.print(Udp.remoteIP());Serial.print(" / ");
    Serial.print(Udp.remotePort()); Serial.print(" / ");
    Serial.print(packetSize);Serial.print(" / ");
    Serial.println(packetBuffer);
  }
Serial.println("");
delay(500);
}*/

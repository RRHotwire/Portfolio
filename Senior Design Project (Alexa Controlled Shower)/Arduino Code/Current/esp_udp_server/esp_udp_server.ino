
//esp udp server 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>

const char *ssid = "udp_comm";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress waterClientIP(192,168,4,4);
IPAddress trackClientIP(192,168,4,3);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
ESP8266WebServer server(80);

#define SDA_PIN D1
#define SCL_PIN D2

double warm = 95;
double hot = 105;
double cold = 80;

const uint16_t I2C_ADDRESS = 0x08;
const uint16_t I2C_MASTER = 0x42;
uint16_t local_address = I2C_ADDRESS; // slave

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];   //Where we get the UDP data
const char MAIN_page[] PROGMEM = R"=====(
<HTML><body>

<style>
#box, div{padding: 12px 28px;   width: 100%;
     font-size: 30px; 
 width: 100%; height:5%;
}
input.submit{
padding: 12px 28px;   width: 100%;
border-radius: 20px;   border: 10px;  font-size: 30px; 
 width: 100%; height:10%;
}

button{ padding: 12px 28px;   width: 100%; height:10%;
border-radius: 20px;   border: 10px;  font-size: 30px;   }

</style>



<form method="post" action="/Warmer">
<center> <input class ="submit" type="submit" id="Warmer"  value="Warmer"></input><br style="line-height:1.75"></center>
</form>

<form method="post" action="/Colder">
<center> <input class ="submit" type="submit" id="Colder"  value="Colder"></input><br style="line-height:1.75"> </center>
</form>


<form method="post" action="/Turn_off_water">
<center> <input class ="submit" type="submit" id="Water off"  value="Water off"></input><br style="line-height:1.75"></center>
</form>

<form method="post" action="/Head_Forward">
<center> <input class ="submit" type="submit" id="Head Forward"  value="Head Forward"></input><br style="line-height:1.75"></center>
</form>

<form method="post" action="/Head_Backward">
<center> <input class ="submit" type="submit" id="Head Backward"  value="Head Backward"></input><br style="line-height:1.75"> </center>
</form>


<center>
<h1>
  Current Temperature is : <span id="ADCValue">0</span><br>
</h1>
</center>


<form method="post" action="/save">
<center><h1>Set Temperature:<input id="box" type="text" id="input_txt" name="input_txt" ><h1><br style="line-height:1.75"></center>
<center><input class ="submit" id="submit" type="submit" value="Submit"></input> <br style="line-height:1.75"></center>
</form>

<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 2000); //2000mSeconds update rate
 
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}

</script>

<form method="post" action="/Forward">
<center><input class ="submit" id="Forward"type ="submit" value="Forward">  </input><br style="line-height:1.75"></center>
</form>

<form method="post" action="/Center">
<center><input class ="submit" id="Center"type ="submit" value="Center"> </input> <br style="line-height:1.75"></center>
</form>

<form method="post" action="/Backward">
<center><input class ="submit" id="Backward" type ="submit" value="Backward"> </input> <br style="line-height:1.75"></center>
</form>

</body>
</HTML>)=====";

void receiveEvent(unsigned int howMany) {

  Serial.print("Received "); Serial.print(howMany); Serial.println(" bytes...");
  String data = "";
  while( Wire.available()){
    data += (char)Wire.read();
  }
  Serial.println("Message received: " + data);
  String msg = data;
  alexaCommandHandler(msg);
}
void alexaCommandHandler(String data){
  if(data == "C"){
      Serial.println("Sending command: Cool the Water");
      delayMicroseconds(1000); 
      udp.beginPacket(waterClientIP, 2000);
      udp.write("C"); //Send one byte to ESP8266 
      udp.endPacket();   
      delayMicroseconds(1000); 
    }
    else if(data == "W"){
      Serial.println("Sending command: Warm the Water");
      delayMicroseconds(1000); 
      udp.beginPacket(waterClientIP, 2000);
      udp.write("W"); //Send one byte to ESP8266 
      udp.endPacket();
      delayMicroseconds(1000);
    }
    else if(data == "warm"){
      Serial.println("Sending command: Set target temperature to " + String(warm));
      udp.beginPacket(waterClientIP, 2000);
      udp.print(warm); //Send one byte to ESP8266 
      udp.endPacket();   
      delayMicroseconds(1000);
    }
    else if(data == "cold"){
      Serial.println("Sending command: Set target temperature to " + String(cold));
      udp.beginPacket(waterClientIP, 2000);
      udp.print(cold); //Send one byte to ESP8266 
      udp.endPacket();   
      delayMicroseconds(1000);
    }
    else if(data == "hot"){
      Serial.println("Sending command: Set target temperature to " + String(hot));
      udp.beginPacket(waterClientIP, 2000);
      udp.print(hot); //Send one byte to ESP8266 
      udp.endPacket();   
      delayMicroseconds(1000);
    }
    else if(data == "Forward"){
      Serial.println("Sending command: Forward");
      udp.beginPacket(trackClientIP, 2000);
      udp.write("F"); //Send one byte to ESP8266
      udp.endPacket();
      delayMicroseconds(1000);
    }
    else if(data == "Center"){
      Serial.println("Sending command: Center");
      udp.beginPacket(trackClientIP, 2000);
      udp.write("C"); //Send one byte to ESP8266 
      udp.endPacket();
      delayMicroseconds(1000);  
    }
    else if(data== "Backward"){
      Serial.println("Sending command: Backward");
      udp.beginPacket(trackClientIP, 2000);
      udp.write("B"); //Send one byte to ESP8266 
      udp.endPacket();
      delayMicroseconds(1000);
    }
    else if(data== "TOW"){
      Serial.println("Sending command: Turning Off Water");
      delayMicroseconds(1000); 
      udp.beginPacket(waterClientIP, 2000);
      udp.write("TOW"); //Send one byte to ESP8266 
      udp.endPacket();
      delayMicroseconds(1000);
    }
    else if(data == "HF"){
      Serial.println("Sending command: Moving Head Forward");
      udp.beginPacket(trackClientIP, 2000);
      udp.write("HF"); //Send one byte to ESP8266 
      udp.endPacket();
      delayMicroseconds(1000);
    }
    else if(data == "HB"){
      Serial.println("Sending command: Moving Head Forward");
      udp.beginPacket(trackClientIP, 2000);
      udp.write("HB"); //Send one byte to ESP8266 
      udp.endPacket();
      delayMicroseconds(1000);
    }
}
void sendMessage(String msg) {
  
  Serial.print("[SERVANT] Sending message: ");
  Serial.print(msg);  
  Wire.beginTransmission(I2C_MASTER); // transmit to device 0x08
  //Sending Side


    Wire.print(msg);

  Wire.endTransmission();    // stop transmitting
  Serial.println("");
}

void handleRoot() { 
  
  server.send(200, "text/html", MAIN_page);
}

float tempReading=0;
void handleADC() {
  if(tempReading == -99 || tempReading ==0){
    server.send(200, "text/plane", "Water Turned Off"); //Send ADC value only to client ajax request
  }
  else{
 String adcValue = String(tempReading);
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
  }
}

void handleSave() {
  String str = "Settings Saved ...\r\n";
       // number of arguments

  Serial.print("number of arguments "); 
  Serial.println(server.args());             
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      str += server.argName(i) + " = " + server.arg(i) + "\r\n";
    
      Serial.println("Temperature Target Value: "+ server.arg(i) + " F");
      String targetTemp = String(server.arg(i));
      targetTemp.replace("input_txt=", "");
      Serial.println("Sending command: Set target temperature to " + targetTemp);
      udp.beginPacket(waterClientIP, 2000);
      udp.print(targetTemp); //Send one byte to ESP8266 
      udp.endPacket();   
      delayMicroseconds(1000);
    }
  }
  
  server.send(200, "text/html", MAIN_page);

}

void requestEvent() {
  Wire.print("Hello sir "); // respond with message of 6 bytes
  // as expected by master
}
//=======================================================================
//                Setup
//=======================================================================
void setup()
{
    Wire.pins(SDA_PIN, SCL_PIN);
    Wire.begin(I2C_ADDRESS); // address required for slave
    
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
    Serial.begin(115200);
    
  Serial.print("I2C Slave started on address: 0x");
  Serial.println(I2C_ADDRESS, HEX);
  
    Serial.println("");
      server.on("/", handleRoot);
    WiFi.softAP(ssid, pass);    //Create Access point
    server.begin();
    //Start UDP
    Serial.println("Starting UDP");
    delayMicroseconds(1000); 
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
    
    server.on("/readADC", handleADC);
    
    server.on("/", handleRoot);
  server.on("/Forward", [](){
    Serial.println("Sending command: Forward");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("F"); //Send one byte to ESP8266
    udp.endPacket();
    delayMicroseconds(1000);
  });

    server.on("/Center", [](){
    Serial.println("Sending command: Center");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("C"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);  
    
  });
  server.on("/Warmer", [](){
    Serial.println("Sending command: Warm the Water");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("W"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);  
    
  });
  server.on("/Colder", [](){
    Serial.println("Sending command: Cool the Water");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("C"); //Send one byte to ESP8266 
    
    udp.endPacket();   
    delayMicroseconds(1000);  

  });

  server.on("/save", handleSave);

  server.on("/Backward", [](){
    Serial.println("Sending command: Backward");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("B"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });
  
  server.on("/Head_Forward", [](){
    Serial.println("Sending command: Moving Head Forward");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("HF"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });

  server.on("/Turn_off_water", [](){
    Serial.println("Sending command: Turning Off Water");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("TOW"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });
  
  server.on("/Head_Backward", [](){
    Serial.println("Sending command: Moving Head Backward");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.write("HB"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);
  });
    
      
}
//======================================================================
//                MAIN LOOP
//======================================================================
String msgRcv;
void handleUPDPacket(){
  String temp;
  Serial.println("UDP Message Recieved: "+ msgRcv);
  delayMicroseconds(1000); 
  if(msgRcv.startsWith("tempReading=")){
    temp = msgRcv;
    temp.replace("tempReading=","" );
    tempReading = temp.toFloat();
    Serial.println("temp = " + String(tempReading));
  }
  delay(1);
}

bool waitForWaterMessage = false;
bool waitForTrackMessage = false;





void loop()
{
//    WiFi client = server.available();
    server.handleClient();
    //randVal = random(300);
    
    int cb = udp.parsePacket();
    if (!cb) 
    {
      //If serial data is recived send it to UDP
      if(Serial.available()>0)
        {
        udp.beginPacket(trackClientIP, 2000);
        //Send UDP requests are to port 2000
        
        char a[1];
        a[0]=char(Serial.read()); //Serial Byte Read
        udp.write(a,1); //Send one byte to ESP8266 
        udp.endPacket();
        delay(1);
        }
    }
    else {
      // We've received a UDP packet, send it to serial

      for(int i = 0; i<UDP_TX_PACKET_MAX_SIZE; i++) packetBuffer[i] = 0;
      udp.read(packetBuffer, cb); // read the packet into the buffer, we are reading only one byte
      msgRcv = String(packetBuffer);
      handleUPDPacket();
      delay(20);
    }
    
}

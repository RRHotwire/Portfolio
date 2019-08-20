
//esp udp server 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
const char *ssid = "udp_comm";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress waterClientIP(192,168,4,4);
IPAddress trackClientIP(192,168,4,3);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
ESP8266WebServer server(80);

/*
<meta 
     name='viewport' 
     content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0' 
/>*/

/*
<script>
function func(ele) {
  var xhttp = new XMLHttpRequest();
  var value = ele.getAttribute("value");

  xhttp.open("POST", value, true);
  xhttp.send();
}
</script>
 */

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];   //Where we get the UDP data
const char MAIN_page[] PROGMEM = R"=====(
<HTML><body>

<style>
#box, div{padding: 12px 28px;   width: 100%;
     font-size: 30px; 
 width: 100%; height:5%;
}
#submit{
padding: 12px 28px;   width: 100%;
border-radius: 20px;   border: 10px;  font-size: 30px; 
 width: 100%; height:10%;
}

button{ padding: 12px 28px;   width: 100%; height:10%;
border-radius: 20px;   border: 10px;  font-size: 30px;   }

</style>




<form method="post" action="/OpenHot">
<center> <input type="submit" id="OpenHot"  value="OpenHot"> Open Hot Water </input><br style="line-height:1.75"></center>
</form>

<form method="post" action="/CloseHot">
<center> <input type="submit" id="CloseHot"  value="CloseHot"> Close Hot Water </input><br style="line-height:1.75"></center> 
</form>

<form method="post" action="/OpenCold">
<center> <input type="submit" id="OpenCold"  value="OpenCold"> Open Cold Water </input><br style="line-height:1.75"></center>
</form>

<form method="post" action="/CloseCold">
<center> <input type="submit" id="CloseCold"  value="CloseCold"> Close Cold Water </input><br style="line-height:1.75"></center>
</form>



<center>
<h1>
  Current Temperature is : <span id="ADCValue">0</span><br>
</h1>
</center>


<form method="post" action="/save">
<center><h1>Set Temperature:<input id="box" type="text" id="input_txt" name="input_txt" ><h1><br style="line-height:1.75"></center>
<center><input id="submit" type="submit" value="Submit" > <br style="line-height:1.75"></center>
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
<center><input id="Forward" onclick="func(this)"  type ="submit" value="Forward"> Forward </input><br style="line-height:1.75"></center>
</form>

<form method="post" action="/Center">
<center><input id="Center" onclick="func(this)"  type ="submit" value="Center"> Center </input> <br style="line-height:1.75"></center>
</form>

<form method="post" action="/Backward">
<center><input id="Backward" onclick="func(this)"  type ="submit" value="Backward"> Backward </input> <br style="line-height:1.75"></center>
</form>

</body>
</HTML>)=====";

void handleRoot() { 
  
  server.send(200, "text/html", MAIN_page);
}

float tempReading=0;
void handleADC() {
 String adcValue = String(tempReading);
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
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
  server.on("/OpenHot", [](){
    Serial.println("Sending command: Open Hot Water");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("OH"); //Send one byte to ESP8266 
    udp.endPacket();
    delayMicroseconds(1000);  
    
  });
  server.on("/CloseHot", [](){
    Serial.println("Sending command: Close Hot Water");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("CH"); //Send one byte to ESP8266 
    
    udp.endPacket();   
    delayMicroseconds(1000);  

  });
  server.on("/OpenCold", [](){
    Serial.println("Sending command: Open Cold Water");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("OC"); //Send one byte to ESP8266 
    udp.endPacket();   
    delayMicroseconds(1000);
    
  });
  server.on("/CloseCold", [](){
    Serial.println("Sending command: Close Cold Water");
    delayMicroseconds(1000); 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.write("CC"); //Send one byte to ESP8266 
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

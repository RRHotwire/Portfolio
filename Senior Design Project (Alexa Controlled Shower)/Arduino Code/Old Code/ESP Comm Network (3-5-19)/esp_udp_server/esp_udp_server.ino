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
const char MAIN_page[] PROGMEM = R"=====(
<HTML><body>


<style type="text/css">
html, body,{
    position: absolute;
    top: 10%;    
    left:0;
    right:0;
    margin:0 auto;
    width: 100%;
    height: 100%
    height: 100px;
    width: 100px;
    border:thin solid black;
    text-overflow:ellipsis;
    overflow:hidden;
}
</style>
<center> <button id="OpenHot" onmousedown=location.href="/OpenHot" type="button"> Open Hot Water </button><br style="line-height:1.75"></center>
<center> <button id="CloseHot" onmousedown=location.href="/CloseHot" type="button"> Close Hot Water </button><br style="line-height:1.75"></center> 
<center> <button id="OpenCold" onmousedown=location.href="/OpenCold" type="button"> Open Cold Water </button><br style="line-height:1.75"></center>
<center> <button id="CloseCold" onmousedown=location.href="/CloseCold" type="button"> Close Cold Water </button><br style="line-height:1.75"></center>


<center>
<div>
  Current Temperature is : <span id="ADCValue">0</span><br>
</div>
</center>

<body>
<form method="post" action="/save">
<center>Set Temperature:</center><center><input type="text" id="input_txt" name="input_txt" ><br style="line-height:1.75"></center>
<center><input type="submit" value="Send Temperature" /> <br style="line-height:1.75"></center>
</form></body>

<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 200); //2000mSeconds update rate
 
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

<center><button id="Forward" onmousedown=location.href="/Forward" type ="button" value="foward"> Foward </button><br style="line-height:1.75"></center>
<center><button id="Center" onmousedown=location.href="/Center" type ="button" value="center"> Center </button> <br style="line-height:1.75"></center>
<center><button id="Backward" onmousedown=location.href="/Backward" type ="button" value="Backward"> Reverse </button> <br style="line-height:1.75"></center>
</div>
</body>
</HTML>)=====";

void handleRoot() { 
  
  server.send(200, "text/html", MAIN_page);
}

int tempReading=0;
void handleADC() {
 String adcValue = String(tempReading);
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleSave() {
  String str = "Settings Saved ...\r\n";

  Serial.print("number of arguments "); 
  Serial.println(server.args());                    // number of arguments

  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      str += server.argName(i) + " = " + server.arg(i) + "\r\n";
    
      Serial.println("Temperature Target Value: "+ server.arg(i) + " F");
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
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
    
    server.on("/readADC", handleADC);
    
    server.on("/", handleRoot);
  server.on("/Forward", [](){
    Serial.println("Sending command: Foward");
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(ClientIP, 2000);
    udp.print("Forward"); //Send one byte to ESP8266
    udp.endPacket();  
  });

    server.on("/Center", [](){
    Serial.println("Sending command: Center");
 
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(ClientIP, 2000);
    udp.print("Center"); //Send one byte to ESP8266 
    udp.endPacket();   
    
  });
  server.on("/OpenHot", [](){
    Serial.println("Sending command: Open Hot Water");
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(ClientIP, 2000);
    udp.print("OpenHot"); //Send one byte to ESP8266 
    udp.endPacket();   
    
  });
  server.on("/CloseHot", [](){
    Serial.println("Sending command: Close Hot Water");
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(ClientIP, 2000);
    udp.print("CloseHot"); //Send one byte to ESP8266 
    udp.endPacket();   
    
  });
  server.on("/OpenCold", [](){
    Serial.println("Sending command: Open Cold Water");
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(ClientIP, 2000);
    udp.print("OpenCold"); //Send one byte to ESP8266 
    udp.endPacket();   
    
  });
  server.on("/CloseCold", [](){
    Serial.println("Sending command: Close Cold Water");
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(ClientIP, 2000);
    udp.print("CloseCold"); //Send one byte to ESP8266 
    udp.endPacket();   
    
  });
  server.on("/save", handleSave);

  server.on("/Backward", [](){
    Serial.println("Sending command: Backward");
    server.send(200, "text/html", MAIN_page);
    udp.beginPacket(ClientIP, 2000);
    udp.print("Backward"); //Send one byte to ESP8266 
    udp.endPacket(); 
  });
    randomSeed(analogRead(0));
      
}
//======================================================================
//                MAIN LOOP
//======================================================================
String msgRcv;
void handleUPDPacket(){
  String temp;
  if(msgRcv.startsWith("temp=")){
    temp = msgRcv;
    temp.remove(0,4);
    tempReading = temp.toFloat();
  }
}
void loop()
{
//    WiFi client = server.available();
    server.handleClient();
    randVal = random(300);
    
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
      msgRcv = String(packetBuffer);
      Serial.println("UDP Message Recieved: "+ msgRcv);
      delay(20);
    }
}

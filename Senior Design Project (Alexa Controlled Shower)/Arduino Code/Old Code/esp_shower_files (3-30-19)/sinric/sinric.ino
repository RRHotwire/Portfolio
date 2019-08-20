#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiudp.h>
#include <functional>

void prepareIds();
boolean connectWifi();
boolean connectudp();
void startHttpServer();
void turnOnRelay();
void turnOffRelay();
void sendRelayState();


const char* ssid2 = "udp_comm";  // CHANGE: esp name
const char* password2 = "password";  // CHANGE: esp password

const char* ssid = "Pixel_5448";  // CHANGE: Wifi name
const char* password = "12345678";  // CHANGE: Wifi password 
String friendlyName = "Alexa";        // CHANGE: Alexa device name
const int relayPin = D1;  // D1 pin. More info: https://github.com/esp8266/Arduino/blob/master/variants/d1_mini/pins_arduino.h#L49-L61

WiFiUDP udp;
IPAddress ipMulti(239, 255, 255, 250);
IPAddress ServerIP(192,168,4,1);
IPAddress waterClientIP(192,168,4,4);
IPAddress trackClientIP(192,168,4,3);


ESP8266WebServer HTTP(80);


boolean udpConnected = false;
unsigned int portMulti = 1900;      // local port to listen on
unsigned int localPort = 1900;      // local port to listen on
boolean wifiConnected = false;
boolean relayState = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
String serial;
String persistent_uuid;
boolean cannotConnectToWifi = false;
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





<center> <button name="button"  type="button" id="OpenHot" onmousedown="func(this)" value="OpenHot"> Open Hot Water </button><br style="line-height:1.75"></center>
<center> <button name="button" type="button" id="CloseHot" onmousedown="func(this)" value="CloseHot"> Close Hot Water </button><br style="line-height:1.75"></center> 
<center> <button name="button" type="button" id="OpenCold" onmousedown="func(this)" value="OpenCold"> Open Cold Water </button><br style="line-height:1.75"></center>
<center> <button name="button" type="button" id="CloseCold" onmousedown="func(this)" value="CloseCold"> Close Cold Water </button><br style="line-height:1.75"></center>

<script>
var wrapper = document.getElementById('counter');
  var counter;
  var count = 0;
  
  function start(ele) {
    func(ele);
    counter = setInterval(function() {
      wrapper.innerHTML = count;
      count++;
    }, 500);
  }
  function end(ele) {
    clearInterval(counter)
  }
  
function func(ele) {
  var xhttp = new XMLHttpRequest();
  var value = ele.getAttribute("value");

  xhttp.open("GET", value, true);
  xhttp.send();
}
</script>


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

<center><button id="Forward" onmousedown="func(this)"  type ="button" value="Forward"> Forward </button><br style="line-height:1.75"></center>
<center><button id="Center" onmousedown="func(this)"  type ="button" value="Center"> Center </button> <br style="line-height:1.75"></center>
<center><button id="Backward" onmousedown="func(this)"  type ="button" value="Backward"> Reverse </button> <br style="line-height:1.75"></center>

</body>
</HTML>)=====";

void setup() {
  Serial.begin(115200);

  // Setup Relay
  pinMode(relayPin, OUTPUT);
  
  prepareIds();
  
  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if(wifiConnected){
    Serial.println("Ask Alexa to discover devices");
    udpConnected = connectudp();
    
    if (udpConnected){
      // initialise pins if needed 
      startHttpServer();
    }
  }  
}

void loop() {

  HTTP.handleClient();
  delay(1);
  
  
  // if there's data available, read a packet
  // check if the WiFi and udp connections were successful
  if(wifiConnected){
    if(udpConnected){    
      // if there’s data available, read a packet
      int packetSize = udp.parsePacket();
      
      if(packetSize) {
        //Serial.println("");
        //Serial.print("Received packet of size ");
        //Serial.println(packetSize);
        //Serial.print("From ");
        IPAddress remote = udp.remoteIP();
        
        for (int i =0; i < 4; i++) {
          Serial.print(remote[i], DEC);
          if (i < 3) {
            Serial.print(".");
          }
        }
        
        Serial.print(", port ");
        Serial.println(udp.remotePort());
        
        int len = udp.read(packetBuffer, 255);
        
        if (len > 0) {
            packetBuffer[len] = 0;
        }

        String request = packetBuffer;
        //Serial.println("Request:");
        //Serial.println(request);
        
        // Issue https://github.com/kakopappa/arduino-esp8266-alexa-wemo-switch/issues/24 fix
        if(request.indexOf("M-SEARCH") >= 0) {
            // Issue https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch/issues/22 fix
            //if(request.indexOf("urn:Belkin:device:**") > 0) {
             if((request.indexOf("urn:Belkin:device:**") > 0) || (request.indexOf("ssdp:all") > 0) || (request.indexOf("upnp:rootdevice") > 0)) {
                Serial.println("Responding to search request ...");
                respondToSearch();
             }
        }
      }
        
      delay(10);
    }
  } else {
      Serial.println("Cannot connect to Wifi");
  }
}

void prepareIds() {
  uint32_t chipId = ESP.getChipId();
  char uuid[64];
  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x"),
        (uint16_t) ((chipId >> 16) & 0xff),
        (uint16_t) ((chipId >>  8) & 0xff),
        (uint16_t)   chipId        & 0xff);

  serial = String(uuid);
  persistent_uuid = "Socket-1_0-" + serial;
}

void respondToSearch() {
    Serial.println("");
    Serial.print("Sending response to ");
    Serial.println(udp.remoteIP());
    Serial.print("Port : ");
    Serial.println(udp.remotePort());

    IPAddress localIP = WiFi.localIP();
    char s[16];
    sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

    String response = 
         "HTTP/1.1 200 OK\r\n"
         "CACHE-CONTROL: max-age=86400\r\n"
         "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
         "EXT:\r\n"
         "LOCATION: http://" + String(s) + ":80/setup.xml\r\n"
         "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
         "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
         "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
         "ST: urn:Belkin:device:**\r\n"
         "USN: uuid:" + persistent_uuid + "::urn:Belkin:device:**\r\n"
         "X-User-Agent: redsonic\r\n\r\n";
  
    // Try changing to this if you have problems discovering
    /* https://github.com/kakopappa/arduino-esp8266-alexa-wemo-switch/issues/26
    String response =
      "HTTP/1.1 200 OK\r\n"
      "CACHE-CONTROL: max-age=86400\r\n"
      "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
      "EXT:\r\n"
      "LOCATION: http://" + String(s) + ":80/setup.xml\r\n"
      "OPT: "http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
      "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
      "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
      "ST: ssdp:all\r\n"
      "USN: uuid:" + persistent_uuid + "::upnp:rootdevice\r\n"
      "X-User-Agent: redsonic\r\n\r\n";
    */

    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(response.c_str());
    udp.endPacket();                    

    /* add yield to fix udp sending response. For more informations : https://www.tabsoverspaces.com/233359-udp-packets-not-sent-from-esp-8266-solved */
    yield(); 
  
    Serial.println("Response sent !");
}
void handleRoot() { 
  
  HTTP.send(200, "text/html", MAIN_page);
}

float tempReading=0;
void handleADC() {
 String adcValue = String(tempReading);
 HTTP.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}
void handleSave() {
  String str = "Settings Saved ...\r\n";

  Serial.print("number of arguments "); 
  Serial.println(HTTP.args());                    // number of arguments

  if (HTTP.args() > 0 ) {
    for ( uint8_t i = 0; i < HTTP.args(); i++ ) {
      str += HTTP.argName(i) + " = " + HTTP.arg(i) + "\r\n";
    
      Serial.println("Temperature Target Value: "+ HTTP.arg(i) + " F");
      String targetTemp = String(HTTP.arg(i));
      targetTemp.replace("input_txt=", "");
      Serial.println("Sending command: Set target temperature to " + targetTemp);
      udp.beginPacket(waterClientIP, 2000);
      udp.print(targetTemp); //Send one byte to ESP8266 
      udp.endPacket();   
    }
  }
  
  HTTP.send(200, "text/html", MAIN_page);

}
void startHttpServer() {
    WiFi.softAP(ssid2, password2); 
    HTTP.on("/index.html", HTTP_GET, [](){
      Serial.println("Got Request index.html ...\n");
      HTTP.send(200, "text/plain", "Hello World!");
    });
    HTTP.on("/readADC", handleADC);
    
    HTTP.on("/", handleRoot);
  HTTP.on("/Forward", [](){
    Serial.println("Sending command: Forward");
    HTTP.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.print("Forward"); //Send one byte to ESP8266
    udp.endPacket();  
    yield();
  });

    HTTP.on("/Center", [](){
    Serial.println("Sending command: Center");
 
    HTTP.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.print("Center"); //Send one byte to ESP8266 
    udp.endPacket(); 
    yield();  
    
  });
  HTTP.on("/OpenHot", [](){
    Serial.println("Sending command: Open Hot Water");
    HTTP.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.print("Open Hot"); //Send one byte to ESP8266 
    udp.endPacket(); 
    yield();  
    
  });
  HTTP.on("/CloseHot", [](){
    Serial.println("Sending command: Close Hot Water");
    HTTP.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.print("Close Hot"); //Send one byte to ESP8266 
    udp.endPacket();   
    yield();  

  });
  HTTP.on("/OpenCold", [](){
    Serial.println("Sending command: Open Cold Water");
    HTTP.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.print("Open Cold"); //Send one byte to ESP8266 
    udp.endPacket();   
    yield();
    
  });
  HTTP.on("/CloseCold", [](){
    Serial.println("Sending command: Close Cold Water");
    HTTP.send(200, "text/html", MAIN_page);
    udp.beginPacket(waterClientIP, 2000);
    udp.print("Close Cold"); //Send one byte to ESP8266 
    udp.endPacket();   
    yield();
    
  });
  HTTP.on("/save", handleSave);

  HTTP.on("/Backward", [](){
    Serial.println("Sending command: Backward");
    HTTP.send(200, "text/html", MAIN_page);
    udp.beginPacket(trackClientIP, 2000);
    udp.print("Backward"); //Send one byte to ESP8266 
    udp.endPacket(); 
    yield();
  });
    HTTP.on("/upnp/control/basicevent1", HTTP_POST, []() {
      Serial.println("########## Responding to  /upnp/control/basicevent1 ... ##########");      

      //for (int x=0; x <= HTTP.args(); x++) {
      //  Serial.println(HTTP.arg(x));
      //}
  
      String request = HTTP.arg(0);      
      Serial.print("request:");
      Serial.println(request);
 
      if(request.indexOf("SetBinaryState") >= 0) {
        if(request.indexOf("<BinaryState>1</BinaryState>") >= 0) {
            Serial.println("Got Turn on request");
            turnOnRelay();
        }
  
        if(request.indexOf("<BinaryState>0</BinaryState>") >= 0) {
            Serial.println("Got Turn off request");
            turnOffRelay();
        }
      }

      if(request.indexOf("GetBinaryState") >= 0) {
        Serial.println("Got binary state request");
        sendRelayState();
      }
            
      HTTP.send(200, "text/plain", "");
    });

    HTTP.on("/eventservice.xml", HTTP_GET, [](){
      Serial.println(" ########## Responding to eventservice.xml ... ########\n");
      
      String eventservice_xml = "<scpd xmlns=\"urn:Belkin:service-1-0\">"
        "<actionList>"
          "<action>"
            "<name>SetBinaryState</name>"
            "<argumentList>"
              "<argument>"
                "<retval/>"
                "<name>BinaryState</name>"
                "<relatedStateVariable>BinaryState</relatedStateVariable>"
                "<direction>in</direction>"
                "</argument>"
            "</argumentList>"
          "</action>"
          "<action>"
            "<name>GetBinaryState</name>"
            "<argumentList>"
              "<argument>"
                "<retval/>"
                "<name>BinaryState</name>"
                "<relatedStateVariable>BinaryState</relatedStateVariable>"
                "<direction>out</direction>"
                "</argument>"
            "</argumentList>"
          "</action>"
      "</actionList>"
        "<serviceStateTable>"
          "<stateVariable sendEvents=\"yes\">"
            "<name>BinaryState</name>"
            "<dataType>Boolean</dataType>"
            "<defaultValue>0</defaultValue>"
           "</stateVariable>"
           "<stateVariable sendEvents=\"yes\">"
              "<name>level</name>"
              "<dataType>string</dataType>"
              "<defaultValue>0</defaultValue>"
           "</stateVariable>"
        "</serviceStateTable>"
        "</scpd>\r\n"
        "\r\n";
            
      HTTP.send(200, "text/plain", eventservice_xml.c_str());
    });
    
    HTTP.on("/setup.xml", HTTP_GET, [](){
      Serial.println(" ########## Responding to setup.xml ... ########\n");

      IPAddress localIP = WiFi.localIP();
      char s[16];
      sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
    
      String setup_xml = "<?xml version=\"1.0\"?>"
            "<root>"
             "<device>"
                "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
                "<friendlyName>"+ friendlyName +"</friendlyName>"
                "<manufacturer>Belkin International Inc.</manufacturer>"
                "<modelName>Socket</modelName>"
                "<modelNumber>3.1415</modelNumber>"
                "<modelDescription>Belkin Plugin Socket 1.0</modelDescription>\r\n"
                "<UDN>uuid:"+ persistent_uuid +"</UDN>"
                "<serialNumber>221517K0101769</serialNumber>"
                "<binaryState>0</binaryState>"
                "<serviceList>"
                  "<service>"
                      "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                      "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                      "<controlURL>/upnp/control/basicevent1</controlURL>"
                      "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                      "<SCPDURL>/eventservice.xml</SCPDURL>"
                  "</service>"
              "</serviceList>" 
              "</device>"
            "</root>\r\n"
            "\r\n";
            
        HTTP.send(200, "text/xml", setup_xml.c_str());
        
        Serial.print("Sending :");
        Serial.println(setup_xml);
    });

    // openHAB support
    HTTP.on("/on.html", HTTP_GET, [](){
         Serial.println("Got Turn on request");
         HTTP.send(200, "text/plain", "turned on");
         turnOnRelay();
       });
 
     HTTP.on("/off.html", HTTP_GET, [](){
        Serial.println("Got Turn off request");
        HTTP.send(200, "text/plain", "turned off");
        turnOffRelay();
       });
 
      HTTP.on("/status.html", HTTP_GET, [](){
        Serial.println("Got status request");
 
        String statrespone = "0"; 
        if (relayState) {
          statrespone = "1"; 
        }
        HTTP.send(200, "text/plain", statrespone);
      
    });
    
    HTTP.begin();  
    Serial.println("HTTP Server started ..");
}
      
// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

boolean connectudp(){
  boolean state = false;
  
  Serial.println("");
  Serial.println("Connecting to udp");
  
  if(udp.beginMulticast(WiFi.localIP(), ipMulti, portMulti)) {
    Serial.println("Connection successful");
    state = true;
  }
  else{
    Serial.println("Connection failed");
  }
  
  return state;
}

void turnOnRelay() {
 digitalWrite(relayPin, HIGH); // turn on relay with voltage HIGH 
 relayState = true;

  String body = 
      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
      "<u:SetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
      "<BinaryState>1</BinaryState>\r\n"
      "</u:SetBinaryStateResponse>\r\n"
      "</s:Body> </s:Envelope>";

  HTTP.send(200, "text/xml", body.c_str());
        
  Serial.print("Sending :");
  Serial.println(body);
}

void turnOffRelay() {
  digitalWrite(relayPin, LOW);  // turn off relay with voltage LOW
  relayState = false;

  String body = 
      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
      "<u:SetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
      "<BinaryState>0</BinaryState>\r\n"
      "</u:SetBinaryStateResponse>\r\n"
      "</s:Body> </s:Envelope>";

  HTTP.send(200, "text/xml", body.c_str());
        
  Serial.print("Sending :");
  Serial.println(body);
}

void sendRelayState() {
  
  String body = 
      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
      "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
      "<BinaryState>";
      
  body += (relayState ? "1" : "0");
  
  body += "</BinaryState>\r\n"
      "</u:GetBinaryStateResponse>\r\n"
      "</s:Body> </s:Envelope>\r\n";
 
   HTTP.send(200, "text/xml", body.c_str());
}

//UDP track motor client

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
const char *ssid = "udp_comm";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress trackClientIP(192,168,4,3);
IPAddress subnet(255, 255, 255, 0);

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
ESP8266WebServer server(80);

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];   //Where we get the UDP data

int i = 0;
int pos = 0;
int steps = 0; 
const int stepping = 0; 
const int dir = 2;

const int TrigPin = 4; //D2; //CHANGE THESE
const int EchoPin = 5; //D1; //CHANGE THESE
float in, cm;
//======================================================================
//                Setup
//======================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.begin(ssid, pass);   //Connect to access point
    WiFi.mode(WIFI_STA);
    WiFi.config(trackClientIP, ServerIP, subnet);
  
    Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    
    //Start UDP
    Serial.println("Starting UDP");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());

  pinMode(stepping, OUTPUT);
  pinMode(dir, OUTPUT);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  Restart(); //Restart function that resets position back to pos 0. 
//****************************
}

//======================================================================
//                Motor Movement Functions
//======================================================================

void foward(){
   if (pos < 4)
   {
   Go_Forward(3100);
   pos++;
   }
   else 
   {
    Serial.println("Foward movement limit reached");
   Go_Forward(0); 
   }
  }
void center(){
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
void backward(){
    
    if (pos > 0)
    {
      Go_Backward(3100);
      pos--;
    }
    else {
      Serial.println("Backward movement limit reached");
      Go_Backward(0);  
    }
  }
  
//======================================================================
//                UDP Message Functions
//======================================================================
void msgSender(String msg){
  udp.beginPacket(ServerIP, 2000);
  udp.print(msg);
  udp.endPacket();
}

String positionUpdateHandler(){
  return ("position="+String(pos));
}

void recvMessageHandler(String msg){
  Serial.println("Recieved... ");

   if(msg == "Forward"){
    Serial.println("Command: Moving Forward");
    msgSender("Track Client recieved Command: Forward");
    foward();
   }
   else if(msg == "Center"){
    Serial.println("Command: Moving to Center");
    msgSender("Track Client recieved Command: Center");

    center();
   }
   else if(msg == "Backward"){
    Serial.println("Command: Moving to Backward");
    msgSender("Track Client recieved Command: Backward");

    backward();
   }
}
//======================================================================
//                MAIN LOOP
//======================================================================
String recvMessage;
void loop()
{

    int cb = udp.parsePacket();
    if (!cb) 
    {
      //If serial data is recived send it to UDP
      if(Serial.available()>0)
        {
        udp.beginPacket(ServerIP, 2000);  //Send Data to Master unit
        //Send UDP requests are to port 2000
        
        char a[1];
        a[0]=char(Serial.read()); //Serial Byte Read
        udp.write(a,1); //Send one byte to ESP8266 
        udp.endPacket();
        }
    }
    else {
      // We've received a UDP packet, send it to serial
      for(int i = 0; i<UDP_TX_PACKET_MAX_SIZE; i++) packetBuffer[i] = 0;
      udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); // read the packet into the buffer, we are reading only one byte
      recvMessage=packetBuffer;
      recvMessageHandler(recvMessage);
      delay(20);
    }
}
//=======================================================================

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

void Go_Backward()
{
digitalWrite(dir, LOW); //Rotate stepper motor in anti clock wise direction ////change to HIGH when on slider

          for( i=1;i<=steps;i++){
          digitalWrite(stepping, HIGH);
        
          digitalWrite(stepping, LOW);
          delay(1);}
}
void Restart()
{
  digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
  Calculate_Distance();
  Calculate_Steps(); 
  Go_Backward(); 
  pos = 0; 
}

 float Calculate_Distance()
{
cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
in = cm *.393701;
in = (int(in * 100.0)) / 100.0; //Keep two decimal places
Serial.print("Distance\t=\t");
Serial.print(in);
Serial.print("in");
Serial.println();
return in;
}

float Calculate_Steps()
{
  //steps  = 0;
  steps = ((12400 * in )/ 48 );
  //steps = int(12400.0 - Steps);
  return steps; 
} 

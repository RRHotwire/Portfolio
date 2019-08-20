#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "Switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
bool ShowerHeadOn();
bool ShowerHeadOff();

// Change this before you flash
const char* ssid = "FLQ6C";
const char* password = "0CB636E86F";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *office = NULL;


bool isShowerHeadOn = false;




// Motor -------------------------

#define SLEEP D6//6 actually enable
#define STEP D5 //4
#define DIR D7 //2

#define STEPS_PER_ROTATION 300
boolean outstandingWork = false;


unsigned long TimerTravel;    //"ALWAYS use unsigned long for timers"

//End of motor--------------------


 
void setup()
{
  Serial.begin(9600);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    office = new Switch("Shower Head", 80, ShowerHeadOn, ShowerHeadOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*office);
  //  upnpBroadcastResponder.addDevice(*kitchen);
  }



// Motor -------------------

  pinMode(SLEEP,OUTPUT);
  pinMode(STEP,OUTPUT);
  pinMode(DIR,OUTPUT);
  
//end motor ---------------


}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      office->serverLoop();
	 }




       if (((millis()-TimerTravel) <= 7100UL) && (outstandingWork == true))  // Set the 'UL' figure to the tine it takes to open or close your curtain
      {
      Serial.println(F("Moving Shower Head")); 
      moveTrack();
      }
      else
      {
        outstandingWork = false;
       
        digitalWrite(SLEEP,HIGH);
      }


      
}

bool ShowerHeadOn() {
    Serial.println("Switch 1 turn on ...");


          Serial.print("Track turned on.... ShowerHead is moving forward...");
          closeTrack();

          
    isShowerHeadOn = true;
    return isShowerHeadOn;
}

bool ShowerHeadOff() {
    Serial.println("Switch 1 turn off ...");

          Serial.print("Track turned off...  ShowerHead is moving backward...");
          openTrack();

    isShowerHeadOn = false;
    return isShowerHeadOn;
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


void openTrack() {
  Serial.print("Shower Head is moving forward...");
  
        Serial.println(F("dir HIGH"));
        digitalWrite(DIR,HIGH);
        TimerTravel= millis();
        Serial.println(F("SLEEP LOW"));
        digitalWrite(SLEEP,LOW);
        delay(500);
        outstandingWork = true;

  
  

}


void closeTrack() {
    Serial.print("Shower Head is moving backward...");
  
        Serial.println(F("dir LOW"));
        digitalWrite(DIR,LOW);
        TimerTravel= millis();
        Serial.println(F("SLEEP LOW"));
        digitalWrite(SLEEP,LOW);
        delay(500);
        outstandingWork = true;
  

}

// for the motor -------------

void stepNow(int totalSteps) {
  Serial.print(totalSteps);
  Serial.println(F(" steps."));

  int i;
  for(i=0;i<totalSteps;++i) {
    digitalWrite(STEP,HIGH);
    delayMicroseconds(1100);
    digitalWrite(STEP,LOW);
    delayMicroseconds(1100);
  }
}

//----------------------------


void moveTrack() {
  delayMicroseconds(500);
  stepNow(STEPS_PER_ROTATION);
}

//------------------------------------------------------------------------------------
  #include <Wire.h>
  #include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------                                               
// Defining I/O Pins
//------------------------------------------------------------------------------------
  #define       TWI_FREQ  400000L   // I2C Frequency Setting To 400KHZ

  char*         TKDssid;
  char*         TKDpassword;
  IPAddress     TKDServer(192,168,4,1);
  WiFiClient    TKDClient;
//====================================================================================
int xpotPin = A0;         
int bpotPin = 16; 
int xval=0;       //Initialize a variable
int bval=0;
//====================================================================================
int i = 0;
int pos = 0;
int steps = 0; 
const int stepping = 0; 
const int dir = 2; 
//====================================================================================
  void setup() 
  {
    /* ----------------------------------------------------------------------
     * Setting The I2C Pins SDA, SCL
     * Because We Didnt Specify Any Pins The Defult
     * SDA = D4 <GPIO2>, SCL = D5 <GPIO14> For ESP8266 Dev Kit Node MCU v3
     --------------------------------------------------------------------- */
    Wire.begin();                   // Begginning The I2C
    
    // Setting Up The I2C Of The MPU9250 ------------------------------------
    Wire.setClock(TWI_FREQ);        // Setting The Frequency MPU9250 Require
        
    // Setting The Serial Port ----------------------------------------------
    Serial.begin(115200);           // Computer Communication
    
    // Setting The Mode Of Pins ---------------------------------------------
   
    
    // Print Message Of I/O Setting Progress --------------------------------
    Serial.println("\nI/O Pins Modes Set .... Done");

    // Starting To Connect --------------------------------------------------
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    WiFi.begin("TAKEONE");          // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process ---------------
    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

    // WiFi Connectivity ----------------------------------------------------
    CheckConnectivity();            // Checking For Connection

    // Stop Blinking To Indicate Connected ----------------------------------
    //digitalWrite(LED0, !HIGH);
    Serial.println("!-- Client Device Connected --!");

    // Printing IP Address --------------------------------------------------
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TKDServer);
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
    
    // Conecting The Device As A Client -------------------------------------
    TKDRequest();

  pinMode(xpotPin,INPUT);
  pinMode(bpotPin,INPUT); 
  pinMode(stepping, OUTPUT);
  pinMode(dir, OUTPUT);

  Restart(pos); //Restart function that resets position back to pos 0. 
  }

//====================================================================================
  
  void loop()
  {
xval = analogRead(xpotPin);   //Read Values from the xpotPin
bval = digitalRead(bpotPin);   
  Serial.print("X= ");            //Display "X="
  Serial.print(xval);
  Serial.print("POS = ");
  Serial.print(pos);
  delay(150);
  }

//====================================================================================

  
//====================================================================================

  void CheckConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.println("");
    }
  }

//====================================================================================

  void TKDRequest()
  {
    // First Make Sure You Got Disconnected
    TKDClient.stop();

    // If Sucessfully Connected Send Connection Message
    if(TKDClient.connect(TKDServer, 9001))
    {
      //========
    }
  }

//====================================================================================
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

void Restart (int pos)
{
  int currentPosition = pos;

  if (currentPosition == 0)
  {
    
  }

  else if (currentPosition == 1)
  {
    Go_Backward(3100);
    pos = 0;
  }

  else if (currentPosition == 2)
  {
    Go_Backward(6200);
    pos = 0;
  }

  else if (currentPosition == 3)
  {
    Go_Backward(9300);
    pos = 0;
  }

  else if (currentPosition == 4)
  {
    Go_Backward(12400);
    pos = 0;
  }
  
}

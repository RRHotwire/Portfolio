//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>

  // Authentication Variables
  char*       TKDssid;              // SERVER WIFI NAME
  char*       TKDpassword;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
  #define     MAXSC     2           // MAXIMUM NUMBER OF CLIENTS
  WiFiServer  TKDServer(9001);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  TKDClient[MAXSC];     // THE SERVER CLIENTS
//====================================================================================
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
//======================================================================================
  void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Computer Communication

  pinMode(xpotPin,INPUT);
  pinMode(bpotPin,INPUT); 
  pinMode(stepping, OUTPUT);
  pinMode(dir, OUTPUT);

  Restart(pos); //Restart function that resets position back to pos 0. 
    
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    // Setting Up A Wifi Access Point
    SetWifi("TAKEONE", "");
  }

//====================================================================================
  
  void loop()
  {
  xval = analogRead(xpotPin);   //Read Values from the xpotPin
  bval = digitalRead(bpotPin);   
  Serial.print("X=");            //Display "X="
  Serial.print(xval);
    IsClients2();
  if (xval >= 0 && xval <= 810)
  {
   //lcd.setCursor(0,1);
   Serial.print("UP"); 
   if (pos < 4)
   {
   Go_Forward(3100);
   pos++;
   }
   else 
   {
   Go_Forward(0); 
   }
  }
 
  
  else if (xval >= 827 && xval <= 1024)
  {
   //lcd.setCursor(0,1);
   Serial.print("DOWN"); 
    if (pos > 0)
    {
   Go_Backward(3100);
   pos--;
    }
    
    else {
    Go_Backward(0);  }
  }
   
  }

//====================================================================================
  
  void SetWifi(char* Name, char* Password)
  {
    // Stop Any Previous WIFI
    WiFi.disconnect();

    // Setting The Wifi Mode
    WiFi.mode(WIFI_AP_STA);
    Serial.println("WIFI Mode : AccessPoint Station");
    
    // Setting The Access Point
    TKDssid      = Name;
    TKDpassword  = Password;
    
    // Starting The Access Point
    WiFi.softAP(TKDssid, TKDpassword);
    Serial.println("WIFI < " + String(TKDssid) + " > ... Started");
    
    // Wait For Few Seconds
    delay(1000);
    
    // Getting Server IP
    IPAddress IP = WiFi.softAPIP();
    
    // Printing The Server IP Address
    Serial.print("AccessPoint IP : ");
    Serial.println(IP);

    // Starting Server
    TKDServer.begin();
    Serial.println("Server Started");
  }

//====================================================================================

  void IsClients()
  {
    if(TKDServer.hasClient())
    {
      WiFiClient TKDClient = TKDServer.available();
      
//if(digitalRead(LED0) == HIGH) digitalWrite(LED0, LOW);
      
  //    digitalWrite(LED1, HIGH);

      while(1)
      {

        //--[ Draft ] ---------------------------------------------------

        
              
        //---------------------------------------------------------------
        // If Clients Are Connected 
        //---------------------------------------------------------------
        
        if(TKDClient.available())
        {
          // Here We Read The Message
          String Message = TKDClient.readStringUntil('\r');
          // Here We Print The Message On The Screen
          Serial.println(Message);
          // Here We Reply To The Client With A Message
          TKDClient.print("\nWe Got Your Message");
          TKDClient.flush();
        }
        
        //---------------------------------------------------------------
        // If Clients Are Disconnected
        //---------------------------------------------------------------
        
        if(!TKDClient || !TKDClient.connected())
        {
          // Here We Turn Off The LED To Indicated The Its Disconnectted
         // digitalWrite(LED1, LOW);
          // Here We Jump Out Of The While Loop
          break;
        }

        //----------------------------------------------------------------
        
      }
      
    }
    else
    {
      // This LED Blinks If No Clients Where Available
    //  digitalWrite(LED0, HIGH);
    //  delay(250);
     // digitalWrite(LED0, LOW);
     // delay(250);
    }
  }

//====================================================================================

  void IsClients2()
  {
    if (TKDServer.hasClient())
    {
      for(int i = 0; i < MAXSC; i++)
      {
        //find free/disconnected spot
        if (!TKDClient[i] || !TKDClient[i].connected())
        {
          if(TKDClient[i]) TKDClient[i].stop();
          TKDClient[i] = TKDServer.available();
          Serial.print("New Client : "); Serial.print(String(i+1) + " - ");
          continue;
        }
      }
      // no free / disconnected spot so reject
   //   digitalWrite(LED1, HIGH);
      WiFiClient TKDClient = TKDServer.available();
      TKDClient.stop();
    }

    //check clients for data -------------------------------------------------------
    
    for(int i = 0; i < MAXSC; i++)
    {
      if (TKDClient[i] && TKDClient[i].connected())
      {
        if(TKDClient[i].available())
        {
          // If Any Data Was Available We Read IT
          while(TKDClient[i].available()) 
          {
            // Read From Client
            Serial.println(TKDClient[i].readStringUntil('\n'));
            // Reply To Client
            TKDClient[i].println("<OK>");
          }
        }
      }
    }
  }

//====================================================================================

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

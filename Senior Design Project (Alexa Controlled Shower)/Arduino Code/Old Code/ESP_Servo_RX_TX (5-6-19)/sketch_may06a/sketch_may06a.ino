#include <SoftwareSerial.h>       //Software Serial library
#include <Servo.h>                //Servo library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 of Arduino act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true                
#define servoPin 9                //Servo is connected to Pin 9 of Arduino
Servo myServo;                    //variable for servo

int curPos = 90;      
int minPos = 10; 
int maxPos = 170;

void setup()
{
  myServo.attach(servoPin);
  myServo.write(maxPos);
  myServo.detach();
  Serial.begin(9600);
  espSerial.begin(115200);
  
  espData("AT+RST\r\n", 3000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1\r\n", 3000, DEBUG);                 //Set the ESP mode as station mode
  
  /* In the next command, enter your WiFi Network name in place of SSID and 
  password in place of PASSWORD*/
  
  espData("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n", 3000, DEBUG);   //Connect to WiFi network
  
 /* while(!esp.find("OK")) 
  {          
      //Wait for connection
  }
  delay(3000);
*/
  espData("AT+CIFSR\r\n", 3000, DEBUG);          //Show the IP address
  
  /* The following statement is to assign Static IP Address to ESP8266. 
  The syntax is AT+CIPSTA=<ip>,<gateway>,<netmask>. 
  This will assign a Static IP Address of 192.168.1.254 (in my case)
  to the ESP8266 Module. Change this value as per your requirements i.e. this IP address 
  shouldn't conflict with any other device. 
  Also, the second and third parameters are Gateway and Net Mask values. 
  You can get these values from ipconfig command in command prompt*/
  
  espData("AT+CIPSTA=\"192.168.1.254\",\"192.168.1.1\",\"255.255.255.0\"\r\n", 3000, DEBUG); // Assign Static IP Address
  //AT+CIPSTA="192.168.6.100","192.168.6.1","255.255.255.0" 
  
  espData("AT+CIPMUX=1\r\n", 3000, DEBUG);       //Allow multiple connections
  espData("AT+CIPSERVER=1,80\r\n", 3000, DEBUG); //Start the Web Server on port 80
}

void loop()
{
  if (espSerial.available())     //check if there is data available on ESP8266
  {
    if (espSerial.find("+IPD,")) //if there is a new command
    {
      String msg;
      espSerial.find("?");                           //run cursor until command is found
      msg = espSerial.readStringUntil(' ');          //read the message
      String command = msg.substring(0, 5);    //command is informed in the first 5 characters "servo"
      String angleStr = msg.substring(6);      //next 3 characters inform the desired angle
      int angleValue = angleStr.toInt();            //convert to integer
      if (DEBUG) 
      {
        Serial.println(command);
        Serial.println(angleValue);
      }
      delay(100);
      //move servo to desired angle
      if(command == "servo") 
      {
         //limit input angle
         if (angleValue >= maxPos) 
         {
           angleValue = maxPos;
         }
         if (angleValue <= minPos) 
         {
           angleValue = minPos;
         }
         myServo.attach(servoPin); //attach servo
         while(curPos != angleValue) 
         {
           if (curPos > angleValue) 
           {
             curPos -= 1;
             myServo.write(curPos);
             delay(10);
           }
           if (curPos < angleValue) 
           {
             curPos += 1;
             myServo.write(curPos);
             delay(10);
           }
         }
         myServo.detach(); //dettach
      }
  }
  }
  }
String espData(String command, const int timeout, boolean debug)
{
  String response = "";
  espSerial.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

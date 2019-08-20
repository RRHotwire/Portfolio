/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int xpotPin = A0;
//int ypotPin = A1;            
int bpotPin = 16; 
int xval=0;       //Initialize a variable
//in yval=0;
int bval=0;
void setup()                                                                                     
{
  //lcd.begin(16,2);   //Display Address
  pinMode(xpotPin,INPUT);
 // pinMode(ypotPin,INPUT);
  pinMode(bpotPin,INPUT); 
  //lcd.print("  Welcome to ");
  //lcd.setCursor(0,1);
  //lcd.print("     Smraza");
 // delay(2000);
 // lcd.clear();
 Serial.begin(115200);
}
void loop()
{ 
  xval = analogRead(xpotPin);   //Read Values from the xpotPin
 // yval = analogRead(ypotPin);   
  bval = digitalRead(bpotPin);   
//  lcd.clear();                 //clear display
 // lcd.setCursor(0, 0) ;       //Display position
  Serial.print("X=");            //Display "X="
  Serial.print(xval);
  //lcd.setCursor(7, 0) ;  
 // Serial.print("Y=");       
 // Serial.print(yval); 
  if (xval >= 0 && xval <= 810)
  {
   //lcd.setCursor(0,1);
   Serial.print("UP"); 
  }
  else if (xval >= 811 && xval <= 826)
  {
 // lcd.setCursor(0,1);
  Serial.print("CENTER");
  }
  
  else if (xval >= 827 && xval <= 1024)
  {
   //lcd.setCursor(0,1);
   Serial.print("DOWN"); 
  }
  
/*
  if (yval >= 0 && yval <= 503)
  {
  // lcd.setCursor(7,1);
   Serial.print("RIGHT"); 
  }
  else if (yval >= 505 && yval <= 1023)
  {
   //lcd.setCursor(7,1);
   Serial.print("LEFT"); 
  }
  
   else
  {
 // lcd.setCursor(7,1);
  lcd.print("CENTER");
  }
 /* if (bval==LOW)
  {
    lcd.setCursor(0, 1) ;   
    lcd.print("Button ON"); 
  }
  else
  {
  lcd.setCursor(0, 1) ;
  lcd.print("Button OFF"); 
  }
 */
  delay(150); //After 150ms the screen will be refreshed
}

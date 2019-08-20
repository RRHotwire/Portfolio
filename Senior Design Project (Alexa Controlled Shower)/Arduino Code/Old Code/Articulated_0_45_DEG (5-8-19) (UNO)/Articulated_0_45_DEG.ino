//www.elegoo.com
//2018.12.19
#include <Servo.h>
Servo myservo;

void setup(){
  myservo.attach(7); 


} 
void loop(){


  myservo.write(0);
  delay(10000);
  myservo.write(45);
  delay(10000);

 

  
}  

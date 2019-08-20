//www.elegoo.com
//2018.12.19
#include <Servo.h>
Servo myservo;

void setup(){
  myservo.attach(13); //D13
//  myservo2.attach(12);

} 
void loop(){
/*  for (int i = 0; i <= 180; i++) // for loop that makes Servo 1 (Pin 13) and Servo 2 (Pin 12) move 0 to 180
  {
  myservo.write(i);
//  myservo2.write(i);
  delay(5);
  }

for (int j = 180; j >= 0; j--) // for loop that makes Servo 1 (Pin 13) and Servo 2 (Pin 12) move 0 to 180
  {
  myservo.write(j);
//  myservo2.write(i);
  delay(5);
  }
  
*/

  myservo.write(90);
  delay(3000);
  myservo.write(45);
  delay(3000);
  myservo.write(135);
  delay(3000);

 
  
}

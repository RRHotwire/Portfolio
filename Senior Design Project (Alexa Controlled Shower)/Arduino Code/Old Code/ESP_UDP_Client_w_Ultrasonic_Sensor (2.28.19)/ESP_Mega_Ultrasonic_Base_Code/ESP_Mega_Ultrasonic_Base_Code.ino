
int i = 0;
int pos = 0;
int steps =0; 
float Steps=0;
//const int stepping = 0; //D3
//const int dir = 2; //D4

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
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

//======================================================================
//                Motor Movement Functions
//======================================================================

void loop()
{
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    Restart();
    delay(1000);

}
//=======================================================================

void Go_Forward ()
{
//digitalWrite(dir, HIGH); //Rotate stepper motor in clock wise direction //change to LOW when on slider
Serial.print("Going Forward\t=\t");
Serial.print(steps);
Serial.print("steps");
Serial.println();
          for( i=1;i<=steps;i++){
          delay(1);
          }  
}

void Go_Backward()
{
//digitalWrite(dir, LOW); //Rotate stepper motor in anti clock wise direction ////change to HIGH when on slider
Serial.print("Going Backward\t=\t");
Serial.print(steps);
Serial.print("steps");
Serial.println();
          for( i=1;i<=steps;i++){
        //  delay(1);
          }
}
void Restart()
{
  Calculate_Distance();
  Calculate_Steps(); 
  Go_Backward(); 
 // pos = 0; 
 return 0;
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

int Calculate_Steps()
{
  //steps  = 0;
  Steps = ((12400 * in )/ 48 );
  steps = int(12400.0 - Steps);
  return steps; 
} 


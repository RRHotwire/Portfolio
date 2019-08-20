const int TrigPin = 2;
const int EchoPin = 3;
float cm;
float in;
void setup()
{
Serial.begin(9600);
pinMode(TrigPin, OUTPUT);
pinMode(EchoPin, INPUT);
}
void loop()
{
digitalWrite(TrigPin, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin, LOW);
cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
in = cm *.393701;
in = (int(in * 100.0)) / 100.0; //Keep two decimal places
Serial.print("Distance\t=\t");
Serial.print(in);
Serial.print("in");
Serial.println();
delay(100);
}



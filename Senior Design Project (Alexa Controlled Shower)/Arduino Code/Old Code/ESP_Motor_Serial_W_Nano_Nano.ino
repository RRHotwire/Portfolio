#include <SoftwareSerial.h>
SoftwareSerial ArduinoNano(3,2);


void setup(){
	
	Serial.begin(115200);
	ArduinoNano.begin(4800);
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop(){
	
	while(ArduinoNano.available()>0){
	float val = ArduinoNano.parseFloat();
	if(ArduinoNano.read()== '\n'){
	Serial.println(val);
	}
}
delay(30);
if (ArduinoNano.read() == HIGH)
{
  digitalWrite(12, HIGH);
}
}

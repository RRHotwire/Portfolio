/* 
 *  Water temperature and flow control system.
 *  
 * Hardware: Thermsistor, fixed resistor, two motorized ball valves, motor driver and arduino
 * 
 * Software: Functions to close and open the ball valves and use the thermsistor to measure temperature
 *           and add a layer of safety by adjusting the valves to reach a safe temperature.
 * 
 * 
 */


//Ball valve for cold water
int enableCold = 10; //ENA 
int in1_cold = 9;   //IN1
int in2_cold = 8;   //IN2

 
//Ball valve for hot water
int enableHot = 5; //ENB
int in1_hot = 7; //IN3
int in2_hot = 6; //IN4

//
int coldTime = 0;
int hotTime = 0;

#define turnTime 1 //determines how long a valve turns
#define maxTurnTime 5000  //Assuming 5 seconds to close the valve fully
#define ThermistorPin 0   //Using Analog Pin 0 on the arduino to read the sensor value

//Values to check temperature

float Vin=5.0;     // [V], using Arduino 5V pin      
float Rt=5000;    // Resistor t [ohm]
float R0=5000;    // value of rct in T0 [ohm]
float T0=298.15;   // use T0 in Kelvin [K]
float Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0
// use the datasheet to get this data.
float T1=273.15;      // [K] in datasheet 0º C
float T2=373.15;      // [K] in datasheet 100° C
float RT1=16330;   // [ohms]  resistence in T1
float RT2=339.9;    // [ohms]   resistence in T2
float beta=0.0;    // initial parameters [K]
float Rinf=0.0;    // initial parameters [ohm]   
float TempK=0.0;   // variable output
float TempC=0.0;   // variable output
float TempF=0.0;

void waterSafetyFunction(){
  if(checkTemp() > 110){
    CloseHot();
  }
  else if(checkTemp() < 60 && hotTime <= maxTurnTime){
    CloseCold();
  }
}

float checkTemp(){

  Vout=Vin*((float)(analogRead(ThermistorPin))/1024.0); // calc for ntc
  Rout=(Rt*Vout/(Vin-Vout));

  TempK=(beta/log(Rout/Rinf)); // calc for temperature
  TempC=TempK-273.15;
  TempF=(TempC *9/5) + 32;
  delay(500);
  Serial.println(TempF);
  return TempF;

  //return 0;



}

void turnOffHot(){
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, LOW);  
  
}
void turnOffCold(){
   digitalWrite(in1_hot, LOW);
  digitalWrite(in2_hot, LOW);
}
void enableValvePins(){
  pinMode(enableCold, OUTPUT);
  pinMode(enableHot, OUTPUT);
  pinMode(in1_cold, OUTPUT);
  pinMode(in2_cold, OUTPUT);
  pinMode(in1_hot, OUTPUT);
  pinMode(in2_hot, OUTPUT);
}

void fullCloseValves(){
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, HIGH);
  analogWrite(enableCold, 200);
  
  digitalWrite(in1_hot, HIGH);
  digitalWrite(in2_hot, LOW);
  analogWrite(enableHot, 200);

  delay(maxTurnTime);
}
void OpenCold(){
  if(coldTime < maxTurnTime){
  digitalWrite(in1_cold, HIGH);
  digitalWrite(in2_cold, LOW);
  analogWrite(enableCold, 200);
  delay(turnTime);
  coldTime += turnTime; 
  }
}
void CloseCold(){
  if(coldTime > 0){
  digitalWrite(in1_cold, LOW);
  digitalWrite(in2_cold, HIGH);
  analogWrite(enableCold, 200);
  delay(turnTime);
  coldTime -= turnTime; 
  }
}
void OpenHot(){
  if(hotTime < maxTurnTime){
  digitalWrite(in1_hot, HIGH);
  digitalWrite(in2_hot, LOW);
  analogWrite(enableHot, 200);
  delay(turnTime);
  hotTime += turnTime;
  }
  
}
void CloseHot(){
  if(hotTime > 0){
  digitalWrite(in1_hot, LOW);
  digitalWrite(in2_hot, HIGH);
  analogWrite(enableHot, 200);
  delay(turnTime);
  hotTime -= turnTime; 
  }
}

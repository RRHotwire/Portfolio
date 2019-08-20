
float Vin=5.0;     // [V]        
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

//return 
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

void setup(){
  beta=(log(RT1/RT2))/((1/T1)-(1/T2));
  Rinf=R0*exp(-beta/T0);
}

void loop(){
  
}

/*
 * This is the Arduino code for  Voltage Sensor
 * you will measure voltage using this module which eliminate use extra resistors
 in the video also you will learn how to measure any voltages.
 * Permission granted to share this code given that this
 * note is kept with the code.
 * Disclaimer: this code is "AS IS" and for educational purpose only.
 * 
 */
/*

 */
int offset =20;// set the correction offset value
void setup() {

  Serial.begin(9600);
}

void loop() {
  
  int volt = analogRead(A0);// read the input
  double voltage = map(volt,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
  
  voltage /=100;// divide by 100 to get the decimal values
  Serial.print("Voltage: ");
  Serial.print(voltage);//print the voltge
  Serial.println("V");

delay(500);
  
  
}

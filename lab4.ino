/* Motors - Written by Mr.K 2021.10.16
   This code is written to drive two motors forward, then reverse in a loop
   constant, motor control */

// declare constants for motor pins
const int LMotor_pin1 = 5; 
const int LMotor_pin2 = 6;
const int RMotor_pin1 = 9;
const int RMotor_pin2 = 10;

void setup() //set pins as OUTPUTS
{
  pinMode(LMotor_pin1, OUTPUT);
  pinMode(LMotor_pin2, OUTPUT);
  pinMode(RMotor_pin1, OUTPUT);
  pinMode(RMotor_pin2, OUTPUT);
}

void loop()
{
  motors(255, 0, 0, 255, 250); //left
  motors(255, 0, 255, 0, 2500); //forward
  motors(0, 255, 255, 0, 550); //right
  motors(255, 0, 255, 0, 4000); //forward
  motors(255, 0, 0, 255, 500); //left
  motors(255, 0, 255, 0, 2400);
  motors(255, 0, 0, 255, 500); //left
  motors(255, 0, 255, 0, 2400);
  motors(255, 0, 0, 255, 500); //left
  motors(255, 0, 255, 0, 4000); //forward
  motors(0, 255, 255, 0, 700); //right
  motors(255, 0, 255, 0, 2400);
}

void motors(int lVal1, int lVal2, int rVal1, int rVal2, int delayTime)
{
  analogWrite(LMotor_pin1, lVal1); //Left motor forward
  analogWrite(LMotor_pin2, lVal2);  
  analogWrite(RMotor_pin1, rVal1); //Right motor forward
  analogWrite(RMotor_pin2, rVal2);
  delay(delayTime);
}

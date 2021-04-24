#include<Stepper.h> //include stepper library 

int led = 12; // the pin that the LED is attached to
int sensor = 13; // the pin that the sensor is attached to
int val = 0; // variable to store the sensor status (value)
int in1Pin = 4; //the pin that the phase 1 is attached to
int in2Pin = 5; //the pin that the phase 2 is attached to
int in3Pin = 6; //the pin that the phase 3 is attached to
int in4Pin = 7; //the pin that the phase 4 is attached to
int step_num =2;
Stepper motor(64, in1Pin, in2Pin, in3Pin, in4Pin);

void setup() {
pinMode(led, OUTPUT); // initialize LED as an output
pinMode(sensor, INPUT); // initialize sensor as an input
pinMode(in1Pin, OUTPUT); // initialize in1pin as an output
pinMode(in2Pin, OUTPUT); // initialize in2pin as an output
pinMode(in3Pin, OUTPUT); // initialize in3pin as an output
pinMode(in4Pin, OUTPUT); // initialize in4pin as an output
motor.setSpeed(300); //speed of the motor 
}

void loop()
{ 
val = digitalRead(sensor); // read sensor value
if (val == HIGH)
{
digitalWrite(led, HIGH); // turn LED ON
delay(1500);
motor.step(360);
}

else
{
digitalWrite(led, LOW); // turn LED OFF
motor.step(0);
delay(10);
}
}

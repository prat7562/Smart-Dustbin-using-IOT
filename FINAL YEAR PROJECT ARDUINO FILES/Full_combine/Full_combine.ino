#include<Stepper.h> //include stepper library 
#define MQ2pin (1)

//PIR and Motor
int LED_PIR = 1; // the pin that the LED is attached to
int sensor = 13; // the pin that the sensor is attached to
int val = 0; // variable to store the sensor status (value)
int in1Pin = 4; //the pin that the phase 1 is attached to
int in2Pin = 5; //the pin that the phase 2 is attached to
int in3Pin = 6; //the pin that the phase 3 is attached to
int in4Pin = 7; //the pin that the phase 4 is attached to
int step_num =1;
Stepper motor(64, in1Pin, in2Pin, in3Pin, in4Pin);

//Ultrasonic
const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 2; // Echo Pin of Ultrasonic Sensor
int LED_uv = 12;

//Gas
float sensorValue;  //variable to store sensor value
int LED_Gas = 2;

void setup()
{
//PIR and Motor
  pinMode(LED_PIR, OUTPUT); // initialize LED as an output
  pinMode(sensor, INPUT); // initialize sensor as an input
  pinMode(in1Pin, OUTPUT); // initialize in1pin as an output
  pinMode(in2Pin, OUTPUT); // initialize in2pin as an output
  pinMode(in3Pin, OUTPUT); // initialize in3pin as an output
  pinMode(in4Pin, OUTPUT); // initialize in4pin as an output
motor.setSpeed(300); //speed of the motor

//Ultrasonic
  pinMode(12,OUTPUT);
  Serial.begin(9600); // Starting Serial Terminal

//Gas
  Serial.begin(9600); // sets the serial port to 9600
  Serial.println("Gas sensor warming up!");
  delay(20000); // allow the MQ-2 to warm up
  pinMode(LED_Gas, OUTPUT);
  digitalWrite(LED_Gas,LOW);
}
//Ultrasonic
long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}

void loop()
{
//PIR and Motor
{ 
val = digitalRead(sensor); // read sensor value
if (val == HIGH)
{
digitalWrite(LED_PIR, HIGH); // turn LED ON
delay(1500);
motor.step(360);
motor.step(360);
}

else
{
digitalWrite(LED_PIR, LOW); // turn LED OFF
motor.step(0);
delay(10);
}
}
//Ultrasonic
{
long duration, cm;
pinMode(pingPin, OUTPUT);
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin, LOW);

pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
cm = microsecondsToCentimeters(duration);
Serial.print(cm);
Serial.print("cm");
Serial.println();

if(cm <= 26)
{
  digitalWrite(11,HIGH);
}
else
  digitalWrite(11,LOW);
}

//Gas
{
  sensorValue = analogRead(MQ2pin); // read analog input pin 1
  
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  
  if(sensorValue > 280)
  {
   digitalWrite(LED_Gas,HIGH); 
   delay(1000); // wait 2s for next reading
  }
  else
  {
   digitalWrite(LED_Gas,LOW); 
   delay(1000); // wait 2s for next reading
  }
}
}

#define MQ2pin (1)
float sensorValue;  //variable to store sensor value
int LED2 = 11;

const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 8; // Echo Pin of Ultrasonic Sensor
int LED = 12;

void setup()
{
pinMode(12,OUTPUT);
Serial.begin(9600); // Starting Serial Terminal
Serial.println("Gas sensor warming up!");
delay(20000); // allow the MQ-2 to warm up
pinMode(LED2, OUTPUT);
digitalWrite(LED2,LOW);
}

long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}

void loop() {
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

char cma[50];

sprintf(cma, "%ld", cm);

Serial.print(cma);
Serial.print("cm");
Serial.println();

if(cm <= 15)
{
  digitalWrite(12,HIGH);
}
else
  digitalWrite(12,LOW);
}

sensorValue = analogRead(MQ2pin); // read analog input pin 1
  
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  
  if(sensorValue > 238)
  {
   digitalWrite(LED2,HIGH); 
   delay(1000); // wait 2s for next reading
  }
  else
  {
   digitalWrite(LED2,LOW); 
   delay(1000); // wait 2s for next reading
  }
}

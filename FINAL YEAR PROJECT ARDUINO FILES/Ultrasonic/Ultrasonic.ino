const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 2; // Echo Pin of Ultrasonic Sensor
int LED = 12;

void setup()
{
pinMode(12,OUTPUT);
Serial.begin(9600); // Starting Serial Terminal
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

if(cm <= 26)
{
  digitalWrite(12,HIGH);
}
else
  digitalWrite(12,LOW);
}
}

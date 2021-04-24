#define MQ2pin (1)

float sensorValue;  //variable to store sensor value
int LED = 12;

void setup()
{
  Serial.begin(9600); // sets the serial port to 9600
  Serial.println("Gas sensor warming up!");
  delay(20000); // allow the MQ-2 to warm up
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
}

void loop()
{
  sensorValue = analogRead(MQ2pin); // read analog input pin 1
  
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  
  if(sensorValue > 280)
  {
   digitalWrite(LED,HIGH); 
   delay(1000); // wait 2s for next reading
  }
  else
  {
   digitalWrite(LED,LOW); 
   delay(1000); // wait 2s for next reading
  }
}

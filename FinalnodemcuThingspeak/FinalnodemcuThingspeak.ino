//Header Files
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <WiFiClient.h>
#include <Servo.h> 

String apiKey = "EBSKCHUH33PN6WY5";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "LODHA";     // replace with your wifi ssid and wpa2 key
const char *pass =  "kccprk12345";
const char* server = "api.thingspeak.com";
WiFiClient client;
//defining Ultrasonic_WET
const int pingPin_wet = 0; // D3 
const int echoPin_wet = 2; // D4
long duration_wet, cm_wet;

//defining Ultrasonic_DRY
const int pingPin_dry = 14; // D5 
const int echoPin_dry = 12; // D6
long duration_dry, cm_dry;

//defining Servo_door
int servopin_door = 5; // D1
Servo Servo_door; 

//defining Servo_tilt
int servopin_tilt = 4; // D2
Servo Servo_tilt; 

//defining Pir Sensor
int calibrationTime = 30;//the time when the sensor outputs a low impulse
long unsigned int lowIn;//the amount of milliseconds the sensor has to be low
long unsigned int pause = 5000;//before we assume all motion has stopped
boolean lockLow = true;
boolean takeLowTime;
int pirPin = 16; // D0

//defining Gas Sensor
int read_gas;
int dPin_gas = 13; // D7

//defining Moisture Sensor
int read_moisture;
int dPin_moisture = 15; // D8
 
//Void setup 
void setup()
{
Serial.begin(9600);
//WiFi Setup
  {
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.println("Connecting to WiFi..");
    }
    Serial.print("Connected to the WiFi network : ");
    Serial.println(ssid);
  }
//Pir Sensor Setup
  {
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  Serial.print("calibrating PIR sensor ");
  for(int i = 0; i < calibrationTime; i++)
  {
  Serial.print(".");
  delay(1000);
  }
  Serial.println("PIR SENSOR ACTIVED");
  delay(50);
  }
//Gas & Moisture Sensor Setup
  {
  pinMode(dPin_gas, OUTPUT);//Gas_sensor
  pinMode(dPin_moisture, OUTPUT);//Moisture_sensor
  digitalWrite(dPin_gas, HIGH); // Turn gas On
  digitalWrite(dPin_moisture, LOW); // Turn moisture Off
  Serial.println("Gas sensor warming up!");
  delay(20000); //MQ-2 warming up
  }
//Servo Motor Setup
  {
  Servo_door.attach(servopin_door);
  Servo_tilt.attach(servopin_tilt); 
  }
}
//void setup finish

void loop()
{
PirSensor();
getSendData_dry();
getSendData_wet();
GasSensor();
Thifunc();
}//loop finish

void PirSensor()
{
  if(digitalRead(pirPin) == HIGH)
  {
   door_open();
   
   if(lockLow)
   {
    lockLow = false;
	  Serial.println("---");
	  Serial.print("motion detected at : ");
	  Serial.print(millis()/1000);
	  Serial.println(" sec");
	  delay(50);
   }
   takeLowTime = true;
   MoistureSensor();
  }
  if(digitalRead(pirPin) == LOW)
  {
   door_close();
   if(takeLowTime)
   {
    lowIn = millis(); //save the time of the transition from high to LOW
	  takeLowTime = false;
   }
   //if the sensor is low for more than the given pause,
   //we assume that no more motion is going to happen
   if(!lockLow && millis() - lowIn > pause)
   {
   //makes sure this block of code is only executed again after
   //a new motion sequence has been detected
   lockLow = true;
   Serial.print("motion ended at : "); //output
   Serial.print((millis() - pause)/1000);
   Serial.println(" sec");
   delay(50);
   }
  }
}//PIR finish

void door_open()
{
   // Make servo go to 90 degrees 
   Servo_door.write(90); 
   delay(1000); 
   // Make servo go to 0 degrees 
   Servo_door.write(0); 
   delay(5000);
}//door open finish

void door_close()
{
   // Make servo go to 0 degrees 
   Servo_door.write(0); 
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo_door.write(90); 
   delay(5000);
}//door close finish

void MoistureSensor()
{
    read_moisture = analogRead_Moisture(); // Read Analog value of second sensor
    delay(2000);
    Serial.print("Moisture sensor = ");
    Serial.println(read_moisture);
    if(read_moisture < 900)
    {
     Tilt_Dry();
     delay(2000);
    }
    else
    {
     Tilt_Wet();
     delay(2000);
    }	
}//moisture and gas finish

int analogRead_Moisture()
{
 digitalWrite(dPin_gas, LOW); //  Turn gas Off
 digitalWrite(dPin_moisture, HIGH); // Turn moisture On
 return analogRead(0);
}//read Moisture

void Tilt_Dry()
{
   // Make servo go to 90 degrees 
   Servo_tilt.write(90); 
   delay(1000); 
   // Make servo go to 0 degrees 
   Servo_tilt.write(135); 
   delay(5000);

   Servo_tilt.write(90); 
   delay(1000);
}//tilt dry finish

void Tilt_Wet()
{
   // Make servo go to 90 degrees 
   Servo_tilt.write(90); 
   delay(1000); 
   // Make servo go to 0 degrees 
   Servo_tilt.write(40); 
   delay(5000);

   Servo_tilt.write(90); 
   delay(1000);
}// tilt wet finish

void getSendData_dry()
{
pinMode(pingPin_dry, OUTPUT);
digitalWrite(pingPin_dry, LOW);
delayMicroseconds(2);
digitalWrite(pingPin_dry, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin_dry, LOW);

pinMode(echoPin_dry, INPUT);
duration_dry = pulseIn(echoPin_dry, HIGH);
cm_dry = microsecondsToCentimeters(duration_dry);
Serial.print("cm_dry = ");   
Serial.println(cm_dry); 
}//dry sensor finish

void getSendData_wet()
{
pinMode(pingPin_wet, OUTPUT);
digitalWrite(pingPin_wet, LOW);
delayMicroseconds(2);
digitalWrite(pingPin_wet, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin_wet, LOW);

pinMode(echoPin_wet, INPUT);
duration_wet = pulseIn(echoPin_wet, HIGH);
cm_wet = microsecondsToCentimeters(duration_wet);
Serial.print("cm_wet = ");   
Serial.println(cm_wet);
}//wet sensor finish

long microsecondsToCentimeters(long microseconds)
{
 return microseconds / 29 / 2;
}//ultrasonic sensor calculation finish

void GasSensor()
{
    read_gas = analogRead_Gas(); // Read Analog value of first sensor
    delay(2000);
    Serial.print("Gas sensor = ");
    Serial.println(read_gas);
    if(read_gas > 238)
    {
     Serial.println("Harmfull Gas detected");
     delay(1000); // wait 2s for next reading
    }
 	else
 	  {
     Serial.println("No Harmfull Gas");  
     delay(1000); // wait 2s for next reading
 	  }
}//gas finish

int analogRead_Gas()
{
 digitalWrite(dPin_gas, HIGH); // Turn gas On
 digitalWrite(dPin_moisture, LOW); // Turn moisture Off
 return analogRead(0);
}// read Gas

void Thifunc() 
{
 if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
 {  
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(cm_wet);

  postStr +="&field2=";
  postStr += String(cm_dry);

  postStr +="&field3=";
  postStr += String(read_gas);
  postStr += "\r\n\r\n\r\n";
 
  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
 
  Serial.print("Wet = : ");
  Serial.print(cm_wet);
  Serial.print("Dry = : ");
  Serial.print(cm_dry);
  Serial.print("Gas = : ");
  Serial.print(read_gas);
  Serial.println("Send to Thingspeak.");
  }
  client.stop();
  Serial.println("Waiting...");
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
} // ThingSpeak finish
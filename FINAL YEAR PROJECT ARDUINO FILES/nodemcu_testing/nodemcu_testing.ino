//Header Files
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <Stepper.h>

//server id's and pass 
const char* ssid = "Redmipratik";
const char* password =  "pratik12";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "pratik";
const char* mqttPassword = "Pratik@12";

//defining Ultrasonic_WET
const int pingPin_wet = 3; // RX
const int echoPin_wet = 1; // TX
long duration_wet, cm_wet;

//defining Ultrasonic_DRY
const int pingPin_dry = 9; // SD2
const int echoPin_dry = 10; // SD3
long duration_dry, cm_dry;

//defining Stepper_door
#define STEPS 32
Stepper stepper_door(STEPS, 5, 4, 0, 2);
int val_door = 1000;

//defining Stepper_tilt
Stepper stepper_tilt(STEPS, 14, 12, 13, 15);
int val_tilt = 1000;

//defining Pir Sensor
int calibrationTime = 30;//the time when the sensor outputs a low impulse
long unsigned int lowIn;//the amount of milliseconds the sensor has to be low
long unsigned int pause = 5000;//before we assume all motion has stopped
boolean lockLow = true;
boolean takeLowTime;
int pirPin = 16; //D0

//defining Gas Sensor
int read_gas;
int dPin_gas = 6; //gpio's 6,7,8,11 pending

//defining Moisture Sensor
int read_moisture;
int dPin_moisture = 7; //gpio's 6,7,8,11 pending

WiFiClient wifiClient;
PubSubClient client(wifiClient);
 
//Void setup 
void setup()
{
Serial.begin(9600);
//WiFi Setup
  {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
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
  Serial.println("PIR SENSOR ACTIVE");
  delay(50);
  }
//Gas & Moisture Sensor Setup
  {
  pinMode(dPin_gas, OUTPUT);//Gas_sensor
  pinMode(dPin_moisture, OUTPUT);//Moisture_sensor
  }
//Stepper Motor Setup
  {
  stepper_door.setSpeed(1000);
  stepper_tilt.setSpeed(1000);
  }
}
//void setup finish


void loop()
{
MQTT_Connect();
PirSensor();
MoistureSensor();
Ultrasonic_DRY();
Ultrasonic_WET();
GasSensor();
}

void MQTT_Connect()
{
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword ))
    {
    Serial.println("Connected");  
    }
    else
    {
    Serial.print("failed with state ");
    Serial.print(client.state());
    delay(2000);
    }
  }
//client.loop();
}//mqtt connect finish

void PirSensor()
{
  if(digitalRead(pirPin) == HIGH)
  {
   door_open();
   
   if(lockLow)
   {
    lockLow = false;
  Serial.println("---");
  Serial.print("motion detected at ");
  Serial.print(millis()/1000);
  Serial.println(" sec");
  delay(50);
   }
   takeLowTime = true;
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
   Serial.print("motion ended at "); //output
   Serial.print((millis() - pause)/1000);
   Serial.println(" sec");
   delay(50);
   }
  }
}//PIR finish

void door_open()
{
 stepper_door.step(val_door);
 delay(5000);
}

void door_close()
{
 stepper_door.step(-val_door);
 delay(1000);
}

/*
void DoorLid()
{
    door_open();
    delay(5000);
    door_close();
    delay(5000);
}//door finish
*/

void MoistureSensor()
{
    read_moisture = analogRead_Moisture(); // Read Analog value of second sensor
    delay(2000);
    Serial.print("Moisture sensor = ");
    Serial.println(read_moisture);
    if(read_moisture < 1000)
    {
     Tilt_Dry();
     delay(5000);
    }
    else
    {
     Tilt_Wet();
     delay(5000);
    } 
}//moisture and gas finish

int analogRead_Moisture()
{
 digitalWrite(dPin_gas, LOW); //  Turn gas Off
 digitalWrite(dPin_moisture, HIGH); // Turn moisture On
 return analogRead(0);
}

void Tilt_Dry()
{
 stepper_tilt.step(val_tilt);
 delay(1000);
}

void Tilt_Wet()
{
 stepper_tilt.step(-val_tilt);
 delay(1000);
}

void Ultrasonic_DRY()
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

 char cma_dry[50];

 sprintf(cma_dry, "%ld", cm_dry);
 client.publish("Ultra_DRY",cma_dry); ///you can use any topic instead 1627134 like esp/test
 client.subscribe("Ultra_DRY");
}//dry finish

void Ultrasonic_WET()
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

 char cma_wet[50];

 sprintf(cma_wet, "%ld", cm_wet);
 client.publish("Ultra_WET",cma_wet); ///you can use any topic instead 1627134 like esp/test
 client.subscribe("Ultra_WET");
}//wet finish


long microsecondsToCentimeters(long microseconds)
{
 return microseconds / 29 / 2;
}
 
void callback(char* topic, byte* payload, unsigned int length)
{ 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++)
  {
   Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

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
 Serial.println("Gas sensor warming up!");
 delay(20000); //MQ-2 warming up
 return analogRead(0);
}


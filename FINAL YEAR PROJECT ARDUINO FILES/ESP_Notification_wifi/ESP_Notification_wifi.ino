#include <Blynk.h>

/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <SPI.h>
BlynkTimer timer;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "aartu-RYLND86oye1rEW3JVclnuvkj3C";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "LODHA";
char pass[] = "kccprk12345";

void notifyUptime(){
  
  long uptime = millis() / 60000L;
  //millis() is a fuction which returns the number of seconds the time has elapsed after the start of the device
   Blynk.email("pratik.lodha.pl@gmail.com", "Subject: Notify", String("Device Running for ") + uptime + " minutes.");
 
  Blynk.notify(String("Running for ") + uptime + " minutes.");
  //   only one notification per 15 seconds is allowed. 
}
void setup(){
Serial.begin(9600);
pinMode(2,INPUT);
Blynk.begin(auth, ssid, pass);

Blynk.notify("Device started");
   // Setup a function to be called every minute
  timer.setInterval(60000L,notifyUptime);
  }

void loop(){
  Blynk.run();
    timer.run();
    }

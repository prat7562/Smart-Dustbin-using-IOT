#include <Servo.h> 
//defining Servo_tilt
int servopin_test = 4; 
Servo servo_test;          //initialize a servo object for the connected servo  
                
int angle = 90;    
 
void setup() 
{ 
  servo_test.attach(4);        // attach the signal pin of servo to pin9 of arduino
} 
  
void loop() 
{ 
   delay(10000);
  for(angle = 90; angle < 180; angle += 5)     // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);                  //command to rotate the servo to the specified angle
    delay(15);                       
  }
  delay(5000);
  
  for(angle = 180; angle < 90; angle -= 5)     // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);                  //command to rotate the servo to the specified angle
    delay(15);                       
  }
  delay(10000);

  for(angle = 90; angle >= 1; angle -= 5)     // command to move from 180 degrees to 0 degrees 
  {                                
    servo_test.write(angle);              //command to rotate the servo to the specified angle
    delay(15);                       
  } 
    delay(5000);

  for(angle = 1; angle < 90; angle += 5)     // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);                  //command to rotate the servo to the specified angle
    delay(15);                       
  }
}
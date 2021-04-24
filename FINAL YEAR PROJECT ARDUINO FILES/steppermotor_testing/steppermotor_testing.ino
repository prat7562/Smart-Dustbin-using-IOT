// Arduino stepper motor control code

#include <Stepper.h> // Include the header file

// change this to the number of steps on your motor
#define STEPS 32

// create an instance of the stepper class using the steps and pins
Stepper stepper(STEPS, 8, 10, 9, 11);

int val = 1000;

void setup() {
  stepper.setSpeed(1000);
}

void loop() {    
    door_open();
    delay(5000);
    door_close();
    delay(5000);
}

void door_open()
{
    stepper.step(val);
    delay(1000);
}

void door_close()
{
    stepper.step(-val);
    delay(1000);
}

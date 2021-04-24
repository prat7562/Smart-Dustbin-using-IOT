int readD1;
int readD2;
int Pin_D1 = 4;
int Pin_D2 = 5;
 
void setup() {
    Serial.begin(9600);
    pinMode(Pin_D1, OUTPUT);
    pinMode(Pin_D2, OUTPUT);
  
}
 
int analogRead1() {
    digitalWrite(Pin_D1, HIGH); // Turn D1 On
    digitalWrite(Pin_D2, LOW); // Turn D2 Off
    delay(20000);
    return analogRead(0);
}
 
int analogRead2() {
    digitalWrite(Pin_D1, LOW); //  Turn D1 Off
    digitalWrite(Pin_D2, HIGH); // Turn D2 On
    return analogRead(0);
}
 
void loop() {
    readD1 = analogRead1(); // Read Analog value of first sensor
    delay(2000);
    readD2 = analogRead2(); // Read Analog value of second sensor
    delay(2000);
    Serial.print("Gas sensor = ");
    Serial.println(readD1);
    Serial.print("Moisture sensor = ");
    Serial.println(readD2);

}

#include <IRremote.h>
#include <IRremoteInt.h>

#define trigPin 3
#define echoPin 2
int value =0;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13,OUTPUT);
}

void loop() {

  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if ( distance < 60&&distance>1){
   digitalWrite(13,HIGH);
  value = (distance-4)*2.5;
  if (value > 100)
  value = 100;
  if (value < 0)  
  value = 0;  
  Serial.println("run volume="+String(value));
  digitalWrite(13,LOW);
}
  delay(50);
}

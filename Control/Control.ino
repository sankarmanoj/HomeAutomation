#include"Light.h"
Light f1("abcd",3);
void setup() {
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);

}

void loop() {
  f1.Update("abcd",100);
  delay(2000);
  f1.Update("abcd",0);
  delay(2000);
  // put your main code here, to run repeatedly
}

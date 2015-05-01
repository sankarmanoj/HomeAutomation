#define B_LED 3
#define trigPin 6
#define echoPin 7
#include<Math.h>
int state = 0;
char recv;
String val;
long duration, distance;

int value;
int curval;
boolean started = true;
void setup()
{
  pinMode(B_LED,OUTPUT);
  Serial.begin(9600);
  digitalWrite(B_LED,HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13,OUTPUT);

}
void loop()
{
  if(curval!=value)
  {
   int diff=curval-value;
   // Serial.print("Diff");
 //   Serial.println(abs(diff));
    //Serial.println(String(curval)+"=curval val="+String(value));
   if(abs(diff)<5)
   {
     curval = value;
   }else{
    curval=0.3*value+0.7*curval;

    float input = exp(float(curval)/45.0);

    int intensity =31*( input-1.0);
    if(intensity<0)
      intensity=0;
    if(intensity>255)
      intensity=255;
      
  
    Serial.println(String(intensity)+"~");
    analogWrite(B_LED,intensity);
   }  
}
  
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  if ( distance < 60&&distance>3){
    digitalWrite(13,HIGH);
    value = (distance-4)*2.5;
    if (value > 100)
      value = 100;
    if (value < 0)  
      value = 0;  
  
    digitalWrite(13,LOW);
  }
  if (Serial.available())
  {
    recv = Serial.read();
    if(recv =='~')
    {
      value = val.toInt();
      val = "";
      started = false;

    }
    else
      val+=recv;
  }
  delay(50);
}


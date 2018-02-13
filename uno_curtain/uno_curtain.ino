#include <SoftwareSerial.h>

#include <PID_v1.h>

/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#define DEBUG


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
SoftwareSerial blue(8,9);
//   avoid using pins with LEDs attached
double setpoint = 0, enc_position, pwm_output;
PID motorPID(&enc_position,&pwm_output,&setpoint,10,30,3,DIRECT);
void setup() {
  Serial.begin(115200);
  blue.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(7,OUTPUT);
  motorPID.SetOutputLimits(-255,255);
  motorPID.SetMode(AUTOMATIC);
  setpoint = 0;
}
long curtime = 0;
long oldPosition  = -999;
String inbuffer = "";
void loop() {
  if(millis()-curtime>750)
  {
    curtime = millis();
    #ifdef DEBUG
    Serial.print("Output = "+String(pwm_output) + " Setpoint = "+String(setpoint)) ;
    Serial.println("Encoder Position =" + String(enc_position));
    #endif
    blue.print(String(enc_position)+"~");
  }
  if(Serial.available())
  {
    setpoint = Serial.parseInt();
    blue.println(setpoint);
    
  }
  if(blue.available())
  {
    char c = blue.read();
    if(c=='~')
    {
      setpoint = inbuffer.toInt();
      inbuffer = "";
    }
    else
    {
      inbuffer += c;
    }
    
  }
  enc_position = myEnc.read();
  motorPID.Compute();
  
  if (enc_position != oldPosition) {
    oldPosition = enc_position;
 //   blue.print("Output = "+String(pwm_output));
  //  blue.println("Encoder Position =" + String(enc_position));
  }
  if(pwm_output==0)
  {
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);  
  //0  Serial.println("OFF");
  }
  else if(pwm_output<0)
  {
    digitalWrite(6,LOW);
    digitalWrite(7,HIGH); 
  //  Serial.println("FORWARD");
  }
  else
  {
    digitalWrite(6,HIGH);
    digitalWrite(7,LOW);  
  //  Serial.println("BACKWARD");
  }
  if(abs(pwm_output)>70)
  analogWrite(5,abs(pwm_output));
  else
  analogWrite(5,0);
  
}

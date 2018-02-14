#include <SoftwareSerial.h>
#include <PID_v1.h>
#include <Encoder.h>
#define DEBUG //Enable Debug prints status to the Serial output

Encoder myEnc(2, 3);      //Measure movement from encoder
SoftwareSerial blue(8,9); //SoftwareSerial to communicate with bluetooth module

double setpoint = 0, enc_position, pwm_output;
PID motorPID(&enc_position,&pwm_output,&setpoint,10,30,3,DIRECT); //PID Controller
void setup() {
  Serial.begin(115200);
  blue.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(7,OUTPUT);
  motorPID.SetOutputLimits(-255,255); //Allow negative PID output for reverse movement
  motorPID.SetMode(AUTOMATIC);
  setpoint = 0;
}
long curtime = 0;
long oldPosition  = -999;
String inbuffer = "";
void loop() {
  if(millis()-curtime>750)  //Periodically print debug
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
  if(blue.available()) //If input is available from bluetooth module
  {
    char c = blue.read(); //Read characters till tilda
    if(c=='~')
    {
      setpoint = inbuffer.toInt(); //Change setpoint
      inbuffer = "";
    }
    else
    {
      inbuffer += c;
    }

  }
  enc_position = myEnc.read();
  motorPID.Compute();

  // if (enc_position != oldPosition) {
    // oldPosition = enc_position;
 //   blue.print("Output = "+String(pwm_output));
  //  blue.println("Encoder Position =" + String(enc_position));
  // }


  if(pwm_output==0) //If output is zero, disable motor
  {
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
  //0  Serial.println("OFF");
  }
  else if(pwm_output<0) //If output is negative, set motor direction to reverse
  {
    digitalWrite(6,LOW);
    digitalWrite(7,HIGH);
  //  Serial.println("FORWARD");
  }
  else //If output is postiive, set motor direction to forward
  {
    digitalWrite(6,HIGH);
    digitalWrite(7,LOW);
  //  Serial.println("BACKWARD");
  }


  if(abs(pwm_output)>70) //Prevent powering of motor if output is too low for the motor to turn
  analogWrite(5,abs(pwm_output));
  else
  analogWrite(5,0);

}

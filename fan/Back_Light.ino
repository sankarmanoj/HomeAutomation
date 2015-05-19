#define B_LED 3
int state = 0;
char recv;
String val;
int cur_value = 0;
void setup()
{
  pinMode(B_LED,OUTPUT);
  Serial.begin(9600);
  digitalWrite(B_LED,HIGH);
  for ( int i =0;i<255;i++)
  {
    
    delay(sqrt(255-i));
    analogWrite(B_LED,i);
    
  }
  Serial.write("started");
}
void loop()
{
  boolean started = true;
    if (Serial.available())
    {
      recv = Serial.read();
      if(recv =='~')
      {
       started = true;
       
      }
      else if (recv=='!')
      {
        state = val.toInt();
        val = "";
        started = false;
        analogWrite(B_LED,state);
        Serial.println(state);
      }
      else 
      {
        val +=recv;
       Serial.println(val); 
      }
    }
  
}


int fan = 13;
int time = 0;
int start =0;
class TimePin
{
  private:
  int start;
  int time;
  int pin;
  
  public:
  int cycle;
  int on_time;
 TimePin( int p,int cycle, int on_time)
  {
   start = micros();
  time = micros();
  pin = p;
  
  }
  void reset()
  {
    start = micros();
  }
 
  void check()
  {
    time = micros();
   if((time-start)>cycle)
   {
     digitalWrite(pin,LOW);
     this->reset();
   }
   if((time-start)>on_time)
   {
     digitalWrite(pin,HIGH);
   }
    
  }
  
  
};
TimePin control = TimePin(fan,16666,8333);

void setup()
{
  pinMode(fan,OUTPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  start = micros();
}
void loop()
{
 control.check(); 
}

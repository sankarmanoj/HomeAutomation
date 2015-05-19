#include"Arduino.h"
class Light
{
public:
	Light(String id,int p);
	void Update(String id,int value);
	~Light();
private:
	String uid;
	int pin;
	/* data */

};
Light::Light(String id,int p)
{
	pinMode(13,OUTPUT);
	digitalWrite(13,LOW);
	if(id.length()!=4)
	{
		digitalWrite(13,HIGH);
	}

	pinMode(pin,OUTPUT);
	uid=id;
	pin = p;


}
void Light::Update(String id,int value)
	{
		if(uid!=id)
		{
			digitalWrite(13,HIGH);
			return;
		}
		
    float input = exp(float(value)/45.0);

    int intensity =31*( input-1.0);
    if(intensity<0)
      intensity=0;
    if(intensity>255)
      intensity=255;
  	analogWrite(pin,intensity);


	}
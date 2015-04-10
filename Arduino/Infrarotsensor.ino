#include <Average.h>

Average<int> baselineValues(5);
int baselineValue=0;
Average<int> rollingValues(5);
int rollingValue=0;
float x = 0;
boolean positionVonRechtsAnfahren = false;

void setup(){
  Serial.begin(9600);
   for(int i=0; i<5;i++){
      baselineValues.push(analogRead(3));
  }
  baselineValue = baselineValues.mean();
}

void loop(){
  //Read value from distance Sensor
  int analogValue = analogRead(3);
  
  if(abs(baselineValue - analogValue) < 40)
  {
    rollingValue = rollingValues.rolling(analogValue);
    Serial.println(rollingValue);    
    int difference = baselineValue - rollingValue;    
    if(difference >= 31&&!positionVonRechtsAnfahren)
    {
      Serial.println("DE CHÜBEL ISCH DA!!!!");
      Serial.println("Von links angefahren");
    }
    if(difference >= 16&&positionVonRechtsAnfahren)
    {
      Serial.println("DE CHÜBEL ISCH DA!!!!");
      Serial.println("Von rechts angefahren");
    }
  }
  delay(250);
}

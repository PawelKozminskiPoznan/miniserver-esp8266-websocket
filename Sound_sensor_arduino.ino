/*
 * Rui Santos 
 * Complete Project Details http://randomnerdtutorials.com
*/
#include <ArduinoJson.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHT11_PIN 12
#define ledPin 13
#define sensorPin A0

DHT dht(DHT11_PIN,DHT11);
float temp,humi,prev_temp,prev_humi;
int ad_value = 0;
int previous = 0;

bool clap = false;
bool threshold = false;

unsigned long clap_detector = 0;
unsigned long previousMillis = 0;
unsigned long TMillis = 0;
const long interval = 5;
const long Tinterval = 10000;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.begin (115200);
  dht.begin(); 
}
  
void loop (){
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ad_value=analogRead(sensorPin);
        if(!threshold and ((ad_value - previous) >350) ){
		      threshold = true;
          clap_detector = millis();
		      if(!clap) {
			      char json[] = "{\"type\":\"on\",\"value\":\"ON\"}"; 
            Serial.println(json);
			      digitalWrite(ledPin,HIGH);
			      clap = true;
		      }
		      else{
			      char json[] = "{\"type\":\"off\",\"value\":\"OFF\"}";
            digitalWrite(ledPin,LOW);
            Serial.println(json);
			      clap = false;
		      }
        }
      previous = ad_value;
      if(threshold and ((currentMillis - clap_detector) >= 1000)) threshold = false;
    }

    if (currentMillis - TMillis >= Tinterval) {
      TMillis = currentMillis;
      temp = dht.readTemperature();
      humi = dht.readHumidity();
      if(prev_temp != temp){
        String jsonString2 = "{\"type\":\"temp\",\"value\":\"";
        jsonString2 += temp;
        jsonString2 +="\"}"; 
        Serial.println(jsonString2);
      }
      if(prev_humi != humi){
        String jsonString3 = "{\"type\":\"humi\",\"value\":\"";
        jsonString3 += humi;
        jsonString3 +="\"}"; 
        Serial.println(jsonString3);
      }
      prev_temp = temp;
      prev_humi = humi;
    }

    if (Serial.available()) {
      char type = (char)Serial.read();
      //StaticJsonBuffer<200> jb;
      //JsonObject& root = jb.parseObject(h);
      //char type = root["type"];
      if (type == 'c'){
          char json1[] = "{\"type\":\"on\",\"value\":\"ON\"}";
          digitalWrite(ledPin,HIGH);
          Serial.println(json1);
      }
      else if (type == 'b'){
          char json[] = "{\"type\":\"off\",\"value\":\"OFF\"}";
          digitalWrite(ledPin,LOW);
          Serial.println(json);
      }
      else if (type == 'z'){
          String jsonString1 = "{\"type\":\"temp\",\"value\":\"";
          jsonString1 += temp;
          jsonString1 +="\"}"; 
          Serial.println(jsonString1);
      }
      else if (type == 'x'){
          String jsonString = "{\"type\":\"humi\",\"value\":\"";
          jsonString += humi;
          jsonString +="\"}";
          Serial.println(jsonString);
      }
    }
}

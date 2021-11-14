#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#define ledmerah 21
#define ledkuning 23
#define ledhijau 22

const char* ssid = "Dua Bersaudara";
const char* pass = "TanyaOrang2B";

String response = "";

DynamicJsonDocument doc(2048);

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 1*60*1000;


void setup() {
  Serial.begin(115200);
  pinMode(ledmerah, OUTPUT);
  pinMode(ledkuning, OUTPUT);
  pinMode(ledhijau, OUTPUT);
  //Initiate Wifi Connect

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  //Wait for connect
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("Wifi Connected with IP : ");
  Serial.println(WiFi.localIP());

}
  

void loop(void) {
  HTTPClient http;
  currentTime = millis();

  if(currentTime - previousTime > timeoutTime){
    Serial.println("Send Request");
    previousTime = currentTime;

    //The API URL
    String request = "https://api.tomorrow.io/v4/timelines?location=-7.723832329117377, 110.41749061560046&fields=temperature&timesteps=current&units=metric&apikey=NQXl4xd8Wr6f6FdWX6idsTWUkksjWUtG";

    //Start Req
    http.begin(request);

    //Use HTTP Get req
    http.GET();

    //Respone server
    response = http.getString();

    //Parse JSON, read error if any
    DeserializationError error = deserializeJson(doc, response);
    if (error){
      Serial.print(F("deserializeJson() failed: "));
      Serial.println (error.c_str());
      return;
    }
    JsonObject data_timelines_0 = doc["data"]["timelines"][0];
    const char* data_timelines_0_intervals_0_startTime = data_timelines_0["intervals"][0]["startTime"];
    float data_timelines_0_intervals_0_values_temperature = data_timelines_0["intervals"][0]["values"]["temperature"];
    Serial.print("Waktu :");
    Serial.println(data_timelines_0_intervals_0_startTime);
    Serial.print("Suhu :");
    Serial.println(data_timelines_0_intervals_0_values_temperature);
    if (data_timelines_0_intervals_0_values_temperature < 10){
      digitalWrite(ledhijau, HIGH);
    }else if (data_timelines_0_intervals_0_values_temperature < 20){
      digitalWrite(ledkuning, HIGH);
    }else {
      digitalWrite(ledmerah, HIGH);
    }
    //close connection
    http.end();
  }
}

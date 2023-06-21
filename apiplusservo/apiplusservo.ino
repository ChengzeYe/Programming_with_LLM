// pass this google doc key with the key=API_KEY parameter: AIzaSyDe8mU5g5d0lWDFJTz4QHZsV88yT_4jWmk
// In your Arduino code, include the API key as a parameter when making requests to the Google Docs API. For example: https://docs.googleapis.com/v1/documents/{documentId}?key={YOUR_API_KEY}
// Google Doc URL: https://docs.google.com/document/d/1x2cjAc0rpqVSGzSL8vtFM7Gyqd6IlzeDXJQCIGbUovE/edit 
// documentId: 1x2cjAc0rpqVSGzSL8vtFM7Gyqd6IlzeDXJQCIGbUovE



#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0;    // variable to store the servo position
int servoPin = 2;
const char* ssid = "WLAN-990454"; //Enter the router name
const char* password = "5159341473380940"; //Enter the router password
//const char* ssid = "KabelBox-5720";
//const char* password = "93862697736661691359";
const char* sheetId = "1MvLMi6_GJr42eQFieYQUfyNza_aeABIqt7L_H2IDDFg";
const char* sheetName = "Sheet1!A1:A1";

const char* apiEndpoint = "https://sheets.googleapis.com/v4/spreadsheets";
const char* apiKey = "AIzaSyDe8mU5g5d0lWDFJTz4QHZsV88yT_4jWmk";
String previous_state = "off";
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi successfully");
  	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 2500); // attaches the servo on pin 18 to the servo object

}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://sheets.googleapis.com/v4/spreadsheets/" + String(sheetId) + "/values/" + String(sheetName) + "?key=" + String(apiKey);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      // Parse the JSON response
      StaticJsonDocument<2000> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.println("Failed to parse JSON");
      } else {
        // Access the values in the JSON response
        JsonObject root = doc.as<JsonObject>();
        JsonArray rows = root["values"].as<JsonArray>();

        for (JsonVariant row : rows) {
          String value = row[0].as<String>();
          String current_value = value;
          Serial.println(value);
          if(previous_state != current_value){
            swith();
            previous_state = current_value;
          }
        }
      }
    } else {
      Serial.println("HTTP request failed");
    }

    http.end();
  }

  delay(3000);
}
void swith(){
  	for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
	for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
}

// pass this google doc key with the key=API_KEY parameter: AIzaSyDe8mU5g5d0lWDFJTz4QHZsV88yT_4jWmk
// In your Arduino code, include the API key as a parameter when making requests to the Google Docs API. For example: https://docs.googleapis.com/v1/documents/{documentId}?key={YOUR_API_KEY}
// Google Doc URL: https://docs.google.com/document/d/1x2cjAc0rpqVSGzSL8vtFM7Gyqd6IlzeDXJQCIGbUovE/edit 
// documentId: 1x2cjAc0rpqVSGzSL8vtFM7Gyqd6IlzeDXJQCIGbUovE

// const char* ssid = "eduroam";  //"KabelBox-2A0D";  // your_SSID home: KabelBox-2A0D
// const char* password = "201301Wxl!"; // your_password home: 33306838362884391459
// const char* username = "pi00zyme@fau.de";


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//const char* ssid = "WLAN-990454"; //Enter the router name
//const char* password = "5159341473380940"; //Enter the router password
const char* ssid = "KabelBox-5720";
const char* password = "93862697736661691359";
const char* sheetId = "1MvLMi6_GJr42eQFieYQUfyNza_aeABIqt7L_H2IDDFg";
const char* sheetName = "Sheet1!A1:A10";

const char* apiEndpoint = "https://sheets.googleapis.com/v4/spreadsheets";
const char* apiKey = "AIzaSyDe8mU5g5d0lWDFJTz4QHZsV88yT_4jWmk";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi successfully");

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
          Serial.println(value);
        }
      }
    } else {
      Serial.println("HTTP request failed");
    }

    http.end();
  }

  delay(1000);
}

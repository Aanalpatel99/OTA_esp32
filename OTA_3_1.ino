#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

const char* ssid =  "R&D RPI"; //"AP";    //
const char* password = "wcspassword"; //"123123$a";    //
String url = "https://quietistic-electrod.000webhostapp.com/version.php"; //"http://27.57.160.125:3000/";
String url_update = "https://quietistic-electrod.000webhostapp.com/upload_firmware/OTA_3_2.ino.bin"; //"http://27.57.160.125:3000/upload_firmware"; 
String url_post = "https://quietistic-electrod.000webhostapp.com/post.php";

bool ledState = 1;
const int ledPin = 2;

float  FIRMWARE_VERSION = 2.0;
int updateError = 0;

String json_str;
JsonObject obj_test;
DynamicJsonDocument doc(1024);

void setup(){
  uint8_t mac[6];
  esp_efuse_mac_get_default(mac);
  String mac_ID = String(mac[0]) + ":" + String(mac[1]) + ":" + String(mac[2]) + ":" + String(mac[3]) + ":" + String(mac[4]) + ":" + String(mac[5]);
  
  JsonObject obj_mac_ver = doc.createNestedObject();
  obj_mac_ver["mac_id"] = mac_ID;
  obj_mac_ver["Version"] = FIRMWARE_VERSION;

  serializeJson(obj_mac_ver, json_str);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("WiFi connected.");
  Serial.println("WiFi connected.");
  HTTPClient http;
  http.begin(url_post);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(json_str);
  // Serial.println(WiFi.localIP());
}

void loop() {
  digitalWrite(ledPin, ledState);
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200) {
    String response = http.getString();
    float newVersion = response.toFloat();
    Serial.println("firmware version available: " + String(newVersion));
    if (FIRMWARE_VERSION == newVersion && !updateError){
      Serial.println("no update available");
    }
    else{
      HTTPClient http;
      http.begin(url_update);
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        WiFiClient* client = http.getStreamPtr();
        if (Update.begin(contentLength)) {
          Serial.println("Downloading firmware binary...");
          size_t written = Update.writeStream(*client);
          if (written == contentLength) {
            Serial.println("Firmware update complete. Restarting...");
            Update.end();
            updateError = 0;
            ESP.restart();
          } 
          else {
            Serial.println("Error updating firmware");
            Update.abort();
            updateError = 1;
          }
        } 
        else {
          Serial.println("Could not begin firmware update");
          updateError = 1;
        }
      } 
      else {
        Serial.println("Firmware update not available");
        updateError = 1;
      }
    }

  }
  else {
    Serial.println("Error checking firmware version. HTTP code: " + String(httpResponseCode));
  }
  
  http.end();
  delay(6000);
}
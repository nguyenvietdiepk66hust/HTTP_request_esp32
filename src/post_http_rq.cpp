/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-http-request
 */

#include <WiFi.h>
#include <HTTPClient.h>

const char WIFI_SSID[] = "HiPEMs_50 GHz";         // CHANGE IT
const char WIFI_PASSWORD[] = "12342468"; // CHANGE IT

String HOST_NAME   = "https://openweathermap.org/api"; // CHANGE IT
String PATH_NAME   = "6d7d4fc3132ad76712e9331a552d2482";      // CHANGE IT
//String PATH_NAME   = "/products/arduino.php";      // CHANGE IT
String queryString = "temperature=26&humidity=70";

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  HTTPClient http;

  http.begin(HOST_NAME + PATH_NAME + "?" + queryString);
  http.addHeader("TEMP&HUM", "application/weather");
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop() {
  
}

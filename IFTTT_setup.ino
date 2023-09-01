#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// Replace with your Wi-Fi credentials
char ssid[] = "Veekay";
char pass[] = "12345678";

// IFTTT Webhook details
const char* IFTTT_HOST = "maker.ifttt.com";
const int IFTTT_PORT = 80;
const char* IFTTT_EVENT = "Sunlight_detected";  
const char* IFTTT_KEY = "kIvrCx7adEJ8pnUvI-RFnsBDmZLxHIRKRunP-eIf3LO";    

// Analog pin connected to LDR
const int ldrPin = A0;

// Light level threshold (adjust as needed)
const int threshold = 500;

WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, IFTTT_HOST, IFTTT_PORT);

void setup() {
  Serial.begin(9600);
  delay(2000);
  
  // Connect to Wi-Fi
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int lightLevel = analogRead(ldrPin);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  if (lightLevel > threshold) {
    sendToIFTTT("SunlightDetected");
  }

  delay(60000); // Wait for a minute before checking again
}

void sendToIFTTT(String value) {
  String url = "/trigger/" + IFTTT_EVENT + "/with/key/" + IFTTT_KEY;
  String json = "{\"value1\":\"" + value + "\"}";

  client.beginRequest();
  client.post(url);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", json.length());
  client.beginBody();
  client.print(json);
  client.endRequest();

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  
  Serial.print("IFTTT Response Code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

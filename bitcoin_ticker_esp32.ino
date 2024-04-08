#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>  // if using hardware SPI
#include <Wire.h> // if using hardware I2C
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "AndroidAP_4983";
const char* password = "1111111111";

// CoinGecko API URL to get Bitcoin price in USD
const char* bitcoin_api_url = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd";

// Define display object
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 22, 21, U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  u8g2.begin(); // Initialize the display

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  u8g2.clearBuffer();
  
  // Fetch Bitcoin price from CoinGecko API
  float bitcoin_price = getBitcoinPrice();
  
  // Display text on OLED display
  u8g2.setFont(u8g2_font_ncenB10_tr); // Set font size
  u8g2.setCursor(0, 15); // Set position for the text
  u8g2.print("BTC Price");

  // Print Bitcoin price below the text
  u8g2.setFont(u8g2_font_ncenB24_tr); // Set font size
  u8g2.setCursor(0, 50); // Set position for the price
  u8g2.print(bitcoin_price, 0); // Display Bitcoin price with 2 decimal places
  u8g2.print(" $");

  u8g2.sendBuffer();
  
  delay(60000); // Update every minute
}

float getBitcoinPrice() {
  HTTPClient http;
  http.begin(bitcoin_api_url);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    return doc["bitcoin"]["usd"].as<float>();
  } else {
    Serial.println("Error fetching Bitcoin price");
    return 0.0;
  }
}

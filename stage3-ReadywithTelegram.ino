#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_PCF8574.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Telegram API Token and Chat ID
const String BOT_TOKEN = "";
const String CHAT_ID = "";

// Button and debounce settings
const int buttonPin = 15;  
bool buttonState = HIGH;  // Initial state of button (not pressed)
unsigned long lastDebounceTime = 0;
const int DEBOUNCE_DELAY = 50;

// LCD setup
LiquidCrystal_PCF8574 lcd(0x27); // Adjust the I2C address as needed

void setup() {
  // Initialize serial and LCD
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  delay(1000);
  lcd.clear();

  // Set up button pin
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Check for button press
  if (reading == LOW && buttonState == HIGH && (millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    lastDebounceTime = millis();
    buttonState = LOW;

    // Notify on LCD and send Telegram message
    lcd.setCursor(0, 0);
    lcd.print("Button Pressed!");
    sendTelegramMessage("Button Press!");

    delay(2000);  // Hold display for 2 seconds
    lcd.clear();
  }

  if (reading == HIGH && buttonState == LOW && (millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    buttonState = HIGH;
  }
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + message;
    
    http.begin(url.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();  // Get the response from the server
      Serial.println("Telegram message sent successfully.");
      Serial.println("Response from Telegram: " + response);
      lcd.setCursor(0, 1);
      lcd.print("Msg sent!");
    } else {
      Serial.print("Error in sending message, HTTP response code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Not connected to WiFi");
  }
}

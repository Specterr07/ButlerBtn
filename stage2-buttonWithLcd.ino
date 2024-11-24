#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define BUTTON_PIN 15  // GPIO pin where the button is connected
#define DEBOUNCE_DELAY 50  // Debounce time in milliseconds

// Create an LCD object with the specified I2C address
LiquidCrystal_PCF8574 lcd(0x27); // Adjust the address if necessary

int buttonState = HIGH;      // Current reading of the button
int lastButtonState = HIGH;  // Previous reading of the button
unsigned long lastDebounceTime = 0;  // Time of the last state change

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Configure button pin with pull-up resistor

  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  lcd.setBacklight(1); // Turn on the LCD backlight (if needed)
  lcd.setCursor(0, 0);  // Set cursor to first row
  lcd.print("Button Status:");  // Print message on the LCD
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);  // Read the state of the button

  // If the button state has changed, reset the debounce timer
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Check if the debounce time has passed and the button state is stable
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;

      // If the button is pressed (LOW due to pull-up)
      if (buttonState == LOW) {
        Serial.println("Button Pressed!");
        lcd.setCursor(0, 1);  // Set cursor to the second row
        lcd.print("Pressed!    ");  // Display pressed status (clear any previous text)
      } else {
        // If the button is not pressed (HIGH due to pull-up)
        Serial.println("Button Unpressed!");
        lcd.setCursor(0, 1);  // Set cursor to the second row
        lcd.print("Released!  ");  // Display unpressed status
      }
    }
  }

  lastButtonState = reading;  // Update the last button state
}

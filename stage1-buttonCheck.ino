#define BUTTON_PIN 15  // GPIO pin where the button is connected
#define DEBOUNCE_DELAY 50  // Debounce time in milliseconds

int buttonState = HIGH;      // Current reading of the button
int lastButtonState = HIGH;  // Previous reading of the button
unsigned long lastDebounceTime = 0;  // Time of the last state change

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Configure button pin with pull-up resistor
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
      }
    }
  }

  lastButtonState = reading;  // Update the last button state
}

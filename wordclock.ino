const int button1Pin = 2;
const int button2Pin = 3;
const int ledPin = 13;

int ledState = HIGH;

int button1State;
int button2State;

int lastButton1State = LOW;
int lastButton2State = LOW;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;   // the debounce time; increase if the output flickers

void setup() {
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, ledState);

  Serial.begin(9600);
}

void loop() {
  int pin1Reading = digitalRead(button1Pin);
  int pin2Reading = digitalRead(button2Pin);

  if (pin1Reading == HIGH && pin2Reading == HIGH) {
    // Don't bother handling when both buttons are high.
    return;
  }

  if (buttonPressed(pin1Reading, &button1State, &lastButton1State)) {
    ledState = !ledState;
    Serial.write("Button 1 clicked\n");
  }

  if (buttonPressed(pin2Reading, &button2State, &lastButton2State)) {
    ledState = !ledState;
    Serial.write("Button 2 clicked\n");
  }

  digitalWrite(ledPin, ledState);
}

boolean buttonPressed(int pinReading, int *buttonState, int *lastButtonState) {
  // Debouncing logic from the example Arduino Debounce sketch.
  boolean result = false;
  
  if (pinReading != *lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (pinReading != *buttonState) {
      Serial.write("Button\n");
      *buttonState = pinReading;

      if (pinReading == HIGH) {
        result = true;
      }
    }
  }
  
  *lastButtonState = pinReading;
  return result;
}

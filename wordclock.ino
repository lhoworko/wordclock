#include <FastLED.h>

#define BUTTON_PIN 3
#define LED_PIN 13

#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS 64

#define WIDTH 13
#define HEIGHT 8
#define NUM_LEDS (WIDTH * HEIGHT)

#define STARTUP_SECONDS 3

CRGB leds[NUM_LEDS];

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;   // the debounce time; increase if the output flickers

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode(BUTTON_PIN, INPUT);

  uint32_t start_time = millis();
  for (uint32_t ms = millis(); ms < (start_time + (STARTUP_SECONDS * 1000)); ms = millis()) {
    int32_t yHueDelta32 = ((int32_t)cos16(ms * (27/1)) * (350 / WIDTH));
    int32_t xHueDelta32 = ((int32_t)cos16(ms * (39/1)) * (310 / HEIGHT));
    drawFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();
  }
}

void loop() {
  int buttonReading = digitalRead(BUTTON_PIN);

  if (buttonPressed(buttonReading)) {

  }
}

boolean buttonPressed(int pinReading) {
  // Debouncing logic from the example Arduino Debounce sketch.
  boolean result = false;
  
  if (pinReading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (pinReading != buttonState) {
      buttonState = pinReading;

      if (pinReading == HIGH) {
        result = true;
      }
    }
  }
  
  lastButtonState = pinReading;
  return result;
}

void drawFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8) {
  byte lineStartHue = startHue8;
  
  for( byte y = 0; y < HEIGHT; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;    
      
    for (byte x = 0; x < WIDTH; x++) {
      pixelHue += xHueDelta8;
      leds[xy(x, y)]  = CHSV(pixelHue, 255, 255);
    }
  }
}

uint16_t xy(uint8_t x, uint8_t y) {
  uint16_t i;
  
  if (y & 0x01) {
    uint8_t reverseX = (WIDTH - 1) - x;
    i = (y * WIDTH) + reverseX;
  } else {
    i = (y * WIDTH) + x;
  }
  
  return i;
}

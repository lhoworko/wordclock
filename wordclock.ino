#include <FastLED.h>
#include <RTClib.h>
#include "ledGrid.h"

#define BUTTON_PIN 3
#define LED_PIN 13

#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS 120

#define WIDTH 13
#define HEIGHT 8
#define NUM_LEDS (WIDTH * HEIGHT)

#define STARTUP_SECONDS 3

RTC_DS3231 rtc;
CRGB leds[NUM_LEDS];

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;   // the debounce time; increase if the output flickers

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  rtc.begin();
  
  delay(1000);
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  bootAnimation();
  resetClock();
}

void loop() {
  int buttonReading = digitalRead(BUTTON_PIN);
  DateTime currentTime = rtc.now();

  if (buttonPressed(buttonReading)) {
    rtc.adjust(currentTime + (5 * 60));
  }
    
  drawTime(currentTime);
  delay(100);
}

// Returns true when the button is initially pressed. Only returns true once, holding down does nothing.
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

// Plays the bootup animation
void bootAnimation() {
  uint32_t start_time = millis();
  for (uint32_t ms = millis(); ms < (start_time + (STARTUP_SECONDS * 1000)); ms = millis()) {
    int32_t yHueDelta32 = ((int32_t)cos16(ms * (27/1)) * (350 / WIDTH));
    int32_t xHueDelta32 = ((int32_t)cos16(ms * (39/1)) * (310 / HEIGHT));
    drawFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);

    FastLED.show();
  }
}

// Draw a single frame of the bootup animation
void drawFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8) {
  byte lineStartHue = startHue8;

  for (byte y = 0; y < HEIGHT; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;

    for (byte x = 0; x < WIDTH; x++) {
      pixelHue += xHueDelta8;
      leds[xy(x, y)]  = CHSV(pixelHue, 255, 255);
    }
  }
}

// Reset all pixels of the clock to black
void resetClock() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      leds[xy(x, y)] = CRGB::Black;
    }
  }
}

void drawTime(DateTime now) {
  CRGB color = getColor(now.unixtime());

  resetClock();

  uint8_t minuteIndex = now.minute() / 5;
  uint8_t hourIndex = (now.hour() + (minuteIndex > 6 ? 1 : 0)) % 12;

  drawWord(IT, color);
  drawWord(IS, color);

  drawWord(MINUTES_LIST[minuteIndex], color);

  // Not NOTHING, QUARTER PAST, HALF PAST, QUARTER TO
  if (minuteIndex % 3 != 0) {
    drawWord(MINUTES, color);
  }

  if (minuteIndex > 0 && minuteIndex <= 6) {
    drawWord(PAST, color);
  } else if (minuteIndex > 6) {
    drawWord(TO, color);
  }

  drawWord(HOURS_LIST[hourIndex], color);

  if (minuteIndex == 0) {
    drawWord(OCLOCK, color);
  }

  FastLED.show();
}

// Draw a single word into the leds array - does not turn on the leds
void drawWord(int *word, CRGB color) {
  int numWords = *word;

  for (int currWord = 0; currWord < numWords; currWord++) {
    int currWordOffset = currWord * 3;
    int x = *(word + currWordOffset + 1);
    int y = *(word + currWordOffset + 2);
    int len = *(word + currWordOffset + 3);

    for (int i = 0; i < len; i++) {
      leds[xy(x + i, y)] = color;
    }
  }
}

// Get the CRGB value given the current time
CRGB getColor(unsigned long currTime) {
  // # 255 0 0 -> 255 255 0 -> 0 255 0 -> 0 255 255 -> 0 0 255 -> 255 0 255 -> 255 0 0
  short inRange = (currTime % (255 * 6)) / 255;

  switch (inRange) {
    case 0: return CRGB(255, currTime % 256, 0);
    case 1: return CRGB(255 - (currTime % 256), 255, 0);
    case 2: return CRGB(0, 255, currTime % 256);
    case 3: return CRGB(0, 255 - (currTime % 256), 255);
    case 4: return CRGB(currTime % 256, 0, 255);
    default: return CRGB(255, 0, 255 - (currTime % 256));
  }
}

// Translate (x, y) into an index into the led strip
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

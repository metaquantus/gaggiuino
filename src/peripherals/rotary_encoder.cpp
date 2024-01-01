#include "rotary_encoder.h"

#include "../log.h"
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

// pins on the encoder board
#define SS_SWITCH        24
#define SS_NEOPIX        6
// default I2C address
#define SEESAW_ADDR      0x36

#ifdef ROTARY_ENCODER
Adafruit_seesaw ssRotary; // running on default Wire I2C
// the rgb led on the encoder board
seesaw_NeoPixel ssRotaryPixel = seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800);
#endif
static bool ssRotaryStarted = false;
static int32_t encoder_position = 0;


bool rotaryEncoderInit(void) {
#ifdef ROTARY_ENCODER
  if (!ssRotary.begin(SEESAW_ADDR) || !ssRotaryPixel.begin(SEESAW_ADDR)) {
    LOG_ERROR("Couldn't find Adafruit seesaw on default address");
    return false;
  }
  uint32_t version = ((ssRotary.getVersion() >> 16) & 0xFFFF);
  if (version != 4991) {
    LOG_ERROR("Wrong Adafruit seesaw firmware loaded: %d", version);
    return false;
  }
  LOG_INFO("Found Adafruit seesaw Product 4991");

  // set not so bright!
  ssRotaryPixel.setBrightness(20);
  ssRotaryPixel.show();

  // use a pin for the built in encoder switch
  ssRotary.pinMode(SS_SWITCH, INPUT_PULLUP);
  delay(10);
  ssRotary.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
  ssRotary.enableEncoderInterrupt();
  ssRotaryStarted = true;
  LOG_INFO("Adafruit seesaw Product 4991 Rotary Encoder initialized");
#endif
  return ssRotaryStarted;
}

bool isRotaryEncoderStarted(void) {
  return ssRotaryStarted;
}

void setRotaryEncoderPixelColor(uint8_t red, uint8_t green, uint8_t blue) {
#ifdef ROTARY_ENCODER
  if ( ssRotaryStarted ) {
    ssRotaryPixel.setPixelColor(0, red, green, blue);
  }
#endif
}

bool rotaryEncoderSwitchDigitalRead(void) {
#ifdef ROTARY_ENCODER
  return ssRotaryStarted ? ssRotary.digitalRead(SS_SWITCH) : 0;
#else
  return 0;
#endif
}

uint32_t colorWheel(byte WheelPos) {
#ifdef ROTARY_ENCODER
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return ssRotaryPixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return ssRotaryPixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ssRotaryPixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
#else
  return WheelPos;
#endif
}

int32_t getRotaryEncoderPosition(void) {
#ifdef ROTARY_ENCODER
  if (ssRotaryStarted) {
    int32_t new_position = ssRotary.getEncoderPosition();
    if (encoder_position != new_position) {
      // change the neopixel color
      ssRotaryPixel.setPixelColor(0, colorWheel(new_position & 0xFF));
      ssRotaryPixel.show();
      encoder_position = new_position;      // and save for next round
    }
  }
#endif
  return encoder_position;
}

void setRotaryEncoderPosition(int32_t pos) {
#ifdef ROTARY_ENCODER
  if ( ssRotaryStarted ) {
    ssRotary.setEncoderPosition(pos);
  }
#endif
}

// delta value since last read
int32_t getRotaryEncoderDelta(void) {
#ifdef ROTARY_ENCODER
  return ssRotaryStarted ? ssRotary.getEncoderDelta() : 0;
#else
  return 0;
#endif
}

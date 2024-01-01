#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>

// This roughly covers the full slider scale in one full turn 
// for a 20 step encoder
#define ROTARY_ENCODER_MANUAL_FLOW_SCALE 6

bool rotaryEncoderInit(void);
bool isRotaryEncoderStarted(void);
int32_t getRotaryEncoderPosition(void);
int32_t getRotaryEncoderDelta(void);
void setRotaryEncoderPosition(int32_t pos);
void setRotaryEncoderPixelColor(uint8_t red, uint8_t green, uint8_t blue);
bool rotaryEncoderSwitchDigitalRead(void);

#endif
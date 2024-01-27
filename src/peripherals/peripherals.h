/* 09:32 15/03/2023 - change triggering comment */
#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "pindef.h"
#include "sensors_state.h"
#include "peripherals.h"
#include <Arduino.h>

extern SensorState currentState;

static inline void pinInit(void) {
  #if defined(LEGO_VALVE_RELAY)
    pinMode(valvePin, OUTPUT_OPEN_DRAIN);
  #else
    pinMode(valvePin, OUTPUT);
  #endif
  pinMode(relayPin, OUTPUT);
  #ifdef steamValveRelayPin
  pinMode(steamValveRelayPin, OUTPUT);
  #endif
  #ifdef steamBoilerRelayPin
  pinMode(steamBoilerRelayPin, OUTPUT);
  #endif
  #ifdef CATURRA
  pinMode(brewPin,  INPUT);
  pinMode(steamPin, INPUT);
  pinMode(waterPin, INPUT);
  #else
  pinMode(brewPin,  INPUT_PULLUP);
  pinMode(steamPin, INPUT_PULLUP);
  #ifdef waterPin
  pinMode(waterPin, INPUT_PULLUP);
  #endif
  #endif
}

static inline void switchLED(uint32_t pin, bool state) {
#ifdef CATURRA
  digitalWrite(pin, state ? LOW : HIGH);
#endif
}

// Actuating the heater element
static inline void setBoilerOn(void) {
  digitalWrite(relayPin, HIGH);  // boilerPin -> HIGH
  // turn on brew temp LED on brew mode or water mode (or none which is preparing for brew mode)
  switchLED(brewTempLED, currentState.brewSwitchState || !currentState.steamSwitchState);
  // turn on steam temp LED on steam mode (only)
  switchLED(steamTempLED, currentState.steamSwitchState && !currentState.brewSwitchState);
}

static inline void setBoilerOff(void) {
  digitalWrite(relayPin, LOW);  // boilerPin -> LOW
  switchLED(brewTempLED, false);
  switchLED(steamTempLED, false);
}

static inline void setSteamValveRelayOn(void) {
  #ifdef steamValveRelayPin
  digitalWrite(steamValveRelayPin, HIGH);  // steamValveRelayPin -> HIGH
  #endif
}

static inline void setSteamValveRelayOff(void) {
  #ifdef steamValveRelayPin
  digitalWrite(steamValveRelayPin, LOW);  // steamValveRelayPin -> LOW
  #endif
}

static inline void setSteamBoilerRelayOn(void) {
  #ifdef steamBoilerRelayPin
  digitalWrite(steamBoilerRelayPin, HIGH);  // steamBoilerRelayPin -> HIGH
  #endif
}

static inline void setSteamBoilerRelayOff(void) {
  #ifdef steamBoilerRelayPin
  digitalWrite(steamBoilerRelayPin, LOW);  // steamBoilerRelayPin -> LOW
  #endif
}

//Function to get the state of the brew switch button
//returns true or false based on the read P(power) value
static inline bool brewState(void) {
#ifdef CATURRA
  return digitalRead(brewPin) == HIGH;
#else
  return digitalRead(brewPin) == LOW; // pin will be low when switch is ON.
#endif
}

// Returns HIGH when switch is OFF and LOW when ON
// pin will be high when switch is ON.
static inline bool steamState(void) {
#ifdef CATURRA
  return digitalRead(steamPin) == HIGH;
#else
  return digitalRead(steamPin) == LOW; // pin will be low when switch is ON.
#endif
}

static inline bool waterPinState(void) {
#ifdef waterPin
#ifdef CATURRA
  return digitalRead(waterPin) == HIGH;
#else
  return digitalRead(waterPin) == LOW; // pin will be low when switch is ON.
#endif
#else
  return false;
#endif
}

static inline void openValve(void) {
  #if defined LEGO_VALVE_RELAY
    digitalWrite(valvePin, LOW);
  #else
    digitalWrite(valvePin, HIGH);
  #endif
}

static inline void closeValve(void) {
  #if defined LEGO_VALVE_RELAY
    digitalWrite(valvePin, HIGH);
  #else
    digitalWrite(valvePin, LOW);
  #endif
}

#endif

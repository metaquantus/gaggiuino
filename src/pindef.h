/* 09:32 15/03/2023 - change triggering comment */
#ifndef PINDEF_H
#define PINDEF_H

// STM32F4 pins definitions
#ifdef CATURRA
// SPI2
#define thermoDO      PB14
#define thermoDI      PB15 // not used
#define thermoCS      PB12
#define thermoCLK     PB13

#define zcPin         PB5

#if defined INT_RELAY
#define relayPin      PC5
#else
#define relayPin      PC9  // Ext R1
#endif
#define dimmerPin     PC11
#define valvePin      PB4

#define brewPin       PC6
#define steamPin      PC7
#define waterPin      PC8
// analog read
#define pressurePin   PA0

#define led1Pin       PC0
#define led2Pin       PC1
#define led3Pin       PC2
#define led4Pin       PC3

#define brewLED       led3Pin
#define steamLED      led4Pin
#define brewTempLED   led1Pin
#define steamTempLED  led2Pin

#else

#define thermoDO      PB4
#define thermoDI      PA7 // not used
#define thermoCS      PA6
#define thermoCLK     PA5

#define zcPin         PA0
#define brewPin       PC14
#define relayPin      PA15
#define dimmerPin     PA1
#define steamPin      PC15
#define valvePin      PC13
#if defined(SINGLE_BOARD)
#define waterPin      PB15
#else
#define waterPin      PA12
#endif

#define brewLED       PNUM_NOT_DEFINED
#define steamLED      PNUM_NOT_DEFINED
#define brewTempLED   PNUM_NOT_DEFINED
#define steamTempLED  PNUM_NOT_DEFINED


#endif

#ifdef PCBV2
// PCB V2
#define steamValveRelayPin PB12
#define steamBoilerRelayPin PB13
#endif

#define HX711_sck_1   PB0
#define HX711_dout_1  PB8
#define HX711_dout_2  PB9

#define USART_LCD     Serial2 // PA2(TX) & PA3(RX)
#define USART_ESP     Serial1 // PA9(TX) & PA10(RX)
#define USART_DEBUG   Serial  // USB-CDC (Takes PA8,PA9,PA10,PA11)

#endif

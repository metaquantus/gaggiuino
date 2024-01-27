#ifndef CATURRA_ADC_H
#define CATURRA_ADC_H

#ifdef CATURRA
#include "Arduino.h"
#include "analog.h"
#include "STM32ADC.h"

// Oversampling settings (16 over-samples default)
#define ADC_SAMPLES_DIV 4
#define ADC_SAMPLES 1 << ADC_SAMPLES_DIV

/**
  This is the internal MCU ADC. We set it to the slowest clock and sampling rate,
  even so, it's still very fast for our purposes so we have room for doing averaging over-sampling.
  The parent class sets the ADC in continuous conversion mode writing to a DMA buffer on 
  each interrupt.
  In addition to the pressure in channel 0, we read the internal MCU temperature
  and the analog voltage reference for calibration.
  Note the the parent class programs the the ADC directly using the STM32 HAL libraries,
  any use of the Arduino analogRead function will mess up the ADC. If there's need to read 
  another analog pin, just add another channel to this class.
*/
class CaturraADC : public STM32ADC {
public:
    CaturraADC(uint32_t pressPin) : STM32ADC(12, ADC_CLOCK_SYNC_PCLK_DIV8, true, true) {
        // external pressure sensor reading scaled down and offset from 5V to 3.3V 
        pRank = addChannel(pressPin, ADC_SAMPLETIME_480CYCLES);
        // internal temperature sensor, for monitoring MCU health (usually a few °C above ambient temperature when iddle)
        tRank = addChannel(ATEMP, ADC_SAMPLETIME_480CYCLES);
        // measured analog voltage reference, used for calibrating other ADC readings
        vRank = addChannel(AVREF, ADC_SAMPLETIME_480CYCLES);
        calculateTempCalibration();
    }
    
    uint32_t getPressureValue() {
        return pVal;
    }
    uint32_t getTempValue() {
        return tVal;
    }
    uint32_t getVRefValue() {
        return vVal;
    }
    float getPressure();
    float getMcuTemp();
    float getVRef();
    // get previous error code if any and reset error state
    short getError() {
      short err = errorCode;
      // reset state
      errorCode = 0;
      return err;
    }
    
protected:
    short errorCode = 0;
    uint32_t pSum = 0;
    int pCount = 0;
    uint32_t pVal = 0;
    int pRank;

    uint32_t tSum = 0;
    int tCount = 0;
    uint32_t tVal = 0;
    int tRank;

    uint32_t vSum = 0;
    int vCount = 0;
    uint32_t vVal = 0;
    int vRank;

    float calTempSlope = 1;
    float calTempOffset = 0;

    void calculateTempCalibration();
    void onInterrupt() override;
    void onError() override;
    
};
#endif

#endif
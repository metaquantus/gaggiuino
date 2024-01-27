#include "caturra_adc.h"
#include "pindef.h"

#ifdef CATURRA

void CaturraADC::onInterrupt() {
    // get the samples and average them

    // pressure
    if ( pCount == ADC_SAMPLES ) {
        pVal = pSum >> ADC_SAMPLES_DIV;
        pCount = 0;
        pSum = 0;
    }
    pSum += getValue(pRank);
    pCount++;

    // internal temperature
    if ( tCount == ADC_SAMPLES ) {
        tVal = tSum >> ADC_SAMPLES_DIV;
        tCount = 0;
        tSum = 0;
    }
    tSum += getValue(tRank);
    tCount++;

    // Analog voltage reference
    if ( vCount == ADC_SAMPLES ) {
        vVal = vSum >> ADC_SAMPLES_DIV;
        vCount = 0;
        vSum = 0;
    }
    vSum += getValue(vRank);
    vCount++;

}

void CaturraADC::onError() {
    // reinitializes the hardware
    end();
    begin();
    // TODO: get the actual error
    errorCode = 1;
}

float CaturraADC::getPressure() {
    // adjust also for VREF
    float p = (float)(pVal - 170.f) / 312.167f * getVRef()/3.3f;

    if ( p < 0 ) {
        p = 0;
    } else if ( p > 12.f) {
        p = 12.f;
    }
    return p;
}

float CaturraADC::getVRef() {
    // VDDA calculated based on the measured vref and calibration data stored in ROM
    // calibration values are in millivolts
    return (float)VREFINT_CAL_VREF * (float)*VREFINT_CAL_ADDR / vVal / 1000;
}

float CaturraADC::getMcuTemp() {
    // adjust for VREF
  float adjtemp = getVRef() / (float)VREFINT_CAL_VREF * 1000.f * (float)tVal;

  // get calibrated temperature using simple linear function
  return (float) (calTempSlope * (float) adjtemp + calTempOffset);
}

void CaturraADC::calculateTempCalibration() {
    // calibration values for the internal temperature sensor are stored
    // in ROM at manufacturing time
    // Use a simple linear equation to interpolate temperatures
    calTempSlope = (float)((uint16_t)TEMPSENSOR_CAL2_TEMP - (uint16_t)TEMPSENSOR_CAL1_TEMP) / ((uint16_t)*TEMPSENSOR_CAL2_ADDR - (uint16_t)*TEMPSENSOR_CAL1_ADDR);
    calTempOffset = (uint16_t)TEMPSENSOR_CAL1_TEMP - calTempSlope * (uint16_t)*TEMPSENSOR_CAL1_ADDR;

};

// the global instance, note that the object is initialized by the pressure sensor code
CaturraADC ADS(pressurePin);

#endif
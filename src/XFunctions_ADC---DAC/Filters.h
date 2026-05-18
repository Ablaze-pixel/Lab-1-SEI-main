#ifndef FILTERS_H
#define FILTERS_H

// Filtre și conversii generice pentru date de la senzor.
float filter_saltAndPepperFilter(float newValue);
float filter_weightedAverageFilter(float currentVal, float alpha = 0.2f);
bool  filter_onOffHysteresis(float currentValue, float setPoint, float hysteresis, bool previousState);
float filter_applySaturation(float value, float minVal, float maxVal);

// Control PID - setează parametrii de tuning.
void  filter_pidSetGains(float kp, float ki, float kd);

// Control PID - calculează ieșire pe bază de eroare și timp.
float filter_pidCompute(float setPoint, float currentValue, float dt_ms);

// Control PID - resetează integratorul și derivatorul.
void  filter_pidReset(void);

// Conversii generice ADC -> Volt -> parametru fizic.
float filter_adcToVoltage(float rawValue, float vRef, float adcResolution);
float filter_linearSensorConversion(float voltage, float slope, float offset);
float filter_adc_ToPhysical(float rawValue, float vRef, float adcResolution, float slope, float offset);
float filter_adc_VoltageToTemperature(float voltage, float vRef, float seriesResistor, float r25, float beta);

#endif // FILTERS_H

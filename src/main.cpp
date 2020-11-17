#include <Arduino.h>

#include <arduinoFFT.h>
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define CHANNEL A0
const uint16_t samples = 512;		   //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 5000; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/

#include "fft_util.h"

void setup()
{
	Serial.begin(1000000);
	sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
}

void loop()
{
	//Print out what frequency is the most dominant.
	CalcFFT(CHANNEL, sampling_period_us, samples);
	delay(2000);		  /* Repeat after delay */
}
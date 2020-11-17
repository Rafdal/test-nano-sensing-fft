#include <Arduino.h>
#include <Wire.h>

#include <arduinoFFT.h>
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define CHANNEL A0
const uint16_t samples = 256;		   //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 750; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

#include "fft_util.h"
#include "gyro_util.h"

void setup()
{
	Serial.begin(921600);
	sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
	while (!Serial);
	Serial.println("Started");
	delay(500);

	gyroSetup();
	
}
imu_data_t IMUdata;
void loop()
{
	//Print out what frequency is the most dominant.
	microseconds = micros();
	for (int i = 0; i < samples; i++)
	{
		IMUdata = readGyro(IMUdata);
		vReal[i] = IMUdata.x;
		vImag[i] = 0;
		while (micros() - microseconds < sampling_period_us)
		{
			//empty loop
		}
		microseconds += sampling_period_us;
	}

	CalcFFT(vReal, vImag, CHANNEL, sampling_period_us, samples);
	
			  /* Repeat after delay */
	// imu_data_t gyro = readGyro();
	delay(2000);
}
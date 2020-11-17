#include <Arduino.h>
#include <Wire.h>

#include <arduinoFFT.h>
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define CHANNEL A0
#define SAMPLING 512
const uint16_t samples = SAMPLING;		   //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 900; //Hz, must be less than 10000 due to ADC

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
#include "sd_util.h"

#define MAX_VUELTAS 50
uint8_t vueltas;

void setup()
{
	Serial.begin(921600);
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
	delay(5000);

	gyroSetup();
	sdSetup(4);
	
	digitalWrite(13, LOW);
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

	fft_data_t data = CalcFFT(vReal, vImag, CHANNEL, sampling_period_us, samples);
	
	sdLog(data);
	vueltas++;
	// Serial.println(vueltas); 
	if (vueltas >= MAX_VUELTAS)
	{
		for(;;){
			digitalWrite(13, HIGH);
			delay(1000);
			digitalWrite(13, LOW);
			delay(1000);
			NRF_WDT->RR[0] = WDT_RR_RR_Reload; 
		}
	}
}
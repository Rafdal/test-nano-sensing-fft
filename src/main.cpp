#include <Arduino.h>
#include <Wire.h>

#include <arduinoFFT.h>
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define CHANNEL A0
#define SAMPLING 256
const uint16_t samples = SAMPLING;		   //This value MUST ALWAYS be a power of 2
double samplingFrequency = 475; //Hz, must be less than 10000 due to ADC

// 80	= 44 + 0 + 5
// 229	= 47 + 0 + 8
// 300	= 39 + 0 + 2
// 350	= 46 + 0 + 5
// 400	= 42 + 0 + 6
//*440 	= 51 + 0 + 5
//*450	= 38 + 0 + 10
//*460	= 40 + 0 + 8
//*470	= 29 + 0 + 6
//#475	= 25 + 0 + 4
//%480	= 37 + 1
//!490	= 72 + 15
//!500	= 101 + 23
//!512	= 165 + 48
//!850	= 651 + 213

unsigned int sampling_period_us;
unsigned long us, ms_stamp;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vImag[samples];

double vX[samples];
double vY[samples];
double vZ[samples];

#include "IMU_util.h"
#include "sd_util.h"
#include "fft_util.h"

#define MAX_VUELTAS 50
uint8_t vueltas;

void calcSamplingPeriod(double freq)
{
	sampling_period_us = round(1000000 * (1.0 / freq));
}

void setup()
{
	Serial.begin(921600);
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	calcSamplingPeriod(samplingFrequency);
	IMUSetup();

	delay(1000);
	sdSetup(384);

	// Depurar el gyro
	delay(1000);

	digitalWrite(13, LOW);
	Serial.println(F("Started")); 
}

imu_data_t raw_data[samples];
imu_data_t current_data;

void sd_log_xyz(fft_data_t x, fft_data_t y, fft_data_t z);

void loop()
{
	// Print out what frequency is the most dominant.
	us = micros();
	ms_stamp = millis();
	for (int i = 0; i < samples; i++)
	{
		raw_data[i] = readGyro(raw_data[i]);
		vImag[i] = 0;
		while (micros() - us < sampling_period_us);

		us += sampling_period_us;
	}
	Serial.println("Read!"); 
	// unsigned long t2cap = millis() - ms_stamp;

	// ms_stamp = millis();
	// Serial.println("x, y, z");
	uint16_t zero_count = 0;
	uint8_t row_zeros = 0;
	uint8_t spikes = 0;
	for (int i = 0; i < samples; i++)
	{
		if (raw_data[i].x == 0)
			zero_count++;
		if (raw_data[i].y == 0)
			zero_count++;
		if (raw_data[i].z == 0)
			zero_count++;

		if (raw_data[i].x == 0 && raw_data[i].y == 0 && raw_data[i].z == 0)
			row_zeros++;
		
		if (abs(raw_data[i].x) > 6 || abs(raw_data[i].y) > 6 || abs(raw_data[i].z) > 5)
		{
			spikes++;
			// printIMU(current_data);
			vX[i] = current_data.x;
			vY[i] = current_data.y;
			vZ[i] = current_data.z;
		}
		else
		{
			current_data = filterIMU(raw_data[i], current_data, 0.25);
			// printIMU(current_data);
			vX[i] = current_data.x;
			vY[i] = current_data.y;
			vZ[i] = current_data.z;
		}
		
	}
	Serial.println(F("Filtered!")); 
	// Serial.print(F("Time to capt (ms): "));
	// Serial.println(t2cap); 
	// Serial.print(F("Time to print (ms): "));
	// Serial.println(millis() - ms_stamp); 
	/* Serial.print(F("ROW ZEROS: "));
	Serial.println(row_zeros); 
	Serial.print(F("SPIKES: "));
	Serial.println(spikes); 
	Serial.print(F("ZERO Count: "));
	Serial.print(zero_count); 
	Serial.print("\t@ ");
	Serial.print(samplingFrequency); 
	Serial.println(F(" Hz"));  */

	fft_data_t fft_x = CalcFFT(vX, vImag, samples);
	fft_data_t fft_y = CalcFFT(vY, vImag, samples);
	fft_data_t fft_z = CalcFFT(vZ, vImag, samples);
	
	sd_log_xyz(fft_x, fft_y, fft_z);

	vueltas++;
	Serial.println(vueltas); 

	if (vueltas >= MAX_VUELTAS)
	{
		while(1);
	}
}


void sd_log_xyz(fft_data_t x, fft_data_t y, fft_data_t z)
{
	uint16_t size = x.size;

	File dataFile = SD.open("dataset.csv", FILE_WRITE);
	if (dataFile) 
    {
        for (uint16_t i = 0; i < size; i++)
        {
            dataFile.print(x.data[i]);
			dataFile.print(',');          
        }
		for (uint16_t i = 0; i < size; i++)
        {
            dataFile.print(y.data[i]);
			dataFile.print(',');          
        }
		for (uint16_t i = 0; i < size; i++)
        {
            dataFile.print(z.data[i]);
			if (i < size-1)
			{
				dataFile.print(',');
			}
        }
		dataFile.println();
    }
    else
    {
        Serial.println(F("Error abriendo el archivo"));
    }
    dataFile.close();
}
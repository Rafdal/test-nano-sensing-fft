#include <SPI.h>
#include <SD.h>

void sdSetup(uint8_t cs_pin)
{
    pinMode(cs_pin, OUTPUT);
    digitalWrite(cs_pin, LOW);   // Activar SD
    if (!SD.begin(cs_pin)) {
        Serial.println(F("SD card failed, or not present"));
        pinMode(LED_BUILTIN, OUTPUT);
        for (;;){
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
            NRF_WDT->RR[0] = WDT_RR_RR_Reload;       
        }
    }
    Serial.println(F("SD card initialized.\n"));

    if (SD.exists("dataset.csv"))
        SD.remove("dataset.csv");

    File dataFile = SD.open("dataset.csv", FILE_WRITE);


    if (dataFile) 
    {
        for (uint16_t i = 0; i < SAMPLING/2; i++)
        {
            dataFile.print(i);
            Serial.print(i);
            if (i < (SAMPLING/2)-1)
            {
                dataFile.print(',');
                Serial.print(',');
            }            
        }
        dataFile.println();
        Serial.println();
    }
    else
    {
        Serial.println(F("Error abriendo el archivo"));
    }
    dataFile.close();
}


void sdLog(fft_data_t fft)
{
    unsigned long us = micros();
    File dataFile = SD.open("dataset.csv", FILE_WRITE);
    if (dataFile) 
    {
        for (uint16_t i = 0; i < fft.size; i++)
        {
            dataFile.print((int)(fft.data[i]));
            Serial.print((int)(fft.data[i]));
            if (i < fft.size-1)
            {
                dataFile.print(',');
                Serial.print(',');
            }            
        }
        dataFile.println();
        Serial.println();
    }
    else
    {
        Serial.println(F("Error abriendo el archivo"));
    }
    dataFile.close();
    // Serial.print(F("Data logged in: "));
    // Serial.print(micros() - us);
    // Serial.println(F(" us")); 
}
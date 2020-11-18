#include <SPI.h>
#include <SD.h>

void sdSetup(uint16_t n_cols)
{
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);   // Activar SD
    if (!SD.begin(4)) {
        Serial.println(F("SD card failed, or not present"));
        for(;;);
    }
    Serial.println(F("SD card initialized.\n"));

    if (SD.exists("dataset.csv"))
        SD.remove("dataset.csv");

    File dataFile = SD.open("dataset.csv", FILE_WRITE);

    if (dataFile) 
    {
        for (uint16_t i = 0; i < n_cols ; i++)
        {
            dataFile.print(i);
            Serial.print(i);
            if (i < n_cols-1)
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

void sdLog(float data[], uint16_t size)
{
    unsigned long us = micros();
    File dataFile = SD.open("dataset.csv", FILE_WRITE);
    if (dataFile) 
    {
        for (uint16_t i = 0; i < size; i++)
        {
            dataFile.print(data[i]);
            Serial.print(data[i]);
            if (i < size-1)
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

void sdRawLog(File dataFile, float data[], uint16_t size)
{
    if (dataFile) 
    {
        for (uint16_t i = 0; i < size; i++)
        {
            dataFile.print(data[i]);
            Serial.print(data[i]);
            if (i < size-1)
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
}
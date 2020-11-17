#include <SPI.h>
#include <SD.h>

void sdSetup(uint8_t cs_pin)
{
    pinMode(cs_pin, OUTPUT);
    digitalWrite(cs_pin, LOW);   // Activar SD
    if (!SD.begin(cs_pin)) {
        Serial.println(F("SD card failed, or not present"));
        while (1);
    }
    Serial.println(F("SD card initialized."));

    if (SD.exists("dataset.csv"))
        SD.remove("dataset.csv");
}


void sdLog(fft_data_t fft)
{
    // unsigned long us = micros();
    File dataFile = SD.open("dataset.csv", FILE_WRITE);
    if (dataFile) 
    {
        for (uint16_t i = 0; i < fft.size; i++)
        {
            dataFile.print((int)(fft.data[i]));
            dataFile.print(',');
        }
        dataFile.println();
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
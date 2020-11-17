#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

typedef struct fft_data
{
  double data[SAMPLING];
  uint16_t size;
}fft_data_t;


void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
    case SCL_INDEX:
      abscissa = (i * 1.0);
      break;
    case SCL_TIME:
      abscissa = ((i * 1.0) / samplingFrequency);
      break;
    case SCL_FREQUENCY:
      abscissa = ((i * 1.0 * samplingFrequency) / samples);
      break;
    }
    // Serial.print(abscissa, 6);
    // if (scaleType == SCL_FREQUENCY)
      // Serial.print("Hz");
    // Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}

fft_data_t CalcFFT(double vR[], double vI[], uint8_t channel, unsigned int sampling, uint16_t samples)
{
  double data[samples], win[samples];
  /* Print the results of the sampling according to time */
  for (uint16_t i = 0; i < samples; i++)
    data[i] = vR[i];
  
  // Serial.println("Data:");
  // PrintVector(vR, samples, SCL_TIME);
  // ! Filtro FFT Hann para vibraciones
  FFT.Windowing(vR, samples, FFT_WIN_TYP_HANN, FFT_FORWARD);
  // Serial.println("Weighed data:");
    for (uint16_t i = 0; i < samples; i++)
      win[i] = vR[i];
  // PrintVector(vR, samples, SCL_TIME);
  FFT.Compute(vR, vImag, samples, FFT_FORWARD); /* Compute FFT */
  // Serial.println("Computed Real values:");
  // PrintVector(vR, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(vR, vImag, samples); /* Compute magnitudes */
  // Serial.println("Computed magnitudes:");
  // PrintVector(vR, (samples >> 1), SCL_FREQUENCY);
  fft_data_t output;
  for (uint16_t i = 0; i < (samples >> 1); i++)
      output.data[i] = vR[i];

  output.size = (samples >> 1);

  return output;
  /* for (uint16_t i = 0; i < samples; i++)
  {
    Serial.print(data[i]);
    Serial.print(',');
    Serial.print(win[i]);
    Serial.print(',');
    if (i < (samples >> 1))
    {
      Serial.print(vR[i]); 
    }

    Serial.println();
  } */
}
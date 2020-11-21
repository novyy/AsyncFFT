// Name: AsyncFFT.h
// Date: 21-Nov-2020
// Purpose: Exapmle generates 1 ses of signal and transform it in time slices
// Author: Piotr Nowinski

#include "AsyncFFT.h"
#include "FFTUtils.h"

const uint32_t BUFFER_SIZE = 1024;
const uint32_t SAMPLE_RATE = 1000;

afft::Array data(BUFFER_SIZE);
AsyncFFT fft;


void prepareData()
{
    const uint32_t frequency_1 = 15.0;
    const uint32_t frequency_2 = 5.0;
    double usec = 0;   
    
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        const double value_1 = sin(2 * PI * frequency_1 * double(usec) * 0.000001);
        const double value_2 = sin(2 * PI * frequency_2 * double(usec) * 0.000001);
        usec += 1000000 / SAMPLE_RATE;
        data[i] = value_1/* + value_2*/;
    }
}

void setup()
{
    Serial.begin(115200);
    while(!Serial){}
    Serial.println("");

    prepareData();
        
    fft.init(data.size());
        
    fft.compute(data, [](afft::Array& data)
    {
        for(uint32_t index = 0; index < data.size() / 2; index++)
        {
            const uint32_t freq = afft::utils::indexToHz(index, BUFFER_SIZE, SAMPLE_RATE);
            if(freq > 50) break;
            Serial.println(String(freq) + " Hz - " + afft::utils::amplitude(data[index]));
        }
    });
}

void loop() 
{
    // Spend max 3 milliseconds on transformation
    fft.loop(3000);
}

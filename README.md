# AsyncFFT
AsyncFFT allows you to perform long FFT in short time slices

## When do you need it
When you need to periodically do some time critical operationa and you can't afford to hangs up for dozens of milliseconds
to do FFT. You can devide transformation on short time slices.

## Target platform
ESP8266

## Reference

### Constructors:
AsyncFTT::AsyncFTT()

### Methods
#### void AsyncFFT::init(uint32_t N) 
* Initialize object. If you don't call this method then it will increase the execution time of first loop() call.
* N - size of samples buffer you want to process

#### void compute(afft::Array& data, const afft::CompleteCallback& complete)
* schedule transformation and then return immediately
* data - array with complex numbers
* complete - completion callback

#### void loop(uint32_t timeout);
* give chance to do some transformation
* timeout - max time in microseconds which can be spend on tranformation


# AsyncFFT
AsyncFFT allows you to perform long FFT in short time slices

## When do you need it
When you need to periodically do some time critical operationa and you can't afford to hangs up for dozens of milliseconds
to do FFT you can devide transformation on short time slices.

## Target platform
ESP8266

## Reference

### Constructors:

AsyncFTT::AsyncFTT()

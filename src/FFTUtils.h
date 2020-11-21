
#include <cmath>
#include <complex>

namespace afft
{
    namespace utils
    {
        inline uint32_t indexToHz(uint32_t index, uint32_t bufferSize, uint32_t sampleRate)
        {
            return ((index + 1) * sampleRate/2) / (bufferSize/2);
        }

        inline double amplitude(std::complex<double> value)
        {
            return sqrt(value.real() * value.real() + value.imag() * value.imag());
        }
    }
}
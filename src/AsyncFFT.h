// Name: AsyncFFT.h
// Date: 21-Nov-2020
// Purpose: AsyncFFT allows you to stagger transformation
// Author: Piotr Nowinski

#pragma once

#include <complex>
#include <valarray>
#include <cmath>
#include <memory>

namespace afft
{
    typedef std::complex<double> Complex;
    typedef std::valarray<Complex> Array;

    namespace impl
    {
        struct Context
        {
            Complex phiT;
            Complex T;
            uint32_t n;
            uint32_t k;
            uint32_t l;
            uint32_t a;
            Array& data;
        };
    }
}

class AsyncFFT
{
public:
    void init(uint32_t N);
    void compute(afft::Array& data, const std::function<void(afft::Array&)> complete);
    void loop(uint32_t timeout);

private:
    void compute(std::unique_ptr<afft::impl::Context>& pCtx, uint32_t timeout) const;
    void decimate(afft::Array& data) const;

private:
    std::unique_ptr<afft::impl::Context> m_pCtx;
    afft::Complex m_phiT;
    uint32_t m_N;
    std::function<void(afft::Array&)> m_complete;
};

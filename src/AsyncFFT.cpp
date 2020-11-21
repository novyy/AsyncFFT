// Name: AsyncFFT.h
// Date: 21-Nov-2020
// Purpose: AsyncFFT allows you to stagger transformation
// Author: Piotr Nowinski

#include "AsyncFFT.h"

void AsyncFFT::init(uint32_t N)
{
    const double thetaT = M_PI / N;
    m_phiT = afft::Complex(cos(thetaT), -sin(thetaT));
    m_N = N;
}

void AsyncFFT::loop(uint32_t timeout)
{
    if(m_pCtx != nullptr) compute(m_pCtx, timeout);
}

void AsyncFFT::compute(afft::Array& data, const std::function<void(afft::Array&)> complete)
{
    if(data.size() != m_N) init(data.size());
    m_complete = complete;
    m_pCtx = std::unique_ptr<afft::impl::Context>(new afft::impl::Context{m_phiT, afft::Complex(), 0, m_N, 0, 0, data});
}

void AsyncFFT::compute(std::unique_ptr<afft::impl::Context>& pCtx, uint32_t timeout) const
{
    const uint32_t startTime = micros();
    bool resume = (pCtx->k != m_N);
    while (pCtx->k > 1)
    {
        if(resume == false)
        {
            pCtx->n = pCtx->k;
            pCtx->k >>= 1;
            pCtx->phiT = pCtx->phiT * pCtx->phiT;
            pCtx->T = 1.0L;
        }

        if(resume == false) uint32_t l = pCtx->l = 0;
        for (; pCtx->l < pCtx->k; pCtx->l++)
        {
            if(resume == false) pCtx->a = pCtx->l;
            for (; pCtx->a < m_N; pCtx->a += pCtx->n)
            {
                if(((micros() - startTime) >= timeout) && resume == false) return;
                resume = false;
                
                const uint32_t b = pCtx->a + pCtx->k;
                const afft::Complex t = pCtx->data[pCtx->a] - pCtx->data[b];
                pCtx->data[pCtx->a] += pCtx->data[b];
                pCtx->data[b] = t * pCtx->T;
            }
            pCtx->T *= pCtx->phiT;
        }
    }

    decimate(pCtx->data);
    m_complete(pCtx->data);
    pCtx.reset();
}

void AsyncFFT::decimate(afft::Array& data) const
{
    uint32_t N = data.size();
    uint32_t m = (unsigned int)log2(N);
    for (uint32_t a = 0; a < N; a++)
    {
        uint32_t b = a;
        b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
        b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
        b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
        b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
        if (b > a)
        {
            const afft::Complex t = data[a];
            data[a] = data[b];
            data[b] = t;
        }
    }
}

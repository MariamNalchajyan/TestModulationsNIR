
#include <stdio.h>
#include <cuda_runtime.h>

#include <vector>
#include "Generation.h"

#include <thrust/complex.h>


using std::generate;
using std::vector;



__global__ void Kernel(thrust::complex<double>* s1, double* keys1, thrust::complex<double>* s2, double* keys2,
    thrust::complex<double>* VFN, double* time_keys, double* freq_keys,
    int s1_size, int s2_size, int time_keys_size, int freq_keys_size, double sampling_frequency)
{
    int n = blockIdx.y * blockDim.y + threadIdx.y;
    int g = blockIdx.x * blockDim.x + threadIdx.x;
    double k = 1. + g * sampling_frequency / (s2_size - 1) / 1600.e6;//1600e6 - это f1

    thrust::complex<double> temp_sum(0, 0);
  
    if ((n < time_keys_size) && (g < freq_keys_size)) {

        for (int m = 0; m < s1_size; m++) {
            double index = (double)(m + n-s2_size) / k;
            int i1 = (int)index;
            int i2 = i1 + 1;
            if (i1 >= 0 && (i2 < s2_size))
            {
                double re = s2[i1].real() + (index / sampling_frequency - keys2[i1]) * (s2[i2].real() - s2[i1].real()) / (keys2[i2] - keys2[i1]);
                double im = s2[i1].imag() + (index / sampling_frequency - keys2[i1]) * (s2[i2].imag() - s2[i1].imag()) / (keys2[i2] - keys2[i1]);
                temp_sum += s1[m] * thrust::complex < double >(re, -im)* exp(thrust::complex<double>(0, -1) * 2. * M_PI * (k - 1) * 1600.e6 * (double)m / sampling_frequency);
            }
            else continue;

        }
        VFN[n * freq_keys_size + g] = temp_sum;
        freq_keys[g] = (g * sampling_frequency / (s2_size - 1));///*(k - 1) * 1600.e6*/);
        time_keys[n] = ((double)(n - s2_size) / sampling_frequency);
    }
}


void cudaModifVFN(Signal& signal1, Signal& signal2, Signal2D& outsignal, double sampling_frequency)
{
    outsignal.signal.clear();
    outsignal.t_keys.clear();
    outsignal.f_keys.clear();

    int N1 = signal1.keys.size();
    int N2 = signal2.keys.size();
    

    outsignal.signal.resize((N1 + N2));
    outsignal.t_keys.resize((N1 + N2));
    outsignal.f_keys.resize(N2);

    // Host vectors
    vector<thrust::complex<double>> h_s1(N1);
    vector<thrust::complex<double>> h_s2(N2);
    vector<double>h_k1 = signal1.keys;
    vector<double>h_k2 = signal2.keys;
    vector<thrust::complex<double>> h_vfn((N1 + N2) * N2);
    vector<double>h_time_keys((N1 + N2));
    vector<double>h_freq_keys(N2);

    for (int i = 0; i < N1; i++)
    {
        h_s1[i] = (thrust::complex<double>(signal1.signal[i].real(), signal1.signal[i].imag()));
    }
    for (int i = 0; i < N2; i++)
    {
        h_s2[i] = (thrust::complex<double>(signal2.signal[i].real(), signal2.signal[i].imag()));
    }

    // Allocate device memory
    thrust::complex<double>* d_s1, * d_s2, * d_vfn;
    double* d_k1, * d_k2;
    double* d_t, * d_f;
    cudaMalloc(&d_s1, N1 * sizeof(thrust::complex<double>));
    cudaMalloc(&d_s2, N2 * sizeof(thrust::complex<double>));
    cudaMalloc(&d_k1, N1 * sizeof(double));
    cudaMalloc(&d_k2, N2 * sizeof(double));
    cudaMalloc(&d_vfn, (N1+N2) * N2 * sizeof(thrust::complex<double>));
    cudaMalloc(&d_t, (N1 + N2) * sizeof(double));
    cudaMalloc(&d_f, N2 * sizeof(double));

    // Copy data to the device
    cudaMemcpy(d_s1, h_s1.data(), N1 * sizeof(thrust::complex<double>), cudaMemcpyHostToDevice);
    cudaMemcpy(d_s2, h_s2.data(), N2 * sizeof(thrust::complex<double>), cudaMemcpyHostToDevice);
    cudaMemcpy(d_k1, h_k1.data(), N1 *sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_k2, h_k2.data(), N2*sizeof(double), cudaMemcpyHostToDevice);

    dim3 threads(32,32);
    dim3 blocks(N1/32, (N1 + N2)/32);

    // Launch kernel
    Kernel << <blocks, threads >> > (d_s1, d_k1, d_s2, d_k2, d_vfn, d_t, d_f, N1, N2, (N1 + N2), N2, sampling_frequency);

    // Copy back to the host
    cudaMemcpy(h_vfn.data(), d_vfn, (N1 + N2) * N2 * sizeof(thrust::complex<double>), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_time_keys.data(), d_t, (N1 + N2) * sizeof(double), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_freq_keys.data(), d_f, N2 * sizeof(double), cudaMemcpyDeviceToHost);

    for (int i = 0; i < (N1 + N2); i++)
    {
        outsignal.signal[i].resize(N2);
        for (int j = 0; j < N2; j++)
        {
            outsignal.signal[i][j] = h_vfn[i * N2 + j];
        }
    }
    outsignal.t_keys = h_time_keys;
    outsignal.f_keys = h_freq_keys;
    //int iter = 0;
    //for (int n = -(int)(signal2.signal.size()); n <= (int)(signal1.signal.size() - 1); n++)
    //{
    //    outsignal.t_keys[iter] = ((double)n / sampling_frequency);
    //    iter++;
    //}
    //for (int g = 0; g < signal2.signal.size(); g++)
    //{
    //    outsignal.f_keys[g] = (g * sampling_frequency / (signal2.signal.size() - 1)/*(k - 1) * 1600.e6*/);
    //}

    cutFrequency(outsignal);
    
    // Free memory on device
    cudaFree(d_s1);
    cudaFree(d_s2);
    cudaFree(d_k1);
    cudaFree(d_k2);
    cudaFree(d_vfn);
    cudaFree(d_t);
    cudaFree(d_f);

 
}
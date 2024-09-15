#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <complex>
#include "functions.h"
#include <algorithm>
#include <numeric>


using namespace std;

static bool RandomBit(double low_chance)
{
	return (rand() < RAND_MAX * low_chance);
}
struct Signal
{
	vector<complex<double>> signal;
	vector<double> keys;
};
struct Signal2D
{
	vector<vector<complex<double>>>signal;
	vector<double>t_keys;
	vector<double>f_keys;
};





//vector<double>source_bits;
static Signal MSK(int N, double Br, double sampling_frequency)
{
	//source_bits.clear();

	//duration - продолжительность сигнала [с]
	//Br - битовая скорость передачи [бит/с]
	//sampling_period - период дискретизации [с]

	Signal ret_signal;//I(t) и Q(t) компоненты (комплексная огибающая сигнала)
	double sampling_period = 1. / sampling_frequency;
	int samples_in_bit = (int)(sampling_frequency /Br);//столько отсчетов надо для  1 бита
	double separated_bit_summ = 0;
	int index =0;
	double t = 0;
	while (true)
	{
		double separated_bit = RandomBit(0.5) * 2. - 1;
		for (int n_sample = 0; n_sample < samples_in_bit; n_sample++)
		{
			
			//source_bits.push_back(separated_bit);
			ret_signal.signal.push_back(
				complex<double>(
					cos(M_PI / 2. * (separated_bit_summ + Br * separated_bit * t)),
					sin(M_PI / 2. * (separated_bit_summ + Br * separated_bit * t))
					));
		
			t += sampling_period;
			ret_signal.keys.push_back(t);
			index++;
			if (index == N)
				return ret_signal;
		}
		separated_bit_summ += separated_bit;
	}
	
}
static Signal BPSK(int N, double Br, double sampling_frequency)
{
	//source_bits.clear();

	
	//Br - битовая скорость передачи [бит/с]
	//sampling_period - период дискретизации [с]

	Signal ret_signal;//I(t) и Q(t) компоненты (комплексная огибающая сигнала)
	double sampling_period = 1. / sampling_frequency;
	int samples_in_bit = (int)(sampling_frequency / Br);//столько отсчетов надо для получения 1 бита

	int index = 0;
	double t = 0;
	while (true)
	{
		double bit = RandomBit(0.5);
		for (int n_sample = 0; n_sample < samples_in_bit; n_sample++)
		{
			//if(bit==true)source_bits.push_back(1);
			//else source_bits.push_back(-1);

			ret_signal.signal.push_back(
				complex<double>(
					cos(bit * M_PI),
					sin(bit * M_PI)
					));
			t += sampling_period;
			ret_signal.keys.push_back(t);
			index++;
			if (index == N)
				return ret_signal;
		}

	}
	return ret_signal;
}

static Signal FHSS(int N,double Br,double sampling_frequency)
{
	
	double sampling_period = 1. / sampling_frequency;

	double carriers[51] = {
		969e6, 972e6, 975e6, 978e6, 981e6,
		984e6, 987e6, 990e6, 993e6, 996e6,
		999e6, 1002e6, 1005e6, 1008e6, 1053e6,
		1056e6, 1059e6, 1062e6, 1065e6, 1113e6,
		1116e6, 1119e6, 1122e6, 1125e6, 1128e6,
		1131e6, 1134e6, 1137e6, 1140e6, 1143e6,
		1146e6, 1149e6, 1152e6, 1155e6, 1158e6,
		1161e6, 1164e6, 1167e6, 1170e6, 1173e6,
		1176e6, 1179e6, 1182e6, 1185e6, 1188e6,
		1191e6, 1194e6, 1197e6, 1200e6, 1203e6,
		1209e6 };

	//double carriers[51] = {
	//	969, 972, 975, 978, 981,
	//	984, 987, 990, 993, 996,
	//	999, 1002, 1005, 1008, 1053,
	//	1056, 1059, 1062, 1065, 1113,
	//	1116, 1119, 1122, 1125, 1128,
	//	1131, 1134, 1137, 1140, 1143,
	//	1146, 1149, 1152, 1155, 1158,
	//	1161, 1164, 1167, 1170, 1173,
	//	1176, 1179, 1182, 1185, 1188,
	//	1191, 1194, 1197, 1200, 1203,
	//	1209 };

	double I = 0.0;
	double Q = 0.0;
	double phase = 0.0;
	double phase_shift = 0.0;
	int bits_per_word = 1;

	int samples_in_bit = (int)(sampling_frequency / Br);
	double freq_shift = 0;

	double dbl_index = 0;
	double t = 0;

	double separated_bit_summ = 0;
	auto comp_j = complex<double>(0, 1);

	Signal ret_signal=BPSK(N,Br,sampling_frequency);

	int carrier_index = 0;

	for (int i = 0; i < ret_signal.signal.size(); i++)
	{
		if (i % (bits_per_word * samples_in_bit) == 0)
		{
			carrier_index = 51.0 * (double)rand() / RAND_MAX;
			freq_shift = (carriers[carrier_index] - carriers[0]);
		}

		ret_signal.signal[i] = ret_signal.signal[i] * exp(comp_j * 2.0 * M_PI * freq_shift * ret_signal.keys[i]);
	}
	return ret_signal;

}


static Signal OFDM(double B, int N, int M,double Bitrate,double sampling_frequency)
{
	// B- полоса частот
	// N кол-во поднесущих 
	//M кол-во отсчетов каждого сигнала

	Signal ret_signal;
	ret_signal.signal.resize(M);
	ret_signal.keys.resize(M);

	double df = B / N;//разность поднесущих
	double newBr = Bitrate / N;
	vector<vector<complex<double>>>vector_of_BPSK(N, vector<complex<double>>(M));
	for (int i = 0; i < N; i++)
	{
		vector_of_BPSK[i] = BPSK(M, newBr, sampling_frequency).signal;
		for (int j = 0; j < M; j++)
		{
			vector_of_BPSK[i][j] *= exp(1.i * 2. * M_PI * (double)(i+1) * df * (double)j / sampling_frequency);
		}
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			ret_signal.signal[i] += vector_of_BPSK[j][i];
		}
		ret_signal.keys[i] = (double)i / sampling_frequency;
	}
	return ret_signal;
}
static double DiapasonRand(double a, double b)
{
	double tmp = static_cast<double>(rand()) / RAND_MAX;
	return a + tmp * (b - a);
}
static vector<double> GenerateDelays(int satellites_number,double min_delay,double max_delay)
{
	vector<double> del;
	for (int j = 0; j < satellites_number; j++)
	{
		del.push_back(DiapasonRand(min_delay, max_delay));
	}
	return del;
}


static void addNoise(vector<complex<double>>& buf, double SNR)
{
	//srand(0);
	double energy = std::accumulate(buf.begin(), buf.end(), 0.0,
		[&](double a, complex <double> b)
		{
			return a + b.real() * b.real() + b.imag() * b.imag();
		});

	vector <complex<double>> noise(buf.size(), 0.0);
	vector <double> ampl(buf.size(), 0.0);
	std::transform(ampl.begin(), ampl.end(), ampl.begin(),
		[&](double a)
		{
			int count_rep = 20;
			a = 0.0;
			for (int rep = 0; rep < count_rep; rep++)
			{
				a += ((double)rand() / RAND_MAX) * 2.0 - 1.0;
			}
			a /= count_rep;
			return a;
		});
	std::transform(noise.begin(), noise.end(), ampl.begin(), noise.begin(),
		[&](complex<double> a, double ampl)
		{
			double fi = (double)rand() / RAND_MAX * 2.0 * M_PI;
			return complex <double>(ampl * cos(fi), ampl * sin(fi));
		});
	double noise_energy = std::accumulate(noise.begin(), noise.end(), 0.0,
		[&](double a, complex <double> b)
		{
			return a + b.real() * b.real() + b.imag() * b.imag();
		});
	double norm_coef = energy * pow(10.0, -SNR / 10.0) / noise_energy;
	std::transform(buf.begin(), buf.end(), noise.begin(), buf.begin(),
		[&](complex <double> a, complex <double> b)
		{
			return complex <double>(a.real() + sqrt(norm_coef) * b.real(),
				a.imag() + sqrt(norm_coef) * b.imag());
		});
}



static void transformSignal2(Signal& base_signal, double time_delay, double freq_delay, int new_sample_size, double sampling_freq, double SNR, Signal& ret_signal, bool ofdm = false, double scale = 1)
{
	int time_shift;
	vector<double>new_keys;
	vector<complex<double>>new_signal;

	time_shift = time_delay * sampling_freq;
	
	if (ofdm)
	{
		sampling_freq *= scale;
	}
	
	if (time_shift < base_signal.signal.size() && (time_shift + new_sample_size - 1) < base_signal.signal.size())
	{
		for (int i = 0; i < (new_sample_size); i++)
		{
			ret_signal.signal.push_back(
				base_signal.signal[(unsigned int)(i + time_shift)]);
			ret_signal.keys.push_back(base_signal.keys[i]);
			new_keys.push_back(((double)i * 1. / sampling_freq));
		}
		if (ofdm)
		{
			Interpol(ret_signal.signal, ret_signal.keys, new_signal, new_keys);
			ret_signal.signal = new_signal;
			ret_signal.keys = new_keys;
		}
		for (int i = 0; i < (new_sample_size); i++)
		{
			ret_signal.signal[i] = ret_signal.signal[i] * exp(1.i * 2. * M_PI * freq_delay * (1. / sampling_freq) * (double)i);
		}
		
	}

	addNoise(ret_signal.signal, SNR);
}
static void Сorrelate(Signal& signal1, Signal& signal2, Signal& correlation,double sampling_frequency)
{
	for (int n = -(int)(signal2.signal.size()); n <= (int)(signal1.signal.size() - 1); n++)
	{
		complex<double> counter = complex<double>(0, 0);
		for (unsigned int m = 0; m < signal1.signal.size(); m++)
		{
			if ((m + n) >= 0 && (m + n) < signal2.signal.size())
			{
				counter += signal1.signal[m] * conj(signal2.signal[m + n]);
			}
			else continue;
		}
		
		correlation.signal.push_back(counter);
		correlation.keys.push_back((double)n/sampling_frequency);
	}
}
 
static void cutFrequency(Signal2D& insignal)
 {
	 complex<double> buf;
	 for (int i = 0; i < insignal.signal.size() - 1; i++)
	 {
		 for (int j = 0; j < insignal.signal[i].size() / 2; j++)
		 {
			 buf = insignal.signal[i][j];
			 insignal.signal[i][j] = insignal.signal[i][j + insignal.signal[i].size() / 2];
			 insignal.signal[i][j + insignal.signal[i].size() / 2] = buf;
		 }
	 }
	 vector<double> bufKey;
	 bufKey.resize(insignal.f_keys.size());
	 for (int j = 0; j < insignal.f_keys.size(); j++)
	 {
		 if (j < insignal.f_keys.size() / 2)
			 bufKey[j] = -insignal.f_keys[-j + insignal.f_keys.size() / 2];
		 if (j > insignal.f_keys.size() / 2)
			 bufKey[j] = insignal.f_keys[j - insignal.f_keys.size() / 2];
	 }
	 insignal.f_keys = bufKey;
 }
static void CalcVFN(Signal& signal1, Signal& signal2, Signal2D& outsignal,double sampling_frequency)
 {
	//обычная ВФН, расчет с БФП

	 outsignal.signal.resize(signal1.signal.size() + signal2.signal.size());
	 outsignal.f_keys.resize(signal1.signal.size());
	 outsignal.t_keys.resize(signal1.signal.size() + signal2.signal.size());
	 vector<complex<double>>mass(signal1.signal.size());
	 int iter = 0;
	 
	 for (int n = -(int)(signal2.signal.size()); n <= (int)(signal1.signal.size() - 1); n++)
	 {
		 mass.clear();
		 mass.resize(signal1.signal.size());
		 complex<double> counter = complex<double>(0, 0);
		 for (int m = 0; m < signal1.signal.size(); m++)
		 {
			 if ((m + n) >= 0 && (m + n) < signal2.signal.size())
			 {
				 mass[m]= signal1.signal[m] * conj(signal2.signal[m + n]);
			 }
			 else continue;
			 outsignal.f_keys[m]=(m * sampling_frequency / (signal1.signal.size()-1));
		 }
		 mass = FFT(mass, -1);
		 outsignal.signal[iter] = mass;
		 outsignal.t_keys[iter]=((double)n / sampling_frequency);
		 iter++;
	 }

	
	 cutFrequency(outsignal);

	
 }

static void ModifVFN(Signal& signal1, Signal& signal2, Signal2D& outsignal, double sampling_frequency)
{
	//модтфицированная ВФН(с масштабированием-интерполяцией), расчет в лоб
	outsignal.signal.resize(signal1.signal.size()+ signal2.signal.size());
	outsignal.t_keys.resize(signal1.signal.size()+ signal2.signal.size());
	int iter = 0;

	for (int n = -(int)(signal2.signal.size()); n <= (int)(signal1.signal.size() - 1); n++)
	{

		for (int g = 0; g < signal2.signal.size();g++)
		{
			double k = 1. + g * sampling_frequency / (signal2.signal.size()-1) / 1600.e6;//1600e6 - это f1


			complex<double> sum(0, 0);
			for (int m = 0; m < signal1.signal.size(); m++)
			{
				double index = (double)(m + n) / k;
				int i1 = (int)index;
				int i2 = i1 + 1;
				if (i1 >= 0 && (i2 < signal2.signal.size())) 
				sum += signal1.signal[m] * conj(LinInterpol(signal2.signal[i1], signal2.signal[i2], signal2.keys[i1], signal2.keys[i2], index/sampling_frequency))
					* exp(-1.i * 2. * M_PI * (k - 1) * 1600.e6 * (double)m / sampling_frequency);
				else continue;

			}
			outsignal.signal[iter].push_back(sum);
			if (iter == 0)
			{
				outsignal.f_keys.push_back(g * sampling_frequency / (signal2.signal.size()-1)/*(k - 1) * 1600.e6*/);
			}
		}
		
		outsignal.t_keys[iter] = ((double)n / sampling_frequency);
		iter++;
	}


	cutFrequency(outsignal);


}

static vector<double>GetProjection(Signal2D& matrix, int index)
 {
	 vector<double>res(matrix.signal.size());
	 for (int j = 0; j < matrix.signal.size(); j++)
	 {
		 res[j] = abs(matrix.signal[j][index]);
	 }
	 return res;
 }
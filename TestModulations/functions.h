#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <complex>
using namespace std;

static double MaxElement(vector <double>x)
{
	double max = x[0];
	for (int i = 0; i < x.size(); i++)
	{
		if (max < x[i]) max = x[i];
	}
	return max;
}
static double MinElement(vector <double> x)
{
	double min = x[0];
	for (int i = 0; i < x.size(); i++)
	{
		if (min > x[i]) min = x[i];
	}
	return min;
}
static vector<double> MaxElementWithIndex(vector <double> x)
{
	vector<double>res(2);

	double max = x[0];
	for (int i = 0; i < x.size(); i++)
	{
		if (max < x[i])
		{
			max = x[i];
			res[0] = max;
			res[1] = i;
		}
	}
	return res;
}


static vector<double> GetMaxMin(vector<vector<complex<double>>>& matrix)
{
	vector<double>max_min;

	int maxi, maxj, mini, minj;
	double max = abs(matrix[0][0]);
	double min = abs(matrix[0][0]);
	maxi = 0;
	maxj = 0;
	int M = matrix.size();
	int N = matrix[0].size();
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (abs(matrix[i][j]) > max)
			{
				max = abs(matrix[i][j]); maxi = i; maxj = j;
			}
		}
	}
	max_min.push_back(max);
	max_min.push_back(min);
	max_min.push_back(maxi);
	max_min.push_back(maxj);

	return max_min;
}



//одномерное быстрое Фурье преобразование(is=-1 прямое,is=+1 обратное )
static vector<complex<double>> FFT(vector<complex<double>>& in, double is)
{
	vector<complex<double >> F = in;

	complex<double>  temp, w, c;
	long i, i1, j, j1, istep;
	long m, mmax;
	long n = F.size();
	double fn, r1, theta;
	fn = (double)n;
	double r = M_PI * is;
	j = 1;
	for (i = 1; i <= n; i++)
	{
		i1 = i - 1;
		if (i < j)
		{
			j1 = j - 1;
			temp = F[j1];
			F[j1] = F[i1];
			F[i1] = temp;
		}
		m = n / 2;
		while (j > m) { j -= m;	m = (m + 1) / 2; }
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = 2 * mmax;
		r1 = r / (double)mmax;
		for (m = 1; m <= mmax; m++)
		{
			theta = r1 * (double)(m - 1);
			w = complex<double>(cos(theta), sin(theta));
			for (i = m - 1; i < n; i += istep)
			{
				j = i + mmax;
				c = F[j];
				temp = w * c;
				F[j] = F[i] - temp;
				F[i] = F[i] + temp;
			}
		}
		mmax = istep;
	}
	if (is > 0)  for (i = 0; i < n; i++)
	{
		F[i]/= fn;
		}
	return F;
}




static complex<double>LinInterpol(complex<double>y1, complex<double>y2, double x1, double x2, double x)
{
	double re = y1.real() + (x - x1) * (y2.real() - y1.real()) / (x2 - x1);
	double im = y1.imag() + (x - x1) * (y2.imag() - y1.imag()) / (x2 - x1);
	return complex<double>(re, im);

}
static complex<double>DblIndexInterpol(vector<complex<double>>signal, double index, double sampling_period)
{
	double curr_time = index * sampling_period;
	int i1 = (int)index;
	int i2 = i1 + 1;
	double x1 = i1 * sampling_period;
	double x2 = i2 * sampling_period;
	if (i1 >= 0 && i1 < (signal.size() - 1))
	{
		return LinInterpol(signal[i1], signal[i2], x1, x2, curr_time);
	}
	else
	{
		return complex<double>(0, 0);
	}
}
static void Interpol(vector<complex<double>> vec_y, vector<double> vec_x, vector<complex<double>>& new_vec_y, vector<double>new_vec_x)
{
	new_vec_y.clear();
	new_vec_y.resize(new_vec_x.size());

	for (int i = 0; i < new_vec_x.size(); i++)
	{
		for (int j = 0; j < vec_x.size()-1; j++)
		{
			if ((new_vec_x[i] >= vec_x[j]) && (new_vec_x[i] < vec_x[j + 1]))
			{
				new_vec_y[i]=(LinInterpol(vec_y[j], vec_y[j + 1], vec_x[j], vec_x[j + 1], new_vec_x[i]));
				break;
			}
		}
	}

}


static double PeakCriteria(vector<double>& input)
{
	double max = input[0];
	double srednee = 0;
	for (int i = 0; i < input.size(); i++)
	{
		if (max < input[i]) max = input[i];
		srednee += input[i];

	}
	srednee /= input.size();

	double res = 0;
	for (int i = 0; i < input.size(); i++)
	{
		res += (input[i] - srednee) * (input[i] - srednee);
	}
	return (max - srednee) / sqrt(res / input.size());
}

static int roundUp(double numToround, int multiple)
{
	int numToRound = round(numToround);
    if (multiple == 0)
        return numToRound;

    int remainder = abs(numToRound) % multiple;
    if (remainder == 0)
        return numToRound;

    if (numToRound < 0)
        return -(abs(numToRound) - remainder);
    else
        return numToRound + multiple - remainder;
}
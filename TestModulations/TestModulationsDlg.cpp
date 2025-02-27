
// TestModulationsDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "TestModulations.h"
#include "TestModulationsDlg.h"
#include "afxdialogex.h"
#include "Header.cuh"
#include <iostream>
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CTestModulationsDlg



CTestModulationsDlg::CTestModulationsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTMODULATIONS_DIALOG, pParent)
	, vfn_time(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestModulationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, checkbox);
	DDX_Control(pDX, IDC_CHECK2, cuda_checkbox);

	DDX_Text(pDX, IDC_EDIT1, vfn_time);
}

BEGIN_MESSAGE_MAP(CTestModulationsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestModulationsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestModulationsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestModulationsDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// Обработчики сообщений CTestModulationsDlg

BOOL CTestModulationsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	pic1.Create(GetDlgItem(IDC_PIC1)->GetSafeHwnd());
	pic2.Create(GetDlgItem(IDC_PIC2)->GetSafeHwnd());


	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTestModulationsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);

		
		
	}
	else
	{
		CDialogEx::OnPaint();
		/*if (FN_f.size() != 0)
		{
			pic1.DrawGrid(FN_t, MinElement(VFN.t_keys), MinElement(FN_t), MaxElement(VFN.t_keys), MaxElement(FN_t), VFN.t_keys);
			pic1.Draw(L"red", FN_t, MinElement(VFN.t_keys), MinElement(FN_t), MaxElement(VFN.t_keys), MaxElement(FN_t), VFN.t_keys);


			pic2.DrawGrid(FN_f, MinElement(VFN.f_keys), MinElement(FN_f), MaxElement(VFN.f_keys), MaxElement(FN_f), VFN.f_keys);
			pic2.Draw(L"red", FN_f, MinElement(VFN.f_keys), MinElement(FN_f), MaxElement(VFN.f_keys), MaxElement(FN_f), VFN.f_keys);
		}*/
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTestModulationsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestModulationsDlg::OnBnClickedButton1()
{
	VFN.signal.clear();
	VFN.f_keys.clear();
	VFN.t_keys.clear();
	FN_f.clear();
	FN_f.clear();
	
	vector<double>source_signal;
	vector<double>time_keys;
	vector<double>freq_keys;
	
	double f0 = 1600.e6;//несущая
	double Br = 60.e6;//скорость передачи данных
	double sampling_frequency = 500.e6;
	int N = 20;//кол-во поднесущих-
	double M = 8192.;//кол-во отсчетов каждого сигнала
	double B = 200.e6;//ширина спектра
	
	
	//Signal manip_signal = MSK(M, Br, sampling_frequency);
	//Signal manip_signal = BPSK(M, Br, sampling_frequency);
	Signal manip_signal = OFDM(B,N,M,Br,sampling_frequency);
	bool is_ofdm = true;

	
	//сигналы полученные спутниками 
	Signal sputnik1_signal, sputnik2_signal;
	//сигналы полученные спутниками 
	double time_delay1 = 2100. / sampling_frequency;
	double freq_delay1 = 1600.e4;
	double time_delay2 = 1000. / sampling_frequency;
	double freq_delay2 = 1600.e3;
	double scale1 = 1. + abs(freq_delay1 / f0);
	double scale2 = 1. + abs(freq_delay2 / f0);
	int new_sample_size = M/4;



	//vector<complex<double>>spectr = FFT(manip_signal.signal, -1);
	//vector<double>ampl_sp(spectr.size());
	//vector<double>fr;
	//for (int i = 0; i < spectr.size(); i++)
	//{
	//	ampl_sp[i] = abs(spectr[i]);
	//	fr.push_back((double)i * sampling_frequency / spectr.size());
	//}
	//pic1.Draw(L"red",ampl_sp, MinElement(fr), MinElement(ampl_sp), MaxElement(fr), MaxElement(ampl_sp), fr);


	transformSignal2(manip_signal, time_delay1, freq_delay1, new_sample_size, sampling_frequency, 10, sputnik1_signal,is_ofdm,scale1);
	transformSignal2(manip_signal, time_delay2, freq_delay2, new_sample_size, sampling_frequency, 10, sputnik2_signal,is_ofdm,scale2);
	
	
	/*vector<complex<double>>spectr1 = FFT(sputnik1_signal.signal, -1);
	vector<double>ampl_sp1(spectr1.size());
	vector<double>fr1;
	for (int i = 0; i < spectr1.size(); i++)
	{
		ampl_sp1[i] =  abs(spectr1[i]);
		fr1.push_back((double)i * (sampling_frequency*scale1) / spectr1.size());
	}
	pic1.Draw(L"",L"",L"red", ampl_sp1, MinElement(fr1), MinElement(ampl_sp1), MaxElement(fr1), MaxElement(ampl_sp1), fr1);


	vector<complex<double>>spectr2 = FFT(sputnik2_signal.signal, -1);
	vector<double>ampl_sp2(spectr2.size());
	vector<double>fr2;
	for (int i = 0; i < spectr2.size(); i++)
	{
		ampl_sp2[i] = abs(spectr2[i]);
		fr2.push_back((double)i * (sampling_frequency * scale2) / spectr1.size());
	}
	pic2.Draw(L"", L"", L"red", ampl_sp2, MinElement(fr2), MinElement(ampl_sp2), MaxElement(fr2), MaxElement(ampl_sp2), fr2);*/

	
	///////////////////////////  Коррелляция   //////////////////////////////
	/*Signal correlation;
	Сorrelate(sputnik1_signal, sputnik2_signal, correlation,sampling_frequency);
	int corr_size = correlation.signal.size();
	vector<double>ampl_corr(corr_size);
	for (int i = 0; i < corr_size; i++)
	{
		ampl_corr[i] = abs(correlation.signal[i]);
	}
	int index_max = MaxElementWithIndex(ampl_corr)[1];
	pic2.Draw(L"t,c", L"ВКФ", L"red", ampl_corr, MinElement(correlation.keys), MinElement(ampl_corr), MaxElement(correlation.keys), roundUp(MaxElement(ampl_corr), 10), correlation.keys, true, index_max);*/


//	pic2.Draw(L"", L"", L"red", ampl_corr, MinElement(correlation.keys), MinElement(ampl_corr), MaxElement(correlation.keys), MaxElement(ampl_corr), correlation.keys);

	std::ofstream out("info.txt", std::ios::app);
	out << endl << endl << endl;
	/////////////      ВФН   //////////////////////////////
	int start, end; 
	if (checkbox.GetState() == BST_CHECKED)
	{
		if (cuda_checkbox.GetState() != BST_CHECKED)
		{
			out << "ModifVFN" << endl;
			start = clock();
			//ModifVFN(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
			ModifVFNwithFFT(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
			end = clock();
		}
		else
		{
			out << "CudaModifVFN" << endl;
			start = clock();
			cudaModifVFN(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
			end = clock();
		}
	}
	else
	{
		out << "VFN" << endl;
		start = clock();
		//CalcVFN(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
		CalcVFNwithFFT(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
		end = clock();
	}
	vfn_time = (double)(end - start) / CLOCKS_PER_SEC;
	out << "sample_size = " << new_sample_size << endl;
	out << "vfn.f.size = " << VFN.f_keys.size() << endl;
	out << "vfn.t.size = " << VFN.t_keys.size() << endl;
	out << "time = " << vfn_time << endl;
	UpdateData(FALSE);
	vector<double>maximums = GetMaxMin(VFN.signal);
	int max_t_index = maximums[3];
	int max_f_index = maximums[2];
	FN_f.resize(VFN.signal[0].size());
	FN_t.resize(VFN.signal.size());
	for (int i = 0; i < VFN.signal[0].size(); i++)
	{
		FN_f[i] = abs(VFN.signal[max_f_index][i]);
	}
	FN_t = GetProjection(VFN, max_t_index);
	pic1.Draw(L"t,с", L"Сечение №1 модиф. ВФН OFDM-сигналов", L"red", FN_t, MinElement(VFN.t_keys), MinElement(FN_t), MaxElement(VFN.t_keys), roundUp(MaxElement(FN_t), 10), VFN.t_keys, true, max_f_index);
	pic2.Draw(L"f,Гц", L"Сечение №2 модиф. ВФН OFDM-сигналов", L"red", FN_f, MinElement(VFN.f_keys), MinElement(FN_f), MaxElement(VFN.f_keys), roundUp(MaxElement(FN_f), 10), VFN.f_keys, true, max_t_index);


	//double f_shift = VFN.f_keys[MaxElementWithIndex(FN_f)[1]];
	//double t_shift = VFN.t_keys[MaxElementWithIndex(FN_t)[1]];
		

	
}


void CTestModulationsDlg::OnBnClickedButton2()
{
	//vector<double>source_signal;
	//vector<double>time_keys;
	//vector<double>freq_keys;

	//double f0 = 1600.e6;//несущая
	//double Br = 60.e6;//скорость передачи данных
	//double sampling_frequency = 500.e6;
	//int N = 20;//кол-во поднесущих-
	//double M = 8192.;//кол-во отсчетов каждого сигнала
	//double B = 200.e6;//ширина спектра


	////Signal manip_signal = MSK(M, Br, sampling_frequency);
	//Signal manip_signal = BPSK(M, Br, sampling_frequency);
	////Signal manip_signal = OFDM(B, N, M, Br, sampling_frequency);
	//bool is_ofdm = false;


	////сигналы полученные спутниками 
	//double time_delay1 = 1100. / sampling_frequency;
	//double freq_delay1 = 0;// 1600.e4;
	//double time_delay2 = 1000. / sampling_frequency;
	//double freq_delay2 = 0;
	//double scale1 = 1. + abs(freq_delay1 / f0);
	//double scale2 = 1. + abs(freq_delay2 / f0);
	//int new_sample_size = M / 4;


	//vector<double>axis_criteria_t, axis_criteria_f, axis_noise;
	//ofstream out("research.txt", ios::app);

	//out << "ВКФ с BPSK" << endl;
	//for (int noise = -10; noise <= 10; noise += 2)
	//{
	//	axis_noise.push_back(noise);
	//	double sum1 = 0, sum2 = 0;
	//	for (int j = 0; j < 100; j++)
	//	{
	//		Signal sputnik1_signal, sputnik2_signal;
	//		transformSignal2(manip_signal, time_delay1, freq_delay1, new_sample_size, sampling_frequency, (double)noise, sputnik1_signal, is_ofdm, scale1);
	//		transformSignal2(manip_signal, time_delay2, freq_delay2, new_sample_size, sampling_frequency, (double)noise, sputnik2_signal, is_ofdm, scale2);


	//		VFN.signal.clear();
	//		VFN.f_keys.clear();
	//		VFN.t_keys.clear();
	//		FN_f.clear();
	//		FN_f.clear();


	//		//CalcVFN(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
	//		////ModifVFN(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
	//		//vector<double>maximums = GetMaxMin(VFN.signal);
	//		//int max_t_index = maximums[3];
	//		//int max_f_index = maximums[2];
	//		//FN_f.resize(VFN.signal[0].size());
	//		//FN_t.resize(VFN.signal.size());
	//		//for (int i = 0; i < VFN.signal[0].size(); i++)
	//		//{
	//		//	FN_f[i] = abs(VFN.signal[max_f_index][i]);
	//		//}
	//		//FN_t = GetProjection(VFN, max_t_index);

	//		//sum1 += PeakCriteria(FN_t);
	//		//sum2 += PeakCriteria(FN_f);
	//		Signal correlation;
	//		Сorrelate(sputnik1_signal, sputnik2_signal, correlation, sampling_frequency);
	//		int corr_size = correlation.signal.size();
	//		vector<double>ampl_corr(corr_size);
	//		for (int i = 0; i < corr_size; i++)
	//		{
	//			ampl_corr[i] = abs(correlation.signal[i]);
	//		}
	//		sum1 += PeakCriteria(ampl_corr);
	//	}

	//	axis_criteria_t.push_back(sum1 / 100);
	//	axis_criteria_f.push_back(sum2 / 100);
	//	out << noise << " " << sum1 / 100 << " " << sum2 / 100 << endl;
	//	//int index_max = MaxElementWithIndex(ampl_corr)[1];
	//	//pic2.Draw(L"t,c", L"ВКФ", L"red", ampl_corr, MinElement(correlation.keys), MinElement(ampl_corr), MaxElement(correlation.keys), roundUp(MaxElement(ampl_corr), 10), correlation.keys, true, index_max);


	//}
	//pic1.Draw(L"ОСШ,дБ", L"Критерий выраженности главного максимума ВКФ", L"red", axis_criteria_t, MinElement(axis_noise), MinElement(axis_criteria_t),
	//	MaxElement(axis_noise), MaxElement(axis_criteria_t), axis_noise);
	////pic2.Draw(L"ОСШ,дБ", L"Критерий выраженности главного максимума в сечении №2 модиф.ВФН", L"red", axis_criteria_f, MinElement(axis_noise), MinElement(axis_criteria_f),
	////	MaxElement(axis_noise), MaxElement(axis_criteria_f), axis_noise);


	vector<double>noise = { -10,-8,-6,-4,-2,0,2,4,6,8,10 };
	//vector<double>modif_c_t = { 7.48457 ,8.3484 ,9.62734 ,15.3385 ,17.9838 ,18.6836 ,19.1949 ,19.7735 ,20.665 ,20.8202 ,20.6419 };
	//vector<double>modif_c_f = { 6.85459 ,7.45718,9.74019,17.5538, 21.7653 , 26.0039 ,28.6059 , 30.8173 , 32.4781 , 32.842 ,32.9188 };
	vector<double>c_t = { 7.89324  ,7.88824 ,7.53723 , 7.53464 ,7.06275 ,7.15532 ,7.6404 ,7.53268 ,7.42549 ,7.52962 ,7.70935 };
	vector<double>c_f = { 6.20184 , 6.40267, 6.01664, 6.9511 ,7.53818 ,8.64662 ,10.1323 , 10.4809, 10.6354 ,10.7496 ,10.9709 };
	//pic1.Draw_two(L"ОСШ,дБ", L"Критерий выраженности главного максимума в сечении №1", c_t, modif_c_t, MinElement(noise), 7,
	//	MaxElement(noise), roundUp(MaxElement(modif_c_t), 10), noise, noise);
	//pic2.Draw_two(L"ОСШ,дБ", L"Критерий выраженности главного максимума в сечении №2", c_f, modif_c_f, MinElement(noise), 5,
	//	MaxElement(noise), roundUp(MaxElement(modif_c_f), 10), noise, noise);

	vector<double>c_t_bpsk = { 8.72282, 12.011 ,16.0621,18.7299 ,20.2634 ,21.5054 , 22.1035 ,22.2449 ,22.2662,22.3643 ,22.4509 };
	vector<double>c_f_bpsk = { 7.23069, 11.3202 ,16.965 ,22.7257 ,28.9713 , 34.0736 ,37.9742 ,40.5646 , 42.0833 , 42.9867 , 43.5223 };
	//vector<double>c_t_msk = { 8.75971 ,11.3344 ,15.933 ,18.7119,20.1983,21.4075, 21.8812 ,22.3852 ,22.2925 ,22.3896 ,22.4871 };
	//vector<double>c_f_msk = { 7.27802 ,10.4335 ,16.6512 ,23.2124 ,29.0335,34.2339, 38.0743 , 40.639 ,42.0935 ,42.9757 ,43.5121 };
	pic1.Draw_two(L"ОСШ,дБ", L"Критерий выраженности главного максимума в сечении №1", c_t_bpsk, c_t, MinElement(noise),7 ,
	MaxElement(noise), 23, noise, noise);
	pic2.Draw_two(L"ОСШ,дБ", L"Критерий выраженности главного максимума в сечении №2", c_f_bpsk, c_f, MinElement(noise), 6,
		MaxElement(noise), 44, noise, noise);


	/*vector<double>corr_bpsk = { 9.01398, 12.216 ,15.6765,18.5454,20.4603 ,21.4277 ,21.9639 ,22.245 ,22.3422 ,22.4111 ,22.4102};
	vector<double>corr2_bpsk = { 5.20389 , 5.12079 , 5.10485 ,5.16141, 4.94613 , 4.79373,  4.66661 ,4.5911 ,4.60006 ,4.68991 ,4.65043 };
	vector<double>corr_msk = { 7.9477 , 12.3052 , 15.2155 , 18.1716 ,19.8807 , 20.7929 , 21.018 ,21.2811 ,21.334, 21.4172 ,21.4413 };
	vector<double>corr2_msk = { 5.16717, 5.05691 , 4.86151 , 4.88827 , 4.8195 ,4.58144 , 4.24293 , 4.43065 , 4.24352 , 4.44728, 4.475  };
	pic1.Draw_two(L"ОСШ,дБ", L"Критерий выраженности главного максимума ВКФ BPSK-сигналов", corr_bpsk, corr2_bpsk, MinElement(noise),4 ,
		MaxElement(noise), 23, noise, noise);
	pic2.Draw_two(L"ОСШ,дБ", L"Критерий выраженности главного максимума ВКФ MSK-сигналов", corr_msk, corr2_msk, MinElement(noise), 4,
		MaxElement(noise), 23, noise, noise);*/
}

void CTestModulationsDlg::OnBnClickedButton3()
{
	vector<double>time1, time2, accel, number;
	number = { /*100,*/512,1024,2048,4096,7000,8192};
	time1 = {/*0.004,*/ 0.446  ,2.234 ,16.497 ,129.655 ,659.697,1062.42 };
	time2 = { /*0.067,*/9.2,69.874,552.613 ,4482.2 ,21863.221,35503 };
	for (int i = 0; i < time1.size(); i++)
	{
		accel.push_back(time2[i] / time1[i]);
		number[i] *= (2 * number[i]/1.e6);
		time1[i] /= 60;
		time2[i] /= 60;
	}


	pic1.Draw(L"MN,10^6", L"Коэффициент ускорения", L"red", accel, MinElement(number), MinElement(accel), /*roundUp(*/MaxElement(number)/*, 10)*/, MaxElement(accel), number);
	//pic1.Draw(L"MN,10^6", L"Время выполнения,мин", L"red", time1, MinElement(number), MinElement(time1), /*roundUp(*/MaxElement(number)/*, 10)*/, MaxElement(time1), number);
	//pic2.Draw(L"MN,10^6", L"Время выполнения,мин", L"red", time2, MinElement(number), MinElement(time2), /*roundUp(*/MaxElement(number)/*, 10)*/, MaxElement(time2), number);


	///////////////////////////////////////////////
	//VFN.signal.clear();
	//VFN.f_keys.clear();
	//VFN.t_keys.clear();
	//FN_f.clear();
	//FN_f.clear();

	//vector<double>source_signal;
	//vector<double>time_keys;
	//vector<double>freq_keys;

	//double f0 = 1600.e6;//несущая
	//double Br = 60.e6;//скорость передачи данных
	//double sampling_frequency = 500.e6;
	//int N = 20;//кол-во поднесущих-
	//double M = 8192;//кол-во отсчетов каждого сигнала
	//double B = 200.e6;//ширина спектра


	////Signal manip_signal = MSK(M, Br, sampling_frequency);
	////Signal manip_signal = BPSK(M, Br, sampling_frequency);
	//Signal manip_signal = OFDM(B, N, M, Br, sampling_frequency);
	//bool is_ofdm = true;

	//double time_delay1 = 1100. / sampling_frequency;
	//double freq_delay1 = 1600.e4;
	//double time_delay2 = 1000. / sampling_frequency;
	//double freq_delay2 = 1600.e3;
	//double scale1 = 1. + abs(freq_delay1 / f0);
	//double scale2 = 1. + abs(freq_delay2 / f0);

	//
	//vector<double>accel_coef, axis_array_size;
	//Signal sputnik1_signal, sputnik2_signal;
	//vector<double>t1, t2;
	//std::ofstream out("accelCoef.txt", std::ios::app);
	//out << endl;
	//for (int i = 16; i >= 4; i-=2)
	//{
	//	sputnik1_signal.keys.clear();
	//	sputnik1_signal.signal.clear();
	//	sputnik2_signal.keys.clear();
	//	sputnik2_signal.signal.clear();
	//	
	//	int new_sample_size = M / 2;

	//	transformSignal2(manip_signal, time_delay1, freq_delay1, new_sample_size, sampling_frequency, 10, sputnik1_signal, is_ofdm, scale1);
	//	transformSignal2(manip_signal, time_delay2, freq_delay2, new_sample_size, sampling_frequency, 10, sputnik2_signal, is_ofdm, scale2);

	//	VFN.f_keys.clear();
	//	VFN.t_keys.clear();
	//	VFN.signal.clear();

	//	int start1, start2, end1, end2;
	//	start1 = clock();
	//	ModifVFN(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
	//	end1 = clock();
	//	double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
	//	t1.push_back(time1);

	//	VFN.f_keys.clear();
	//	VFN.t_keys.clear();
	//	VFN.signal.clear();

	//	start2 = clock();
	//	cudaModifVFN(sputnik1_signal, sputnik2_signal, VFN, sampling_frequency);
	//	end2 = clock();
	//	double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
	//	t2.push_back(time2);

	//	accel_coef.push_back(time1 / time2);
	//	out << VFN.f_keys.size()<<"  "<< VFN.t_keys.size() << "  " << time1<<"  "<<time2<<"  "<<time1 / time2 << endl;
	//	axis_array_size.push_back(new_sample_size);
	//	
	//}
	//out.close();
	//pic1.Draw_two(L"", L"", t1, t2, MinElement(axis_array_size), MinElement(t2), MaxElement(axis_array_size), MaxElement(t1), axis_array_size, axis_array_size);
	//pic2.Draw(L"N", L"Коэф ускорения", L"red", accel_coef, MinElement(axis_array_size), MinElement(accel_coef), MaxElement(axis_array_size), MaxElement(accel_coef), axis_array_size);

}

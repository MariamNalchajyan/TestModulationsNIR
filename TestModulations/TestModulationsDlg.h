
// TestModulationsDlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include "Generation.h"


// Диалоговое окно CTestModulationsDlg
class CTestModulationsDlg : public CDialogEx
{
// Создание
public:
	CTestModulationsDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMODULATIONS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	Signal2D VFN;
	vector<double>FN_f;
	vector<double>FN_t;
	Drawer pic1,pic2;
	afx_msg void OnBnClickedButton2();
	CButton checkbox;
	CButton cuda_checkbox;

	double vfn_time;
	afx_msg void OnBnClickedButton3();
};

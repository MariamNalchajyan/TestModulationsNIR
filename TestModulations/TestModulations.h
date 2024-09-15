
// TestModulations.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTestModulationsApp:
// Сведения о реализации этого класса: TestModulations.cpp
//

class CTestModulationsApp : public CWinApp
{
public:
	CTestModulationsApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTestModulationsApp theApp;

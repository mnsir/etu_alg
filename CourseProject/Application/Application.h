﻿
// Application.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CApplicationApp:
// Сведения о реализации этого класса: Application.cpp
//

class CApplicationApp : public CWinApp
{
public:
	CApplicationApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CApplicationApp theApp;

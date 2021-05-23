
// ApplicationDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "afxdialogex.h"

#include "driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CApplicationDlg



CApplicationDlg::CApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CApplicationDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_LOAD, &CApplicationDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CApplicationDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// Обработчики сообщений CApplicationDlg

BOOL CApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CApplicationDlg::OnPaint()
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
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include <sstream>
#include <string>
#include <string_view>
#include <list>

void CApplicationDlg::OnBnClickedAdd()
{
	
}


void CApplicationDlg::OnBnClickedLoad()
{
	// TODO: добавьте свой код обработчика уведомлений
	std::list<Driver> drivers;
	using namespace std::literals::string_view_literals;
	pugi::xml_document xml;
	xml.load_file(L"base.xml");
	auto doc = xml.document_element();
	for (auto node : doc.children())
	{
		auto& driver = drivers.emplace_back();
		for (auto attribute : node.attributes())
		{
			if (attribute.name() == L"last_name"sv)
				driver.SetLastName(attribute.value());
			else if (attribute.name() == L"first_name"sv)
				driver.SetFirstName(attribute.value());
			else if (attribute.name() == L"second_name"sv)
				driver.SetSecondName(attribute.value());
			else if (attribute.name() == L"driver_class"sv)
				driver.SetDriverClass(attribute.value());
			else if (attribute.name() == L"employment_date"sv)
				driver.SetEmploymentDate(attribute.value());
			else if (attribute.name() == L"salary"sv)
				driver.SetSalary(attribute.value());
		}
	}
	size_t i = drivers.size();
}


void CApplicationDlg::OnBnClickedButton3()
{
	std::wistringstream ss(
		LR"(Мишустин    Михаил     Владимирович  10 30.06.2015 41000
		Белоусов    Андрей     Рэмович       7  06.11.2013 32000
		Григоренко  Дмитрий    Юрьевич       13 17.02.2006 17000
		Абрамченко  Виктория   Валериевна    8  18.05.2020 25000
		Борисов     Юрий       Иванович      13 07.01.2008 31000
		Голикова    Татьяна    Алексеевна    9  06.05.2015 58000
		Новак       Александр  Валентинович  3  04.10.2001 34000
		Оверчук     Алексей    Логвинович    9  11.07.2008 44000
		Трутнев     Юрий       Петрович      10 01.08.2014 25000
		Хуснуллин   Марат      Шакирзянович  10 16.07.2010 52000
		Чернышенко  Дмитрий    Николаевич    6  13.08.2001 16000
		Зиничев     Евгений    Николаевич    M  05.12.2017 52000
		Козлов      Александр  Александрович 10 18.05.2009 39000
		Колокольцев Владимир   Александрович 7  10.01.2014 16000
		Котяков     Антон      Олегович      5  28.04.2009 26000
		Кравцов     Сергей     Сергеевич     9  01.09.2006 52000
		Лавров      Сергей     Викторович    10 12.10.2020 24000
		Любимова    Ольга      Борисовна     9  15.05.2008 29000
		Мантуров    Денис      Валентинович  M  03.08.2020 57000
		Матыцин     Олег       Васильевич    4  27.06.2008 19000
		Мурашко     Михаил     Альбертович   0  14.04.2016 55000
		Патрушев    Дмитрий    Николаевич    1  15.11.2010 59000
		Решетников  Максим     Геннадьевич   2  28.01.2009 39000
		Савельев    Виталий    Геннадьевич   4  12.06.2008 28000
		Силуанов    Антон      Германович    13 26.04.2018 16000
		Файзуллин   Ирек       Энварович     5  23.06.2014 41000
		Фальков     Валерий    Николаевич    M  04.08.2020 29000
		Чекунков    Алексей    Олегович      4  01.11.2004 26000
		Чуйченко    Константин Анатольевич   2  04.03.2020 18000
		Шадаев      Максут     Игоревич      0  24.10.2005 30000
		Шойгу       Сергей     Кужугетович   1  30.12.2011 47000
		Шульгинов   Николай    Григорьевич   2  30.07.2019 46000)"
	);
	// TODO: добавьте свой код обработчика уведомлений
	pugi::xml_document xml;
	xml.append_child(L"document");
	auto doc = xml.document_element();
	std::wstring s;
	while (std::getline(ss, s)) {
		auto node = doc.append_child(L"driver");
		std::wistringstream line(s);
		line >> s;
		node.append_attribute(L"last_name").set_value(s.c_str());
		line >> s;
		node.append_attribute(L"first_name").set_value(s.c_str());
		line >> s;
		node.append_attribute(L"second_name").set_value(s.c_str());
		line >> s;
		node.append_attribute(L"driver_class").set_value(s.c_str());
		line >> s;
		node.append_attribute(L"employment_date").set_value(s.c_str());
		line >> s;
		node.append_attribute(L"salary").set_value(s.c_str());
	}
	MessageBoxW(xml.save_file(L"base.xml") ? L"ok" : L"not ok", L"result", 0);
}

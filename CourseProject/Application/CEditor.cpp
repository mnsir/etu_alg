// CEditor.cpp: файл реализации
//

#include "pch.h"
#include "Application.h"
#include "CEditor.h"
#include "afxdialogex.h"


// Диалоговое окно CEditor

IMPLEMENT_DYNAMIC(CEditor, CDialog)

CEditor::CEditor(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EDITOR, pParent)
	, m_sLastName(_T(""))
	, m_sFirstName(_T(""))
	, m_sSecondName(_T(""))
	, m_sDriverClass(_T(""))
	, m_dtEmploymentDate(COleDateTime::GetCurrentTime())
	, m_sSalary(_T(""))
{
	
}

CEditor::~CEditor()
{
}

void CEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LAST_NAME, m_sLastName);
	DDX_Text(pDX, IDC_EDIT_FIRST_NAME, m_sFirstName);
	DDX_Text(pDX, IDC_EDIT_SECOND_NAME, m_sSecondName);
	DDX_CBString(pDX, IDC_COMBO_DRIVER_CLASS, m_sDriverClass);
	DDX_DateTimeCtrl(pDX, IDC_DATE_EMPLOTMENT, m_dtEmploymentDate);
	DDX_Text(pDX, IDC_EDIT_SALARY, m_sSalary);
	DDX_Control(pDX, IDC_COMBO_DRIVER_CLASS, m_cCombo);
}


BEGIN_MESSAGE_MAP(CEditor, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений CEditor


BOOL CEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetRedraw(FALSE);
	for (auto str : { L"M", L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12", L"13" })
		m_cCombo.AddString(str);
	SetRedraw(TRUE);
	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

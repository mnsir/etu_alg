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
}


BEGIN_MESSAGE_MAP(CEditor, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений CEditor

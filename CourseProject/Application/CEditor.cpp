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
{

}

CEditor::~CEditor()
{
}

void CEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditor, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений CEditor

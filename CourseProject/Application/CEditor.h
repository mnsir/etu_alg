#pragma once


// Диалоговое окно CEditor

class CEditor : public CDialog
{
	DECLARE_DYNAMIC(CEditor)

public:
	CEditor(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CEditor();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};

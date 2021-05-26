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
public:
	CString m_sLastName;
	CString m_sFirstName;
	CString m_sSecondName;
	CString m_sDriverClass;
	COleDateTime m_dtEmploymentDate;
	CString m_sSalary;
	virtual BOOL OnInitDialog();
	CComboBox m_cCombo;
};

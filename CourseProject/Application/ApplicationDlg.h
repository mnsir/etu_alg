
// ApplicationDlg.h: файл заголовка
//

#pragma once

#include "database.h"

extern UINT DO_UPDATE_LIST;

// Диалоговое окно CApplicationDlg
class CApplicationDlg : public CDialogEx
{
// Создание
public:
	CApplicationDlg(CWnd* pParent = nullptr);	// стандартный конструктор

	DataBase db_;
	BOOL m_bNeedUpdate = FALSE;

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	virtual BOOL OnInitDialog();


// Реализация
protected:
	// Созданные функции схемы сообщений
	DECLARE_MESSAGE_MAP()
public:

	CListCtrl m_cList;

	afx_msg void OnHdnItemclickList(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT DoUpdateList(WPARAM, LPARAM);

	afx_msg void OnLoadUnload();
	afx_msg void OnUpdateLoadUnload(CCmdUI* pCmdUI);

	afx_msg void OnAdd();
	afx_msg void OnUpdateAdd(CCmdUI* pCmdUI);
	afx_msg void OnEdit();
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnRemove();
	afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


// ApplicationDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "afxdialogex.h"

#include "CEditor.h"

#include "driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CApplicationDlg



CApplicationDlg::CApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
{
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_cList);
}

static UINT DO_UPDATE_LIST = ::RegisterWindowMessageW(_T("DoUpdateList"));

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogEx)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CApplicationDlg::OnHdnItemclickList)

	ON_REGISTERED_MESSAGE(DO_UPDATE_LIST, DoUpdateList)

	ON_COMMAND(ID_LOAD_UNLOAD, OnLoadUnload)
	ON_UPDATE_COMMAND_UI(ID_LOAD_UNLOAD, OnUpdateLoadUnload)

	ON_COMMAND(ID_ADD, OnAdd)
	ON_UPDATE_COMMAND_UI(ID_ADD, OnUpdateAdd)
	ON_COMMAND(ID_EDIT, OnEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT, OnUpdateEdit)
	ON_COMMAND(ID_REMOVE, OnRemove)
	ON_UPDATE_COMMAND_UI(ID_REMOVE, OnUpdateRemove)

END_MESSAGE_MAP()


// Обработчики сообщений CApplicationDlg

BOOL CApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	db_.Update = [this]() { m_bNeedUpdate = TRUE; };

	// Добавить столбцы
	LPCTSTR str[] = { _T("ФИО:"), _T("Класс:"), _T("Стаж:"), _T("Оклад:") };
	for (int i = 0; i < 4; ++i) {
		m_cList.InsertColumn(i, str[i]);
		m_cList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}

	{
		CRect rect;
		GetClientRect(&rect);
		rect.top += 50;
		m_cList.MoveWindow(&rect, FALSE);
	}

	return TRUE;
}

#include <sstream>
#include <string>
#include <string_view>
#include <list>



void CApplicationDlg::OnLoadUnload()
{
	if (db_.Size() == 0U)
	{
		db_.Load();
	}
	else
	{
		auto res = MessageBox(_T("Сохранить перед выгрузкой"), _T("Выгрузка"), MB_YESNOCANCEL);
		if (res == IDYES) db_.Unload(true);
		else if (res == IDNO) db_.Unload(false);
	}

}

void CApplicationDlg::OnUpdateLoadUnload(CCmdUI* pCmdUI)
{
	if (db_.Empty())
		pCmdUI->SetText(_T("Загрузить"));
	else
		pCmdUI->SetText(_T("Выгрузить"));
}



void CApplicationDlg::OnHdnItemclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	switch (phdr->iItem) {
	case 0:
		db_.Predicate = [](const Driver& lhs, const Driver& rhs) { return lhs.GetFullName() < rhs.GetFullName(); };
		break;
	case 1:
		db_.Predicate = [](const Driver& lhs, const Driver& rhs) { return lhs.GetDriverClass() < rhs.GetDriverClass(); };
		break;
	case 2:
		db_.Predicate = [](const Driver& lhs, const Driver& rhs) { return lhs.GetEmploymentDate() > rhs.GetEmploymentDate(); };
		break;
	case 3:
		db_.Predicate = [](const Driver& lhs, const Driver& rhs) { return lhs.GetSalary() < rhs.GetSalary(); };
		break;
	}
	db_.Sort();
	*pResult = 0;
}


LRESULT CApplicationDlg::DoUpdateList(WPARAM, LPARAM)
{
	SetRedraw(FALSE);
	m_cList.DeleteAllItems();
	for (size_t pos = {}; pos < db_.Size(); ++pos) {
		int i = static_cast<int>(pos);
		m_cList.InsertItem(i, L"");

		m_cList.SetItemData(i, pos);

		const auto& driver = db_.Get(pos);
		m_cList.SetItemText(i, 0, driver.GetFullName().c_str());
		m_cList.SetItemText(i, 1, static_cast<const wchar_t*>(driver.GetDriverClass()));
		m_cList.SetItemText(i, 2, driver.GetExperience().c_str());
		CString s;
		s.Format(_T("%d руб."), driver.GetSalary());
		m_cList.SetItemText(i, 3, s);
	}

	// Ширина столбцов
	CHeaderCtrl* pHeaderCtrl = m_cList.GetHeaderCtrl();
	for (int i = 0; i < pHeaderCtrl->GetItemCount(); ++i)
	{
		m_cList.SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = m_cList.GetColumnWidth(i);
		m_cList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = m_cList.GetColumnWidth(i);
		m_cList.SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));
	}
	SetRedraw(TRUE);
	Invalidate();
	return 0;
}


void CApplicationDlg::OnAdd()
{
	CEditor dlg;
	if (dlg.DoModal() == IDOK)
	{
		Driver driver;
		driver.SetLastName(dlg.m_sLastName.operator LPCTSTR());
		driver.SetFirstName(dlg.m_sFirstName.operator LPCTSTR());
		driver.SetSecondName(dlg.m_sSecondName.operator LPCTSTR());
		driver.SetDriverClass(DriverClass(dlg.m_sDriverClass));
		SYSTEMTIME st;
		dlg.m_dtEmploymentDate.GetAsSystemTime(st);
		driver.SetEmploymentDate(CTime(st).GetTime());
		driver.SetSalary(_ttoi(dlg.m_sSalary));

		db_.Push(std::move(driver));
	}
}


void CApplicationDlg::OnUpdateAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CApplicationDlg::OnEdit()
{
	size_t pos = m_cList.GetItemData(m_cList.GetSelectionMark());
	const auto& ref = db_.Get(pos);
	CEditor dlg;
	dlg.m_sLastName = ref.GetLastName().c_str();
	dlg.m_sFirstName = ref.GetFirstName().c_str();
	dlg.m_sSecondName = ref.GetSecondName().c_str();
	dlg.m_sDriverClass = static_cast<const wchar_t*>(ref.GetDriverClass());
	dlg.m_dtEmploymentDate = ref.GetEmploymentDate();
	dlg.m_sSalary.Format(_T("%d"), ref.GetSalary());

	if (dlg.DoModal() == IDOK)
	{
		Driver driver = db_.Pop(pos);
		driver.SetLastName(dlg.m_sLastName.operator LPCTSTR());
		driver.SetFirstName(dlg.m_sFirstName.operator LPCTSTR());
		driver.SetSecondName(dlg.m_sSecondName.operator LPCTSTR());
		driver.SetDriverClass(DriverClass(dlg.m_sDriverClass));
		SYSTEMTIME st;
		dlg.m_dtEmploymentDate.GetAsSystemTime(st);
		driver.SetEmploymentDate(CTime(st).GetTime());
		driver.SetSalary(_ttoi(dlg.m_sSalary));

		db_.Push(std::move(driver));
	}
}


void CApplicationDlg::OnUpdateEdit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!db_.Empty() && m_cList.GetItemCount() > 0 && m_cList.GetSelectedCount() == 1U);
}



void CApplicationDlg::OnRemove()
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	MessageBoxW(L"ok", _T(__FUNCTION__), 0);
}


void CApplicationDlg::OnUpdateRemove(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!db_.Empty());
}


BOOL CApplicationDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_bNeedUpdate) {
		PostMessage(DO_UPDATE_LIST);
		m_bNeedUpdate = FALSE;
	}
	UpdateDialogControls(this, TRUE);
	return CDialogEx::PreTranslateMessage(pMsg);
}

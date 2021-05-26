
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
	ON_COMMAND(ID_SEARCH, OnSearch)
	ON_UPDATE_COMMAND_UI(ID_SEARCH, OnUpdateSearch)

END_MESSAGE_MAP()


// Обработчики сообщений CApplicationDlg

BOOL CApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавить столбцы
	LPCTSTR str[] = { _T("ИД:"), _T("ФИО:"), _T("Класс:"), _T("Стаж:"), _T("Оклад:") };
	for (int i = 0; i < 5; ++i) {
		m_cList.InsertColumn(i, str[i]);
		m_cList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}

	CRect rect;
	GetClientRect(&rect);
	rect.top += 50;
	m_cList.MoveWindow(&rect, FALSE);

	return TRUE;
}

void CApplicationDlg::OnLoadUnload()
{
	if (m_bLoaded)
	{
		auto res = MessageBox(_T("Сохранить перед выгрузкой"), _T("Выгрузка"), MB_YESNOCANCEL);
		switch (res) {
		case IDYES:
			::Unload(db_, (LPCTSTR)m_sBaseFileName);
			m_bLoaded = FALSE;
			break;
		case IDNO:
			::Unload(db_);
			m_bLoaded = FALSE;
			break;
		case IDCANCEL:
			break;
		}
	}
	else
	{
		LPCTSTR lpszFilter = _T("База данных XML (*.xml)|*.xml|");
		CFileDialog dlg(TRUE, _T("xml"), _T("*.xml"), OFN_FILEMUSTEXIST, lpszFilter);
		if (dlg.DoModal() == IDOK) {
			dlg.GetPathName();
			m_sBaseFileName = dlg.GetPathName();
			::Load(db_, (LPCTSTR)m_sBaseFileName);
			m_bLoaded = TRUE;
		}
	}
	m_bNeedUpdate = TRUE;
}

void CApplicationDlg::OnUpdateLoadUnload(CCmdUI* pCmdUI)
{
	if (m_bLoaded) pCmdUI->SetText(_T("Выгрузить"));
	else pCmdUI->SetText(_T("Загрузить"));
}

void CApplicationDlg::OnHdnItemclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	switch (phdr->iItem) {
	case 0:
		db_.Sort(&decltype(db_)::Item::GetId);
		break;
	case 1:
		db_.Sort(&decltype(db_)::Item::GetFullName);
		break;
	case 2:
		db_.Sort(&decltype(db_)::Item::GetDriverClass);
		break;
	case 3:
		db_.Sort(&decltype(db_)::Item::GetExperience);
		break;
	case 4:
		db_.Sort(&decltype(db_)::Item::GetSalary);
		break;
	}
	m_bNeedUpdate = TRUE;
	*pResult = 0;
}

LRESULT CApplicationDlg::DoUpdateList(WPARAM, LPARAM)
{
	SetRedraw(FALSE);
	auto listCount = m_cList.GetItemCount();
	auto baseCount = static_cast<int>(db_.GetItems().size());
	while (listCount < baseCount) {	// выровнять количество строк в списке
		m_cList.InsertItem(0, L"");
		listCount++;
	}
	while (listCount > baseCount) { // выровнять количество строк в списке
		m_cList.DeleteItem(m_cList.GetItemCount() - 1);
		listCount--;
	}
	int i = 0;
	for (const auto& driver : db_.GetItems()) {
		m_cList.SetItemData(i, driver.GetId());
		m_cList.SetItemText(i, 0, std::to_wstring(driver.GetId()).c_str());
		m_cList.SetItemText(i, 1, driver.GetFullName().c_str());
		m_cList.SetItemText(i, 2, static_cast<const wchar_t*>(driver.GetDriverClass()));
		m_cList.SetItemText(i, 3, driver.GetExperience().c_str());
		CString s;
		s.Format(_T("%d руб."), driver.GetSalary());
		m_cList.SetItemText(i, 4, s);
		i++;
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
		auto& driver = db_.CreateItem();
		driver.SetLastName(dlg.m_sLastName.operator LPCTSTR());
		driver.SetFirstName(dlg.m_sFirstName.operator LPCTSTR());
		driver.SetSecondName(dlg.m_sSecondName.operator LPCTSTR());
		driver.SetDriverClass(DriverClass(dlg.m_sDriverClass));
		SYSTEMTIME st;
		dlg.m_dtEmploymentDate.GetAsSystemTime(st);
		driver.SetEmploymentDate(CTime(st).GetTime());
		driver.SetSalary(_ttoi(dlg.m_sSalary));

		m_bNeedUpdate = TRUE;
	}
}


void CApplicationDlg::OnUpdateAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bLoaded);
}

void CApplicationDlg::OnEdit()
{
	size_t id = m_cList.GetItemData(m_cList.GetSelectionMark());
	auto pDriver = db_.Search(id);
	if (pDriver != nullptr) {
		auto& ref = *pDriver;
		CEditor dlg;
		dlg.m_sLastName = ref.GetLastName().c_str();
		dlg.m_sFirstName = ref.GetFirstName().c_str();
		dlg.m_sSecondName = ref.GetSecondName().c_str();
		dlg.m_sDriverClass = static_cast<const wchar_t*>(ref.GetDriverClass());
		dlg.m_dtEmploymentDate = ref.GetEmploymentDate();
		dlg.m_sSalary.Format(_T("%d"), ref.GetSalary());

		if (dlg.DoModal() == IDOK)
		{
			ref.SetLastName(dlg.m_sLastName.operator LPCTSTR());
			ref.SetFirstName(dlg.m_sFirstName.operator LPCTSTR());
			ref.SetSecondName(dlg.m_sSecondName.operator LPCTSTR());
			ref.SetDriverClass(DriverClass(dlg.m_sDriverClass));
			SYSTEMTIME st;
			dlg.m_dtEmploymentDate.GetAsSystemTime(st);
			ref.SetEmploymentDate(CTime(st).GetTime());
			ref.SetSalary(_ttoi(dlg.m_sSalary));

			m_bNeedUpdate = TRUE;
		}
	}
}

void CApplicationDlg::OnUpdateEdit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bLoaded && m_cList.GetItemCount() > 0 && m_cList.GetSelectedCount() == 1U);
}

void CApplicationDlg::OnRemove()
{
	size_t id = m_cList.GetItemData(m_cList.GetSelectionMark());
	db_.Search(id);
	auto pDriver = db_.Search(id);
	if (pDriver) {
		MessageBox((pDriver->GetFullName() + L" удален!").c_str());
		db_.RemoveItem(id);
		m_bNeedUpdate = TRUE;
	}
}

void CApplicationDlg::OnUpdateRemove(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bLoaded && m_cList.GetItemCount() > 0 && m_cList.GetSelectedCount() == 1U);
}

void CApplicationDlg::OnSearch()
{
	auto pEdit = GetDlgItem(IDC_EDIT_SEARCH);
	if (pEdit) {
		CString text;
		pEdit->GetWindowTextW(text);
		size_t id = static_cast<size_t>(_ttoi(text));
		auto pDriver = db_.Search(id);
		if (pDriver) {
			MessageBox((pDriver->GetFullName() + L" найден!").c_str());
		}
		else {
			MessageBox(L"Сотрудника с таким ИД нет");
		}
	}
}

void CApplicationDlg::OnUpdateSearch(CCmdUI* pCmdUI)
{
	BOOL b = FALSE;
	auto pEdit = GetDlgItem(IDC_EDIT_SEARCH);
	if (pEdit && pEdit->GetWindowTextLength() > 0) b = TRUE;
	pCmdUI->Enable(b);
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

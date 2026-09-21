
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "0wnIRC.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

namespace
{
	constexpr UINT ID_SWITCHBAR_PANE = AFX_IDW_CONTROLBAR_FIRST + 60;
	constexpr UINT IDC_SWITCHBAR_TABS = 1;
	constexpr int kSwitchBarHeight = 28;

	int FindWindowIndex(const CArray<HWND, HWND>& windows, HWND hWindow)
	{
		for (int i = 0; i < windows.GetSize(); ++i)
		{
			if (windows[i] == hWindow)
			{
				return i;
			}
		}

		return -1;
	}
}

BEGIN_MESSAGE_MAP(CSwitchBar, CPane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_SWITCHBAR_TABS, &CSwitchBar::OnSelChange)
END_MESSAGE_MAP()

BOOL CSwitchBar::Create(CWnd* pParentWnd, UINT nID)
{
	return CPane::Create(_T("Switchbar"), pParentWnd, CRect(0, 0, 0, 0), FALSE, nID,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
}

void CSwitchBar::SyncFromWindows(HWND hMDIClient, HWND hActiveChild)
{
	if (!::IsWindow(m_wndTabs.GetSafeHwnd()))
	{
		return;
	}

	m_bSyncing = TRUE;

	if (!::IsWindow(hMDIClient))
	{
		m_windowOrder.RemoveAll();
		while (m_wndTabs.GetItemCount() > 0)
		{
			m_wndTabs.DeleteItem(m_wndTabs.GetItemCount() - 1);
		}
		m_bSyncing = FALSE;
		return;
	}

	int nActiveIndex = -1;
	CArray<HWND, HWND> currentWindows;

	for (HWND hChild = ::GetWindow(hMDIClient, GW_CHILD);
		hChild != nullptr;
		hChild = ::GetWindow(hChild, GW_HWNDNEXT))
	{
		if ((::GetWindowLongPtr(hChild, GWL_EXSTYLE) & WS_EX_MDICHILD) == 0)
		{
			continue;
		}

		currentWindows.Add(hChild);
		if (FindWindowIndex(m_windowOrder, hChild) < 0)
		{
			m_windowOrder.Add(hChild);
		}
	}

	for (int i = m_windowOrder.GetUpperBound(); i >= 0; --i)
	{
		if (FindWindowIndex(currentWindows, m_windowOrder[i]) < 0)
		{
			m_windowOrder.RemoveAt(i);
		}
	}

	for (int i = 0; i < m_windowOrder.GetSize(); ++i)
	{
		const HWND hChild = m_windowOrder[i];
		if (!::IsWindow(hChild))
		{
			continue;
		}

		CString strTitle;
		const int nTitleLength = ::GetWindowTextLength(hChild);
		LPTSTR pszTitle = strTitle.GetBuffer(nTitleLength + 1);
		::GetWindowText(hChild, pszTitle, nTitleLength + 1);
		strTitle.ReleaseBuffer();
		if (strTitle.IsEmpty())
		{
			strTitle = _T("(untitled)");
		}

		TCITEM item = {};
		item.mask = TCIF_TEXT | TCIF_PARAM;
		item.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTitle));
		item.lParam = reinterpret_cast<LPARAM>(hChild);

		if (i < m_wndTabs.GetItemCount())
		{
			m_wndTabs.SetItem(i, &item);
		}
		else
		{
			m_wndTabs.InsertItem(i, &item);
		}

		if (hChild == hActiveChild)
		{
			nActiveIndex = i;
		}
	}

	while (m_wndTabs.GetItemCount() > m_windowOrder.GetSize())
	{
		m_wndTabs.DeleteItem(m_wndTabs.GetItemCount() - 1);
	}

	if (nActiveIndex >= 0)
	{
		m_wndTabs.SetCurSel(nActiveIndex);
	}
	else if (m_wndTabs.GetItemCount() > 0)
	{
		const int nCurrentSelection = m_wndTabs.GetCurSel();
		if (nCurrentSelection < 0 || nCurrentSelection >= m_wndTabs.GetItemCount())
		{
			m_wndTabs.SetCurSel(0);
		}
	}

	m_bSyncing = FALSE;
}

CSize CSwitchBar::CalcFixedLayout(BOOL, BOOL)
{
	return CSize(32767, kSwitchBarHeight);
}

void CSwitchBar::LayoutTabs()
{
	if (::IsWindow(m_wndTabs.GetSafeHwnd()))
	{
		CRect rect;
		GetClientRect(&rect);
		rect.DeflateRect(2, 2);
		m_wndTabs.MoveWindow(rect);
	}
}

int CSwitchBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (!m_wndTabs.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TCS_BUTTONS | TCS_SINGLELINE,
		CRect(0, 0, 0, 0), this, IDC_SWITCHBAR_TABS))
	{
		TRACE0("Failed to create switchbar tabs\n");
		return -1;
	}

	if (CFont* pFont = GetFont())
	{
		m_wndTabs.SetFont(pFont);
	}
	else
	{
		m_wndTabs.SetFont(CFont::FromHandle(static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT))));
	}
	LayoutTabs();

	return 0;
}

void CSwitchBar::OnSize(UINT nType, int cx, int cy)
{
	CPane::OnSize(nType, cx, cy);
	LayoutTabs();
}

void CSwitchBar::OnSelChange(NMHDR*, LRESULT* pResult)
{
	if (m_bSyncing)
	{
		*pResult = 0;
		return;
	}

	const int nSelectedIndex = m_wndTabs.GetCurSel();
	if (nSelectedIndex >= 0)
	{
		TCITEM item = {};
		item.mask = TCIF_PARAM;
		if (m_wndTabs.GetItem(nSelectedIndex, &item))
		{
			if (CWnd* pMainWnd = AfxGetMainWnd())
			{
				pMainWnd->SendMessage(WM_SWITCHBAR_ACTIVATE_CHILD, 0, item.lParam);
			}
		}
	}

	*pResult = 0;
}

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_MESSAGE(WM_SWITCHBAR_SYNC, &CMainFrame::OnSwitchBarSync)
	ON_MESSAGE(WM_SWITCHBAR_ACTIVATE_CHILD, &CMainFrame::OnSwitchBarActivateChild)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);

	if (!m_wndSwitchBar.Create(this, ID_SWITCHBAR_PANE))
	{
		TRACE0("Failed to create switchbar\n");
		return -1;
	}
	m_wndSwitchBar.EnableDocking(CBRS_ALIGN_BOTTOM);

	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndSwitchBar, AFX_IDW_DOCKBAR_BOTTOM);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_NAME);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	SyncSwitchBar();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::SyncSwitchBar()
{
	if (::IsWindow(m_wndSwitchBar.GetSafeHwnd()))
	{
		m_wndSwitchBar.SyncFromWindows(m_hWndMDIClient, MDIGetActive() != nullptr ? MDIGetActive()->GetSafeHwnd() : nullptr);
	}
}

LRESULT CMainFrame::OnSwitchBarSync(WPARAM, LPARAM)
{
	SyncSwitchBar();
	return 0;
}

LRESULT CMainFrame::OnSwitchBarActivateChild(WPARAM, LPARAM lp)
{
	const HWND hChild = reinterpret_cast<HWND>(lp);
	if (!::IsWindow(hChild))
	{
		return 0;
	}

	if (::IsWindow(m_hWndMDIClient))
	{
		if (::IsIconic(hChild))
		{
			::SendMessage(m_hWndMDIClient, WM_MDIRESTORE, reinterpret_cast<WPARAM>(hChild), 0);
		}

		::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, reinterpret_cast<WPARAM>(hChild), 0);
	}

	return 0;
}

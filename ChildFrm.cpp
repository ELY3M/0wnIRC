
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "pch.h"
#include "framework.h"
#include "0wnIRC.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_CLOSE, &CChildFrame::OnFileClose)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

namespace
{
	const int kMainPaneWidth = 700;
	const int kMainPaneMinWidth = 200;
	const int kSidePaneWidth = 220;
	const int kSidePaneMinWidth = 100;
}

// CChildFrame construction/destruction

CChildFrame::CChildFrame() noexcept
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers
void CChildFrame::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.
	SendMessage(WM_CLOSE);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// create splitter with a main pane and a secondary pane
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to create splitter window\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CChildView), CSize(kMainPaneWidth, 0), pContext))
	{
		TRACE0("Failed to create main view pane\n");
		return FALSE;
	}

	CCreateContext nickListContext = {};
	CCreateContext* pNickListContext = nullptr;
	if (pContext != nullptr)
	{
		nickListContext = *pContext;
		nickListContext.m_pCurrentFrame = this;
		nickListContext.m_pNewViewClass = RUNTIME_CLASS(CNickListView);
		nickListContext.m_pLastView = DYNAMIC_DOWNCAST(CView, m_wndSplitter.GetPane(0, 0));
		pNickListContext = &nickListContext;
	}

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CNickListView), CSize(kSidePaneWidth, 0), pNickListContext))
	{
		TRACE0("Failed to create secondary nick-list pane\n");
		return FALSE;
	}

	m_wndSplitter.SetColumnInfo(0, kMainPaneWidth, kMainPaneMinWidth);
	m_wndSplitter.SetColumnInfo(1, kSidePaneWidth, kSidePaneMinWidth);
	m_wndSplitter.RecalcLayout();

	UNREFERENCED_PARAMETER(lpcs);
	return TRUE;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWndEx::OnSetFocus(pOldWnd);

	if (CWnd* pPane = GetActivePane())
	{
		pPane->SetFocus();
	}
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	CWnd* pPane = GetActivePane();

	// let the view have first crack at the command
	if (pPane != nullptr)
	{
		if (pPane->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	if (CWnd* pMainPane = GetMainPane())
	{
		if (pMainPane != pPane && pMainPane->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}
	
	// otherwise, do default handling
	return CMDIChildWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CWnd* CChildFrame::GetMainPane() const
{
	if (m_wndSplitter.GetSafeHwnd() == nullptr)
	{
		return nullptr;
	}

	CWnd* pMainPane = m_wndSplitter.GetPane(0, 0);
	if (pMainPane != nullptr && pMainPane->GetSafeHwnd() != nullptr)
		return pMainPane;

	return nullptr;
}

CWnd* CChildFrame::GetActivePane() const
{
	CWnd* pFocusedWnd = CWnd::GetFocus();
	if (pFocusedWnd != nullptr && m_wndSplitter.GetSafeHwnd() != nullptr)
	{
		for (int nCol = 0; nCol < 2; ++nCol)
		{
			CWnd* pPane = m_wndSplitter.GetPane(0, nCol);
			if (pPane != nullptr && (pPane == pFocusedWnd || pPane->IsChild(pFocusedWnd)))
			{
				return pPane;
			}
		}
	}

	if (m_wndSplitter.GetSafeHwnd() != nullptr)
	{
		if (CWnd* pActivePane = m_wndSplitter.GetActivePane())
		{
			return pActivePane;
		}
	}

	return GetMainPane();
}

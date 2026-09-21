
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
	ON_WM_CREATE()
END_MESSAGE_MAP()

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

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create splitter with a main pane and a secondary pane
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to create splitter window\n");
		return -1;
	}

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CChildView), CSize(700, 0), nullptr))
	{
		TRACE0("Failed to create main view pane\n");
		return -1;
	}

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CChildView), CSize(220, 0), nullptr))
	{
		TRACE0("Failed to create secondary view pane\n");
		return -1;
	}

	m_wndSplitter.SetColumnInfo(0, 700, 200);
	m_wndSplitter.SetColumnInfo(1, 220, 100);
	m_wndSplitter.RecalcLayout();

	return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWndEx::OnSetFocus(pOldWnd);

	if (CWnd* pMainPane = GetMainPane())
	{
		pMainPane->SetFocus();
	}
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if (CWnd* pMainPane = GetMainPane())
	{
		if (pMainPane->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
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

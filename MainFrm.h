
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

constexpr UINT WM_SWITCHBAR_SYNC = WM_APP + 1;
constexpr UINT WM_SWITCHBAR_ACTIVATE_CHILD = WM_APP + 2;
constexpr UINT WM_SWITCHBAR_REMOVE_CHILD = WM_APP + 3;

class CSwitchBar : public CPane
{
public:
	BOOL Create(CWnd* pParentWnd, UINT nID);
	void SyncFromWindows(HWND hActiveChild, HWND hHintChild, HWND hRemovedChild = nullptr);

protected:
	CTabCtrl m_wndTabs;
	CArray<HWND, HWND> m_windowOrder;
	BOOL m_bSyncing = FALSE;

	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	void LayoutTabs();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CSwitchBar        m_wndSwitchBar;
	CMFCToolBarImages m_UserImages;

	void SyncSwitchBar();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSwitchBarSync(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSwitchBarActivateChild(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSwitchBarRemoveChild(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

};


// 0wnIRC.h : main header file for the 0wnIRC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy0wnIRCApp:
// See 0wnIRC.cpp for the implementation of this class
//

class CMy0wnIRCApp : public CWinAppEx
{
public:
	CMy0wnIRCApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CSplitterWnd m_wndSplitter;

// Implementation
protected:
	HMENU  m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	BOOL  m_bHiColorIcons;
	
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnName();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern CMy0wnIRCApp theApp;

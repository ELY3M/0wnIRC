
// 0wnIRC.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "0wnIRC.h"
#include "MainFrm.h"

#include "ChildFrm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy0wnIRCApp
CString inifile;
CString nick1;
CString nick2;
CString nick3;
CString ident;
CString realname;



CString server;
CString port;
CString serverpass; 



BEGIN_MESSAGE_MAP(CMy0wnIRCApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMy0wnIRCApp::OnAppAbout)
	ON_COMMAND(ID_NAME, &CMy0wnIRCApp::OnName)
	ON_COMMAND(ID_FILE_NEW, &CMy0wnIRCApp::OnFileNew)
END_MESSAGE_MAP()


// CMy0wnIRCApp construction

CMy0wnIRCApp::CMy0wnIRCApp() noexcept
{
	m_bHiColorIcons = FALSE;


	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
/*
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
*/

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	//SetAppID(_T("My0wnIRC.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMy0wnIRCApp object

CMy0wnIRCApp theApp;


// CMy0wnIRCApp initialization

BOOL CMy0wnIRCApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization	
	SetRegistryKey(_T("0wnirc"));

	
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);
	inifile.Format(_T("%s\\config.ini"), dir);
	

	WCHAR onick1[255];
	GetPrivateProfileString(_T("Settings"), _T("nick1"), _T("nick1"), onick1, 255, inifile);
	nick1 = onick1;  
	nick1.ReleaseBuffer();

	WCHAR onick2[255];
	GetPrivateProfileString(_T("Settings"), _T("nick2"), _T("nick2"), onick2, 255, inifile);
	nick2 = onick2;
	nick2.ReleaseBuffer();

	WCHAR onick3[255];
	GetPrivateProfileString(_T("Settings"), _T("nick3"), _T("nick3"), onick3, 255, inifile);
	nick3 = onick3;
	nick3.ReleaseBuffer();

	WCHAR oident[255];
	GetPrivateProfileString(_T("Settings"), _T("ident"), _T("own"), oident, 255, inifile);
	ident = oident;
	ident.ReleaseBuffer();  

	WCHAR orealname[255];
	GetPrivateProfileString(_T("Settings"), _T("realname"), _T("own"), orealname, 255, inifile);
	realname = orealname;
	realname.ReleaseBuffer();

	if (nick1 == "") {
		nick1 = "nick";
		WritePrivateProfileString(_T("Settings"), _T("nick1"), nick1, inifile);
	}

	if (nick2 == "") {
		nick2 = "nick2";
		WritePrivateProfileString(_T("Settings"), _T("nick2"), nick2, inifile);
	}

	if (nick3 == "") {
		nick3 = "nick3";
		WritePrivateProfileString(_T("Settings"), _T("nick3"), nick3, inifile);
	}

	if (ident == "") {
		ident = "own";
		WritePrivateProfileString(_T("Settings"), _T("ident"), ident, inifile);
	}

	if (realname == "") {
		realname = "own";
		WritePrivateProfileString(_T("Settings"), _T("realname"), realname, inifile);
	}


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;




	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_My0wnIRCTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_My0wnIRCTYPE));




	// The main window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();


	return TRUE;
}

int CMy0wnIRCApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	if (m_hMDIMenu != nullptr)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != nullptr)
		FreeResource(m_hMDIAccel);

	return CWinAppEx::ExitInstance();
}

// CMy0wnIRCApp message handlers

void CMy0wnIRCApp::OnFileNew()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_My0wnIRCTYPE, m_hMDIMenu, m_hMDIAccel);
}

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CMy0wnIRCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


//setting for nicks  
class CNameDlg : public CDialogEx
{
public:
	CNameDlg() noexcept;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSavename();
	CEdit m_nick1;
	CEdit m_nick2;
	CEdit m_nick3;
	CEdit m_ident;
	CEdit m_realname;
};

CNameDlg::CNameDlg() noexcept : CDialogEx(IDD_NAME)
{


	//CString test;
	//test.Format(_T("Nick1: %s\nNick2: %s\nNick3: %s\nIdent: %s\nRealname: %s"), nick1, nick2, nick3, ident, realname);
	//AfxMessageBox(test);

	

}


void CNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, NICK1, m_nick1);
	DDX_Control(pDX, NICK2, m_nick2);
	DDX_Control(pDX, NICK3, m_nick3);
	DDX_Control(pDX, IDENT, m_ident);
	DDX_Control(pDX, REALNAME, m_realname);



	CEdit* pNick1 = (CEdit*)GetDlgItem(NICK1);
	if (pNick1)
	{
		pNick1->SetWindowText(nick1);
	}

	CEdit* pNick2 = (CEdit*)GetDlgItem(NICK2);
	if (pNick2)
	{
		pNick2->SetWindowText(nick2);
	}

	CEdit* pNick3 = (CEdit*)GetDlgItem(NICK3);
	if (pNick3)
	{
		pNick3->SetWindowText(nick3);
	}

	CEdit* pIdent = (CEdit*)GetDlgItem(IDENT);
	if (pIdent)
	{
		pIdent->SetWindowText(ident);
	}

	CEdit* pRealname = (CEdit*)GetDlgItem(REALNAME);
	if (pRealname)
	{
		pRealname->SetWindowText(realname);
	}



	/*
		m_nick1.SetWindowText(nick1);
		m_nick2.SetWindowText(nick2);
		m_nick3.SetWindowText(nick3);
		m_ident.SetWindowText(ident);
		m_realname.SetWindowText(realname);
	*/






}

BEGIN_MESSAGE_MAP(CNameDlg, CDialogEx)
	ON_BN_CLICKED(SAVENAME, &CNameDlg::OnBnClickedSavename)
END_MESSAGE_MAP()

// App command to run the dialog
void CMy0wnIRCApp::OnName()
{
	CNameDlg nameDlg;
	nameDlg.DoModal();


}



// CMy0wnIRCApp customization load/save methods
void CMy0wnIRCApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMy0wnIRCApp::LoadCustomState()
{
}

void CMy0wnIRCApp::SaveCustomState()
{
}



void CNameDlg::OnBnClickedSavename()
{

	GetDlgItemText(NICK1, nick1);
	GetDlgItemText(NICK2, nick2);
	GetDlgItemText(NICK3, nick3);
	GetDlgItemText(IDENT, ident);
	GetDlgItemText(REALNAME, realname);

	WritePrivateProfileString(_T("Settings"), _T("nick1"), nick1, inifile);
	WritePrivateProfileString(_T("Settings"), _T("nick2"), nick2, inifile);
	WritePrivateProfileString(_T("Settings"), _T("nick3"), nick3, inifile);
	WritePrivateProfileString(_T("Settings"), _T("ident"), ident, inifile);
	WritePrivateProfileString(_T("Settings"), _T("realname"), realname, inifile);



	//TEST
	CString test;
	test.Format(_T("Nick1: %s\nNick2: %s\nNick3: %s\nIdent: %s\nRealname: %s"), nick1, nick2, nick3, ident, realname);
	AfxMessageBox(test);
	


	CDialogEx:IDOK;



}

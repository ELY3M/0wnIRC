// Savename.cpp : implementation file
//

#include "pch.h"
#include "0wnIRC.h"
#include "afxdialogex.h"
#include "Savename.h"


/*

// In your CWinApp derived class (e.g., MyWinApp)
void MyWinApp::InitInstance() {
    // ... other initialization code ...

    // Set the INI file path
    free((void*)m_pszProfileName);
    m_pszProfileName = ::_tcsdup(_T("config.ini"));

    // ... rest of InitInstance() ...
}

// To read a string value
CString strValue;
AfxGetApp()->GetProfileString(_T("Settings"), _T("WindowSize"), _T("800x600"), strValue);

// To write a string value
AfxGetApp()->WriteProfileString(_T("Settings"), _T("WindowSize"), _T("1024x768"));

*/
// Savename dialog



IMPLEMENT_DYNAMIC(Savename, CDialogEx)


Savename::Savename(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAME, pParent)
{


	///AfxMessageBox(_T("FUCK YOU!!!!"));

}

Savename::~Savename()
{


}

void Savename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Savename, CDialogEx)
    ON_BN_CLICKED(SAVENAME, &Savename::OnBnClickedSavename)
END_MESSAGE_MAP()


// Savename message handlers

void Savename::OnBnClickedSavename()
{



    // TODO: Add your control notification handler code here
    CString nick1;
    CString nick2;
    CString nick3;
    CString ident;
    CString realname;

	
	CEdit* pNick1 = (CEdit*)GetDlgItem(NICK1);
	pNick1->GetWindowText(nick1);
	
	CEdit* pNick2 = (CEdit*)GetDlgItem(NICK2);
	pNick2->GetWindowText(nick2);
	
	CEdit* pNick3 = (CEdit*)GetDlgItem(NICK3);
	pNick3->GetWindowText(nick3);
	
	CEdit* pIdent = (CEdit*)GetDlgItem(IDENT);
	pIdent->GetWindowText(ident);
	
	CEdit* pRealname = (CEdit*)GetDlgItem(REALNAME);
	pRealname->GetWindowText(realname);
		

	GetDlgItemText(NICK1, nick1);
	GetDlgItemText(NICK2, nick2);
	GetDlgItemText(NICK3, nick3);
	GetDlgItemText(IDENT, ident);
	GetDlgItemText(REALNAME, realname);

	CString test; 
	test.Format(_T("Nick1: %s\nNick2: %s\nNick3: %s\nIdent: %s\nRealname: %s"), nick1, nick2, nick3, ident, realname);
	AfxMessageBox(test);  


	AfxGetApp()->WriteProfileString(_T("Settings"), _T("nick1"), nick1);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("nick2"), nick2);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("nick3"), nick3);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("ident"), ident);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("realname"), realname);
	
	CDialogEx::OnOK();
	//Savename::Savename();


}

void Savename::OnEnChangeNick1()
{
	CString nick1;

	CEdit* pNick1 = (CEdit*)GetDlgItem(NICK1);	
	pNick1->GetWindowText(nick1);

	GetDlgItemText(NICK1, nick1);

	AfxGetApp()->WriteProfileString(_T("Settings"), _T("nick1"), nick1);



	AfxMessageBox(nick1);
	MessageBox(nick1);  



}

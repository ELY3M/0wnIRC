 #pragma once
#include "afxdialogex.h"


// Savename dialog
#include "Savename.h"
class Savename : public CDialogEx
{
	DECLARE_DYNAMIC(Savename)

public:
	Savename(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Savename();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSavename();
	afx_msg void OnEnChangeNick1();
};


// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView view

class CChildView : public CView
{
	DECLARE_DYNCREATE(CChildView)
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	virtual void OnDraw(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

class CNickListView : public CListView
{
	DECLARE_DYNCREATE(CNickListView)

public:
	CNickListView();
	virtual ~CNickListView();

protected:
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
};

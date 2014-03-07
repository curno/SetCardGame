#pragma once
#include "MainView.h"

class MainFrame : public CFrameWnd
{
	
public:
	MainFrame();
protected: 
	DECLARE_DYNCREATE(MainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

public:
	virtual ~MainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	MainView    *m_wndView;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

    static void SubmissionFailed();
    static void AskForUserName();
    static void SubmissionSuccess(const ::std::string &data);
};


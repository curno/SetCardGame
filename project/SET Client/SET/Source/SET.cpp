
#include "Include/stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Include/Set.h"
#include "Include/UI/MainFrame.h"
#include "Include/Web/HttpServer.h"
#include "Include/UI/PlayerNameDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(SetApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &SetApp::OnAppAbout)
END_MESSAGE_MAP()


SetApp::SetApp()
{
	SetAppID(_T("MFCApplication6.AppID.NoVersion"));
}

SetApp theApp;

BOOL SetApp::InitInstance()
{
	CWinApp::InitInstance();

	EnableTaskbarInteraction(FALSE);

	MainFrame* pFrame = new MainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();


    MainFrame::AskForUserName();

    if (PlayerName.GetLength() > 0)
        pFrame->SetWindowText(CString("Welcome ") + PlayerName + ", Let's SET!");
    else
        pFrame->SetWindowText(TEXT("Let's SET!"));
	return TRUE;
}

int SetApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void SetApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}




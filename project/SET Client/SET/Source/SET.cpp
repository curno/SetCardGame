
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
END_MESSAGE_MAP()


SetApp::SetApp()
{
	SetAppID(_T("SETGAME.NoVersion"));
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








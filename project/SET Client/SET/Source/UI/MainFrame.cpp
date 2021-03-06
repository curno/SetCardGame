#include "Include/stdafx.h"
#include "Include/Set.h"

#include "Include/UI/MainFrame.h"
#include "Include/Utils/GlobalConfiguration.h"
#include "Include/UI/PlayerNameDialog.h"

// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

// MainFrame

MainFrame::MainFrame()
{
    m_wndView = new MainView;
    // TODO: 
}

MainFrame::~MainFrame()
{
}

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		return -1;
	}
	return 0;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// MainFrame 

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

void MainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_wndView->SetFocus();
}

BOOL MainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// pass the message to view.
	if (m_wndView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// go default
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void MainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    CFrameWnd::OnGetMinMaxInfo(lpMMI);
    lpMMI->ptMinTrackSize = CPoint(GlobalConfiguration::Instance().WindowMinimumWidth,
        GlobalConfiguration::Instance().WindowMinimumHeight);

}

void MainFrame::SubmissionFailed()
{
    ::std::wstring content = TEXT("Can not connect to Server: \n");
    content += ::std::wstring(TEXT("    ")) +  GlobalConfiguration::Instance().HostName;
    
    ::MessageBox(AfxGetMainWnd()->m_hWnd, content.c_str(), TEXT("Oh.."), NULL);
}

void MainFrame::SubmissionSuccess(const ::std::string &data)
{
    ScoreDialog::AnalysisAndShowScoreDialog(data);
}

void MainFrame::AskForUserName()
{
    theApp.PlayerName = PlayerNameDialog::GetPlayerName();
    if (theApp.PlayerName.IsEmpty())
        ::MessageBox(AfxGetMainWnd()->m_hWnd, TEXT("You didn't enter any name. \nYou will play the game in offline mode. \nHave fun!"), TEXT("Let's Play!"), NULL);
    else
    {
        CString text = TEXT("Welcome To Set Game, ");
        text += theApp.PlayerName + TEXT("\nHave fun!");
        ::MessageBox(AfxGetMainWnd()->m_hWnd, text, TEXT("Let's Play!"), NULL);
    }

}

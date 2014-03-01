
#include "Include/stdafx.h"
#include "SET.h"
#include "Include/UI/PlayerNameDialog.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(PlayerNameDialog, CDialogEx)

PlayerNameDialog::PlayerNameDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(PlayerNameDialog::IDD, pParent)
    , PlayerName(_T(""))
{

}

PlayerNameDialog::~PlayerNameDialog()
{
}

void PlayerNameDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, PlayerName);
	DDV_MaxChars(pDX, PlayerName, 20);
}


BEGIN_MESSAGE_MAP(PlayerNameDialog, CDialogEx)
END_MESSAGE_MAP()



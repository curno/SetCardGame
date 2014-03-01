#pragma once

#include "Res/resource.h"

class PlayerNameDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PlayerNameDialog)

public:
	PlayerNameDialog(CWnd* pParent = NULL); 
	virtual ~PlayerNameDialog();

	enum { IDD = IDD_PLAYERNAMEDIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
    CString PlayerName;

    static CString GetPlayName()
    {
        PlayerNameDialog dialog;
        if (dialog.DoModal() == IDOK)
            return dialog.PlayerName;
        else
            return CString("");
    }
};

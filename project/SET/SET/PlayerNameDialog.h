#pragma once

#include "Res/resource.h"
// PlayerNameDialog 对话框

class PlayerNameDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PlayerNameDialog)

public:
	PlayerNameDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PlayerNameDialog();

// 对话框数据
	enum { IDD = IDD_PLAYERNAMEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

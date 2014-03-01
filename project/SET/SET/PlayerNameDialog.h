#pragma once

#include "Res/resource.h"
// PlayerNameDialog �Ի���

class PlayerNameDialog : public CDialogEx
{
	DECLARE_DYNAMIC(PlayerNameDialog)

public:
	PlayerNameDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PlayerNameDialog();

// �Ի�������
	enum { IDD = IDD_PLAYERNAMEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

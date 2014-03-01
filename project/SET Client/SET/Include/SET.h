#pragma once


#include "Res/resource.h"    



class SetApp : public CWinApp
{
public:
	SetApp();


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
    CString PlayerName;
};

extern SetApp theApp;


// MFCApplication6.h : MFCApplication6 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "Res/resource.h"       // ������


// SetApp:
// �йش����ʵ�֣������ MFCApplication6.cpp
//

class SetApp : public CWinApp
{
public:
	SetApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern SetApp theApp;

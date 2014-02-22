
// SET.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "Resource/resource.h"		// main symbols


// SETApp:
// See SET.cpp for the implementation of this class
//

class SETApp : public CWinApp
{
public:
	SETApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern SETApp theApp;
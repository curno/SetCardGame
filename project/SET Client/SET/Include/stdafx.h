
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include "winuser.h"



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include "Mmsystem.h"
#pragma comment(lib, "Winmm.lib")
#include "winhttp.h"
#pragma comment(lib, "Winhttp.lib")
// stl header
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <random>
#include <sstream>
#include <cassert>

// opengl header
//#include <gl/GL.h>
//#include <gl/GLU.h>

#include "glew.h"
#include "wglew.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")

#include "Keyword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#undef min
#undef max










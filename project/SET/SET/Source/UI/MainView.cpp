
// MainView.cpp : MainView 类的实现
//

#include "Include/stdafx.h"
#include "Include/Set.h"
#include "Include/UI/MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// MainView

MainView::MainView()
{
    Game_ = ref<Game>(new Game);
    GameScene_ = ref<VisualGameScene>(new VisualGameScene(Game_));
    Game_->DealMore();
    Game_->DealMore();
    Game_->DealMore();
    Game_->DealMore();
    Game_->DealMore();
    Game_->DealMore();
    Game_->DealMore();
}

MainView::~MainView()
{
}


BEGIN_MESSAGE_MAP(MainView, CWnd)
	ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// MainView 消息处理程序

BOOL MainView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void MainView::OnPaint() 
{
    CClientDC dc(this);
    wglMakeCurrent(dc.m_hDC, GLRC_);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (GameScene_ != nullptr)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, GameScene_->Size.cx, 0, GameScene_->Size.cy, -100, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // TODO
        GameScene_->Render();
    }

    

    SwapBuffers(dc.m_hDC);
    wglMakeCurrent(nullptr, nullptr);
}


int MainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CClientDC dc(this);
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    int pixel_format = ChoosePixelFormat(dc.m_hDC, &pfd);
    SetPixelFormat(dc.m_hDC, pixel_format, &pfd);
    GLRC_ = wglCreateContext(dc.m_hDC);
    return 0;
}

void MainView::OnDestroy()
{
    CWnd::OnDestroy();

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(GLRC_); // release opengl rendering content.
    GLRC_ = nullptr;
}


void MainView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    CClientDC dc(this);
    wglMakeCurrent(dc.m_hDC, GLRC_);
    glViewport(0, 0, cx, cy);

    if (GameScene_ != nullptr)
        GameScene_->Size = CSize(cx, cy);
}


BOOL MainView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // suppress erase background.
}



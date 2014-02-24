
// MainView.cpp : MainView 类的实现
//

#include "Include/stdafx.h"
#include "Include/Set.h"
#include "Include/UI/MainView.h"
#include "Include/Rendering/VisualObject.h"

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
    ON_WM_MOUSEMOVE()
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

    InitOpenGL();
    RenderWithOpenGL();

    //SwapBuffers(dc.m_hDC);
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
        PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    int pixel_format = ChoosePixelFormat(dc.m_hDC, &pfd);
    SetPixelFormat(dc.m_hDC, pixel_format, &pfd);
    GLRC_ = wglCreateContext(dc.m_hDC);
    wglMakeCurrent(dc.m_hDC, GLRC_);
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

    glViewport(0, 0, cx, cy);

    if (GameScene_ != nullptr)
        GameScene_->Size = CSize(cx, cy);
}


BOOL MainView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // suppress erase background.
}

void MainView::OnMouseMove(UINT nFlags, CPoint point)
{
    CWnd::OnMouseMove(nFlags, point);

    static const unsigned int NameBufferSize = 512;
    static VisualObject::GLNameType NameBuffer[NameBufferSize];

    glSelectBuffer(NameBufferSize, NameBuffer);
    glRenderMode(GL_SELECT);
    
    glInitNames();
    glPushName(0);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(point.x, viewport[3] - point.y, 5, 5, viewport);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderWithOpenGL();
    glFlush();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    auto s = glGetString(GL_VENDOR);
    int hits = glRenderMode(GL_RENDER);
    VisualObject::GLNameType *ptr = NameBuffer;
    for (int i = 0; i < hits; ++i)
    {
        int name_count = *ptr;
        ptr += 3;
        for (int j = 0; j < name_count; ++j)
        {
            auto *object = GameScene_->GetObjectByGLName(*ptr);
            if (object != nullptr)
                object->OnMouseMove();
            ++ptr;
        }
    }

    //InvalidateRect(NULL);
}

void MainView::RenderWithOpenGL()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 0.2f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    if (GameScene_ != nullptr)
    {
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, GameScene_->Size.cx, 0, GameScene_->Size.cy, -2000, 2000);
        glMatrixMode(GL_MODELVIEW);

        GameScene_->Render();
    }
}

void MainView::InitOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



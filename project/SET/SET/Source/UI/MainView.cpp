#include "Include/stdafx.h"
#include "Include/Set.h"
#include "Include/UI/MainView.h"
#include "Include/Rendering/VisualObject.h"
#include "Include/Rendering/Geometry.h"
#include "Include/Animation/AnimationManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define LARGE_SCALE 2000

// MainView
MainView::MainView() : CurrentObject_(nullptr)
{
    Game_ = ref<Game>(new Game);
    GameScene_ = ref<VisualGameScene>(new VisualGameScene(Game_));
}

MainView::~MainView()
{
}

IMPLEMENT_DYNCREATE(MainView, CView)

BEGIN_MESSAGE_MAP(MainView, CView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL MainView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;
	return TRUE;
}

int MainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CClientDC dc(this);

    // pixel format
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER; // use double buffer.
    pfd.iPixelType = PFD_TYPE_RGBA; // rgba format
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;

    int pixel_format = ChoosePixelFormat(dc.m_hDC, &pfd); // create pixel format
    SetPixelFormat(dc.m_hDC, pixel_format, &pfd); 

    GLRC_ = wglCreateContext(dc.m_hDC); // create gl rendering content.

    wglMakeCurrent(dc.m_hDC, GLRC_); // make current

    SetTimer(1, 20, NULL);
    
    Invalidate(NULL);
    return 0;
}

void MainView::OnDestroy()
{
    CWnd::OnDestroy();

    wglMakeCurrent(nullptr, nullptr); // make current null
    wglDeleteContext(GLRC_); // release opengl rendering content.
    GLRC_ = nullptr;
}

void MainView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    glViewport(0, 0, cx, cy); // set view port to the whole view.

    if (GameScene_ != nullptr)
        GameScene_->Size = Dimension(cx, cy, LARGE_SCALE);
    Invalidate(NULL);
}


BOOL MainView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // suppress erase background.
}

void MainView::OnMouseMove(UINT nFlags, CPoint point)
{
    static const int MouseRadius = 3;
    CWnd::OnMouseMove(nFlags, point);

    // pick object.
    VisualObject *object = PickObject(point, MouseRadius, MouseRadius);

    if (object != CurrentObject_)
    {
        // send message to leaved object
        if (CurrentObject_ != nullptr)
            CurrentObject_->OnMouseLeave();
        // send message to entered object
        if (object != nullptr)
            object->OnMouseEnter();
    }
    
    // send message to hover objects.
    object->OnMouseMove(); // move mouse.

    CurrentObject_ = object; // update current hovered objects.

    Invalidate(NULL);
}

void MainView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CView::OnLButtonDown(nFlags, point);

    if (CurrentObject_ == nullptr)
        return;
    // if game scene is animating, block visual card click.
    if (GameScene_->IsAnimating())
    {
        if (IsType(CurrentObject_, VisualCard))
            return;
    }
       
    CurrentObject_->OnMouseButtonDown(); // send message.

}


void MainView::RenderWithOpenGL()
{
    if (GameScene_ != nullptr)
    {
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, GameScene_->Size.Width, 0, GameScene_->Size.Height, -GameScene_->Size.Depth, GameScene_->Size.Depth);
        glMatrixMode(GL_MODELVIEW);

        // render
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

    // enable light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // light parameter
    GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.6f, 0.6f, 0.6f, 0.6f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 0.6f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

}

VisualObject *MainView::PickObject(CPoint &point, int w, int h)
{
    static const unsigned int NameBufferSize = 512;
    static VisualObject::GLNameType NameBuffer[NameBufferSize];

    glSelectBuffer(NameBufferSize, NameBuffer);
    glRenderMode(GL_SELECT);

    // init view 
    glInitNames();
    glPushName(0);
    
    // get view port
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // set pick projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // save
    glLoadIdentity();
    gluPickMatrix(point.x, viewport[3] - point.y, w, h, viewport);

    RenderWithOpenGL(); // render
    glFlush();

    glMatrixMode(GL_PROJECTION); // restore
    glPopMatrix();

    // get selected object.
    int hits = glRenderMode(GL_RENDER);

    // process hits.
    VisualObject::GLNameType *ptr = NameBuffer;
    VisualObject *retval = nullptr;

    // for every hits, found least deep hit.
    VisualObject::GLNameType min_depth = 0xffffffff;
    for (int i = 0; i < hits; ++i)
    {
        int name_count = *ptr;
        if (*(ptr + 1) < min_depth)
        {
            min_depth = *(ptr + 2);
            // get the object
            auto *object = GameScene_->GetObjectByGLName(*(ptr + 2 + name_count));
            if (object != nullptr)
                retval = object;
        }
        ptr += 3 + name_count;
        
    }
    return retval;
}


void MainView::OnTimer(UINT_PTR nIDEvent)
{
    CWnd::OnTimer(nIDEvent);
    AnimationManager::Instance().PerformAllAnimation(); // perform animation
    Invalidate(NULL); // always refresh. 
}

void MainView::OnDraw(CDC* pDC)
{
    CClientDC dc(this);

    InitOpenGL();
    RenderWithOpenGL();

    SwapBuffers(dc.m_hDC); // swap buffer and begin another rendering process.
}




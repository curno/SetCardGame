#include "Include/stdafx.h"
#include "Include/Set.h"
#include "Include/UI/MainView.h"
#include "Include/Rendering/VisualObject.h"
#include "Include/Rendering/Geometry.h"
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

IMPLEMENT_DYNCREATE(MainView, CView)

BEGIN_MESSAGE_MAP(MainView, CView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
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
        GameScene_->Size = CSize(cx, cy);
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

    // pick objects.
    ::std::set<VisualObject *> objects = PickObject(point, MouseRadius, MouseRadius);

    // send message to entered objects.
    ::std::set<VisualObject *> entered_objects;
    ::std::set_difference(objects.begin(), objects.end(),
        CurrentHoveredObjects_.begin(), CurrentHoveredObjects_.end(),
        inserter(entered_objects, entered_objects.end()));
    for each (VisualObject *object in entered_objects)
        object->OnMouseEnter();

    // send message to leaved objects
    ::std::set<VisualObject *> leaved_objects;
    ::std::set_difference(CurrentHoveredObjects_.begin(), CurrentHoveredObjects_.end(),
        objects.begin(), objects.end(),
        inserter(leaved_objects, leaved_objects.end()));
    for each (VisualObject *object in leaved_objects)
        object->OnMouseLeave();

    // send message to hover objects.
    for each (VisualObject *object in objects)
        object->OnMouseMove(); // move mouse.

    CurrentHoveredObjects_ = objects; // update current hovered objects.

    if (CurrentHoveredObjects_.size() > 0)
    {

            for each (VisualObject *object in CurrentHoveredObjects_)
            {
                if (dynamic_cast<VisualCard *>(object))
                {
                    if (animations.find(object) == animations.end())
                    {
                        auto animation1 = MakeGenericAnimation(3000, RotateVisualObject(dynamic_cast<VisualObject *>(object)->shared_from_this(), 0, 1, 0, PI * 2));
                        animation1->Behavior = AnimationBehavior::WiredBehavior();
                        //auto animation = ::std::make_shared<LoopAnimation>(animation1);
                        animations.insert(make_pair(object, animation1));
                        animation1->Start();
                            
                    }
                }
            }
    }

    Invalidate(NULL);
}

void MainView::RenderWithOpenGL()
{
    if (GameScene_ != nullptr)
    {
        glMatrixMode(GL_PROJECTION);
        glOrtho(0, GameScene_->Size.cx, 0, GameScene_->Size.cy, -2000, 2000);
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
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 0.2f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

}

::std::set<VisualObject *> MainView::PickObject(CPoint &point, int w, int h)
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

    // get selected objects.
    int hits = glRenderMode(GL_RENDER);
    VisualObject::GLNameType *ptr = NameBuffer;
    ::std::set<VisualObject *> retval;
    for (int i = 0; i < hits; ++i)
    {
        int name_count = *ptr;
        ptr += 3;
        for (int j = 0; j < name_count; ++j)
        {
            auto *object = GameScene_->GetObjectByGLName(*ptr);
            if (object != nullptr)
                retval.insert(object);
            ++ptr;
        }
    }
    return retval;
}


void MainView::OnTimer(UINT_PTR nIDEvent)
{
    CWnd::OnTimer(nIDEvent);
    for each (auto animation in animations)
        animation.second->OnTimer();
    Invalidate(NULL);
}

void MainView::OnDraw(CDC* pDC)
{
    CClientDC dc(this);

    InitOpenGL();
    RenderWithOpenGL();

    SwapBuffers(dc.m_hDC); // swap buffer and begin another rendering process.
}




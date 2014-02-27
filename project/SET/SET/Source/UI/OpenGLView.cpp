#include "Include/stdafx.h"

#include "Include/UI/OpenGLView.h"
#include "Include/Animation/AnimationManager.h"


// MainView
OpenGLView::OpenGLView()
{
}

OpenGLView::~OpenGLView()
{
}

IMPLEMENT_DYNCREATE(OpenGLView, CView)

BEGIN_MESSAGE_MAP(OpenGLView, CView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

int OpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    CClientDC thisdc(this);
    InitGLRC(thisdc);
    SetTimer(1, 20, NULL);
    Invalidate(NULL);
    return 0;
}

void OpenGLView::OnDestroy()
{
    __super::OnDestroy();

    wglMakeCurrent(nullptr, nullptr); // make current null
    wglDeleteContext(GLRC_); // release opengl rendering content.
    GLRC_ = nullptr;
}

void OpenGLView::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);
    MakeCurrent();
    glViewport(0, 0, cx, cy); // set view port to the whole view.
    CancelCurrent();
    Invalidate(NULL);
}


BOOL OpenGLView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // suppress erase background.
}

void OpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
    static const int MouseRadius = 3;
    __super::OnMouseMove(nFlags, point);

    // pick object.
    VisualObject *object = PickObject(point, MouseRadius, MouseRadius);

    if (object != CurrentObject_.get())
    {
        // send message to leaved object
        if (CurrentObject_ != nullptr)
            CurrentObject_->OnMouseLeave();
        // send message to entered object
        if (object != nullptr)
            object->OnMouseEnter();
    }


    // send message to hover objects.
    if (object != nullptr)
        object->OnMouseMove(); // move mouse.

    if (object != nullptr)
        CurrentObject_ = object->shared_from_this(); // update current hovered objects.

    Invalidate(NULL);
}

void OpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CView::OnLButtonDown(nFlags, point);

    if (CurrentObject_ == nullptr)
        return;

    CurrentObject_->OnMouseButtonDown(); // send message.

}


void OpenGLView::RenderWithOpenGL() { }

void OpenGLView::InitOpenGL() { }

VisualObject *OpenGLView::PickObject(CPoint &point, int w, int h)
{
    static const unsigned int NameBufferSize = 512;
    static VisualObject::GLNameType NameBuffer[NameBufferSize];
    MakeCurrent();
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
            auto *object = GetObjectByGLName(*(ptr + 2 + name_count));
            if (object != nullptr)
                retval = object;
        }
        ptr += 3 + name_count;

    }
    CancelCurrent();
    return retval;
}

void OpenGLView::OnDraw(CDC* pDC)
{
    CClientDC dc(this);
    MakeCurrent();

    InitOpenGL();
    RenderWithOpenGL();

    CancelCurrent();
    SwapBuffers(dc.m_hDC); // swap buffer and begin another rendering process.
}


void OpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL OpenGLView::InitGLRC(HDC hdc)
{
    HWND dummy_window = CreateWindow(TEXT("STATIC"), NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
    HDC dummy_dc = ::GetDC(dummy_window);

    // dummy pixel format
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
    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd); // create pixel format
    SetPixelFormat(dummy_dc, pixel_format, &pfd);
    auto dummy_glrc = wglCreateContext(dummy_dc); // create gl rendering content.
    wglMakeCurrent(dummy_dc, dummy_glrc); // make current

    bool success = true;
    // get function address.
    PROC wglChoosePixelFormatARB_ = wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummy_glrc);

    float fAttributes[] = { 0, 0 };
    int pixelFormat;
    UINT numFormats;
    int attributes[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, 1, //Number of buffers (must be 1 at time of writing)
        WGL_SAMPLES_ARB, 4,        //Number of samples
        0
    };
    success &= ((PFNWGLCHOOSEPIXELFORMATARBPROC)wglChoosePixelFormatARB_)(hdc, attributes, fAttributes,
        1, &pixelFormat, &numFormats) == TRUE;

    success &= SetPixelFormat(hdc, pixelFormat, NULL) == TRUE;
    GLRC_ = wglCreateContext(hdc); // create gl rendering content.
    success &= GLRC_ != nullptr;
    // if failed, same as dummy dc.
    if (!success)
    {
        int pixel_format = ChoosePixelFormat(hdc, &pfd); // create pixel format
        SetPixelFormat(hdc, pixel_format, &pfd);
        GLRC_ = wglCreateContext(hdc); // create gl rendering content.
    }
    else
        glEnable(GL_MULTISAMPLE);

    wglMakeCurrent(hdc, GLRC_); // make current

    return success;
}

VisualObject * OpenGLView::GetObjectByGLName(VisualObject::GLNameType name)
{
    return nullptr;
}

void OpenGLView::MakeCurrent()
{
    wglMakeCurrent(GetDC()->m_hDC, GLRC_);
}

void OpenGLView::CancelCurrent()
{
    wglMakeCurrent(NULL, NULL);
}

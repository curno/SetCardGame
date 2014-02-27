#include "Include/stdafx.h"

#include "Include/UI/PanelView.h"

IMPLEMENT_DYNCREATE(PanelView, OpenGLView)

BOOL PanelView::PreCreateWindow(CREATESTRUCT& cs)
{
    return __super::PreCreateWindow(cs);
}

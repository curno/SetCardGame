#pragma once

#include "stdafx.h"
#include "OpenGLView.h"

// MainView ����

class PanelView : public OpenGLView
{
    DECLARE_DYNCREATE(PanelView)
private:
    // ����
public:
    PanelView() { }


    // ��д
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    // ʵ��
public:
    virtual ~PanelView() { }
};


#include "Include/stdafx.h"

#include "Include/Rendering/VisualObject.h"


void VisualObject::Render()
{
    glMatrixMode(GL_MODELVIEW); // Select ModelView matrix.
    glPushMatrix(); // save current matrix
    glMultMatrixd(Transformation_.Data); // add current transform to current matrix
    PrepareRendering(); // prepare to render.
    RenderContent(); // render
    glPopMatrix(); // restore the saved matrix.
}

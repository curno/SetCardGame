#include "Include/stdafx.h"

#include "Include/Rendering/VisualGameScene.h"


void VisualGameScene::InitializeGameScene()
{
    for (int row = 0; row < RowCount; ++row)
    {
        for (int column = 0; column < ColumnCount; ++column)
        {
            Cards_[row][column] = ::std::make_shared<VisualCard>(nullptr);
            
        }
    }
}

void VisualGameScene::OnResize(const CSize &size)
{
    __super::OnResize(size);
    
}

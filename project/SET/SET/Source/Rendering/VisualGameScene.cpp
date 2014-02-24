#include "Include/stdafx.h"

#include "Include/Rendering/VisualGameScene.h"


void VisualGameScene::InitializeGameScene()
{
    // init no cards.
    Transformation_.Rotate(0.0, 0.0, 1.0, 0.01);
}

void VisualGameScene::OnResize(const CSize &size)
{
    __super::OnResize(size);
    ArrangeCards(size);
    return;
}

int VisualGameScene::GetColumnCount() const
{
    int retval = 0;
    
    for (int column = 0; column < ColumnCount; ++column)
        for (int row = 0; row < RowCount; ++row)
            if (Cards_[row][column] != nullptr)
                retval = column;
    return retval + 1;
}

void VisualGameScene::ArrangeCards(const CSize &)
{
    UpdateLayoutParameter();

    for (int row = 0; row < RowCount; ++row)
        for (int column = 0; column < ColumnCount; ++column)
            if (Cards_[row][column] != nullptr)
            {
                // for every card in the scene.
                ref<VisualCard> card = Cards_[row][column];

                CSize size;
                CPoint position;
                // get size and position
                GetSlotGeometryForCard(card, row, column, position, size);

                // apply
                card->Size = size;
                card->Position = position;
            }
}


bool VisualGameScene::GetEmptySlot(int row_hint, int column_hint, int &row_result, int &column_result)
{
    for (int column = column_hint; column < ColumnCount; ++column)
    {
        int row_start = column == column_hint ? row_hint : 0;
        for (int row = row_start; row < RowCount; ++row)
        {
            if (Cards_[row][column] == nullptr)
            {
                row_result = row;
                column_result = column;
                return true;
            }
        }
    }
    return false;
}

void VisualGameScene::UpdateLayoutParameter()
{
    CSize size = Size;
    LayoutParameter_.ColumnCount = GetColumnCount();
    if (LayoutParameter_.ColumnCount == 0)
        return;

    LayoutParameter_.RowCount = RowCount;

    LayoutParameter_.CellWidth = size.cx * MaginRatio / LayoutParameter_.ColumnCount * CellRatio;
    LayoutParameter_.CellHeight = size.cy * MaginRatio / LayoutParameter_.RowCount * CellRatio;

    LayoutParameter_.XBase = size.cx * (1 - MaginRatio) / 2.0 + size.cx * MaginRatio / LayoutParameter_.ColumnCount * (1 - CellRatio) / 2.0;
    LayoutParameter_.YBase = size.cy * (1 - MaginRatio) / 2.0 + size.cy * MaginRatio / LayoutParameter_.RowCount * (1 - CellRatio) / 2.0;

    LayoutParameter_.XSpace = size.cx * MaginRatio / LayoutParameter_.ColumnCount;
    LayoutParameter_.YSpace = size.cy * MaginRatio / LayoutParameter_.RowCount;
}

void VisualGameScene::GetSlotGeometryForCard(const ref<VisualCard> card, int row, int column, CPoint &position, CSize &size)
{
    // the width/height ratio of the card.
    static double ratio = 0.618;

    // set size.
    // width space superfluous 
    if (LayoutParameter_.CellWidth / LayoutParameter_.CellHeight >= ratio)
        size = CSize(static_cast<int>(ratio *LayoutParameter_.CellHeight),
        static_cast<int>(LayoutParameter_.CellHeight));
    else // height superfluous 
        size = CSize(static_cast<int>(LayoutParameter_.CellWidth),
        static_cast<int>(LayoutParameter_.CellWidth / ratio));

    // set position
    position = CPoint(static_cast<int>(LayoutParameter_.XBase + column * LayoutParameter_.XSpace + (LayoutParameter_.CellWidth - size.cx) / 2.0),
        static_cast<int>(LayoutParameter_.YBase + row * LayoutParameter_.YSpace + (LayoutParameter_.CellHeight - size.cy) / 2.0));
}

void VisualGameScene::DealCards(const ::std::unordered_set<CardRef> &cards)
{
    // create visual cards for every new card.
    int row = 0; 
    int column = 0;
    for (auto i = cards.begin(); i != cards.end(); ++i)
    {
        // for every card
        CardRef card = *i;
        ref<VisualCard> visual_card = ref<VisualCard>(new VisualCard(card)); // create visual card
        if (!GetEmptySlot(row, column, row, column)) // get card slot.
            assert(false);
        CPoint position;
        CSize size;
        GetSlotGeometryForCard(visual_card, row, column, position, size); // get visual card geometry
        
        // set geometry
        visual_card->Position = position; 
        visual_card->Size = size;

        // save visual card.
        AddChild(visual_card);
        Cards_[row][column] = visual_card;
    }
}

VisualGameScene::VisualGameScene(ref<Game> game) : Game_(game)
{
    Game_->Scene = this;
    Size = CSize(300, 200);
    Position = CPoint(0, 0);
    InitializeGameScene();
}

void VisualGameScene::PrepareRendering()
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

const double VisualGameScene::MaginRatio = 0.8;
const double VisualGameScene::CellRatio = 0.8;

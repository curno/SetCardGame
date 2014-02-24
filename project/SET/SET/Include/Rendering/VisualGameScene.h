#pragma once

#include "VisualScene.h"
#include "VisualCard.h"
#include "../Model/Game.h"

class VisualGameScene : public VisualScene
{
    static const int RowCount = 3;
    static const int ColumnCount = 7;
private:
    ref<Game> Game_;
    ref<VisualCard> Cards_[RowCount][ColumnCount]; // 3 * 7 = 21 visual card slots in the game scene, which can be empty.
public:
    VisualGameScene(ref<Game> game);
    
    void Update();
    void DealCards(const ::std::unordered_set<CardRef> &cards);
protected:
    virtual void OnResize(const CSize &size) override;
    virtual void PrepareRendering() override;
private:
    void InitializeGameScene();
    // this function set the geometry of the visual cards in the game scene according to current scene size.
    void ArrangeCards(const CSize &size);
    int GetColumnCount() const; 

    // this function get next slot whose index is returned by row and column.
    // the row_hint and column_hint will be the index from which to search, following in the column-first order.
    // return true if a empty slot is found, otherwise false.
    bool GetEmptySlot(int row_hint, int column_hint, int &row, int &column);
    void GetSlotGeometryForCard(const ref<VisualCard> card, int row, int column, CPoint &position, CSize &size);
    void UpdateLayoutParamter();

    // this is the layout parameters the scene use to arrange cards.
    struct 
    {
        int RowCount;
        int ColumnCount;

        // (XBase, YBase) is the position of the card Cards_[0][0]
        double XBase;
        double YBase;
        // (XSpace, YSpace) is the distance between two adjacent cards, in x and y direction each.
        double XSpace;
        double YSpace;

        // (CellWidth, CellHeight) is the cell space each card can use.
        double CellWidth;
        double CellHeight;
    } LayoutParameter_;

    static const double MaginRatio;
    static const double CellRatio;

private:
    // game logic
    void DealCard()
    {
        Game_->DealMore();
    }
};
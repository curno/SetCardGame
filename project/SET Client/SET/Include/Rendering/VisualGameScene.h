#pragma once

#include "../stdafx.h"
#include "VisualScene.h"
#include "VisualCard.h"
#include "../Model/Game.h"



class VisualGameScene : public VisualScene
{
    static const int RowCount = 3;
    static const int ColumnCount = 7;
    static const double MaginRatio;
    static const double CellRatio;
private:
    ref<Game> Game_;
    VisualCardRef Cards_[RowCount][ColumnCount]; // 3 * 7 = 21 visual card slots in the game scene, which can be empty.
    ref<Animation> DealCardAnimation_;
    ::std::vector<VisualCardRef> CurrentChoosedCard_;
    ref<Animation> HintCardsAnimation_;

    Point DealCardStartPoint_;
public:
    VisualGameScene(ref<Game> game);
    
    bool IsAnimating();
protected:
    virtual void OnResize(const CSize &size) override;
    virtual void PrepareRendering() override;
    virtual void RenderContent() override;

private:
    void InitializeGameScene();
    // create visual cards and add them to the scene with animations.
    void DealCards(const ::std::unordered_set<CardRef> &cards);

    // this function set the geometry of the visual cards in the game scene according to current scene size.
    void ArrangeCards(const CSize &size);
    // get count of column which has as least one card.
    int GetColumnCount() const; 

    // this function get next slot whose index is returned by row and column.
    // the row_hint and column_hint will be the index from which to search, following in the column-first order.
    // return true if a empty slot is found, otherwise false.
    bool GetEmptySlot(int row_hint, int column_hint, int &row, int &column);

    VisualCardRef GetVisualCard(CardRef card);
    // get the position and size for card card at slot (row, column)
    void GetSlotGeometryForCard(const VisualCardRef card, int row, int column, Point &position, Dimension &size);

    void EmptySlot(VisualCardRef card);

    // update LayoutParameter_ according to the view size;
    void UpdateLayoutParameter();

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

    // This function create a animation for the card when the card is dealed.
    // At first, the card is flipped over. The animation consists of three steps:
    // 1. The card moving from up outside of the scene (from) to the above of the final position. 
    // 2. Then the card moves down from current position to the final position.
    // 3. While step 2, the card flip over, so the content of card will be seen.
    ref<Animation> DealCardAnimation(VisualCardRef card, Point position, Dimension dimension);
    void DiscardCardAnimation(VisualCardRef card);
public:
    void Start();
<<<<<<< HEAD:project/SET Client/SET/Include/Rendering/VisualGameScene.h
    void Stop();


=======
>>>>>>> e5015f7ea8d62124b330cd85b669868caac403ea:project/SET/SET/Include/Rendering/VisualGameScene.h
    // give more card to use if could. the card fly from position from.
    void Deal();
    
    // ask for hint
    bool Hint();
    // game logic
    void OnCardChoosed(VisualCardRef visual_card);
    void OnCardCancleChoosed(VisualCardRef visual_card);

    ref<Game> GetGame() { return Game_; }
    void SetDealCardStartPosition(const Point &point) { DealCardStartPoint_ = point; }


private:
    // remove a visual card from the scene, with animation
    void DiscardCard(VisualCardRef card, bool animation = true);
    void Clear();
<<<<<<< HEAD:project/SET Client/SET/Include/Rendering/VisualGameScene.h
    void DisableAllCards();

=======
    
>>>>>>> e5015f7ea8d62124b330cd85b669868caac403ea:project/SET/SET/Include/Rendering/VisualGameScene.h
public:
    static const double SlopeTheta;
};
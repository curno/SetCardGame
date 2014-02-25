#include "Include/stdafx.h"

#include "Include/Rendering/VisualGameScene.h"
#include "Res/resource.h"
#include "Include/Rendering/TextureManager.h"
#include "Include/Animation/VisualObjectAnimations.h"
#include "Include/Animation/GroupAnimation.h"
#include "Include/Animation/SequentialAnimation.h"


void VisualGameScene::InitializeGameScene()
{
    // init no cards.
    Material_ = Material::GetMaterial("white rubber");
}

void VisualGameScene::OnResize(const CSize &size)
{
    __super::OnResize(size);
    ArrangeCards(size);

    GetTransformation().Reset();
    GetTransformation().RotateByCenter(Size.Width / 2.0, Size.Height / 2.0, 0.0,
        1.0, 0.0, 0.0, -SlopeTheta);
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

                Dimension size;
                Point position;
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
    Dimension size = Size;
    LayoutParameter_.ColumnCount = GetColumnCount();
    if (LayoutParameter_.ColumnCount == 0)
        return;

    LayoutParameter_.RowCount = RowCount;

    LayoutParameter_.CellWidth = size.Width * MaginRatio / LayoutParameter_.ColumnCount * CellRatio;
    LayoutParameter_.CellHeight = size.Height * MaginRatio / LayoutParameter_.RowCount * CellRatio;

    LayoutParameter_.XBase = size.Width * (1 - MaginRatio) / 2.0 + size.Width * MaginRatio / LayoutParameter_.ColumnCount * (1 - CellRatio) / 2.0;
    LayoutParameter_.YBase = size.Height * (1 - MaginRatio) / 2.0 + size.Height * MaginRatio / LayoutParameter_.RowCount * (1 - CellRatio) / 2.0;

    LayoutParameter_.XSpace = size.Width * MaginRatio / LayoutParameter_.ColumnCount;
    LayoutParameter_.YSpace = size.Height * MaginRatio / LayoutParameter_.RowCount;
}

void VisualGameScene::GetSlotGeometryForCard(const ref<VisualCard> card, int row, int column, Point &position, Dimension &size)
{
    // the width/height ratio of the card.
    double ratio = 1.0 / VisualCard::HeightPerWidthRatio;

    Coordinate width;
    // width space superfluous 
    if (LayoutParameter_.CellWidth / LayoutParameter_.CellHeight >= ratio)
        width = static_cast<Coordinate>(ratio * LayoutParameter_.CellHeight);
    else // height superfluous 
        width = static_cast<Coordinate>(LayoutParameter_.CellWidth);

    // set position
    position = Point(static_cast<int>(LayoutParameter_.XBase + column * LayoutParameter_.XSpace + (LayoutParameter_.CellWidth - width) / 2.0),
        static_cast<int>(LayoutParameter_.YBase + row * LayoutParameter_.YSpace + (LayoutParameter_.CellHeight - width * VisualCard::HeightPerWidthRatio) / 2.0));

    // set size.
    size = Dimension(width, static_cast<Coordinate>(width * VisualCard::HeightPerWidthRatio), static_cast<Coordinate>(width * VisualCard::DepthPerWidthRatio));

    // move position to center as final result
    position += Dimension(size.Width / 2, size.Height / 2, size.Depth / 2);
}

void VisualGameScene::DealCards(const ::std::unordered_set<CardRef> &cards)
{
    // create visual cards for every new card.
    int row = 0; 
    int column = 0;
    ::std::vector<ref<VisualCard>> new_cards;
    for (auto i = cards.begin(); i != cards.end(); ++i)
    {
        // for every card
        CardRef card = *i;
        ref<VisualCard> visual_card = ref<VisualCard>(new VisualCard(card)); // create visual card
        if (!GetEmptySlot(row, column, row, column)) // get card slot.
            assert(false);
        // save visual card.
        AddChild(visual_card);
        Cards_[row][column] = visual_card;

        new_cards.push_back(visual_card);
    }

    ref<GroupAnimation> animation = ::std::make_shared<GroupAnimation>();
    DealCardAnimation_ = animation;
    UpdateLayoutParameter();
    for (int row = 0; row < RowCount; ++row)
    {
        for (int column = 0; column < ColumnCount; ++column)
        {
            if (Cards_[row][column] != nullptr)
            {
                Point position;
                Dimension size;
                GetSlotGeometryForCard(Cards_[row][column], row, column, position, size); // get visual card geometry
                if (::std::find(new_cards.begin(), new_cards.end(), Cards_[row][column])
                     != new_cards.end())
                {
                   animation->AddAnimation(DealCardAnimation(Cards_[row][column], position, size));
                }
                else
                {
                    ref<Animation> move = MakeGenericAnimation(500, MoveVisualObject(Cards_[row][column].get(), position));
                    ref<Animation> rotate = MakeGenericAnimation(500, TransformVisualObject(Cards_[row][column].get(), Transformation()));
                    animation->AddAnimation(move);
                    animation->AddAnimation(rotate);
                }
            }
        }
    }
    animation->Start();
}

VisualGameScene::VisualGameScene(ref<Game> game) : Game_(game)
{
    Game_->Scene = this;
    Size = Dimension(300, 200);
    Position = Point(0, 0);
    InitializeGameScene();
}

void VisualGameScene::PrepareRendering()
{
    __super::PrepareRendering();
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

void VisualGameScene::RenderContent()
{
    static const int Outer = 1;
    // first render ground
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture(IDB_BOARD));
    glBegin(GL_QUADS);
    glNormal3d(0.0, 0.0, 1.0);
    glTexCoord2d(-Outer, -Outer);
    glVertex3d(-Size.Width * Outer, -Size.Height * Outer, 0);
    glTexCoord2d(1 + Outer, -Outer);
    glVertex3d((1 + Outer) * Size.Width, -Size.Height * Outer, 0);
    glTexCoord2d(1 + Outer, 1 + Outer);
    glVertex3d((1 + Outer) * Size.Width, (1 + Outer) * Size.Height, 0);
    glTexCoord2d(-Outer, 1 + Outer);
    glVertex3d(-Size.Width * Outer, (1 + Outer) * Size.Height, 0);
    glEnd();

    __super::RenderContent();
}

ref<Animation> VisualGameScene::DealCardAnimation(ref<VisualCard> card, Point position, Dimension dimension)
{
    static const double speed = 1;
    static const int TurnDuration = 500;
    card->Rotate(0.0, 1.0, 0.0, PI);
    Point start_point(position.X, static_cast<Coordinate>(Size.Height + dimension.Height / 2.0 + 100), static_cast<Coordinate>(dimension.Depth * 1.6));
    card->Position = start_point;
    card->Size = dimension;
    Point end_point(position.X, position.Y, start_point.Z);
    int duration = static_cast<int>(abs((end_point.Y - start_point.Y) / speed));
    ref<Animation> move_animation = MakeGenericAnimation(duration, MoveVisualObject(card.get(), start_point, end_point));

    Point start_point_down = end_point;
    Point end_point_down = position;
    ref<Animation> move_animation_down = MakeGenericAnimation(TurnDuration, MoveVisualObject(card.get(), start_point_down, end_point_down));
    ref<Animation> turn_animation = MakeGenericAnimation(TurnDuration, RotateVisualObject(card.get(), 0.0, 1.0, 0.0, PI));
    ref<GroupAnimation> group_animation = ::std::make_shared<GroupAnimation>();
    group_animation->AddAnimation(move_animation_down);
    group_animation->AddAnimation(turn_animation);
    
    auto sequential_animation = ::std::make_shared<SequentialAnimation>();
    sequential_animation->AddAnimation(move_animation);
    sequential_animation->AddAnimation(group_animation);

    return sequential_animation;
}

void VisualGameScene::OnMouseButtonDown()
{
    if (Game_->MoreToDeal())
        Game_->DealMore();
}

const double VisualGameScene::MaginRatio = 0.8;
const double VisualGameScene::CellRatio = 0.8;
const double VisualGameScene::SlopeTheta = 0.45;

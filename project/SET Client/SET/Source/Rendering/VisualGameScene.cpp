#include "Include/stdafx.h"

#include "Include/Rendering/VisualGameScene.h"
#include "Res/resource.h"
#include "Include/Rendering/TextureManager.h"
#include "Include/Animation/VisualObjectAnimations.h"
#include "Include/Animation/GroupAnimation.h"
#include "Include/Animation/SequentialAnimation.h"
#include "Include/Sound/SoundPlayer.h"
#include "Include/Utils/Random.h"

void VisualGameScene::InitializeGameScene()
{
    // init no cards.
    Material_ = Material::GetMaterial("default");
}

void VisualGameScene::OnResize(const CSize &size)
{
    __super::OnResize(size);
    ArrangeCards(size);

    // slightly lean the desk to gain a 3D visual effect.
    GetTransformation().Reset();
    GetTransformation().RotateByCenter(0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, -SlopeTheta);
    GetTransformation().RotateByCenter(0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, -SlopeTheta / 2);
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
                VisualCardRef card = Cards_[row][column];

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

    LayoutParameter_.XBase = size.Width * (1 - MaginRatio) / 2.0 + size.Width * MaginRatio / LayoutParameter_.ColumnCount * (1 - CellRatio) / 2.0 - Size.Width / 2.0;
    LayoutParameter_.YBase = size.Height * (1 - MaginRatio) / 2.0 + size.Height * MaginRatio / LayoutParameter_.RowCount * (1 - CellRatio) / 2.0 - size.Height / 2.0;

    LayoutParameter_.XSpace = size.Width * MaginRatio / LayoutParameter_.ColumnCount;
    LayoutParameter_.YSpace = size.Height * MaginRatio / LayoutParameter_.RowCount;
}

void VisualGameScene::GetSlotGeometryForCard(const VisualCardRef card, int row, int column, Point &position, Dimension &size)
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
    ::std::vector<VisualCardRef> new_cards;
    for (auto i = cards.begin(); i != cards.end(); ++i)
    {
        // for every card
        CardRef card = *i;
        VisualCardRef visual_card = VisualCardRef(new VisualCard(card, this)); // create visual card
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
                    animation->AddAnimation(move);
                }
            }
        }
    }
    animation->Start();
}

VisualGameScene::VisualGameScene(ref<Game> game) : Game_(game)
{
    Size = Dimension(300, 200);
    Position = Point(0, 0);
    InitializeGameScene();
}

void VisualGameScene::PrepareRendering()
{
    __super::PrepareRendering();
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
    glVertex3d(-Size.Width * (Outer + 0.5), -Size.Height * (Outer + 0.5), 0);
    glTexCoord2d(1 + Outer, -Outer);
    glVertex3d(Size.Width * (Outer + 0.5), -Size.Height * (Outer + 0.5), 0);
    glTexCoord2d(1 + Outer, 1 + Outer);
    glVertex3d(Size.Width * (Outer + 0.5), Size.Height * (Outer + 0.5), 0);
    glTexCoord2d(-Outer, 1 + Outer);
    glVertex3d(-Size.Width * (Outer + 0.5), Size.Height * (Outer + 0.5), 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    __super::RenderContent();
}

ref<Animation> VisualGameScene::DealCardAnimation(VisualCardRef card, Point position, Dimension dimension)
{
    static const double speed = 1.5;
    static const int TurnDuration = 500;

    // Init flip the card over in the scene
    int index = this->IndexOf(card);
    this->GetTransformation(index).Rotate(0.0, 1.0, 0.0, PI); // flip card.

    // start from up outside the scene
    Point start_point;
    start_point = DealCardStartPoint_;
    // start_point = Point(position.X, static_cast<Coordinate>(Size.Height + dimension.Height / 2.0 + 100), static_cast<Coordinate>(dimension.Depth * 1.6));
    // set start point
    card->Position = start_point;
    // set size
    card->Size = dimension;
    // end point is right above the correct position
    Point end_point(position.X, position.Y, start_point.Z);
    // calc duration with speed
    int duration = static_cast<int>((end_point - start_point).Length() / speed);
    // create move animation. Step 1
    ref<Animation> move_animation = MakeGenericAnimation(duration, MoveVisualObject(card.get(), start_point, end_point));
    // create the flip animation. Step 3
    Animation *turn_animation = new GenericAnimation<::Rotate>(duration, ::Rotate(this->GetTransformation(index), 0.0, 1.0, 0.0, PI));
    turn_animation->DeleteWhenStopped = true; // the flip animation deletes itself
    // set the flip animation (Step 3) to be started when the move animation (Step 1) stops.
    move_animation->StopOperation = MakeGenericOperation(
        [turn_animation]()
    {
        SoundPlayer::Instance().Play(IDR_PUT_CARD);
        turn_animation->Start();
    });

    // create the down animation. (Step 2)
    Point start_point_down = end_point;
    Point end_point_down = position;
    ref<Animation> move_animation_down = MakeGenericAnimation(TurnDuration, MoveVisualObject(card.get(), start_point_down, end_point_down));

    // create a sequential animation for step 1 and step 2
    auto sequential_animation = ::std::make_shared<SequentialAnimation>();
    sequential_animation->AddAnimation(move_animation);
    sequential_animation->AddAnimation(move_animation_down);

    return sequential_animation;
}

void VisualGameScene::DiscardCardAnimation(VisualCardRef card)
{
    static const double SpeedMax = 0.6;
    static const double SpeedMin = 1;
    static const double ThetaMax = 3.3 * PI / 2;
    static const double ThetaMin = -3.3 * PI / 2;
    static const double TransMax = 1;
    static const double TransMin = -1;

    double speed = Random::NextDouble() * (SpeedMax - SpeedMin) + SpeedMin;
    double theta = Random::NextDouble() * (ThetaMax - ThetaMin) + ThetaMin;
    double trans = Random::NextDouble() * (TransMax - TransMin) + TransMin;

    Point start_point = card->Position;
    Point end_point(static_cast<Coordinate>(card->Position.X + card->Size.Width * trans), -card->Size.Height * 5, card->Position.Z * 5);

    int duration = static_cast<int>((end_point - start_point).Length() / speed);

    // move
    ref<Animation> move_animation = MakeGenericAnimation(duration, MoveVisualObject(card.get(), end_point));
    move_animation->Behavior = AnimationBehavior::WiredBehavior();

    // rotate
    ref<Animation> rotate_animation = MakeGenericAnimation(duration, ::Rotate(card->GetTransformation(), 0.0, 0.0, 1.0, theta));

    // create group.
    GroupAnimation *animation = new GroupAnimation;
    animation->AddAnimation(move_animation);
    animation->AddAnimation(rotate_animation);
    animation->DeleteWhenStopped = true;
    animation->StopOperation = MakeGenericOperation(
        [card, this](){
        this->RemoveChild(card); // remove card.
    });
    animation->Start();

}

void VisualGameScene::OnCardChosen(VisualCardRef visual_card)
{
    CurrentChosenCard_.push_back(visual_card);
    if (CurrentChosenCard_.size() == Card::CardCountInASet) 
    {
        ::std::vector<CardRef> cards;
        for each(VisualCardRef visual_card in CurrentChosenCard_)
            cards.push_back(visual_card->Card);
        bool success = Game_->CheckAndScore(cards);
        // If not a SET
        if (!success)
        {
            for each (auto visual_card in CurrentChosenCard_)
                visual_card->CancelChosen();
            CurrentChosenCard_.clear();
        }
        else
        {
            // is a set, discard the selected cards.           
            for each (auto visual_card in CurrentChosenCard_)
                DiscardCard(visual_card);
            CurrentChosenCard_.clear();

            // play sound
            SoundPlayer::Instance().Play(IDR_SCORE);
        }
    }
}

void VisualGameScene::OnCardCancleChosen(VisualCardRef visual_card)
{
    CurrentChosenCard_.erase(::std::find(CurrentChosenCard_.begin(), CurrentChosenCard_.end(), visual_card));
}

void VisualGameScene::EmptySlot(VisualCardRef card)
{
    for (int row = 0; row < RowCount; ++row)
        for (int column = 0; column < ColumnCount; ++column)
            if (Cards_[row][column] == card)
            {
                Cards_[row][column] = nullptr;
                return;
            }
}
void VisualGameScene::DiscardCard(VisualCardRef card, bool animation)
{
    card->Discarded(); // discard card
    EmptySlot(card); // empty slot
    if (animation)
        DiscardCardAnimation(card); // animate
    else
        RemoveChild(card);
}

bool VisualGameScene::Hint()
{
    ::std::vector<CardRef> cards;
    if (Game_->Hint(cards))
    {
        // animation.
        for each (CardRef card in cards)
            GetVisualCard(card)->Shake();
        return true;
    }
    return false;
}

VisualCardRef VisualGameScene::GetVisualCard(CardRef card)
{
    for (int i = 0; i < RowCount; ++i)
    {
        for (int j = 0; j < ColumnCount; ++j)
            if (Cards_[i][j] != nullptr && Cards_[i][j]->Card == card)
                return Cards_[i][j];
    }
    return nullptr;
}

void VisualGameScene::Start()
{
    Clear();
    Game_->Start();
    DealCards(Game_->GetCardsOnDesk());
}

void VisualGameScene::Stop()
{
    Game_->Stop();
    DisableAllCards();
}

void VisualGameScene::Clear()
{
    AnimationManager::Instance().StopAllAnimation();
    DealCardAnimation_ = nullptr;
    CurrentChosenCard_.clear();
    HintCardsAnimation_ = nullptr;
    for (int i = 0; i < RowCount; ++i)
        for (int j = 0; j < ColumnCount; ++j)
        {
            if (Cards_[i][j] != nullptr)
                DiscardCard(Cards_[i][j], false);
        }

}

void VisualGameScene::Deal()
{
    if (Game_->GameState == Game::State::Active)
    {
        if (Game_->MoreToDeal())
        {
            ::std::unordered_set<CardRef> new_cards;
            Game_->DealMore(new_cards);
            DealCards(new_cards);
        }
    }
}

void VisualGameScene::DisableAllCards()
{
    for (int row = 0; row < RowCount; ++row)
    {
        for (int column = 0; column < ColumnCount; ++column)
            if (Cards_[row][column] != nullptr)
                Cards_[row][column]->Enabled = false;
    }
}

void VisualGameScene::SetDealCardStartPosition(const Point &point)
{
    DealCardStartPoint_ = point;
}

ref<Game> VisualGameScene::GetGame()
{
    return Game_;
}

const double VisualGameScene::MaginRatio = 0.8;
const double VisualGameScene::CellRatio = 0.8;
const double VisualGameScene::SlopeTheta = 0.45;

#include "models/GameModel.h"

CardModel* GameModel::getCard(int id)
{
    if (id < 0 || id >= static_cast<int>(cards.size()))
        return nullptr;
    return &cards[id];
}

const CardModel* GameModel::getCard(int id) const
{
    if (id < 0 || id >= static_cast<int>(cards.size()))
        return nullptr;
    return &cards[id];
}

int GameModel::handTopId() const
{
    if (handStack.empty())
        return -1;
    return handStack.back();
}

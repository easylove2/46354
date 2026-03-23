#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "models/CardModel.h"
#include <vector>

// 运行时状态：主牌区 + 手牌栈（栈顶为 vector 最后一个）+ 牌库（抽顶为 vector 最后一个）。
class GameModel
{
public:
    std::vector<CardModel> cards;
    std::vector<int> handStack;
    std::vector<int> reserveStack;
    std::vector<int> playFieldIds;

    void clear()
    {
        cards.clear();
        handStack.clear();
        reserveStack.clear();
        playFieldIds.clear();
    }

    CardModel* getCard(int id);
    const CardModel* getCard(int id) const;

    int handTopId() const;
    bool handEmpty() const { return handStack.empty(); }
};

#endif

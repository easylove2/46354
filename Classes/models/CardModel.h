#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "utils/CardTypes.h"
#include "cocos2d.h"

/// 单张牌的运行时数据。
struct CardModel
{
    int id = 0;
    CardSuit suit = CardSuit::Clubs;
    CardFace face = CardFace::A;
    cocos2d::Vec2 layoutPos;
    bool faceUp = true;
    bool onPlayField = true;
};

#endif

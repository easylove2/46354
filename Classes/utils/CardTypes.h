#ifndef __CARD_TYPES_H__
#define __CARD_TYPES_H__

#include "cocos2d.h"

/// 花色：0 梅花、1 方块、2 红桃、3 黑桃
enum class CardSuit : int
{
    None = -1,
    Clubs,
    Diamonds,
    Hearts,
    Spades,
};

/// 点数：0=A … 12=K
enum class CardFace : int
{
    None = -1,
    A,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
};

inline bool cardSuitIsRed(CardSuit s)
{
    return s == CardSuit::Diamonds || s == CardSuit::Hearts;
}

/// 用于 ±1 匹配的等级 1…13
int cardFaceRank(CardFace f);

const char* cardFaceFileSuffix(CardFace f);

#endif

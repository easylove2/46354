#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "utils/CardTypes.h"
#include <utility>
#include <vector>

// 将关卡中的主牌区花色点数随机化：对 52 张牌洗牌后依次发到 Playfield 槽位。
void shuffleFullDeck(std::vector<std::pair<CardFace, CardSuit>>& out);

#endif

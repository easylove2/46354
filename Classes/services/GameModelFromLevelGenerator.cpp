#include "services/GameModelFromLevelGenerator.h"

#include <algorithm>
#include <random>

void shuffleFullDeck(std::vector<std::pair<CardFace, CardSuit>>& out)
{
    out.clear();
    for (int f = 0; f <= 12; ++f)
    {
        for (int s = 0; s <= 3; ++s)
            out.push_back({static_cast<CardFace>(f), static_cast<CardSuit>(s)});
    }
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(out.begin(), out.end(), rng);
}

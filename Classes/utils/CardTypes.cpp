#include "utils/CardTypes.h"

int cardFaceRank(CardFace f)
{
    int v = static_cast<int>(f);
    if (v < 0 || v > 12)
        return 0;
    return v + 1;
}

const char* cardFaceFileSuffix(CardFace f)
{
    static const char* kSuffix[] = {
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
    };
    int v = static_cast<int>(f);
    if (v < 0 || v > 12)
        return "A";
    return kSuffix[v];
}

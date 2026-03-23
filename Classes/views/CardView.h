#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "models/CardModel.h"
#include "cocos2d.h"
#include <functional>

// 组合牌面：通用底图 + 花色 + 点数图。
class CardView : public cocos2d::Node
{
public:
    static CardView* create(int cardId, const CardModel& model);
    bool initWithCard(int cardId, const CardModel& model);

    void setModel(const CardModel& model);
    int getCardId() const { return _cardId; }

    // 显示正面（花色+点数）或仅背面；切换时可带翻牌动画。
    void setFaceUp(bool faceUp, bool animated);

    bool isFaceUp() const { return _faceUp; }

    void setClickCallback(const std::function<void(int)>& cb) { _clickCb = cb; }

    static cocos2d::Size defaultCardSize();

protected:
    void rebuildSprites();

    int _cardId = 0;
    CardModel _model;
    cocos2d::Sprite* _bg = nullptr;
    cocos2d::Sprite* _suit = nullptr;
    cocos2d::Sprite* _face = nullptr;
    bool _faceUp = true;
    std::function<void(int)> _clickCb;
};

#endif

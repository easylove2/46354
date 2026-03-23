#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "controllers/GameController.h"
#include <map>
#include <vector>

class CardView;

// 视图层：主场景节点、布局与 UI；业务逻辑在 GameController。
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(GameScene);

private:
    void buildUi();
    void loadLevel();
    void ensureCardViews();
    // animateHandFaces：手牌顶变化时是否播放翻牌动画；撤销/首次加载时为 false。
    void syncViewsFromModel(bool animateHandFaces = false);

    void onCardClicked(int cardId);
    void onUndo(cocos2d::Ref*);
    void onClose(cocos2d::Ref*);

    cocos2d::Vec2 handPosForIndex(int indexInStack) const;
    cocos2d::Vec2 reservePosForIndex(size_t indexInDeck) const;

    GameController _controller;
    std::map<int, CardView*> _cardViews;

    cocos2d::Node* _playFieldRoot = nullptr;
    cocos2d::Node* _handRoot = nullptr;
    cocos2d::Node* _reserveRoot = nullptr;
};

#endif

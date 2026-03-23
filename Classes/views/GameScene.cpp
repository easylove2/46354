#include "views/GameScene.h"
#include "views/CardView.h"
#include "models/GameModel.h"

#include <algorithm>

USING_NS_CC;

// 设计分辨率 1080x2080：底部左侧牌库（全背面），右侧仅手牌顶一张正面；其余手牌背面叠在顶牌左侧。
static const float kDesignW = 1080.f;
static const float kHandBandH = 580.f;
static const float kReserveDrawCenterX = kDesignW * 0.28f;
static const float kHandTopCenterX = kDesignW * 0.74f;
static const float kHandBandY = kHandBandH * 0.5f;
static const float kReserveDy = 4.f;
// 背面手牌与最右侧正面顶牌之间的额外水平间距。
static const float kHandGapBackToFace = 56.f;

static float handDxForCount(size_t n)
{
    if (n > 16)
        return 32.f;
    if (n > 12)
        return 38.f;
    if (n > 8)
        return 44.f;
    return 52.f;
}

Scene* GameScene::createScene()
{
    return GameScene::create();
}

Vec2 GameScene::reservePosForIndex(size_t indexInDeck) const
{
    return Vec2(kReserveDrawCenterX, kHandBandH * 0.5f + kReserveDy * static_cast<float>(indexInDeck));
}

Vec2 GameScene::handPosForIndex(int indexInStack) const
{
    const GameModel& m = _controller.model();
    size_t n = m.handStack.size();
    if (n == 0)
        return Vec2(kHandTopCenterX, kHandBandY);
    float dx = handDxForCount(n);
    float x = kHandTopCenterX;
    for (int k = static_cast<int>(n) - 2; k >= indexInStack; --k)
    {
        x -= (k == static_cast<int>(n) - 2) ? (dx + kHandGapBackToFace) : dx;
    }
    return Vec2(x, kHandBandY);
}

void GameScene::ensureCardViews()
{
    for (const auto& c : _controller.model().cards)
    {
        if (_cardViews.count(c.id))
            continue;
        CardView* cv = CardView::create(c.id, c);
        if (!cv)
            continue;
        cv->setClickCallback([this](int cid) { onCardClicked(cid); });
        _cardViews[c.id] = cv;
    }
}

void GameScene::syncViewsFromModel(bool animateHandFaces)
{
    ensureCardViews();

    auto reparent = [](CardView* cv, Node* parent) {
        if (cv->getParent() == parent)
            return;
        cv->retain();
        cv->removeFromParent();
        parent->addChild(cv);
        cv->release();
    };

    const GameModel& model = _controller.model();

    for (size_t i = 0; i < model.handStack.size(); ++i)
    {
        int id = model.handStack[i];
        auto it = _cardViews.find(id);
        if (it == _cardViews.end())
            continue;
        CardView* cv = it->second;
        Vec2 p = handPosForIndex(static_cast<int>(i));
        reparent(cv, _handRoot);
        cv->setVisible(true);
        cv->setLocalZOrder(200 + static_cast<int>(i));
        cv->stopAllActions();
        cv->setScale(1.0f);
        cv->setPosition(p);
        bool isHandTop = (i + 1 == model.handStack.size());
        cv->setFaceUp(isHandTop, animateHandFaces);
    }

    for (int pid : model.playFieldIds)
    {
        auto it = _cardViews.find(pid);
        if (it == _cardViews.end())
            continue;
        CardView* cv = it->second;
        const CardModel* pm = model.getCard(pid);
        if (!pm)
            continue;
        reparent(cv, _playFieldRoot);
        cv->setVisible(true);
        cv->setLocalZOrder(100 + pid);
        cv->stopAllActions();
        cv->setScale(1.0f);
        cv->setPosition(pm->layoutPos);
        cv->setFaceUp(true, false);
    }

    for (size_t i = 0; i < model.reserveStack.size(); ++i)
    {
        int id = model.reserveStack[i];
        auto it = _cardViews.find(id);
        if (it == _cardViews.end())
            continue;
        CardView* cv = it->second;
        reparent(cv, _reserveRoot);
        cv->setVisible(true);
        cv->setLocalZOrder(400 + static_cast<int>(i));
        cv->stopAllActions();
        cv->setScale(1.0f);
        cv->setPosition(reservePosForIndex(i));
        cv->setFaceUp(false, false);
    }

    for (auto& kv : _cardViews)
    {
        int cid = kv.first;
        bool inHand = std::find(model.handStack.begin(), model.handStack.end(), cid) != model.handStack.end();
        bool inField = std::find(model.playFieldIds.begin(), model.playFieldIds.end(), cid) != model.playFieldIds.end();
        bool inReserve = std::find(model.reserveStack.begin(), model.reserveStack.end(), cid) != model.reserveStack.end();
        if (!inHand && !inField && !inReserve)
        {
            kv.second->setVisible(false);
            kv.second->stopAllActions();
        }
    }
}

void GameScene::onCardClicked(int cardId)
{
    _controller.handleCardClick(cardId);
}

void GameScene::onUndo(Ref*)
{
    _controller.undo();
}

void GameScene::onClose(Ref*)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::buildUi()
{
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto vs = Director::getInstance()->getVisibleSize();

    auto bg = LayerColor::create(Color4B(30, 80, 40, 255), vs.width + origin.x * 2, vs.height + origin.y * 2);
    bg->setPosition(Vec2::ZERO);
    addChild(bg, -10);

    auto bottomBar = LayerColor::create(Color4B(88, 48, 118, 255), vs.width + origin.x * 2, kHandBandH);
    bottomBar->setPosition(Vec2(origin.x, origin.y));
    addChild(bottomBar, -9);

    auto hint = Label::createWithSystemFont(
        "Match +/-1 with hand top. Tap reserve top to draw. Undo.",
        "Arial",
        22.0f);
    if (hint)
    {
        hint->setColor(Color3B::WHITE);
        hint->setPosition(Vec2(origin.x + vs.width * 0.5f, origin.y + vs.height - 40));
        addChild(hint, 10);
    }

    float margin = 24.f;
    Label* lblUndo = Label::createWithSystemFont(u8"\u56de\u9000", "Arial", 28.0f);
    if (!lblUndo)
        lblUndo = Label::createWithSystemFont("Undo", "Arial", 28.0f);
    auto lblExit = Label::createWithSystemFont("Exit", "Arial", 28.0f);
    auto undoItem = MenuItemLabel::create(lblUndo, CC_CALLBACK_1(GameScene::onUndo, this));
    auto closeItem = MenuItemLabel::create(lblExit, CC_CALLBACK_1(GameScene::onClose, this));

    if (undoItem)
        undoItem->setPosition(Vec2(origin.x + vs.width - margin, origin.y + kHandBandH * 0.5f));
    if (closeItem)
        closeItem->setPosition(Vec2(origin.x + vs.width - margin, origin.y + vs.height - margin));

    auto menu = Menu::create(undoItem, closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 20);
}

void GameScene::loadLevel()
{
    if (!_controller.loadLevel("levels/level_01.json"))
    {
        CCLOG("GameScene: failed to load levels/level_01.json");
        auto origin = Director::getInstance()->getVisibleOrigin();
        auto vs = Director::getInstance()->getVisibleSize();
        auto err = Label::createWithSystemFont(
            "Level load failed: levels/level_01.json missing.\nCopy levels/ and res/ next to the exe.",
            "Arial",
            22.0f);
        if (err)
        {
            err->setColor(Color3B::RED);
            err->setPosition(Vec2(origin.x + vs.width * 0.5f, origin.y + vs.height * 0.5f));
            addChild(err, 100);
        }
        return;
    }
}

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    _controller.setOnStateChanged([this](bool anim) { syncViewsFromModel(anim); });

    _playFieldRoot = Node::create();
    _playFieldRoot->setPosition(Vec2::ZERO);
    addChild(_playFieldRoot, 0);

    _handRoot = Node::create();
    _handRoot->setPosition(Vec2::ZERO);
    addChild(_handRoot, 5);

    _reserveRoot = Node::create();
    _reserveRoot->setPosition(Vec2::ZERO);
    addChild(_reserveRoot, 4);

    buildUi();
    loadLevel();

    return true;
}

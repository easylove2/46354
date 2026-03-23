#include "views/CardView.h"
#include "utils/CardTypes.h"

USING_NS_CC;

static const char* suitImagePath(CardSuit s)
{
    switch (s)
    {
    case CardSuit::Clubs: return "res/suits/club.png";
    case CardSuit::Diamonds: return "res/suits/diamond.png";
    case CardSuit::Hearts: return "res/suits/heart.png";
    case CardSuit::Spades: return "res/suits/spade.png";
    default: return "res/suits/club.png";
    }
}

Size CardView::defaultCardSize()
{
    return Size(140.f, 200.f);
}

CardView* CardView::create(int cardId, const CardModel& model)
{
    CardView* p = new (std::nothrow) CardView();
    if (p && p->initWithCard(cardId, model))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool CardView::initWithCard(int cardId, const CardModel& model)
{
    if (!Node::init())
        return false;
    _cardId = cardId;
    _model = model;

    setContentSize(defaultCardSize());
    setAnchorPoint(Vec2(0.5f, 0.5f));

    _faceUp = true;
    rebuildSprites();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* t, Event*) {
        Vec2 p = this->convertToNodeSpace(t->getLocation());
        float hw = _contentSize.width * 0.5f;
        float hh = _contentSize.height * 0.5f;
        Rect r(-hw, -hh, _contentSize.width, _contentSize.height);
        if (!r.containsPoint(p))
            return false;
        if (_clickCb)
            _clickCb(_cardId);
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::setModel(const CardModel& model)
{
    _model = model;
    rebuildSprites();
}

void CardView::rebuildSprites()
{
    if (_bg)
    {
        _bg->removeFromParent();
        _bg = nullptr;
    }
    if (_suit)
    {
        _suit->removeFromParent();
        _suit = nullptr;
    }
    if (_face)
    {
        _face->removeFromParent();
        _face = nullptr;
    }

    Size sz = getContentSize();

    _bg = Sprite::create("res/card_general.png");
    if (_bg)
    {
        _bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        float sx = sz.width / _bg->getContentSize().width;
        float sy = sz.height / _bg->getContentSize().height;
        _bg->setScale(sx, sy);
        _bg->setPosition(Vec2(0, 0));
        addChild(_bg, 0);
    }

    _suit = Sprite::create(suitImagePath(_model.suit));
    if (_suit)
    {
        _suit->setScale(0.35f);
        _suit->setPosition(Vec2(-sz.width * 0.32f, sz.height * 0.32f));
        addChild(_suit, 1);
    }

    const char* color = cardSuitIsRed(_model.suit) ? "red" : "black";
    std::string numPath = StringUtils::format("res/number/big_%s_%s.png", color, cardFaceFileSuffix(_model.face));
    _face = Sprite::create(numPath);
    if (_face)
    {
        _face->setScale(0.9f);
        _face->setPosition(Vec2(0, sz.height * 0.02f));
        addChild(_face, 1);
    }

    if (_suit)
        _suit->setVisible(_faceUp);
    if (_face)
        _face->setVisible(_faceUp);
}

void CardView::setFaceUp(bool faceUp, bool animated)
{
    if (_faceUp == faceUp)
        return;
    if (!animated)
    {
        stopAllActions();
        setScaleX(1.0f);
        setScaleY(1.0f);
        _faceUp = faceUp;
        if (_suit)
            _suit->setVisible(faceUp);
        if (_face)
            _face->setVisible(faceUp);
        return;
    }

    stopAllActions();
    float half = 0.11f;
    auto scaleDown = ScaleTo::create(half, 0.0f, 1.0f);
    auto swap = CallFunc::create([this, faceUp]() {
        _faceUp = faceUp;
        if (_suit)
            _suit->setVisible(faceUp);
        if (_face)
            _face->setVisible(faceUp);
    });
    auto scaleUp = ScaleTo::create(half, 1.0f, 1.0f);
    runAction(Sequence::create(scaleDown, swap, scaleUp, nullptr));
}

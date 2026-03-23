#include "controllers/GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "utils/CardTypes.h"

#include <algorithm>
#include <cstdlib>

bool GameController::loadLevel(const std::string& relativePath)
{
    if (!loadLevelConfigFromJson(relativePath, _model))
        return false;
    _undo.clear();
    if (_onChanged)
        _onChanged(false);
    return true;
}

void GameController::pushSnapshot()
{
    _undo.pushFromModel(_model);
}

bool GameController::tryPromoteHand(int cardId)
{
    auto& h = _model.handStack;
    auto it = std::find(h.begin(), h.end(), cardId);
    if (it == h.end() || it == h.end() - 1)
        return false;
    pushSnapshot();
    int v = *it;
    h.erase(it);
    h.push_back(v);
    if (_onChanged)
        _onChanged(true);
    return true;
}

bool GameController::tryMatchFromField(int fieldId)
{
    if (_model.handEmpty())
        return false;
    int topId = _model.handTopId();
    const CardModel* top = _model.getCard(topId);
    const CardModel* fld = _model.getCard(fieldId);
    if (!top || !fld)
        return false;
    int r1 = cardFaceRank(top->face);
    int r2 = cardFaceRank(fld->face);
    if (std::abs(r1 - r2) != 1)
        return false;

    pushSnapshot();
    _model.handStack.pop_back();
    auto& pf = _model.playFieldIds;
    pf.erase(std::remove(pf.begin(), pf.end(), fieldId), pf.end());
    CardModel* fm = _model.getCard(fieldId);
    if (fm)
        fm->onPlayField = false;
    _model.handStack.push_back(fieldId);

    if (_onChanged)
        _onChanged(true);
    return true;
}

bool GameController::tryDrawFromReserve(int cardId)
{
    if (_model.reserveStack.empty())
        return false;
    if (cardId != _model.reserveStack.back())
        return false;
    pushSnapshot();
    _model.reserveStack.pop_back();
    _model.handStack.insert(_model.handStack.begin(), cardId);
    if (_onChanged)
        _onChanged(true);
    return true;
}

bool GameController::handleCardClick(int cardId)
{
    bool inHand = std::find(_model.handStack.begin(), _model.handStack.end(), cardId) != _model.handStack.end();
    bool inField = std::find(_model.playFieldIds.begin(), _model.playFieldIds.end(), cardId) != _model.playFieldIds.end();
    bool inReserve = std::find(_model.reserveStack.begin(), _model.reserveStack.end(), cardId) != _model.reserveStack.end();

    if (inReserve)
        return tryDrawFromReserve(cardId);

    if (inHand)
    {
        if (cardId == _model.handTopId())
            return false;
        return tryPromoteHand(cardId);
    }
    if (inField)
        return tryMatchFromField(cardId);
    return false;
}

bool GameController::undo()
{
    if (!_undo.popToModel(_model))
        return false;
    if (_onChanged)
        _onChanged(false);
    return true;
}

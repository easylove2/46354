#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "managers/UndoManager.h"
#include "models/GameModel.h"
#include <functional>
#include <string>

// 协调模型与输入：手牌换顶、主牌区 ±1 匹配、牌库抽牌、撤销。
class GameController
{
public:
    void setOnStateChanged(std::function<void(bool animateHandFaces)> cb) { _onChanged = std::move(cb); }

    GameModel& model() { return _model; }
    const GameModel& model() const { return _model; }

    bool loadLevel(const std::string& relativePath);
    bool handleCardClick(int cardId);
    bool undo();

private:
    void pushSnapshot();
    bool tryPromoteHand(int cardId);
    bool tryMatchFromField(int fieldId);
    bool tryDrawFromReserve(int cardId);

    GameModel _model;
    UndoManager _undo;
    std::function<void(bool)> _onChanged;
};

#endif

#include "managers/UndoManager.h"

#include <algorithm>

void UndoManager::pushFromModel(const GameModel& m)
{
    Snapshot s;
    s.hand = m.handStack;
    s.playField = m.playFieldIds;
    s.reserve = m.reserveStack;
    _stack.push_back(std::move(s));
}

bool UndoManager::popToModel(GameModel& m)
{
    if (_stack.empty())
        return false;
    Snapshot s = _stack.back();
    _stack.pop_back();
    m.handStack = s.hand;
    m.playFieldIds = s.playField;
    m.reserveStack = s.reserve;
    for (auto& c : m.cards)
    {
        bool onField = std::find(s.playField.begin(), s.playField.end(), c.id) != s.playField.end();
        c.onPlayField = onField;
    }
    return true;
}

void UndoManager::clear()
{
    _stack.clear();
}

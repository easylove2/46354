#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/GameModel.h"
#include <vector>

class UndoManager
{
public:
    void pushFromModel(const GameModel& m);
    bool popToModel(GameModel& m);
    void clear();
    bool empty() const { return _stack.empty(); }

private:
    struct Snapshot
    {
        std::vector<int> hand;
        std::vector<int> playField;
        std::vector<int> reserve;
    };
    std::vector<Snapshot> _stack;
};

#endif

#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "models/GameModel.h"
#include <string>

// 从关卡 JSON 加载到运行时 GameModel（主牌区随机发牌逻辑在 services 中参与）。
bool loadLevelConfigFromJson(const std::string& relativePath, GameModel& outModel);

#endif

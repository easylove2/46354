# 代码目录说明（与《纸牌程序设计【需求一】》MVC 架构一致）

```
Classes/
├── configs/              # 静态配置
│   ├── models/           # 关卡等配置声明（LevelConfig.h）
│   └── loaders/          # 配置加载（LevelConfigLoader：JSON → GameModel）
├── models/               # 运行时数据（CardModel、GameModel）
├── views/                # 视图（CardView、GameScene）
├── controllers/          # 控制器（GameController：点击、匹配、抽牌、撤销）
├── managers/             # 管理器（UndoManager，由 Controller 持有，非单例）
├── services/             # 无状态服务（GameModelFromLevelGenerator：洗牌/发牌策略）
└── utils/                # 工具（CardTypes 等）
```

扩展方式简述：

- **新卡牌展示**：扩展 `CardView` / 资源；模型仍用 `CardModel`。
- **新撤销类型**：在 `UndoManager::Snapshot` 中增加字段，并在 `pushFromModel` / `popToModel` 与 `GameController` 中同步维护。

#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "json/document-wrapper.h"

#include <algorithm>
#include <random>
#include <utility>
#include <vector>

USING_NS_CC;

static bool parseVec2(const rapidjson::Value& obj, Vec2& out)
{
    if (!obj.IsObject() || !obj.HasMember("x") || !obj.HasMember("y"))
        return false;
    out.x = static_cast<float>(obj["x"].GetDouble());
    out.y = static_cast<float>(obj["y"].GetDouble());
    return true;
}

bool loadLevelConfigFromJson(const std::string& relativePath, GameModel& outModel)
{
    outModel.clear();
    auto* fu = FileUtils::getInstance();
    std::string json = fu->getStringFromFile(relativePath);
    if (json.empty())
        json = fu->getStringFromFile(std::string("Resources/") + relativePath);
    if (json.empty())
        return false;

    rapidjson::Document doc;
    doc.Parse(json.c_str());
    if (doc.HasParseError() || !doc.IsObject())
        return false;

    int nextId = 0;

    std::vector<std::pair<CardFace, CardSuit>> playfieldDeck;
    shuffleFullDeck(playfieldDeck);
    size_t playfieldDeal = 0;

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const auto& arr = doc["Playfield"];
        for (unsigned int i = 0; i < arr.Size(); ++i)
        {
            const auto& o = arr[i];
            if (!o.IsObject())
                continue;
            CardModel c;
            c.id = nextId++;
            if (playfieldDeal < playfieldDeck.size())
            {
                c.face = playfieldDeck[playfieldDeal].first;
                c.suit = playfieldDeck[playfieldDeal].second;
                ++playfieldDeal;
            }
            else
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> df(0, 12);
                std::uniform_int_distribution<int> ds(0, 3);
                c.face = static_cast<CardFace>(df(gen));
                c.suit = static_cast<CardSuit>(ds(gen));
            }
            if (!parseVec2(o["Position"], c.layoutPos))
                return false;
            c.faceUp = true;
            c.onPlayField = true;
            outModel.cards.push_back(c);
            outModel.playFieldIds.push_back(c.id);
        }
    }

    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const auto& arr = doc["Stack"];
        for (unsigned int i = 0; i < arr.Size(); ++i)
        {
            const auto& o = arr[i];
            if (!o.IsObject())
                continue;
            CardModel c;
            c.id = nextId++;
            c.face = static_cast<CardFace>(o["CardFace"].GetInt());
            c.suit = static_cast<CardSuit>(o["CardSuit"].GetInt());
            Vec2 p;
            parseVec2(o["Position"], p);
            c.layoutPos = p;
            c.faceUp = true;
            c.onPlayField = false;
            outModel.cards.push_back(c);
            outModel.handStack.push_back(c.id);
        }
    }

    if (doc.HasMember("Reserve") && doc["Reserve"].IsArray())
    {
        const auto& arr = doc["Reserve"];
        for (unsigned int i = 0; i < arr.Size(); ++i)
        {
            const auto& o = arr[i];
            if (!o.IsObject())
                continue;
            CardModel c;
            c.id = nextId++;
            c.face = static_cast<CardFace>(o["CardFace"].GetInt());
            c.suit = static_cast<CardSuit>(o["CardSuit"].GetInt());
            Vec2 p;
            parseVec2(o["Position"], p);
            c.layoutPos = p;
            c.faceUp = true;
            c.onPlayField = false;
            outModel.cards.push_back(c);
            outModel.reserveStack.push_back(c.id);
        }
    }

    return !outModel.cards.empty();
}

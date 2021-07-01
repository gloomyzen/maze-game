#include "mapsDatabase.h"
#include "common/debugModule/logManager.h"
#include "common/utilityModule/stringUtility.h"
#include <map>

using namespace mb::databasesModule;

mapsDatabase::mapsDatabase() = default;

mapsDatabase::~mapsDatabase() {
    for (auto& [_, db] : mapsDb) {
        delete db;
        db = nullptr;
    }
}

void mapsDatabase::load(const rapidjson::Document& json) {
    if (getPath().empty()) {
        LOG_ERROR("mapsDatabase::load: path is empty!");
        return;
    }
    if (!json.IsObject()) {
        LOG_ERROR(STRING_FORMAT("mapsDatabase::load: file from path '%s' is not object!", getPath().c_str()));
        return;
    }
    auto tiles = json.FindMember("maps");
    if (tiles != json.MemberEnd() && tiles->value.IsObject()) {
        auto object = tiles->value.GetObject();
        for (auto characterIt = object.MemberBegin(); characterIt != object.MemberEnd(); ++characterIt) {
            if (characterIt->name.IsString() && characterIt->value.IsObject()) {
                auto tempId = characterIt->name.GetString();
                auto item = new sMapData();
                item->id = std::atoi(tempId);
                if (item->load(characterIt->value.GetObject())) {
                    mapsDb.insert({item->id, item});
                }
            } else {
                LOG_ERROR(STRING_FORMAT("mapsDatabase::load: file from path '%s' has errors!", getPath().c_str()));
            }
        }
    }
}

sMapData* mapsDatabase::getMapById(int id) {
    auto find = mapsDb.find(id);
    if (find != mapsDb.end()) {
        return find->second;
    }
    return nullptr;
}

bool sMapData::load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject& object) {
    if (object.HasMember("mapPath") && object["mapPath"].IsString()) {
        mapPath = object["mapPath"].GetString();
    } else {
        LOG_ERROR("sMapData::load: missing property 'mapPath'!");
        return false;
    }
    if (object.HasMember("wallProperty") && object["wallProperty"].IsString()) {
        wallProperty = object["wallProperty"].GetString();
    } else {
        LOG_ERROR("sMapData::load: missing property 'wallProperty'!");
        return false;
    }
    if (object.HasMember("spawnPlayerProperty") && object["spawnPlayerProperty"].IsString()) {
        spawnPlayerProperty = object["spawnPlayerProperty"].GetString();
    } else {
        LOG_ERROR("sMapData::load: missing property 'spawnPlayerProperty'!");
        return false;
    }
    if (object.HasMember("spawnEnemyProperty") && object["spawnEnemyProperty"].IsString()) {
        spawnEnemyProperty = object["spawnEnemyProperty"].GetString();
    } else {
        LOG_ERROR("sMapData::load: missing property 'spawnEnemyProperty'!");
        return false;
    }
    if (object.HasMember("floors") && object["floors"].IsArray()) {
        const auto& array = object["floors"].GetArray();
        for (const auto& item : array) {
            if (item.IsObject()) {
                auto layer = new sMapLayer();
                if (layer->load(item.GetObject())) {
                    floors.push_back(layer);
                }
            }
        }
    } else {
        LOG_ERROR("sMapData::load: missing property 'groups'!");
        return false;
    }
    return true;
}

bool sMapLayer::load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject& object) {
    if (object.HasMember("level") && object["level"].IsNumber()
        && object.HasMember("layers") && object["layers"].IsArray()) {
        level = object["level"].GetInt();
        for (const auto& item : object["layers"].GetArray()) {
            layers.emplace_back(item.GetString());
        }
        return true;
    }
    LOG_ERROR("sMapLayer::load: missing property 'level' or 'layers'!");
    return false;
}

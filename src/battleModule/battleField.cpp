#include "battleField.h"
#include "common/debugModule/logManager.h"
#include "common/utilityModule/stringUtility.h"
#include "databasesModule/databaseManager.h"
#include "databasesModule/mapsDatabase.h"
#include <random>
#include <algorithm>
#include <iterator>

using namespace mb::battleModule;

battleField::battleField() {}

battleField::~battleField() {
    spawnPlayerPositions.clear();
    spawnEnemyPositions.clear();
    delete tiledMap;
    tiledMap = nullptr;
    tileObjMap.clear();
}

void battleField::initLayer(int id) {
    using namespace databasesModule;
    // generate a maze
    auto mapsDb = GET_DATABASE_MANAGER().getDatabase<mapsDatabase>(databaseManager::eDatabaseList::MAPS_DB);
    auto map = mapsDb->getMapById(id);
    if (map == nullptr) {
        LOG_ERROR(STRING_FORMAT("battleField::initLayer: Can't find map by id - %d", id));
        return;
    }
    tiledMap = cocos2d::FastTMXTiledMap::create(map->mapPath);
    world->addChild(tiledMap);
//    collectObjectData(map);
    //insert collisions walls
//    insertWalls(map);
    findProperties(map);

    world->setContentSize(tiledMap->getContentSize());
    objects->setContentSize(tiledMap->getContentSize());
}

cocos2d::Vec2 battleField::getPlayerSpawnPosition() {
    if (!spawnPlayerPositions.empty()) {
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(spawnPlayerPositions.begin(), spawnPlayerPositions.end(), g);
        auto pos = spawnPlayerPositions.front();
        spawnPlayerPositions.erase(spawnPlayerPositions.begin());
        return pos;
    }

    LOG_ERROR(STRING_FORMAT("battleField::getPlayerSpawnPosition: Can't find next spawn positions"));
    return cocos2d::Vec2::ZERO;
}

cocos2d::Vec2 battleField::getEnemySpawnPosition() {
    if (!spawnEnemyPositions.empty()) {
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(spawnEnemyPositions.begin(), spawnEnemyPositions.end(), g);
        auto pos = spawnEnemyPositions.front();
        spawnEnemyPositions.erase(spawnEnemyPositions.begin());
        return pos;
    }

    LOG_ERROR(STRING_FORMAT("battleField::getEnemySpawnPosition: Can't find next spawn positions"));
    return cocos2d::Vec2::ZERO;
}

void battleField::insertWalls(databasesModule::sMapData*) {
    auto layer = tiledMap->getLayer("bg");
    if (layer) {
        const auto& size = layer->getLayerSize();
        for (auto x = 0; x < static_cast<int>(size.width); ++x) {
            for (auto y = 0; y < static_cast<int>(size.height); ++y) {
                auto gid = layer->getTileGIDAt({static_cast<float>(x), static_cast<float>(y)});
                auto piece = getPieceById(gid);
                if (piece.gid != battleFieldIncorrectValue) {
                    auto tile = layer->getTileAt({static_cast<float>(x), static_cast<float>(y)});
                    for (const auto& item : piece.objects) {
                        auto block = new cocos2d::Node();
                        block->setName(STRING_FORMAT("block_%d", gid));
                        block->setContentSize(item.size);
                        block->setPosition(tile->getPosition() + item.pos);
                        auto physics = cocos2d::PhysicsBody::createBox(item.size, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
                        physics->setCategoryBitmask(0x03);
                        physics->setCollisionBitmask(0x03);
                        physics->setGravityEnable(false);
                        physics->setDynamic(false);
                        physics->setRotationEnable(false);
                        physics->setMass(100.f);
                        physics->setMoment(0.f);
                        block->addComponent(physics);
                        objects->addChild(block);
                    }
                }
            }
        }
    }
}

void battleField::collectObjectData(databasesModule::sMapData* map) {
    tileObjMap.clear();
    if (!tiledMap) return;
    auto tileSize = tiledMap->getTileSize();
    auto data = tiledMap->getObjectGroups();
    for (const auto& item : data) {
        sBattleFieldPiece piece;
        for (const auto& obj : item->getObjects()) {
            sBattleFieldObject object;
            if (obj.getType() == cocos2d::Value::Type::MAP) {
                auto values = obj.asValueMap();
                for (const auto& value : values) {
                    if (value.first == "height" && value.second.getType() == cocos2d::Value::Type::FLOAT) {
                        object.size.height = value.second.asFloat();
                    } else if (value.first == "width" && value.second.getType() == cocos2d::Value::Type::FLOAT) {
                        object.size.width = value.second.asFloat();
                    } else if (value.first == "localX" && value.second.getType() == cocos2d::Value::Type::FLOAT) {
                        object.pos.x = value.second.asFloat();
                    } else if (value.first == "localY" && value.second.getType() == cocos2d::Value::Type::FLOAT) {
                        object.pos.y = value.second.asFloat();
                    }
                }
            }
            object.pos = convertPosition(tileSize, object.size, object.pos);
            piece.objects.push_back(object);
        }
        piece.gid = item->getTileGid();
        if (piece.gid != battleFieldIncorrectValue) {
            auto prop = tiledMap->getPropertiesForGID(piece.gid);
            if (prop.getType() == cocos2d::Value::Type::MAP) {
                auto val = prop.asValueMap();
                if (val.find(map->wallProperty) != val.end()) {
                    piece.isWall = val[map->wallProperty].asBool();
                }
            }
            tileObjMap.insert({ piece.gid, piece });
        }
    }
}

sBattleFieldPiece battleField::getPieceById(int id) {
    if (tileObjMap.count(id)) {
        return tileObjMap[id];
    }
    return sBattleFieldPiece();
}

cocos2d::Vec2 battleField::convertPosition(const cocos2d::Size& tileSize, const cocos2d::Size& shapeSize, const cocos2d::Vec2& shapePos) {
    auto y = tileSize.height - (shapePos.y + shapeSize.height);
    return cocos2d::Vec2(shapePos.x, y);
}

void battleField::findProperties(mb::databasesModule::sMapData* map) {
    for (const auto& floorList : map->floors) {
        for (const auto& layerName : floorList->layers) {
            auto layer = tiledMap->getLayer(layerName);
            if (layer) {
                const auto& size = layer->getLayerSize();
                for (auto x = 0; x < static_cast<int>(size.width); ++x) {
                    for (auto y = 0; y < static_cast<int>(size.height); ++y) {
                        auto gid = layer->getTileGIDAt({ static_cast<float>(x), static_cast<float>(y) });
                        auto prop = tiledMap->getPropertiesForGID(gid);
                        if (prop.getType() == cocos2d::Value::Type::MAP) {
                            auto val = prop.asValueMap();
                            if (val.find(map->spawnPlayerProperty) != val.end() && val[map->spawnPlayerProperty].asBool()) {
                                if (auto pos = layer->getTileAt({ static_cast<float>(x), static_cast<float>(y) }))
                                    spawnPlayerPositions.push_back(pos->getPosition());
                            } else if (val.find(map->spawnEnemyProperty) != val.end() && val[map->spawnEnemyProperty].asBool()) {
                                if (auto pos = layer->getTileAt({ static_cast<float>(x), static_cast<float>(y) }))
                                    spawnEnemyPositions.push_back(pos->getPosition());
                            }
                        }
                    }
                }
            } else {
                //todo log
                LOG_ERROR(STRING_FORMAT("battleField::findProperties: Can't find layer by name - %s", layerName.c_str()));
            }
        }
    }
}

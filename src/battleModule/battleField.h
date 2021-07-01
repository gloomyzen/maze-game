#ifndef SLAYER_ESCAPE_BATTLEFIELD_H
#define SLAYER_ESCAPE_BATTLEFIELD_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include <vector>

namespace mb::databasesModule {
    struct sMapData;
}
namespace mb::battleModule {

    static const int battleFieldIncorrectValue = -1;

    struct sBattleFieldObject {
        cocos2d::Size size;
        cocos2d::Vec2 pos;
    };

    struct sBattleFieldPiece {
        int gid = battleFieldIncorrectValue;
        std::vector<sBattleFieldObject> objects;
        bool isWall = false;
    };

    class battleField {
    public:
        battleField();
        ~battleField();

        void setWorldLayer(cocos2d::Node* layer) {
            world = layer;
        }
        void setObjectsLayer(cocos2d::Node* layer) {
            objects = layer;
        }
        cocos2d::Vec2 getPlayerSpawnPosition();
        cocos2d::Vec2 getEnemySpawnPosition();

        void initLayer(int);

    private:
        void findProperties(databasesModule::sMapData*);
        void collectObjectData(databasesModule::sMapData*);
        void insertWalls(databasesModule::sMapData*);
        cocos2d::Vec2 convertPosition(const cocos2d::Size& tileSize, const cocos2d::Size& shapeSize, const cocos2d::Vec2& shapePos);
        sBattleFieldPiece getPieceById(int);
        cocos2d::Node* world = nullptr;// not owner
        cocos2d::Node* objects = nullptr;// not owner
        std::vector<cocos2d::Vec2> spawnPlayerPositions;
        std::vector<cocos2d::Vec2> spawnEnemyPositions;
        cocos2d::FastTMXTiledMap* tiledMap = nullptr; //owner
        std::map<int, sBattleFieldPiece> tileObjMap;
    };
}// namespace mb::battleModule


#endif// SLAYER_ESCAPE_BATTLEFIELD_H

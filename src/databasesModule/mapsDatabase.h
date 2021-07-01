#ifndef SLAYER_ESCAPE_MAPSDATABASE_H
#define SLAYER_ESCAPE_MAPSDATABASE_H

#include "cocos2d.h"
#include "common/databaseModule/databaseInterface.h"
#include "common/utilityModule/jsonHelper.h"
#include <map>
#include <string>
#include <vector>

namespace mb::databasesModule {

    struct sMapLayer {
        int level;
        std::vector<std::string> layers;
        bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&);
    };

    struct sMapData {
        int id;
        std::string mapPath;
        std::string wallProperty;
        std::string spawnPlayerProperty;
        std::string spawnEnemyProperty;
        std::vector<sMapLayer*> floors;
        bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&);
    };

    class mapsDatabase : public common::databaseModule::databaseInterface {
      public:
        mapsDatabase();
        ~mapsDatabase();
        void load(const rapidjson::Document&) override;
        const std::map<int, sMapData*>& getMaps() { return mapsDb; }
        sMapData* getMapById(int);

      private:
        std::map<int, sMapData*> mapsDb;
    };
}// namespace mb::databasesModule


#endif// SLAYER_ESCAPE_MAPSDATABASE_H

#ifndef MERCENARY_BATTLES_DATABASEMANAGER_H
#define MERCENARY_BATTLES_DATABASEMANAGER_H

#include "common/databaseModule/databaseManagerInterface.h"
#include "common/databaseModule/databaseInterface.h"
#include <string>
#include <map>

#define GET_DATABASE_MANAGER() mb::databasesModule::databaseManager::getInstance()

namespace mb::databasesModule {
    class databaseManager : public common::databaseModule::databaseManagerInterface {
      public:
        enum class eDatabaseList {
            CHARACTER_DB = 0,
            TILES_DB,
            MAPS_DB,
        };
        databaseManager();
        ~databaseManager() override;

        static databaseManager& getInstance();
        void cleanup() override;

        void addDatabase(eDatabaseList id, std::string value, common::databaseModule::databaseInterface* db);

        template<typename T>
        T* getDatabase(eDatabaseList key) {
            return getRegisteredDatabase<T>(static_cast<int>(key));
        }
    };
}


#endif// MERCENARY_BATTLES_DATABASEMANAGER_H

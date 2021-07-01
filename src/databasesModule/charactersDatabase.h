#ifndef MERCENARY_BATTLES_CHARACTERSDATABASE_H
#define MERCENARY_BATTLES_CHARACTERSDATABASE_H

#include "cocos2d.h"
#include "common/databaseModule/databaseInterface.h"
#include "common/utilityModule/jsonHelper.h"
#include <map>
#include <string>
#include <vector>
#include <tuple>

namespace mb::databasesModule {

    struct sCharacterStats {
        float hp;
        std::pair<float, float> attack; //attack min and max
        float speed = 1.0f; //movement speed
        float attackSpeed = 1.0f;

        bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&);
    };

    struct sCharacterData {
        int id;
        std::string propertyPath;
        std::string iconPatch;
        sCharacterStats* stats = nullptr;
        bool isSpine = false;
        bool isBoss = false;
        bool hasCollision = true;
        bool flipX = false;
        bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&);
    };

    class charactersDatabase : public common::databaseModule::databaseInterface {
      public:
        charactersDatabase();
        ~charactersDatabase();
        void load(const rapidjson::Document&) override;
        const std::map<int, sCharacterData*>& getCharacters() { return charactersDb; }
        sCharacterData* getCharacterById(int);

      private:
        std::map<int, sCharacterData*> charactersDb;
    };
}// namespace mb::databasesModule


#endif// MERCENARY_BATTLES_CHARACTERSDATABASE_H

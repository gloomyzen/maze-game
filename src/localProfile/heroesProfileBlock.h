#ifndef MERCENARY_BATTLES_HEROESPROFILEBLOCK_H
#define MERCENARY_BATTLES_HEROESPROFILEBLOCK_H

#include "common/profileModule/profileBlockInterface.h"
#include "common/utilityModule/jsonHelper.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace mb::localProfile {

    class heroesProfileBlock : public common::profileModule::profileBlockInterface {
      public:
        heroesProfileBlock();
        ~heroesProfileBlock();

        bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&) override;
        bool save(rapidjson::Value&, rapidjson::Document::AllocatorType&) override;

        std::map<int, int>& getAllHeroes() { return localHeroes; }
        bool hasHero(int id);
        bool addHero(int id);
        bool removeHero(int id);


      private:
        std::map<int, int> localHeroes;
    };
}// namespace mb::localProfile


#endif// MERCENARY_BATTLES_HEROESPROFILEBLOCK_H

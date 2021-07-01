#include "cocos2d.h"
#include "databasesModule/databaseManager.h"
#include "databasesModule/charactersDatabase.h"
#include "databasesModule/buildingsDatabase.h"
#include "databasesModule/databaseTool.h"
#include "common/databaseModule/databaseManagerInterface.h"
#include "common/utilityModule/stringUtility.h"
#include <gtest/gtest.h>

using namespace common::utilityModule;
using namespace common::databaseModule;
//using namespace mb::databasesModule;

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

namespace DBTests::test {
    class TempClass : public testing::Test {};
}// namespace DBTests::test
using namespace DBTests::test;

TEST_F(TempClass, cocos2dSimpleIntegrationTest) {
    auto size1 = cocos2d::Size(1.f, 2.f);
    auto size2 = cocos2d::Size(2.f, 1.f);
    EXPECT_EQ(size1.width, size2.height);
    EXPECT_TRUE(size1.width != size2.width);
}

TEST_F(TempClass, commonStringUtilsTest) {
    EXPECT_EQ(utilityModule::capitalizeString("test room is awesome!"), std::string("Test room is awesome!"));
    EXPECT_EQ(utilityModule::toLowerString("Test RoOm IS aweSome!"), std::string("test room is awesome!"));
    auto stringArray = utilityModule::explodeString("test room is awesome!");
    EXPECT_TRUE(!stringArray.empty());
    EXPECT_TRUE(stringArray.size() == 4);
    auto stringArray1 = utilityModule::explodeString("test room is awesome!", &utilityModule::capitalizeString);
    EXPECT_TRUE(!stringArray1.empty());
    EXPECT_EQ(stringArray1.front(),  std::string("Test"));
    auto stringArray2 = utilityModule::explodeString("TEST ROOM IS AWESOME!", &utilityModule::toLowerString);
    EXPECT_TRUE(!stringArray2.empty());
    EXPECT_EQ(stringArray2.front(),  std::string("test"));
}

TEST_F(TempClass, databaseCharacterTest) {
    using namespace mb::databasesModule;

    GET_DATABASE_MANAGER().addDatabase(databaseManager::eDatabaseList::CHARACTER_DB, "properties/database/characters/db.json", new charactersDatabase());
    GET_DATABASE_MANAGER().executeLoadData();
    auto characterDb = GET_DATABASE_MANAGER().getDatabase<charactersDatabase>(databaseManager::eDatabaseList::CHARACTER_DB);
    EXPECT_TRUE(characterDb->isLoaded());
    EXPECT_TRUE(!characterDb->getCharacters().empty());
    auto characterId = 20001;
    auto character = characterDb->getCharacterById(characterId);
    EXPECT_TRUE(character);
    EXPECT_EQ(character->id, characterId);
    EXPECT_TRUE(!character->bonesString.empty());
    EXPECT_TRUE(!character->iconPatch.empty());

    EXPECT_TRUE(character->stats);
    EXPECT_TRUE(character->stats->attack.first != 0.f);
    EXPECT_TRUE(character->stats->attack.second != 0.f);
    EXPECT_TRUE(character->stats->hp != 0.f);
    EXPECT_TRUE(character->stats->speed != 0.f);
    EXPECT_TRUE(character->stats->attackSpeed != 0.f);
}

TEST_F(TempClass, databaseBuildsTest) {
    using namespace mb::databasesModule;

    GET_DATABASE_MANAGER().addDatabase(databaseManager::eDatabaseList::BUILDING_DB, "properties/database/buildings/db.json", new buildingsDatabase());
    GET_DATABASE_MANAGER().executeLoadData();
    auto buildsDb = GET_DATABASE_MANAGER().getDatabase<buildingsDatabase>(databaseManager::eDatabaseList::BUILDING_DB);
    EXPECT_TRUE(buildsDb->isLoaded());
    EXPECT_TRUE(!buildsDb->getBuildings().empty());
    auto buildID = 30001;
    auto build = buildsDb->getBuildById(buildID);
    EXPECT_TRUE(build);
    EXPECT_EQ(build->id, buildID);
    EXPECT_TRUE(build->level != 0);
    EXPECT_TRUE(build->craftTime != 0);
    EXPECT_TRUE(build->incomeA != 0);
    EXPECT_TRUE(build->incomeB != 0);
    EXPECT_TRUE(build->unitIdA != 0);
    EXPECT_TRUE(build->unitIdB != 0);
    EXPECT_TRUE(!build->propertyPath.empty());
}

TEST_F(TempClass, databaseToolTest) {
    using namespace mb::databasesModule;

    auto buildID = 30001;
    auto builds = databaseTool::getCharacterByBuilding(buildID);
    EXPECT_TRUE(builds.first);
    EXPECT_TRUE(builds.second);
}
#include "databaseManager.h"

using namespace mb::databasesModule;

static databaseManager* databaseManagerInstance = nullptr;

databaseManager::databaseManager() {}

databaseManager::~databaseManager() {}

databaseManager& databaseManager::getInstance() {
    if (databaseManagerInstance == nullptr) {
        databaseManagerInstance = new databaseManager();
    }
    return *databaseManagerInstance;
}

void databaseManager::cleanup() {
    for (auto& [_, db] : databasesMap) {
        delete db;
        db = nullptr;
    }
}

void databaseManager::addDatabase(databaseManager::eDatabaseList id, std::string value, common::databaseModule::databaseInterface* db) {
    auto type = static_cast<int>(id);
    registerDatabase({type, value}, db);
}

#include "heroesProfileBlock.h"

using namespace mb::localProfile;
using namespace rapidjson;

heroesProfileBlock::heroesProfileBlock() {}

heroesProfileBlock::~heroesProfileBlock() {
    localHeroes.clear();
}

bool heroesProfileBlock::load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject& data) {
    auto heroesArray = data.FindMember("ids");
    if (!heroesArray->value.IsArray()) {
        return true;
    }
    auto heroes = heroesArray->value.GetArray();
    int id = 0;
    for (auto it = heroes.Begin(); it != heroes.End(); ++it) {
        if (it->IsNumber()) {
            localHeroes.insert({ id++, it->GetInt() });
        }
    }
    return true;
}

bool heroesProfileBlock::save(rapidjson::Value& data, rapidjson::MemoryPoolAllocator<>& allocator) {
    rapidjson::Value array;
    array.SetArray();
    for (auto item : localHeroes) array.PushBack(item.second, allocator);
    rapidjson::Value key("ids", allocator);
    data.AddMember(key, array, allocator);
    return true;
}

bool heroesProfileBlock::hasHero(int id) {
    auto find = std::find_if(localHeroes.begin(), localHeroes.end(), [id](std::pair<int, int> n){
        return n.second == id;
    });
    return find != localHeroes.end();
}

bool heroesProfileBlock::addHero(int id) {
    if (hasHero(id)) return false;
    localHeroes.insert({localHeroes.size(), id});
    return true;
}

bool heroesProfileBlock::removeHero(int id) {
    if (!hasHero(id)) return false;
    auto find = std::find_if(localHeroes.begin(), localHeroes.end(), [id](std::pair<int, int> n){
           return n.second == id;
    });
    localHeroes.erase(find);
    return true;
}

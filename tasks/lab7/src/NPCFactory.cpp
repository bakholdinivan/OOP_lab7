#include "NPCFactory.h"
#include <random>

std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, const std::string& name, const Point& pos) {
    if (type == "Жаба" || type == "Frog") {
        return std::make_shared<Frog>(name, pos);
    } else if (type == "Бык" || type == "Bull") {
        return std::make_shared<Bull>(name, pos);
    } else if (type == "Дракон" || type == "Dragon") {
        return std::make_shared<Dragon>(name, pos);
    }
    return nullptr;
}

std::shared_ptr<NPC> NPCFactory::createRandomNPC(const std::string& name, double mapWidth, double mapHeight) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> disX(0.0, mapWidth);
    std::uniform_real_distribution<> disY(0.0, mapHeight);
    std::uniform_int_distribution<> disType(0, 2);

    Point pos(disX(gen), disY(gen));
    int typeChoice = disType(gen);

    switch (typeChoice) {
        case 0: return std::make_shared<Frog>(name, pos);
        case 1: return std::make_shared<Bull>(name, pos);
        case 2: return std::make_shared<Dragon>(name, pos);
        default: return std::make_shared<Frog>(name, pos);
    }
}

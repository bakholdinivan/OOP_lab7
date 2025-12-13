#pragma once
#include "NPCTypes.h"
#include <memory>
#include <string>

class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(const std::string& type, const std::string& name, const Point& pos);
    static std::shared_ptr<NPC> createRandomNPC(const std::string& name, double mapWidth, double mapHeight);
};

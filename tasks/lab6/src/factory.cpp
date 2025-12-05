#include <memory>
#include <iostream>
#include "../include/factory.h"
#include "../include/creatures.h"
#include <sstream>
#include <stdexcept>

std::unique_ptr<Npc> NpcFactory::createNpc(
    const std::string& type,
    const std::string& name,
    int x,
    int y)
{
    if (type == "Dragon") {
        return std::make_unique<Dragon>(x, y, name);
    } else if (type == "Bull") {
        return std::make_unique<Bull>(x, y, name);
    } else if (type == "Frog") {
        return std::make_unique<Frog>(x, y, name);
    } else {
        throw std::invalid_argument("Unknown NPC type: " + type);
    }   
}

std::unique_ptr<Npc> NpcFactory::createFromString(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name;
    int x, y;

    iss >> type >> name >> x >> y;
    
    if (iss.fail()) {
        throw std::runtime_error("Failed to read line: " + line);
    }

    return createNpc(type, name, x, y);
}
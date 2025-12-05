#include "NPCFactory.h"
#include "Dragon.h"
#include "Bull.h"
#include "Frog.h"
#include <sstream>

std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, 
                                           const std::string& name, 
                                           double x, double y) {
    if (type == "Dragon") {
        return std::make_shared<Dragon>(name, x, y);
    } else if (type == "Bull") {
        return std::make_shared<Bull>(name, x, y);
    } else if (type == "Frog") {
        return std::make_shared<Frog>(name, x, y);
    }
    return nullptr;
}

std::shared_ptr<NPC> NPCFactory::loadFromString(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name;
    double x, y;
    
    // Формат: Type Name X Y
    if (iss >> type >> name >> x >> y) {
        return createNPC(type, name, x, y);
    }
    
    return nullptr;
}

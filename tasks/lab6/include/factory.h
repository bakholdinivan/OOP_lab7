#ifndef FACTORY_H
#define FACTORY_H

#include "npc.h"
#include <memory>
#include <string>

class NpcFactory {
public:
    static std::unique_ptr<Npc> createNpc(
        const std::string& type,
        const std::string& name,
        int x,
        int y
    );
    
    static std::unique_ptr<Npc> createFromString(const std::string& line);
};

#endif // FACTORY_H
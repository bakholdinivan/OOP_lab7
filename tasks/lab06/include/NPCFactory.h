#pragma once
#include <memory>
#include <string>
#include "NPC.h"

class NPCFactory {
public:
    // Создание NPC по типу
    static std::shared_ptr<NPC> createNPC(const std::string& type, 
                                          const std::string& name, 
                                          double x, double y);
    
    // Загрузка NPC из строки файла
    static std::shared_ptr<NPC> loadFromString(const std::string& line);
};

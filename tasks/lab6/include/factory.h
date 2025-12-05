#ifndef FACTORY_H
#define FACTORY_H

#include "npc.h"
#include <memory>
#include <string>

/**
 * Фабрика персонажей - реализация паттерна Factory
 * Создает объекты NPC по типу и параметрам
 */
class NpcFactory {
public:
    // Создать персонажа по типу, имени и координатам
    static std::unique_ptr<Npc> createNpc(
        const std::string& type,
        const std::string& name,
        int x,
        int y
    );
    
    // Создать персонажа из строки файла (для загрузки)
    static std::unique_ptr<Npc> createFromString(const std::string& line);
};

#endif // FACTORY_H
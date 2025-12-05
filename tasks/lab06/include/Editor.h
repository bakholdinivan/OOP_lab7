#pragma once
#include <vector>
#include <memory>
#include <string>
#include "NPC.h"
#include "BattleVisitor.h"

class Editor {
private:
    std::vector<std::shared_ptr<NPC>> npcs;  // Список всех NPC
    
public:
    // Добавить NPC на карту
    bool addNPC(std::shared_ptr<NPC> npc);
    
    // Проверка уникальности имени
    bool isNameUnique(const std::string& name) const;
    
    // Сохранение в файл
    bool saveToFile(const std::string& filename) const;
    
    // Загрузка из файла
    bool loadFromFile(const std::string& filename);
    
    // Печать всех NPC
    void printAll() const;
    
    // Запуск боевого режима
    void startBattle(double range, BattleVisitor& visitor);
    
    // Удаление мертвых NPC
    void removeDeadNPCs();
    
    // Получить количество NPC
    size_t getNPCCount() const { return npcs.size(); }
    
    // Очистить всех NPC
    void clear() { npcs.clear(); }
    
    // Получить NPC по индексу
    std::shared_ptr<NPC> getNPC(size_t index) const;
};

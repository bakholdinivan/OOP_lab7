#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "character.h"
#include "patterns.h"
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>

/**
 * Игровой мир - контейнер для всех NPC
 * Управляет персонажами, боями и наблюдателями
 */
class GameWorld {
private:
    static constexpr int WORLD_SIZE = 500; // Размер мира 500x500 метров
    std::map<std::string, std::unique_ptr<Character>> npcs_;  // Все NPC по именам
    std::vector<std::shared_ptr<Observer>> observers_;  // Наблюдатели за событиями
    
public:
    // Добавить NPC в игровой мир
    void addNpc(std::unique_ptr<Character> npc);
    
    // Показать всех NPC
    void printAll() const;
    
    // Сохранение в файл
    void saveToFile(const std::string& filename) const;
    
    // Загрузка из файла
    void loadFromFile(const std::string& filename);
    
    // Очистить мир от всех NPC
    void clear();
    
    // Начать бой в пределах заданной дистанции
    void startBattle(double maxDistance);
    
    // Добавить наблюдателя (Observer pattern)
    void addObserver(std::shared_ptr<Observer> observer);
    
    // Получить количество NPC
    size_t getNpcCount() const;
    
private:
    // Уведомить всех наблюдателей о событии
    void notifyAllObservers(const std::string& message);
};

#endif // GAME_WORLD_H

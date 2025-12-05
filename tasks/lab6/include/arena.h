#ifndef ARENA_H
#define ARENA_H

#include "npc.h"
#include "observer.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * Арена - игровое поле для размещения персонажей и проведения боев
 * Управляет всеми NPC, сохранением/загрузкой и оповещением наблюдателей
 */
class Arena {
private:
    static constexpr int MAX_WIDTH = 500;   // Максимальная ширина арены
    static constexpr int MAX_HEIGHT = 500;  // Максимальная высота арены
    
    int width_;   // Текущая ширина
    int height_;  // Текущая высота
    std::map<std::string, std::unique_ptr<Npc>> npcs_;  // Карта персонажей (имя -> объект)
    std::vector<std::shared_ptr<Observer>> observers_;  // Список наблюдателей (паттерн Observer)

public:
    Arena(int width = MAX_WIDTH, int height = MAX_HEIGHT);
    
    // Управление персонажами
    void addNpc(std::unique_ptr<Npc> npc);
    void createAndAddNpc(const std::string& type, 
                         const std::string& name, 
                         int x, int y);
    
    // Вывод информации
    void printAllNpcs() const;
    size_t getNpcCount() const;
    
    // Работа с файлами
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    // Очистка арены
    void clear();
    
    // Паттерн Observer - управление наблюдателями
    void addObserver(std::shared_ptr<Observer> observer);
    void removeObserver(std::shared_ptr<Observer> observer);
    void notifyObservers(const std::string& event);
    
    // Запуск боевого режима
    void startBattle(double range);
};

#endif // ARENA_H
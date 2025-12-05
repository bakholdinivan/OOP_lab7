#ifndef PATTERNS_H
#define PATTERNS_H

#include "character.h"
#include <memory>
#include <string>
#include <fstream>
#include <sstream>

// ============= ПАТТЕРН FACTORY =============
// Фабрика для создания NPC разных типов

class NpcFactory {
public:
    // Создание NPC по типу
    static std::unique_ptr<Character> createNpc(const std::string& type, 
                                                const std::string& name, 
                                                int x, int y);
    
    // Загрузка NPC из строки файла
    static std::unique_ptr<Character> loadFromFile(const std::string& line);
};

// ============= ПАТТЕРН VISITOR =============
// Визитор для проведения боев между NPC

class BattleVisitor {
public:
    // Проверка: может ли атакующий типа победить цель типа
    static bool canDefeat(const std::string& attackerType, const std::string& targetType);
};

// ============= ПАТТЕРН OBSERVER =============
// Наблюдатель за событиями боя

class Observer {
public:
    virtual ~Observer() = default;
    virtual void notify(const std::string& message) = 0;
};

// Наблюдатель для вывода в консоль
class ConsoleObserver : public Observer {
public:
    void notify(const std::string& message) override;
};

// Наблюдатель для записи в файл log.txt
class FileObserver : public Observer {
private:
    std::string filename_;
    
public:
    FileObserver(const std::string& filename);
    void notify(const std::string& message) override;
};

#endif // PATTERNS_H

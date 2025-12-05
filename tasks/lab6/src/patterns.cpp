#include "../include/patterns.h"
#include <sstream>

// ============= РЕАЛИЗАЦИЯ FACTORY =============

std::unique_ptr<Character> NpcFactory::createNpc(const std::string& type, 
                                                  const std::string& name, 
                                                  int x, int y) {
    if (type == "Dragon") {
        return std::make_unique<Dragon>(name, x, y);
    } else if (type == "Bull") {
        return std::make_unique<Bull>(name, x, y);
    } else if (type == "Frog") {
        return std::make_unique<Frog>(name, x, y);
    }
    throw std::invalid_argument("Неизвестный тип NPC: " + type);
}

std::unique_ptr<Character> NpcFactory::loadFromFile(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name;
    int x, y;
    
    if (!(iss >> type >> name >> x >> y)) {
        throw std::runtime_error("Неверный формат строки");
    }
    
    return createNpc(type, name, x, y);
}

// ============= РЕАЛИЗАЦИЯ VISITOR =============

bool BattleVisitor::canDefeat(const std::string& attackerType, const std::string& targetType) {
    // Дракон ест быков
    if (attackerType == "Dragon" && targetType == "Bull") return true;
    
    // Бык топчет лягушек
    if (attackerType == "Bull" && targetType == "Frog") return true;
    
    // Лягушки спасаются как могут (никого не побеждают)
    return false;
}

// ============= РЕАЛИЗАЦИЯ OBSERVER =============

void ConsoleObserver::notify(const std::string& message) {
    std::cout << "[СОБЫТИЕ] " << message << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : filename_(filename) {}

void FileObserver::notify(const std::string& message) {
    std::ofstream file(filename_, std::ios::app);
    if (file.is_open()) {
        file << "[ЛОГ] " << message << std::endl;
    }
}

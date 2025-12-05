#include "../include/game_world.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <set>

// Добавить NPC с проверками
void GameWorld::addNpc(std::unique_ptr<Character> npc) {
    std::string name = npc->getName();
    double x = npc->getX();
    double y = npc->getY();
    
    // Проверка координат (0-500 метров)
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::out_of_range("Координаты NPC должны быть в диапазоне [0, 500]!");
    }
    
    // Проверка уникальности имени
    if (npcs_.count(name) > 0) {
        throw std::invalid_argument("NPC с именем '" + name + "' уже существует!");
    }
    
    npcs_[name] = std::move(npc);
    notifyAllObservers("Добавлен NPC: " + name);
}

// Показать всех NPC
void GameWorld::printAll() const {
    std::cout << "\n=== Персонажи в мире ===\n";
    for (const auto& [name, npc] : npcs_) {
        npc->print();
    }
    std::cout << "Всего: " << npcs_.size() << " персонажей\n";
}

// Сохранить в файл
void GameWorld::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи!");
    }
    
    for (const auto& [name, npc] : npcs_) {
        file << npc->getTypeName() << " " 
             << npc->getName() << " "
             << npc->getX() << " " 
             << npc->getY() << "\n";
    }
}

// Загрузить из файла
void GameWorld::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения!");
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto npc = NpcFactory::loadFromFile(line);
        addNpc(std::move(npc));
    }
}

// Очистить мир
void GameWorld::clear() {
    npcs_.clear();
}

// Начать бой
void GameWorld::startBattle(double maxDistance) {
    std::set<std::string> toRemove; // Используем set для уникальности
    
    // Создаем список пар для боя
    std::vector<std::pair<std::string, std::string>> battles;
    
    for (auto& [name1, npc1] : npcs_) {
        for (auto& [name2, npc2] : npcs_) {
            if (name1 >= name2) continue; // Избегаем дублей и самоатаки
            
            double distance = npc1->calculateDistance(*npc2);
            if (distance <= maxDistance) {
                battles.push_back({name1, name2});
            }
        }
    }
    
    // Проводим бои
    for (const auto& [name1, name2] : battles) {
        // Проверяем, что оба NPC еще живы
        if (toRemove.count(name1) || toRemove.count(name2)) continue;
        
        auto& npc1 = npcs_[name1];
        auto& npc2 = npcs_[name2];
        
        double distance = npc1->calculateDistance(*npc2);
        
        // Используем Visitor pattern для определения боя
        bool npc1_wins = BattleVisitor::canDefeat(npc1->getTypeName(), npc2->getTypeName());
        bool npc2_wins = BattleVisitor::canDefeat(npc2->getTypeName(), npc1->getTypeName());
        
        if (npc1_wins && npc2_wins) {
            // Оба могут убить друг друга - взаимное уничтожение
            std::string message = npc1->getName() + " и " + npc2->getName() + 
                                " убивают друг друга на расстоянии " + 
                                std::to_string(distance) + " м";
            notifyAllObservers(message);
            toRemove.insert(name1);
            toRemove.insert(name2);
        } else if (npc1_wins) {
            // Первый побеждает
            std::string message = npc1->getName() + " убивает " + npc2->getName() + 
                                " на расстоянии " + std::to_string(distance) + " м";
            notifyAllObservers(message);
            toRemove.insert(name2);
        } else if (npc2_wins) {
            // Второй побеждает
            std::string message = npc2->getName() + " убивает " + npc1->getName() + 
                                " на расстоянии " + std::to_string(distance) + " м";
            notifyAllObservers(message);
            toRemove.insert(name1);
        }
        // Иначе ничья - никто не погибает
    }
    
    // Удаляем убитых
    for (const auto& name : toRemove) {
        npcs_.erase(name);
    }
}

// Добавить наблюдателя
void GameWorld::addObserver(std::shared_ptr<Observer> observer) {
    observers_.push_back(observer);
}

// Получить количество NPC
size_t GameWorld::getNpcCount() const {
    return npcs_.size();
}

// Уведомить всех наблюдателей
void GameWorld::notifyAllObservers(const std::string& message) {
    for (auto& observer : observers_) {
        observer->notify(message);
    }
}

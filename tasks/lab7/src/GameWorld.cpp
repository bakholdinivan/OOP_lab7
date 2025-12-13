#include "GameWorld.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

GameWorld::GameWorld(double width, double height) 
    : map_width(width), map_height(height) {}

void GameWorld::addNPC(std::shared_ptr<NPC> npc) {
    std::unique_lock<std::shared_mutex> lock(npcs_mutex);
    npcs.push_back(npc);
}

void GameWorld::addObserver(ObserverPtr observer) {
    battle_visitor.addObserver(observer);
}

// Поток перемещения NPC
void GameWorld::movementThread() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> angle_dis(0.0, 2 * M_PI);

    while (game_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::shared_lock<std::shared_mutex> read_lock(npcs_mutex);
        std::vector<std::shared_ptr<NPC>> npcs_copy = npcs;
        read_lock.unlock();

        // Перемещаем каждый живой NPC
        for (auto& npc : npcs_copy) {
            if (!npc->isAlive()) continue;

            double angle = angle_dis(gen);
            double distance = npc->getMoveDistance();
            Point current = npc->getPosition();
            Point new_pos(
                current.x + distance * std::cos(angle),
                current.y + distance * std::sin(angle)
            );

            // Проверяем границы карты
            if (new_pos.x < 0) new_pos.x = 0;
            if (new_pos.x > map_width) new_pos.x = map_width;
            if (new_pos.y < 0) new_pos.y = 0;
            if (new_pos.y > map_height) new_pos.y = map_height;

            npc->setPosition(new_pos);
        }

        // Проверяем расстояния для боев
        for (size_t i = 0; i < npcs_copy.size(); ++i) {
            for (size_t j = i + 1; j < npcs_copy.size(); ++j) {
                auto& npc1 = npcs_copy[i];
                auto& npc2 = npcs_copy[j];

                if (!npc1->isAlive() || !npc2->isAlive()) continue;

                double distance = npc1->getPosition().distanceTo(npc2->getPosition());

                // Если первый может убить второго и они в зоне досягаемости
                if (npc1->canKill(npc2->getType()) && distance <= npc1->getKillDistance()) {
                    std::lock_guard<std::mutex> lock(battle_queue_mutex);
                    battle_queue.push({npc1, npc2});
                }

                // Если второй может убить первого и они в зоне досягаемости
                if (npc2->canKill(npc1->getType()) && distance <= npc2->getKillDistance()) {
                    std::lock_guard<std::mutex> lock(battle_queue_mutex);
                    battle_queue.push({npc2, npc1});
                }
            }
        }
    }
}

// Поток обработки боев
void GameWorld::battleThread() {
    while (game_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::unique_lock<std::mutex> lock(battle_queue_mutex);
        if (battle_queue.empty()) {
            lock.unlock();
            continue;
        }

        BattleTask task = battle_queue.front();
        battle_queue.pop();
        lock.unlock();

        // Обрабатываем бой
        if (task.attacker && task.defender) {
            task.attacker->accept(battle_visitor, *task.defender);
            
            // Проверяем взаимное уничтожение драконов
            if (task.attacker->getType() == "Дракон" && 
                task.defender->getType() == "Дракон" && 
                !task.defender->isAlive()) {
                task.attacker->kill();
                std::ostringstream oss;
                oss << "Взаимное уничтожение драконов: " << task.attacker->getName() 
                    << " также погибает!";
                battle_visitor.notifyObservers(oss.str());
            }
        }
    }
}

// Поток печати карты
void GameWorld::printThread() {
    int second_counter = 0;
    while (game_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        second_counter++;

        {
            std::lock_guard<std::mutex> console_lock(cout_mutex);
            std::cout << "\n=== Карта (секунда " << second_counter << ") ===" << std::endl;
            printMap();
        }
    }
}

void GameWorld::printMap() const {
    std::shared_lock<std::shared_mutex> lock(npcs_mutex);
    
    int alive_count = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            Point pos = npc->getPosition();
            std::cout << npc->getType() << " \"" << npc->getName() 
                      << "\" - позиция (" << std::fixed << std::setprecision(1) 
                      << pos.x << ", " << pos.y << ")" << std::endl;
            alive_count++;
        }
    }
    std::cout << "Живых NPC: " << alive_count << std::endl;
}

void GameWorld::stop() {
    game_running = false;
}

std::vector<std::shared_ptr<NPC>> GameWorld::getSurvivors() const {
    std::shared_lock<std::shared_mutex> lock(npcs_mutex);
    std::vector<std::shared_ptr<NPC>> survivors;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            survivors.push_back(npc);
        }
    }
    return survivors;
}

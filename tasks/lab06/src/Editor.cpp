#include "Editor.h"
#include "NPCFactory.h"
#include <iostream>
#include <fstream>
#include <algorithm>

bool Editor::addNPC(std::shared_ptr<NPC> npc) {
    // Проверка координат
    if (npc->getX() < 0 || npc->getX() > 500 || 
        npc->getY() < 0 || npc->getY() > 500) {
        return false;
    }
    
    // Проверка уникальности имени
    if (!isNameUnique(npc->getName())) {
        return false;
    }
    
    npcs.push_back(npc);
    return true;
}

bool Editor::isNameUnique(const std::string& name) const {
    for (const auto& npc : npcs) {
        if (npc->getName() == name) {
            return false;
        }
    }
    return true;
}

bool Editor::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            file << npc->getType() << " " 
                 << npc->getName() << " " 
                 << npc->getX() << " " 
                 << npc->getY() << std::endl;
        }
    }
    
    file.close();
    return true;
}

bool Editor::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    npcs.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        auto npc = NPCFactory::loadFromString(line);
        if (npc) {
            npcs.push_back(npc);
        }
    }
    
    file.close();
    return true;
}

void Editor::printAll() const {
    if (npcs.empty()) {
        std::cout << "В подземелье нет NPC." << std::endl;
        return;
    }
    std::cout << "\n=== NPC в подземелье ===" << std::endl;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::cout << npc->toString() << std::endl;
        }
    }
    std::cout << "Всего живых: " << 
        std::count_if(npcs.begin(), npcs.end(), 
                     [](const auto& n) { return n->isAlive(); }) << std::endl;
}

void Editor::startBattle(double range, BattleVisitor& visitor) {
    // Проход по всем парам NPC
    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (npcs[i]->isAlive() && npcs[j]->isAlive()) {
                // Проверка дистанции
                if (npcs[i]->distanceTo(*npcs[j]) <= range) {
                    npcs[i]->accept(visitor, *npcs[j]);
                }
            }
        }
    }
}

void Editor::removeDeadNPCs() {
    npcs.erase(
        std::remove_if(npcs.begin(), npcs.end(),
                      [](const auto& npc) { return !npc->isAlive(); }),
        npcs.end()
    );
}

std::shared_ptr<NPC> Editor::getNPC(size_t index) const {
    if (index < npcs.size()) {
        return npcs[index];
    }
    return nullptr;
}

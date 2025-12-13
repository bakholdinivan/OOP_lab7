#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <atomic>
#include "NPC.h"
#include "BattleVisitor.h"

struct BattleTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class GameWorld {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    mutable std::shared_mutex npcs_mutex;
    
    std::queue<BattleTask> battle_queue;
    std::mutex battle_queue_mutex;
    
    std::atomic<bool> game_running{true};
    
    double map_width;
    double map_height;
    
    BattleVisitor battle_visitor;
    
    mutable std::mutex cout_mutex;  // Мьютекс для защиты std::cout

public:
    GameWorld(double width, double height);
    
    void addNPC(std::shared_ptr<NPC> npc);
    void addObserver(ObserverPtr observer);
    
    // Поток перемещения NPC
    void movementThread();
    
    // Поток обработки боев
    void battleThread();
    
    // Поток печати карты
    void printThread();
    
    void stop();
    
    std::vector<std::shared_ptr<NPC>> getSurvivors() const;
    
    void printMap() const;
};

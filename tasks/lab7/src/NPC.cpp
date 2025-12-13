#include "NPC.h"
#include "BattleVisitor.h"
#include <iostream>

NPC::NPC(const std::string& name, const Point& pos) 
    : name(name), position(pos), alive(true) {}

std::string NPC::getName() const {
    std::lock_guard<std::mutex> lock(npc_mutex);
    return name;
}

Point NPC::getPosition() const {
    std::lock_guard<std::mutex> lock(npc_mutex);
    return position;
}

bool NPC::isAlive() const {
    std::lock_guard<std::mutex> lock(npc_mutex);
    return alive;
}

void NPC::setPosition(const Point& pos) {
    std::lock_guard<std::mutex> lock(npc_mutex);
    position = pos;
}

void NPC::kill() {
    std::lock_guard<std::mutex> lock(npc_mutex);
    alive = false;
}

int NPC::rollDice() const {
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(1, 6);
    return dis(gen);
}

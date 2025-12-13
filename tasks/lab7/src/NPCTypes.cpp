#include "NPCTypes.h"
#include "BattleVisitor.h"

// Frog 
Frog::Frog(const std::string& name, const Point& pos) : NPC(name, pos) {}

std::string Frog::getType() const { return "Жаба"; }

int Frog::getMoveDistance() const { return 1; }

int Frog::getKillDistance() const { return 10; }

bool Frog::canKill(const std::string& targetType) const {
    //  Жаба убивает Быка
    return targetType == "Бык";
}

void Frog::accept(BattleVisitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

// Bull 
Bull::Bull(const std::string& name, const Point& pos) : NPC(name, pos) {}

std::string Bull::getType() const { return "Бык"; }

int Bull::getMoveDistance() const { return 30; }

int Bull::getKillDistance() const { return 10; }

bool Bull::canKill(const std::string& targetType) const {
    //  Бык убивает Жабу
    return targetType == "Жаба";
}

void Bull::accept(BattleVisitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

// Dragon 
Dragon::Dragon(const std::string& name, const Point& pos) : NPC(name, pos) {}

std::string Dragon::getType() const { return "Дракон"; }

int Dragon::getMoveDistance() const { return 50; }

int Dragon::getKillDistance() const { return 30; }

bool Dragon::canKill(const std::string& targetType) const {
    // Дракон убивает Быка, но взаимно погибает с другим Драконом
    return targetType == "Бык" || targetType == "Дракон";
}

void Dragon::accept(BattleVisitor& visitor, NPC& other) {
    visitor.visit(*this, other);
}

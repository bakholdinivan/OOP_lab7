#include "../include/character.h"

// Реализация методов базового класса Character

double Character::calculateDistance(const Character& other) const {
    int dx = pos_x_ - other.pos_x_;
    int dy = pos_y_ - other.pos_y_;
    return std::sqrt(dx * dx + dy * dy);
}

void Character::print() const {
    std::cout << getTypeName() << " \"" << name_ 
              << "\" в позиции (" << pos_x_ << ", " << pos_y_ << ")\n";
}

// Реализация методов Dragon, Bull, Frog будет в patterns.cpp
// так как там определен BattleVisitor

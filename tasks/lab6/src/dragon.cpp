#include "../include/dragon.h"
#include "../include/visitor.h"
#include <iostream>
#include <cmath>
#include <ostream>

Dragon::Dragon(int x, int y, const std::string& name)
    : Npc(x, y, kType, name) {}

const std::string Dragon::kType = "Dragon";

void Dragon::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Dragon::printInfo() const {
    std::cout << "Dragon Info - Name: " << getName()
              << ", Position: (" << getX() << ", " << getY() << ")"
              << std::endl;
}
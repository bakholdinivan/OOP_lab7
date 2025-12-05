#include "../include/frog.h"
#include "../include/visitor.h"
#include <iostream>

Frog::Frog(int x, int y, const std::string& name)
    : Npc(x, y, kType, name) {}

const std::string Frog::kType = "Frog";

void Frog::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Frog::printInfo() const {
    std::cout << "Frog Info - Name: " << getName()
              << ", Position: (" << getX() << ", " << getY() << ")"
              << std::endl;
}
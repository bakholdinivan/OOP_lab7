#include "../include/bull.h"
#include "../include/visitor.h"
#include <iostream>

Bull::Bull(int x, int y, const std::string& name)
    : Npc(x, y, kType, name) {}

const std::string Bull::kType = "Bull";

void Bull::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Bull::printInfo() const {
    std::cout << "Bull Info - Name: " << getName()
              << ", Position: (" << getX() << ", " << getY() << ")"
              << std::endl;
}
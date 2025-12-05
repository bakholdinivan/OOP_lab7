#include "NPC.h"

NPC::NPC(const std::string& name, double x, double y) 
    : name(name), x(x), y(y), alive(true) {}

double NPC::distanceTo(const NPC& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::string NPC::toString() const {
    return getType() + " '" + name + "' в точке (" + 
           std::to_string((int)x) + ", " + std::to_string((int)y) + ")";
}

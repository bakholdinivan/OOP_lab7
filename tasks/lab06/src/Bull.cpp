#include "Bull.h"
#include "Dragon.h"
#include "Frog.h"
#include "BattleVisitor.h"

Bull::Bull(const std::string& name, double x, double y) 
    : NPC(name, x, y) {}

void Bull::accept(BattleVisitor& visitor, NPC& other) {
    if (Dragon* d = dynamic_cast<Dragon*>(&other)) {
        visitor.visit(*this, *d);
    } else if (Bull* b = dynamic_cast<Bull*>(&other)) {
        visitor.visit(*this, *b);
    } else if (Frog* f = dynamic_cast<Frog*>(&other)) {
        visitor.visit(*this, *f);
    }
}

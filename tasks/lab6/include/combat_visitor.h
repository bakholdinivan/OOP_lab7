#ifndef COMBAT_VISITOR_H
#define COMBAT_VISITOR_H

#include "visitor.h"
#include "npc.h"
#include "dragon.h"
#include "bull.h"
#include "frog.h"
#include <string>

class CombatVisitor : public Visitor {
public:
    void visit(Dragon&) override {}
    void visit(Bull&) override {}
    void visit(Frog&) override {}
    
    bool canKill(Npc* attacker, Npc* defender);

private:
    bool dragonVs(const std::string& defenderType);
    bool bullVs(const std::string& defenderType);
    bool frogVs(const std::string& defenderType);
};

#endif // COMBAT_VISITOR_H
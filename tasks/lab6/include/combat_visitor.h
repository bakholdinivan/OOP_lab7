#ifndef COMBAT_VISITOR_H
#define COMBAT_VISITOR_H

#include "visitor.h"
#include "npc.h"
#include "creatures.h"
#include <string>

// Визитор для определения результата боя между персонажами
class CombatVisitor : public Visitor {
public:
    // Методы посещения для паттерна Visitor
    void visit(Dragon&) override {}
    void visit(Bull&) override {}
    void visit(Frog&) override {}
    
    // Проверяет, может ли атакующий победить защитника
    bool canKill(Npc* attacker, Npc* defender);

private:
    // Логика боя для каждого типа существа
    bool dragonVs(const std::string& defenderType);
    bool bullVs(const std::string& defenderType);
    bool frogVs(const std::string& defenderType);
};

#endif // COMBAT_VISITOR_H
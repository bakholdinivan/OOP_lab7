#include "../include/combat_visitor.h"

// ============================================
// ОПРЕДЕЛЕНИЕ ПОБЕДИТЕЛЯ В БОЮ
// ============================================

bool CombatVisitor::canKill(Npc* attacker, Npc* defender) {
    // Определяем тип атакующего и проверяем правила боя
    if (attacker->getType() == "Dragon") {
        return dragonVs(defender->getType());
    } else if (attacker->getType() == "Bull") {
        return bullVs(defender->getType());
    } else if (attacker->getType() == "Frog") {
        return frogVs(defender->getType());
    }
    return false;
}

bool CombatVisitor::dragonVs(const std::string& defenderType) {
    // Дракон ест быков
    return (defenderType == "Bull");
}

bool CombatVisitor::bullVs(const std::string& defenderType) {
    // Бык топчет жаб
    return (defenderType == "Frog");
}

bool CombatVisitor::frogVs(const std::string&) {
    // Жабы спасаются как могут (никого не убивают)
    return false;
}
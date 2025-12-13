#include "BattleVisitor.h"
#include "NPC.h"
#include <sstream>

void BattleVisitor::addObserver(ObserverPtr observer) {
    observers.push_back(observer);
}

void BattleVisitor::notifyObservers(const std::string& message) {
    for (auto& observer : observers) {
        observer->onEvent(message);
    }
}

void BattleVisitor::visit(NPC& attacker, NPC& defender) {
    if (!attacker.isAlive() || !defender.isAlive()) {
        return;
    }

    if (!attacker.canKill(defender.getType())) {
        return;
    }

    double distance = attacker.getPosition().distanceTo(defender.getPosition());
    if (distance > attacker.getKillDistance()) {
        return;
    }

    // Бросаем кубики
    int attackRoll = attacker.rollDice();
    int defenseRoll = defender.rollDice();

    std::ostringstream oss;
    oss << "Бой: " << attacker.getName() << " (" << attacker.getType() 
        << ") атакует " << defender.getName() << " (" << defender.getType() 
        << ") - атака: " << attackRoll << ", защита: " << defenseRoll;

    if (attackRoll > defenseRoll) {
        defender.kill();
        oss << " - " << defender.getName() << " убит!";
    } else {
        oss << " - " << defender.getName() << " выжил!";
    }

    notifyObservers(oss.str());
}

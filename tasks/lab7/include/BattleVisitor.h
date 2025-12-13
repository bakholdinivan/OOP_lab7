#pragma once
#include <vector>
#include <memory>
#include "Observer.h"

class NPC;
class Frog;
class Bull;
class Dragon;

class BattleVisitor {
private:
    std::vector<ObserverPtr> observers;

public:
    void addObserver(ObserverPtr observer);
    void notifyObservers(const std::string& message);

    void visit(NPC& attacker, NPC& defender);
};

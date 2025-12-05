#include "BattleVisitor.h"
#include "Dragon.h"
#include "Bull.h"
#include "Frog.h"
#include "Observer.h"

void BattleVisitor::addObserver(std::shared_ptr<BattleObserver> observer) {
    observers.push_back(observer);
}

void BattleVisitor::notifyKill(const std::string& killer, const std::string& victim) {
    for (auto& observer : observers) {
        observer->onKill(killer, victim);
    }
}

void BattleVisitor::visit(Dragon& dragon, Bull& bull) {
    if (dragon.isAlive() && bull.isAlive()) {
        bull.kill();
        notifyKill(dragon.getName(), bull.getName());
    }
}

void BattleVisitor::visit(Bull& bull, Frog& frog) {
    if (bull.isAlive() && frog.isAlive()) {
        frog.kill();
        notifyKill(bull.getName(), frog.getName());
    }
}

void BattleVisitor::visit(Dragon& dragon, Frog& frog) {
}

void BattleVisitor::visit(Bull& bull, Dragon& dragon) {
}

void BattleVisitor::visit(Frog& frog, Bull& bull) {
}

void BattleVisitor::visit(Frog& frog, Dragon& dragon) {
}

void BattleVisitor::visit(Dragon& d1, Dragon& d2) {
    if (d1.isAlive() && d2.isAlive()) {
        d1.kill();
        d2.kill();
        notifyKill(d1.getName() + " и " + d2.getName(), "друг друга");
    }
}

void BattleVisitor::visit(Bull& b1, Bull& b2) {
    if (b1.isAlive() && b2.isAlive()) {
        b1.kill();
        b2.kill();
        notifyKill(b1.getName() + " и " + b2.getName(), "друг друга");
    }
}

void BattleVisitor::visit(Frog& f1, Frog& f2) {
}

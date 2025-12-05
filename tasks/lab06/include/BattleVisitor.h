#pragma once
#include <vector>
#include <memory>

class NPC;
class Dragon;
class Bull;
class Frog;
class BattleObserver;

class BattleVisitor {
private:
    std::vector<std::shared_ptr<BattleObserver>> observers;  // Наблюдатели событий

public:
    // Добавление наблюдателя
    void addObserver(std::shared_ptr<BattleObserver> observer);
    
    // Уведомление о событии убийства
    void notifyKill(const std::string& killer, const std::string& victim);
    
    // Логика боев для каждой пары типов
    void visit(Dragon& dragon, Bull& bull);
    void visit(Bull& bull, Frog& frog);
    void visit(Dragon& dragon, Frog& frog);
    void visit(Bull& bull, Dragon& dragon);
    void visit(Frog& frog, Bull& bull);
    void visit(Frog& frog, Dragon& dragon);
    
    // Бои одинаковых типов - оба погибают
    void visit(Dragon& d1, Dragon& d2);
    void visit(Bull& b1, Bull& b2);
    void visit(Frog& f1, Frog& f2);
};

#pragma once
#include <string>
#include <memory>
#include <random>
#include <mutex>
#include "Point.h"

class BattleVisitor;
class Observer;

class NPC : public std::enable_shared_from_this<NPC> {
protected:
    std::string name;
    Point position;
    bool alive;
    mutable std::mutex npc_mutex;

public:
    NPC(const std::string& name, const Point& pos);
    virtual ~NPC() = default;

    virtual std::string getType() const = 0;
    virtual int getMoveDistance() const = 0;
    virtual int getKillDistance() const = 0;
    virtual bool canKill(const std::string& targetType) const = 0;

    std::string getName() const;
    Point getPosition() const;
    bool isAlive() const;
    
    void setPosition(const Point& pos);
    void kill();

    virtual void accept(BattleVisitor& visitor, NPC& other) = 0;

    // Методы для броска кубика
    int rollDice() const;
};

using NPCPtr = std::shared_ptr<NPC>;

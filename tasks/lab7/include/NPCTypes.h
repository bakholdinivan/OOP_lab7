#pragma once
#include "NPC.h"

// Жаба: расстояние хода 1, расстояние убийства 10
class Frog : public NPC {
public:
    Frog(const std::string& name, const Point& pos);
    std::string getType() const override;
    int getMoveDistance() const override;
    int getKillDistance() const override;
    bool canKill(const std::string& targetType) const override;
    void accept(BattleVisitor& visitor, NPC& other) override;
};

// Бык: расстояние хода 30, расстояние убийства 10
class Bull : public NPC {
public:
    Bull(const std::string& name, const Point& pos);
    std::string getType() const override;
    int getMoveDistance() const override;
    int getKillDistance() const override;
    bool canKill(const std::string& targetType) const override;
    void accept(BattleVisitor& visitor, NPC& other) override;
};

// Дракон: расстояние хода 50, расстояние убийства 30
class Dragon : public NPC {
public:
    Dragon(const std::string& name, const Point& pos);
    std::string getType() const override;
    int getMoveDistance() const override;
    int getKillDistance() const override;
    bool canKill(const std::string& targetType) const override;
    void accept(BattleVisitor& visitor, NPC& other) override;
};

#pragma once
#include "NPC.h"

class Frog : public NPC {
public:
    Frog(const std::string& name, double x, double y);
    void accept(BattleVisitor& visitor, NPC& other) override;
    std::string getType() const override { return "Frog"; }
};

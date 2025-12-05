#pragma once
#include "NPC.h"

class Bull : public NPC {
public:
    Bull(const std::string& name, double x, double y);
    void accept(BattleVisitor& visitor, NPC& other) override;
    std::string getType() const override { return "Bull"; }
};

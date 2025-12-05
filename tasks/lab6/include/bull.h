#ifndef BULL_H
#define BULL_H

#include "npc.h"
#include <string>

class Bull : public Npc {
public:
    Bull(int x, int y, const std::string& name);
    void accept(Visitor& visitor) override;
    void printInfo() const override;
    
    static const std::string kType;
};

#endif // BULL_H
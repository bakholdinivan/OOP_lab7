#ifndef FROG_H
#define FROG_H

#include "npc.h"
#include <string>

class Frog : public Npc {
public:
    Frog(int x, int y, const std::string& name);
    void accept(Visitor& visitor) override;
    void printInfo() const override;
    
    static const std::string kType;
};

#endif // FROG_H
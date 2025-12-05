#ifndef DRAGON_H
#define DRAGON_H

#include "npc.h"
#include <string>

class Dragon : public Npc {
public:
    Dragon(int x, int y, const std::string& name);
    void accept(Visitor& visitor) override;
    void printInfo() const override;
    
    static const std::string kType;
};

#endif // DRAGON_H
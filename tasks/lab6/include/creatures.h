#ifndef CREATURES_H
#define CREATURES_H

#include "npc.h"
#include <string>

// ============================================
// СУЩЕСТВА ИГРОВОГО МИРА
// ============================================

/**
 * Дракон - летающий хищник
 * Правило боя: пожирает быков
 */
class Dragon : public Npc {
public:
    Dragon(int x, int y, const std::string& name);
    void accept(Visitor& visitor) override;
    void printInfo() const override;
    
    static const std::string kType;
};

/**
 * Бык - наземное животное средней силы
 * Правило боя: давит лягушек копытами
 */
class Bull : public Npc {
public:
    Bull(int x, int y, const std::string& name);
    void accept(Visitor& visitor) override;
    void printInfo() const override;
    
    static const std::string kType;
};

/**
 * Лягушка - слабое существо
 * Правило боя: не может никого победить, спасается как может
 */
class Frog : public Npc {
public:
    Frog(int x, int y, const std::string& name);
    void accept(Visitor& visitor) override;
    void printInfo() const override;
    
    static const std::string kType;
};

#endif // CREATURES_H

#include "../include/creatures.h"
#include "../include/visitor.h"
#include <iostream>
#include <cmath>
#include <ostream>

// ============================================
// РЕАЛИЗАЦИЯ КЛАССА ДРАКОН
// ============================================

Dragon::Dragon(int x, int y, const std::string& name)
    : Npc(x, y, kType, name) {}

const std::string Dragon::kType = "Dragon";

void Dragon::accept(Visitor& visitor) {
    // Применяем паттерн Visitor (double dispatch)
    visitor.visit(*this);
}

void Dragon::printInfo() const {
    std::cout << "🐉 Дракон \"" << getName()
              << "\" находится в точке (" << getX() << ", " << getY() << ")"
              << std::endl;
}

// ============================================
// РЕАЛИЗАЦИЯ КЛАССА БЫК
// ============================================

Bull::Bull(int x, int y, const std::string& name)
    : Npc(x, y, kType, name) {}

const std::string Bull::kType = "Bull";

void Bull::accept(Visitor& visitor) {
    // Применяем паттерн Visitor (double dispatch)
    visitor.visit(*this);
}

void Bull::printInfo() const {
    std::cout << "🐂 Бык \"" << getName()
              << "\" находится в точке (" << getX() << ", " << getY() << ")"
              << std::endl;
}

// ============================================
// РЕАЛИЗАЦИЯ КЛАССА ЛЯГУШКА
// ============================================

Frog::Frog(int x, int y, const std::string& name)
    : Npc(x, y, kType, name) {}

const std::string Frog::kType = "Frog";

void Frog::accept(Visitor& visitor) {
    // Применяем паттерн Visitor (double dispatch)
    visitor.visit(*this);
}

void Frog::printInfo() const {
    std::cout << "🐸 Лягушка \"" << getName()
              << "\" находится в точке (" << getX() << ", " << getY() << ")"
              << std::endl;
}

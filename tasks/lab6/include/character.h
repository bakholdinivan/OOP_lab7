#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <cmath>
#include <iostream>

// Предварительное объявление
class BattleVisitor;

/**
 * Базовый класс NPC (персонажа)
 * Все персонажи имеют имя и координаты
 */
class Character {
protected:
    std::string name_;
    int pos_x_;
    int pos_y_;
    
public:
    Character(const std::string& name, int x, int y) 
        : name_(name), pos_x_(x), pos_y_(y) {}
    
    virtual ~Character() = default;
    
    std::string getName() const { return name_; }
    int getX() const { return pos_x_; }
    int getY() const { return pos_y_; }
    
    virtual std::string getTypeName() const = 0;
    
    // Вычисление расстояния до другого персонажа
    double calculateDistance(const Character& other) const;
    
    // Печать информации о персонаже
    void print() const;
};

/**
 * Дракон - побеждает быков
 */
class Dragon : public Character {
public:
    Dragon(const std::string& name, int x, int y) 
        : Character(name, x, y) {}
    
    std::string getTypeName() const override { return "Dragon"; }
};

/**
 * Бык - побеждает лягушек
 */
class Bull : public Character {
public:
    Bull(const std::string& name, int x, int y) 
        : Character(name, x, y) {}
    
    std::string getTypeName() const override { return "Bull"; }
};

/**
 * Лягушка - никого не побеждает (спасается как может)
 */
class Frog : public Character {
public:
    Frog(const std::string& name, int x, int y) 
        : Character(name, x, y) {}
    
    std::string getTypeName() const override { return "Frog"; }
};

#endif // CHARACTER_H

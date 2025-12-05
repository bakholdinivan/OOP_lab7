#pragma once
#include <string>
#include <memory>
#include <cmath>

class BattleVisitor;

class NPC {
protected:
    std::string name;
    double x, y;
    bool alive;

public:
    NPC(const std::string& name, double x, double y);
    virtual ~NPC() = default;

    // Геттеры
    std::string getName() const { return name; }
    double getX() const { return x; }
    double getY() const { return y; }
    bool isAlive() const { return alive; }
    
    // Установка статуса
    void kill() { alive = false; }
    
    // Расстояние до другого NPC
    double distanceTo(const NPC& other) const;
    
    // Метод для паттерна Visitor
    virtual void accept(BattleVisitor& visitor, NPC& other) = 0;
    
    // Тип персонажа для сохранения
    virtual std::string getType() const = 0;
    
    // Строковое представление
    virtual std::string toString() const;
};

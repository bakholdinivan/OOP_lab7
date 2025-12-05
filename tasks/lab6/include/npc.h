#ifndef NPC_H
#define NPC_H

#include <string>
#include <ostream>

class Visitor;

/**
 * Базовый класс для всех персонажей (NPC - Non-Player Character)
 * Хранит общие свойства: координаты, тип и имя
 */
class Npc {
protected:
    int x_;              // Координата X
    int y_;              // Координата Y
    std::string type_;   // Тип персонажа (Dragon, Bull, Frog)
    std::string name_;   // Уникальное имя

public:
    Npc(int x, int y, const std::string& type, const std::string& name);
    virtual ~Npc() = default;

    // Геттеры для получения свойств персонажа
    int getX() const;
    int getY() const;
    std::string getType() const;
    std::string getName() const;
    
    // Вычисление расстояния до другого персонажа
    double distanceTo(const Npc& other) const;
    
    // Метод accept для паттерна Visitor (double dispatch)
    virtual void accept(Visitor& visitor) = 0;
    
    // Вывод информации о персонаже
    virtual void printInfo() const;
    
    // Оператор вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc);
};

#endif // NPC_H
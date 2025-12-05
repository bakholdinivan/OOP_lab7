#ifndef VISITOR_H
#define VISITOR_H

// Предварительные объявления классов существ
class Dragon;
class Bull;
class Frog;

/**
 * Интерфейс Визитора - паттерн Visitor
 * Позволяет добавлять новые операции над объектами без изменения их классов
 */
class Visitor {
public:
    virtual ~Visitor() = default;
    
    // Методы посещения для каждого типа существ (double dispatch)
    virtual void visit(Dragon& dragon) = 0;
    virtual void visit(Bull& bull) = 0;
    virtual void visit(Frog& frog) = 0;
};

#endif // VISITOR_H
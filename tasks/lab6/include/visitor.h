#ifndef VISITOR_H
#define VISITOR_H

class Dragon;
class Bull;
class Frog;

class Visitor {
public:
    virtual ~Visitor() = default;
    
    virtual void visit(Dragon& dragon) = 0;
    virtual void visit(Bull& bull) = 0;
    virtual void visit(Frog& frog) = 0;
};

#endif // VISITOR_H
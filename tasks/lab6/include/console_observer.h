#ifndef CONSOLE_OBSERVER_H
#define CONSOLE_OBSERVER_H

#include "observer.h"
#include <iostream>

class ConsoleObserver : public Observer {
public:
    void notify(const std::string& event) override {
        std::cout << "[CONSOLE] " << event << std::endl;
    }
};

#endif // CONSOLE_OBSERVER_H
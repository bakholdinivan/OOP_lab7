#ifndef FILE_OBSERVER_H
#define FILE_OBSERVER_H

#include "observer.h"
#include <fstream>
#include <string>

class FileObserver : public Observer {
private:
    std::string filename_;

public:
    FileObserver(const std::string& filename) : filename_(filename) {}
    
    void notify(const std::string& event) override {
        std::ofstream file(filename_, std::ios::app);
        if (file.is_open()) {
            file << "[LOG] " << event << std::endl;
            file.close();
        }
    }
};

#endif // FILE_OBSERVER_H
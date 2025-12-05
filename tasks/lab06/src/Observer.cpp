#include "Observer.h"
#include <iostream>

void ConsoleObserver::onKill(const std::string& killer, const std::string& victim) {
    std::cout << "[СОБЫТИЕ] " << killer << " убил " << victim << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : filename(filename) {}

void FileObserver::onKill(const std::string& killer, const std::string& victim) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << "[СОБЫТИЕ] " << killer << " убил " << victim << std::endl;
        file.close();
    }
}

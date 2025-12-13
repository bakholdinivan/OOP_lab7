#include "Observer.h"
#include <iostream>

std::mutex ConsoleObserver::console_mutex;

void ConsoleObserver::onEvent(const std::string& message) {
    std::lock_guard<std::mutex> lock(console_mutex);
    std::cout << message << std::endl;
}

FileObserver::FileObserver(const std::string& filename) {
    file.open(filename, std::ios::app);
}

FileObserver::~FileObserver() {
    if (file.is_open()) {
        file.close();
    }
}

void FileObserver::onEvent(const std::string& message) {
    std::lock_guard<std::mutex> lock(file_mutex);
    if (file.is_open()) {
        file << message << std::endl;
    }
}

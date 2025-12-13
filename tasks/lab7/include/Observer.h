#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <mutex>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onEvent(const std::string& message) = 0;
};

class ConsoleObserver : public Observer {
private:
    static std::mutex console_mutex;
public:
    void onEvent(const std::string& message) override;
};

class FileObserver : public Observer {
private:
    std::ofstream file;
    mutable std::mutex file_mutex;
public:
    FileObserver(const std::string& filename);
    ~FileObserver();
    void onEvent(const std::string& message) override;
};

using ObserverPtr = std::shared_ptr<Observer>;

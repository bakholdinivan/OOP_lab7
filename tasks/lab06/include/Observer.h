#pragma once
#include <string>
#include <fstream>

class BattleObserver {
public:
    virtual ~BattleObserver() = default;
    virtual void onKill(const std::string& killer, const std::string& victim) = 0;
};

class ConsoleObserver : public BattleObserver {
public:
    void onKill(const std::string& killer, const std::string& victim) override;
};

class FileObserver : public BattleObserver {
private:
    std::string filename;
    
public:
    FileObserver(const std::string& filename);
    void onKill(const std::string& killer, const std::string& victim) override;
};

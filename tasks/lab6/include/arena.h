#ifndef ARENA_H
#define ARENA_H

#include "npc.h"
#include "observer.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Arena {
private:
    static constexpr int MAX_WIDTH = 500;
    static constexpr int MAX_HEIGHT = 500;
    
    int width_;
    int height_;
    std::map<std::string, std::unique_ptr<Npc>> npcs_;
    std::vector<std::shared_ptr<Observer>> observers_;

public:
    Arena(int width = MAX_WIDTH, int height = MAX_HEIGHT);
    
    void addNpc(std::unique_ptr<Npc> npc);
    void createAndAddNpc(const std::string& type, 
                         const std::string& name, 
                         int x, int y);
    
    void printAllNpcs() const;
    size_t getNpcCount() const;
    
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    void clear();
    
    void addObserver(std::shared_ptr<Observer> observer);
    void removeObserver(std::shared_ptr<Observer> observer);
    void notifyObservers(const std::string& event);
    
    void startBattle(double range);
};

#endif // ARENA_H
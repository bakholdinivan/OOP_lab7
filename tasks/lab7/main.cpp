#include "GameWorld.h"
#include "NPCFactory.h"
#include "Observer.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    const double MAP_WIDTH = 100.0;
    const double MAP_HEIGHT = 100.0;
    const int NPC_COUNT = 50;
    const int GAME_DURATION_SEC = 30;

    std::cout << "=== Игра началась ===" << std::endl;
    std::cout << "Размер карты: " << MAP_WIDTH << " x " << MAP_HEIGHT << std::endl;
    std::cout << "Количество NPC: " << NPC_COUNT << std::endl;
    std::cout << "Длительность игры: " << GAME_DURATION_SEC << " секунд" << std::endl;
    std::cout << std::endl;

    // Создаем игровой мир
    GameWorld world(MAP_WIDTH, MAP_HEIGHT);

    // Добавляем наблюдателей
    auto console_observer = std::make_shared<ConsoleObserver>();
    auto file_observer = std::make_shared<FileObserver>("game_log.txt");
    world.addObserver(console_observer);
    world.addObserver(file_observer);

    // Создаем 50 случайных NPC
    for (int i = 0; i < NPC_COUNT; ++i) {
        std::string name = "NPC_" + std::to_string(i + 1);
        auto npc = NPCFactory::createRandomNPC(name, MAP_WIDTH, MAP_HEIGHT);
        world.addNPC(npc);
    }

    std::cout << "Создано " << NPC_COUNT << " NPC" << std::endl;
    std::cout << "Игра запускается..." << std::endl;
    std::cout << std::endl;

    // Запускаем три потока с использованием лямбда-функций
    std::thread movement_thread([&world]() { world.movementThread(); });
    std::thread battle_thread([&world]() { world.battleThread(); });
    std::thread print_thread([&world]() { world.printThread(); });

    // Ждем 30 секунд
    std::this_thread::sleep_for(std::chrono::seconds(GAME_DURATION_SEC));

    // Останавливаем игру
    world.stop();

    // Ждем завершения потоков
    movement_thread.join();
    battle_thread.join();
    print_thread.join();

    // Выводим список выживших
    std::cout << "\n\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
    std::cout << "Список выживших:" << std::endl;
    
    auto survivors = world.getSurvivors();
    if (survivors.empty()) {
        std::cout << "Никто не выжил!" << std::endl;
    } else {
        for (const auto& npc : survivors) {
            Point pos = npc->getPosition();
            std::cout << "  " << npc->getType() << " \"" << npc->getName() 
                      << "\" - позиция (" << pos.x << ", " << pos.y << ")" << std::endl;
        }
        std::cout << "\nВсего выживших: " << survivors.size() << " из " << NPC_COUNT << std::endl;
    }

    return 0;
}

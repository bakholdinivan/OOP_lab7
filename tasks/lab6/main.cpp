#include "include/arena.h"
#include "include/factory.h"
#include "include/console_observer.h"
#include "include/file_observer.h"
#include <iostream>
#include <memory>

int main() {
    try {
        std::cout << "=== Balagur Fate 3 - Dungeon Editor ===" << std::endl;
        std::cout << std::endl;

        Arena arena(500, 500);

        auto consoleObserver = std::make_shared<ConsoleObserver>();
        auto fileObserver = std::make_shared<FileObserver>("log.txt");
        
        arena.addObserver(consoleObserver);
        arena.addObserver(fileObserver);

        std::cout << "1. Создаём NPC..." << std::endl;

        arena.createAndAddNpc("Dragon", "Smaug", 100, 100);
        arena.createAndAddNpc("Dragon", "Alduin", 450, 450);
        arena.createAndAddNpc("Bull", "Minotaur", 120, 120);
        arena.createAndAddNpc("Bull", "Ferdinand", 400, 400);
        arena.createAndAddNpc("Frog", "Pepe", 110, 110);
        arena.createAndAddNpc("Frog", "Kermit", 200, 200);
        std::cout << "Создано NPC: " << arena.getNpcCount() << std::endl;
        std::cout << std::endl;

        std::cout << "2. Список всех NPC на арене:" << std::endl;
        arena.printAllNpcs();
        std::cout << std::endl;

        std::cout << "3.  Сохраняем NPC в файл 'npcs.txt'..." << std::endl;
        arena.saveToFile("npcs.txt");
        std::cout << "Сохранено!" << std::endl;
        std::cout << std::endl;

        std::cout << "4.  Запускаем боевой режим (дальность: 100 метров)..." << std::endl;
        std::cout << "--- НАЧАЛО БОЯ ---" << std::endl;
        arena.startBattle(100.0);
        std::cout << "--- КОНЕЦ БОЯ ---" << std::endl;
        std::cout << std::endl;

        std::cout << "5. Список выживших NPC:" << std::endl;
        arena.printAllNpcs();
        std::cout << "Осталось NPC: " << arena.getNpcCount() << std::endl;
        std::cout << std::endl;

        std::cout << "6. Очищаем арену и загружаем из файла..." << std::endl;
        arena.clear();
        arena.loadFromFile("npcs.txt");
        std::cout << "Загружено NPC: " << arena.getNpcCount() << std::endl;
        arena.printAllNpcs();
        std::cout << std::endl;

        std::cout << "=== Программа завершена успешно ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
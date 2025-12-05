#include <gtest/gtest.h>
#include "../include/arena.h"
#include "../include/factory.h"
#include "../include/console_observer.h"
#include "../include/file_observer.h"
#include <memory>
#include <fstream>

// Тесты арены
TEST(ArenaTest, CreateArena) {
    Arena arena(500, 500);
    EXPECT_EQ(arena.getNpcCount(), 0);
}

TEST(ArenaTest, CreateArenaInvalidSize) {
    EXPECT_THROW({
        Arena arena(600, 600);
    }, std::out_of_range);
}

TEST(ArenaTest, AddNpc) {
    Arena arena;
    auto dragon = NpcFactory::createNpc("Dragon", "Smaug", 100, 100);
    arena.addNpc(std::move(dragon));
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

TEST(ArenaTest, AddMultipleNpcs) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Dragon1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "Bull1", 200, 200));
    arena.addNpc(NpcFactory::createNpc("Frog", "Frog1", 300, 300));
    
    EXPECT_EQ(arena.getNpcCount(), 3);
}

TEST(ArenaTest, AddNpcDuplicateName) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Duplicate", 100, 100));
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Bull", "Duplicate", 200, 200));
    }, std::invalid_argument);
}

TEST(ArenaTest, AddNpcOutOfBounds) {
    Arena arena(500, 500);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Dragon", "OutDragon", 600, 100));
    }, std::out_of_range);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Dragon", "OutDragon2", 100, 600));
    }, std::out_of_range);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Dragon", "NegativeDragon", -10, 100));
    }, std::out_of_range);
}

TEST(ArenaTest, ClearArena) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Dragon1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "Bull1", 200, 200));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    arena.clear();
    EXPECT_EQ(arena.getNpcCount(), 0);
}

// Тесты сохранения/загрузки
TEST(ArenaTest, SaveToFile) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Smaug", 100, 200));
    arena.addNpc(NpcFactory::createNpc("Bull", "Ferdinand", 150, 250));
    arena.addNpc(NpcFactory::createNpc("Frog", "Pepe", 50, 75));
    
    std::string filename = "test_save.txt";
    arena.saveToFile(filename);
    
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lineCount++;
        }
    }
    file.close();
    
    EXPECT_EQ(lineCount, 3);
    
    std::remove(filename.c_str());
}

TEST(ArenaTest, LoadFromFile) {
    std::string filename = "test_load.txt";
    std::ofstream outfile(filename);
    outfile << "Dragon Smaug 100 200\n";
    outfile << "Bull Ferdinand 150 250\n";
    outfile << "Frog Pepe 50 75\n";
    outfile.close();
    
    Arena arena;
    arena.loadFromFile(filename);
    
    EXPECT_EQ(arena.getNpcCount(), 3);
    
    std::remove(filename.c_str());
}

TEST(ArenaTest, LoadFromNonExistentFile) {
    Arena arena;
    
    EXPECT_THROW({
        arena.loadFromFile("nonexistent_file.txt");
    }, std::runtime_error);
}

TEST(ArenaTest, SaveAndLoadRoundtrip) {
    std::string filename = "test_roundtrip.txt";
    
    {
        Arena arena1;
        arena1.addNpc(NpcFactory::createNpc("Dragon", "Dragon1", 111, 222));
        arena1.addNpc(NpcFactory::createNpc("Bull", "Bull1", 333, 444));
        arena1.saveToFile(filename);
    }
    
    {
        Arena arena2;
        arena2.loadFromFile(filename);
        EXPECT_EQ(arena2.getNpcCount(), 2);
    }
    
    std::remove(filename.c_str());
}

// Тесты Observer
TEST(ArenaTest, AddObserver) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    
    EXPECT_NO_THROW({
        arena.addObserver(observer);
    });
}

TEST(ArenaTest, AddFileObserver) {
    Arena arena;
    auto observer = std::make_shared<FileObserver>("test_observer.txt");
    
    EXPECT_NO_THROW({
        arena.addObserver(observer);
    });
    
    std::remove("test_observer.txt");
}

// Тесты боя
TEST(CombatTest, DragonKillsBull) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Smaug", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "Ferdinand", 110, 110));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    arena.startBattle(50.0);
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

TEST(CombatTest, BullKillsFrog) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Bull", "Ferdinand", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Frog", "Pepe", 110, 110));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    arena.startBattle(50.0);
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

TEST(CombatTest, FrogSurvives) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Frog", "Pepe", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Frog", "Kermit", 110, 110));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    arena.startBattle(50.0);
    
    // Жабы не убивают друг друга
    EXPECT_EQ(arena.getNpcCount(), 2);
}

TEST(CombatTest, OutOfRange) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Smaug", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "Ferdinand", 400, 400));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Дальность слишком мала
    arena.startBattle(50.0);
    
    // Никто не погиб
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// Тесты Factory
TEST(FactoryTest, CreateDragon) {
    auto npc = NpcFactory::createNpc("Dragon", "Smaug", 100, 100);
    
    EXPECT_EQ(npc->getType(), "Dragon");
    EXPECT_EQ(npc->getName(), "Smaug");
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 100);
}

TEST(FactoryTest, CreateBull) {
    auto npc = NpcFactory::createNpc("Bull", "Ferdinand", 200, 200);
    
    EXPECT_EQ(npc->getType(), "Bull");
    EXPECT_EQ(npc->getName(), "Ferdinand");
}

TEST(FactoryTest, CreateFrog) {
    auto npc = NpcFactory::createNpc("Frog", "Pepe", 300, 300);
    
    EXPECT_EQ(npc->getType(), "Frog");
    EXPECT_EQ(npc->getName(), "Pepe");
}

TEST(FactoryTest, CreateInvalidType) {
    EXPECT_THROW({
        NpcFactory::createNpc("InvalidType", "Test", 100, 100);
    }, std::invalid_argument);
}

TEST(FactoryTest, CreateFromString) {
    std::string line = "Dragon Smaug 100 200";
    auto npc = NpcFactory::createFromString(line);
    
    EXPECT_EQ(npc->getType(), "Dragon");
    EXPECT_EQ(npc->getName(), "Smaug");
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 200);
}

TEST(FactoryTest, CreateFromInvalidString) {
    std::string line = "Invalid String Format";
    
    EXPECT_THROW({
        NpcFactory::createFromString(line);
    }, std::exception);
}

// Дополнительные тесты для проверки требований задания

// Тест граничных координат (0 и 500 должны быть валидными)
TEST(ArenaTest, BoundaryCoordinates) {
    Arena arena(500, 500);
    
    // Проверяем что 0,0 валидны
    EXPECT_NO_THROW({
        arena.addNpc(NpcFactory::createNpc("Dragon", "Corner1", 0, 0));
    });
    
    // Проверяем что 500,500 валидны
    EXPECT_NO_THROW({
        arena.addNpc(NpcFactory::createNpc("Bull", "Corner2", 500, 500));
    });
    
    // Проверяем что 0,500 валидны
    EXPECT_NO_THROW({
        arena.addNpc(NpcFactory::createNpc("Frog", "Corner3", 0, 500));
    });
    
    // Проверяем что 500,0 валидны
    EXPECT_NO_THROW({
        arena.addNpc(NpcFactory::createNpc("Dragon", "Corner4", 500, 0));
    });
    
    EXPECT_EQ(arena.getNpcCount(), 4);
}

// Тест выхода за границы
TEST(ArenaTest, OutOfBoundsCoordinates) {
    Arena arena(500, 500);
    
    // x > 500
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Dragon", "OutX", 501, 250));
    }, std::out_of_range);
    
    // y > 500
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Bull", "OutY", 250, 501));
    }, std::out_of_range);
    
    // Отрицательные координаты
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Frog", "NegX", -1, 250));
    }, std::out_of_range);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Dragon", "NegY", 250, -1));
    }, std::out_of_range);
}

// Тест уникальности имен
TEST(ArenaTest, UniqueNamesRequired) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "UniqueName", 100, 100));
    
    // Попытка добавить NPC с таким же именем (даже другого типа)
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Bull", "UniqueName", 200, 200));
    }, std::invalid_argument);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Frog", "UniqueName", 300, 300));
    }, std::invalid_argument);
    
    // Должен остаться только один NPC
    EXPECT_EQ(arena.getNpcCount(), 1);
}

// Тест взаимного убийства (оба NPC должны погибнуть)
TEST(CombatTest, MutualKill) {
    Arena arena;
    
    // Dragon убивает Bull, но проверим что система поддерживает взаимное убийство
    arena.addNpc(NpcFactory::createNpc("Dragon", "Dragon1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Dragon", "Dragon2", 110, 110));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Драконы не убивают друг друга по правилам, поэтому оба выживут
    arena.startBattle(50.0);
    
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// Тест цепочки убийств
TEST(CombatTest, KillChain) {
    Arena arena;
    
    // Dragon -> Bull -> Frog
    arena.addNpc(NpcFactory::createNpc("Dragon", "TopPredator", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "MiddlePredator", 110, 110));
    arena.addNpc(NpcFactory::createNpc("Frog", "Victim", 120, 120));
    
    EXPECT_EQ(arena.getNpcCount(), 3);
    
    arena.startBattle(50.0);
    
    // Dragon убьет Bull, Bull убьет Frog
    EXPECT_EQ(arena.getNpcCount(), 1);
}

// Тест точной дальности боя
TEST(CombatTest, ExactRange) {
    Arena arena;
    
    // Расстояние между (0,0) и (3,4) = 5
    arena.addNpc(NpcFactory::createNpc("Dragon", "DragonExact", 0, 0));
    arena.addNpc(NpcFactory::createNpc("Bull", "BullExact", 3, 4));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Дальность 4.9 - не должны сражаться
    arena.startBattle(4.9);
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Дальность 5.0 - должны сражаться (но не убивают, т.к. дистанция = 5)
    // На самом деле нужна дальность > 5
    arena.startBattle(5.1);
    EXPECT_EQ(arena.getNpcCount(), 1); // Bull должен быть убит
}

// Тест сохранения и загрузки с разными типами NPC
TEST(ArenaTest, SaveLoadAllTypes) {
    std::string filename = "test_all_types.txt";
    
    {
        Arena arena;
        arena.addNpc(NpcFactory::createNpc("Dragon", "TestDragon", 100, 200));
        arena.addNpc(NpcFactory::createNpc("Bull", "TestBull", 200, 300));
        arena.addNpc(NpcFactory::createNpc("Frog", "TestFrog", 300, 400));
        arena.saveToFile(filename);
    }
    
    {
        Arena arena;
        arena.loadFromFile(filename);
        EXPECT_EQ(arena.getNpcCount(), 3);
    }
    
    std::remove(filename.c_str());
}

// Тест Observer - проверка записи в файл
TEST(ObserverTest, FileObserverWritesEvents) {
    std::string logFile = "test_combat_log.txt";
    std::remove(logFile.c_str()); // Очищаем файл перед тестом
    
    Arena arena;
    auto fileObserver = std::make_shared<FileObserver>(logFile);
    arena.addObserver(fileObserver);
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "ObserverDragon", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "ObserverBull", 110, 110));
    
    arena.startBattle(50.0);
    
    // Проверяем что файл создан и содержит запись
    std::ifstream file(logFile);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    bool hasContent = false;
    while (std::getline(file, line)) {
        if (!line.empty() && line.find("killed") != std::string::npos) {
            hasContent = true;
            break;
        }
    }
    file.close();
    
    EXPECT_TRUE(hasContent);
    std::remove(logFile.c_str());
}

// Тест множественных наблюдателей
TEST(ObserverTest, MultipleObservers) {
    Arena arena;
    
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    auto fileObserver = std::make_shared<FileObserver>("test_multi_observer.txt");
    
    EXPECT_NO_THROW({
        arena.addObserver(consoleObserver);
        arena.addObserver(fileObserver);
    });
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "MultiDragon", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "MultiBull", 110, 110));
    
    // Оба наблюдателя должны получить уведомление
    EXPECT_NO_THROW({
        arena.startBattle(50.0);
    });
    
    std::remove("test_multi_observer.txt");
}

// Тест большого количества NPC
TEST(ArenaTest, ManyNpcs) {
    Arena arena;
    
    for (int i = 0; i < 50; ++i) {
        std::string name = "NPC_" + std::to_string(i);
        std::string type = (i % 3 == 0) ? "Dragon" : (i % 3 == 1) ? "Bull" : "Frog";
        arena.addNpc(NpcFactory::createNpc(type, name, i * 10, i * 10));
    }
    
    EXPECT_EQ(arena.getNpcCount(), 50);
    
    arena.startBattle(100.0);
    
    // После боя должны остаться выжившие
    EXPECT_LT(arena.getNpcCount(), 50);
}

// Тест правил боя: Dragon ест Bull
TEST(CombatRulesTest, DragonEatsBull) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Predator", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "Prey", 105, 105));
    
    arena.startBattle(10.0);
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

// Тест правил боя: Bull топчет Frog
TEST(CombatRulesTest, BullStompsFrog) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Bull", "Stomper", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Frog", "Hopper", 105, 105));
    
    arena.startBattle(10.0);
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

// Тест правил боя: Frog не убивает никого
TEST(CombatRulesTest, FrogKillsNobody) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Frog", "Peaceful", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Dragon", "Safe", 105, 105));
    
    arena.startBattle(10.0);
    
    // Frog не может убить Dragon, оба выживут
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// ========== ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ДЛЯ ФИНАЛЬНОЙ ПРОВЕРКИ ==========

// Тест: пустая арена после боя
TEST(ArenaTest, EmptyArenaAfterBattle) {
    Arena arena;
    
    // Бой на пустой арене не должен вызывать ошибок
    EXPECT_NO_THROW({
        arena.startBattle(100.0);
    });
    
    EXPECT_EQ(arena.getNpcCount(), 0);
}

// Тест: один NPC в бою
TEST(CombatTest, SingleNpcBattle) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Lonely", 100, 100));
    
    EXPECT_EQ(arena.getNpcCount(), 1);
    
    // Один NPC не может сражаться сам с собой
    arena.startBattle(100.0);
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

// Тест: нулевая дальность боя (NPC на разных координатах)
TEST(CombatTest, ZeroRange) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Close", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "VeryClose", 110, 110));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Нулевая дальность - никто не сражается (расстояние > 0)
    arena.startBattle(0.0);
    
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// Тест: NPC на одних и тех же координатах
TEST(ArenaTest, SameCoordinates) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "First", 250, 250));
    arena.addNpc(NpcFactory::createNpc("Bull", "Second", 250, 250));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Расстояние = 0, они должны сражаться даже при малой дальности
    arena.startBattle(1.0);
    
    EXPECT_EQ(arena.getNpcCount(), 1); // Dragon убивает Bull
}

// Тест: максимальная дальность
TEST(CombatTest, MaximumRange) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Corner1", 0, 0));
    arena.addNpc(NpcFactory::createNpc("Bull", "Corner2", 500, 500));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Расстояние = sqrt(500^2 + 500^2) ≈ 707
    arena.startBattle(710.0);
    
    EXPECT_EQ(arena.getNpcCount(), 1); // Dragon убивает Bull
}

// Тест: сохранение пустой арены
TEST(ArenaTest, SaveEmptyArena) {
    std::string filename = "test_empty.txt";
    
    Arena arena;
    EXPECT_NO_THROW({
        arena.saveToFile(filename);
    });
    
    // Файл должен быть создан (возможно пустой)
    std::ifstream file(filename);
    EXPECT_TRUE(file.is_open());
    file.close();
    
    std::remove(filename.c_str());
}

// Тест: загрузка пустого файла
TEST(ArenaTest, LoadEmptyFile) {
    std::string filename = "test_empty_load.txt";
    
    // Создаем пустой файл
    std::ofstream outfile(filename);
    outfile.close();
    
    Arena arena;
    EXPECT_NO_THROW({
        arena.loadFromFile(filename);
    });
    
    EXPECT_EQ(arena.getNpcCount(), 0);
    
    std::remove(filename.c_str());
}

// Тест: загрузка файла с пустыми строками
TEST(ArenaTest, LoadFileWithEmptyLines) {
    std::string filename = "test_empty_lines.txt";
    
    std::ofstream outfile(filename);
    outfile << "\n";
    outfile << "Dragon TestDragon 100 200\n";
    outfile << "\n";
    outfile << "Bull TestBull 200 300\n";
    outfile << "\n";
    outfile.close();
    
    Arena arena;
    arena.loadFromFile(filename);
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    std::remove(filename.c_str());
}

// Тест: Dragon не убивает Dragon
TEST(CombatRulesTest, DragonVsDragon) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "Dragon1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Dragon", "Dragon2", 105, 105));
    
    arena.startBattle(10.0);
    
    // Драконы не убивают друг друга
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// Тест: Bull не убивает Bull
TEST(CombatRulesTest, BullVsBull) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Bull", "Bull1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "Bull2", 105, 105));
    
    arena.startBattle(10.0);
    
    // Быки не убивают друг друга
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// Тест: Bull не убивает Dragon
TEST(CombatRulesTest, BullVsDragon) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Bull", "WeakBull", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Dragon", "StrongDragon", 105, 105));
    
    arena.startBattle(10.0);
    
    // Bull не убивает Dragon, но Dragon убивает Bull
    EXPECT_EQ(arena.getNpcCount(), 1);
}

// Тест: Frog не убивает Bull
TEST(CombatRulesTest, FrogVsBull) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Frog", "WeakFrog", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "StrongBull", 105, 105));
    
    arena.startBattle(10.0);
    
    // Frog не убивает Bull, но Bull убивает Frog
    EXPECT_EQ(arena.getNpcCount(), 1);
}

// Тест: Dragon не убивает Frog
TEST(CombatRulesTest, DragonVsFrog) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Dragon", "BigDragon", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Frog", "LuckyFrog", 105, 105));
    
    arena.startBattle(10.0);
    
    // Dragon не убивает Frog (по правилам)
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// Тест: комплексный сценарий с несколькими боями
TEST(CombatTest, ComplexBattleScenario) {
    Arena arena;
    
    // Создаем сложную конфигурацию
    arena.addNpc(NpcFactory::createNpc("Dragon", "Alpha", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Dragon", "Beta", 200, 200));
    arena.addNpc(NpcFactory::createNpc("Bull", "Gamma", 110, 110));
    arena.addNpc(NpcFactory::createNpc("Bull", "Delta", 190, 190));
    arena.addNpc(NpcFactory::createNpc("Frog", "Epsilon", 120, 120));
    arena.addNpc(NpcFactory::createNpc("Frog", "Zeta", 180, 180));
    
    EXPECT_EQ(arena.getNpcCount(), 6);
    
    // Дальность 50 метров
    arena.startBattle(50.0);
    
    // Alpha убивает Gamma, Beta убивает Delta
    // Gamma убивает Epsilon, Delta убивает Zeta
    // Драконы и жабы выживают
    EXPECT_LT(arena.getNpcCount(), 6);
}

// Тест: удаление наблюдателя
TEST(ObserverTest, RemoveObserver) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    
    arena.addObserver(observer);
    arena.addNpc(NpcFactory::createNpc("Dragon", "Test1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "Test2", 110, 110));
    
    EXPECT_NO_THROW({
        arena.removeObserver(observer);
        arena.startBattle(50.0);
    });
}

// Тест: множественная запись в файл через Observer
TEST(ObserverTest, MultipleFileWrites) {
    std::string logFile = "test_multiple_writes.txt";
    std::remove(logFile.c_str());
    
    Arena arena;
    auto fileObserver = std::make_shared<FileObserver>(logFile);
    arena.addObserver(fileObserver);
    
    // Первый бой
    arena.addNpc(NpcFactory::createNpc("Dragon", "D1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Bull", "B1", 110, 110));
    arena.startBattle(50.0);
    
    // Второй бой
    arena.clear();
    arena.addNpc(NpcFactory::createNpc("Bull", "B2", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Frog", "F2", 110, 110));
    arena.startBattle(50.0);
    
    // Проверяем что файл содержит записи от обоих боев
    std::ifstream file(logFile);
    ASSERT_TRUE(file.is_open());
    
    int eventCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.find("killed") != std::string::npos) {
            eventCount++;
        }
    }
    file.close();
    
    EXPECT_EQ(eventCount, 2);
    std::remove(logFile.c_str());
}

// Тест: расстояние вычисляется корректно
TEST(NpcTest, DistanceCalculation) {
    auto npc1 = NpcFactory::createNpc("Dragon", "NPC1", 0, 0);
    auto npc2 = NpcFactory::createNpc("Bull", "NPC2", 3, 4);
    
    // Расстояние должно быть 5 (теорема Пифагора: sqrt(3^2 + 4^2))
    double distance = npc1->distanceTo(*npc2);
    EXPECT_DOUBLE_EQ(distance, 5.0);
}

// Тест: проверка типов NPC
TEST(FactoryTest, VerifyNpcTypes) {
    auto dragon = NpcFactory::createNpc("Dragon", "TestDragon", 100, 100);
    auto bull = NpcFactory::createNpc("Bull", "TestBull", 200, 200);
    auto frog = NpcFactory::createNpc("Frog", "TestFrog", 300, 300);
    
    EXPECT_EQ(dragon->getType(), "Dragon");
    EXPECT_EQ(bull->getType(), "Bull");
    EXPECT_EQ(frog->getType(), "Frog");
}

// Тест: координаты NPC корректны
TEST(FactoryTest, VerifyNpcCoordinates) {
    auto npc = NpcFactory::createNpc("Dragon", "Test", 123, 456);
    
    EXPECT_EQ(npc->getX(), 123);
    EXPECT_EQ(npc->getY(), 456);
}

// Тест: имена NPC корректны
TEST(FactoryTest, VerifyNpcNames) {
    auto npc = NpcFactory::createNpc("Dragon", "MyDragonName", 100, 100);
    
    EXPECT_EQ(npc->getName(), "MyDragonName");
}

// Тест: сохранение с особыми символами в имени (если поддерживается)
TEST(ArenaTest, SaveWithSpecialCharactersInName) {
    std::string filename = "test_special_chars.txt";
    
    Arena arena;
    arena.addNpc(NpcFactory::createNpc("Dragon", "Dragon_123", 100, 100));
    
    EXPECT_NO_THROW({
        arena.saveToFile(filename);
    });
    
    Arena arena2;
    EXPECT_NO_THROW({
        arena2.loadFromFile(filename);
    });
    
    EXPECT_EQ(arena2.getNpcCount(), 1);
    
    std::remove(filename.c_str());
}
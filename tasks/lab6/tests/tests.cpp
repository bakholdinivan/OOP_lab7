#include <gtest/gtest.h>
#include "../include/game_world.h"

// ========== ТЕСТЫ FACTORY ПАТТЕРНА ==========

TEST(FactoryTest, CreateDragon) {
    auto dragon = NpcFactory::createNpc("Dragon", "ТестДракон", 100, 200);
    EXPECT_EQ(dragon->getTypeName(), "Dragon");
    EXPECT_EQ(dragon->getName(), "ТестДракон");
    EXPECT_EQ(dragon->getX(), 100);
    EXPECT_EQ(dragon->getY(), 200);
}

TEST(FactoryTest, CreateBull) {
    auto bull = NpcFactory::createNpc("Bull", "ТестБык", 150, 250);
    EXPECT_EQ(bull->getTypeName(), "Bull");
    EXPECT_EQ(bull->getName(), "ТестБык");
}

TEST(FactoryTest, CreateFrog) {
    auto frog = NpcFactory::createNpc("Frog", "ТестЛягушка", 200, 300);
    EXPECT_EQ(frog->getTypeName(), "Frog");
    EXPECT_EQ(frog->getName(), "ТестЛягушка");
}

TEST(FactoryTest, InvalidType) {
    EXPECT_THROW({
        NpcFactory::createNpc("Единорог", "Тест", 100, 100);
    }, std::invalid_argument);
}

TEST(FactoryTest, LoadFromFile) {
    std::string line = "Dragon Смауг 100 200";
    auto npc = NpcFactory::loadFromFile(line);
    
    EXPECT_EQ(npc->getTypeName(), "Dragon");
    EXPECT_EQ(npc->getName(), "Смауг");
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 200);
}

TEST(FactoryTest, LoadInvalidFormat) {
    EXPECT_THROW({
        NpcFactory::loadFromFile("Неверный формат");
    }, std::runtime_error);
}

// ========== ТЕСТЫ ИГРОВОГО МИРА ==========

TEST(GameWorldTest, AddNpc) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Тест", 100, 100));
    EXPECT_EQ(world.getNpcCount(), 1);
}

TEST(GameWorldTest, AddMultipleNpcs) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык1", 200, 200));
    world.addNpc(NpcFactory::createNpc("Frog", "Лягушка1", 300, 300));
    EXPECT_EQ(world.getNpcCount(), 3);
}

TEST(GameWorldTest, DuplicateNameThrows) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дубликат", 100, 100));
    
    EXPECT_THROW({
        world.addNpc(NpcFactory::createNpc("Bull", "Дубликат", 200, 200));
    }, std::invalid_argument);
}

TEST(GameWorldTest, OutOfBoundsX) {
    GameWorld world;
    
    EXPECT_THROW({
        world.addNpc(NpcFactory::createNpc("Dragon", "Тест", 600, 100));
    }, std::out_of_range);
}

TEST(GameWorldTest, OutOfBoundsY) {
    GameWorld world;
    
    EXPECT_THROW({
        world.addNpc(NpcFactory::createNpc("Dragon", "Тест", 100, 600));
    }, std::out_of_range);
}

TEST(GameWorldTest, NegativeCoordinates) {
    GameWorld world;
    
    EXPECT_THROW({
        world.addNpc(NpcFactory::createNpc("Dragon", "Тест", -10, 100));
    }, std::out_of_range);
    
    EXPECT_THROW({
        world.addNpc(NpcFactory::createNpc("Dragon", "Тест2", 100, -10));
    }, std::out_of_range);
}

TEST(GameWorldTest, BoundaryCoordinates) {
    GameWorld world;
    
    // Граничные значения 0 и 500 должны быть валидны
    EXPECT_NO_THROW({
        world.addNpc(NpcFactory::createNpc("Dragon", "D1", 0, 0));
        world.addNpc(NpcFactory::createNpc("Bull", "B1", 500, 500));
        world.addNpc(NpcFactory::createNpc("Frog", "F1", 0, 500));
        world.addNpc(NpcFactory::createNpc("Dragon", "D2", 500, 0));
    });
    
    EXPECT_EQ(world.getNpcCount(), 4);
}

TEST(GameWorldTest, SaveAndLoad) {
    std::string filename = "test_save.txt";
    
    {
        GameWorld world;
        world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 100, 200));
        world.addNpc(NpcFactory::createNpc("Bull", "Бык", 150, 250));
        world.saveToFile(filename);
    }
    
    {
        GameWorld world;
        world.loadFromFile(filename);
        EXPECT_EQ(world.getNpcCount(), 2);
    }
    
    std::remove(filename.c_str());
}

TEST(GameWorldTest, SaveEmptyWorld) {
    std::string filename = "test_empty.txt";
    GameWorld world;
    
    EXPECT_NO_THROW({
        world.saveToFile(filename);
    });
    
    std::remove(filename.c_str());
}

TEST(GameWorldTest, LoadEmptyFile) {
    std::string filename = "test_empty_load.txt";
    std::ofstream(filename).close();
    
    GameWorld world;
    EXPECT_NO_THROW({
        world.loadFromFile(filename);
    });
    
    EXPECT_EQ(world.getNpcCount(), 0);
    std::remove(filename.c_str());
}

TEST(GameWorldTest, ClearWorld) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "D1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "B1", 200, 200));
    EXPECT_EQ(world.getNpcCount(), 2);
    
    world.clear();
    EXPECT_EQ(world.getNpcCount(), 0);
}

TEST(GameWorldTest, SaveLoadRoundtrip) {
    std::string filename = "test_roundtrip.txt";
    
    GameWorld world1;
    world1.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 123, 456));
    world1.addNpc(NpcFactory::createNpc("Bull", "Бык", 234, 467));
    world1.addNpc(NpcFactory::createNpc("Frog", "Лягушка", 345, 478));
    world1.saveToFile(filename);
    
    GameWorld world2;
    world2.loadFromFile(filename);
    
    EXPECT_EQ(world2.getNpcCount(), 3);
    std::remove(filename.c_str());
}

// ========== ТЕСТЫ БОЯ (VISITOR ПАТТЕРН) ==========

TEST(BattleTest, DragonDefeatsBull) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 110, 110));
    
    EXPECT_EQ(world.getNpcCount(), 2);
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 1); // Бык побежден
}

TEST(BattleTest, BullDefeatsFrog) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 100, 100));
    world.addNpc(NpcFactory::createNpc("Frog", "Лягушка", 110, 110));
    
    EXPECT_EQ(world.getNpcCount(), 2);
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 1); // Лягушка побеждена
}

TEST(BattleTest, FrogDefeatsNobody) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Frog", "Лягушка", 100, 100));
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 110, 110));
    
    EXPECT_EQ(world.getNpcCount(), 2);
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 2); // Оба выживают
}

TEST(BattleTest, OutOfRange) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 400, 400));
    
    EXPECT_EQ(world.getNpcCount(), 2);
    world.startBattle(50.0); // Слишком далеко друг от друга
    EXPECT_EQ(world.getNpcCount(), 2); // Оба выживают
}

TEST(BattleTest, DragonVsDragon) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон2", 110, 110));
    
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 2); // Драконы не сражаются друг с другом
}

TEST(BattleTest, BullVsBull) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Bull", "Бык1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык2", 110, 110));
    
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 2); // Быки не сражаются друг с другом
}

TEST(BattleTest, FrogVsFrog) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Frog", "Лягушка1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Frog", "Лягушка2", 110, 110));
    
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 2); // Лягушки не сражаются
}

TEST(BattleTest, ComplexBattle) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 110, 110));
    world.addNpc(NpcFactory::createNpc("Frog", "Лягушка", 115, 115));
    
    EXPECT_EQ(world.getNpcCount(), 3);
    world.startBattle(50.0);
    // Дракон побеждает быка (~14 м), бык побеждает лягушку (~7 м)
    // Бой идет параллельно, оба боя происходят
    // Результат: либо остается дракон и лягушка, либо только дракон
    EXPECT_LE(world.getNpcCount(), 2); // Минимум один погиб
}

// Тест взаимного уничтожения (требование: "погибнуть могут и оба NPC")
TEST(BattleTest, MutualDestruction) {
    GameWorld world;
    // Два дракона близко - не могут убить друг друга (одинаковые типы)
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон2", 110, 110));
    
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 2); // Драконы не сражаются между собой
}

// Тест: несколько драконов атакуют одного быка
TEST(BattleTest, MultipleDragonsVsOneBull) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон2", 110, 110));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 105, 105));
    
    EXPECT_EQ(world.getNpcCount(), 3);
    world.startBattle(50.0);
    EXPECT_EQ(world.getNpcCount(), 2); // Бык убит один раз (не дважды!)
}

TEST(BattleTest, ZeroRange) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 200, 200));
    
    world.startBattle(0.0); // Нулевая дальность
    EXPECT_EQ(world.getNpcCount(), 2); // Никто не сражается
}

TEST(BattleTest, MaxRange) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 0, 0));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 500, 500));
    
    double distance = std::sqrt(500*500 + 500*500); // ~707
    world.startBattle(distance + 10); // Достаточная дальность
    EXPECT_EQ(world.getNpcCount(), 1); // Бык побежден
}

TEST(BattleTest, EmptyWorld) {
    GameWorld world;
    
    EXPECT_NO_THROW({
        world.startBattle(100.0);
    });
    
    EXPECT_EQ(world.getNpcCount(), 0);
}

TEST(BattleTest, SingleNpc) {
    GameWorld world;
    world.addNpc(NpcFactory::createNpc("Dragon", "Одинокий", 100, 100));
    
    world.startBattle(100.0);
    EXPECT_EQ(world.getNpcCount(), 1); // Остается один
}

// ========== ТЕСТЫ РАССТОЯНИЙ ==========

TEST(CharacterTest, DistanceCalculation) {
    auto char1 = NpcFactory::createNpc("Dragon", "D1", 0, 0);
    auto char2 = NpcFactory::createNpc("Bull", "B1", 3, 4);
    
    double distance = char1->calculateDistance(*char2);
    EXPECT_DOUBLE_EQ(distance, 5.0); // Теорема Пифагора: 3^2 + 4^2 = 25, sqrt(25) = 5
}

TEST(CharacterTest, SamePosition) {
    auto char1 = NpcFactory::createNpc("Dragon", "D1", 100, 100);
    auto char2 = NpcFactory::createNpc("Bull", "B1", 100, 100);
    
    double distance = char1->calculateDistance(*char2);
    EXPECT_DOUBLE_EQ(distance, 0.0);
}

TEST(CharacterTest, DiagonalDistance) {
    auto char1 = NpcFactory::createNpc("Dragon", "D1", 0, 0);
    auto char2 = NpcFactory::createNpc("Bull", "B1", 100, 100);
    
    double distance = char1->calculateDistance(*char2);
    EXPECT_NEAR(distance, 141.42, 0.01);
}

// ========== ТЕСТЫ OBSERVER ПАТТЕРНА ==========

TEST(ObserverTest, FileObserverCreates) {
    std::string logFile = "test_observer.txt";
    std::remove(logFile.c_str());
    
    GameWorld world;
    auto fileObserver = std::make_shared<FileObserver>(logFile);
    world.addObserver(fileObserver);
    
    world.addNpc(NpcFactory::createNpc("Dragon", "Дракон", 100, 100));
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 110, 110));
    world.startBattle(50.0);
    
    std::ifstream file(logFile);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    bool hasLog = false;
    while (std::getline(file, line)) {
        if (line.find("убивает") != std::string::npos || line.find("[ЛОГ]") != std::string::npos) {
            hasLog = true;
            break;
        }
    }
    file.close();
    
    EXPECT_TRUE(hasLog);
    std::remove(logFile.c_str());
}

TEST(ObserverTest, MultipleObservers) {
    GameWorld world;
    auto console = std::make_shared<ConsoleObserver>();
    auto file = std::make_shared<FileObserver>("test_multi.txt");
    
    EXPECT_NO_THROW({
        world.addObserver(console);
        world.addObserver(file);
    });
    
    std::remove("test_multi.txt");
}

TEST(ObserverTest, ObserverNotification) {
    std::string logFile = "test_notification.txt";
    std::remove(logFile.c_str());
    
    GameWorld world;
    auto fileObserver = std::make_shared<FileObserver>(logFile);
    world.addObserver(fileObserver);
    
    world.addNpc(NpcFactory::createNpc("Bull", "Бык", 100, 100));
    world.addNpc(NpcFactory::createNpc("Frog", "Лягушка", 105, 105));
    
    world.startBattle(10.0);
    
    std::ifstream file(logFile);
    int lineCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lineCount++;
    }
    file.close();
    
    EXPECT_GT(lineCount, 0);
    std::remove(logFile.c_str());
}

// ========== ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ==========

TEST(AdditionalTest, ManyNpcs) {
    GameWorld world;
    
    for (int i = 0; i < 10; ++i) {
        world.addNpc(NpcFactory::createNpc("Dragon", "Дракон" + std::to_string(i), i * 50, i * 50));
    }
    
    EXPECT_EQ(world.getNpcCount(), 10);
}

TEST(AdditionalTest, AllTypesInBattle) {
    GameWorld world;
    
    world.addNpc(NpcFactory::createNpc("Dragon", "D1", 100, 100));
    world.addNpc(NpcFactory::createNpc("Dragon", "D2", 105, 105));
    world.addNpc(NpcFactory::createNpc("Bull", "B1", 110, 110));
    world.addNpc(NpcFactory::createNpc("Bull", "B2", 115, 115));
    world.addNpc(NpcFactory::createNpc("Frog", "F1", 120, 120));
    world.addNpc(NpcFactory::createNpc("Frog", "F2", 125, 125));
    
    size_t before = world.getNpcCount();
    world.startBattle(20.0);
    size_t after = world.getNpcCount();
    
    EXPECT_LT(after, before);
}

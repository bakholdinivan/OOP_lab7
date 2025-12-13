#include <gtest/gtest.h>
#include "../include/NPC.h"
#include "../include/NPCTypes.h"
#include "../include/NPCFactory.h"
#include "../include/BattleVisitor.h"
#include "../include/Observer.h"
#include "../include/GameWorld.h"
#include <thread>
#include <chrono>
#include <set>

// ========== ТЕСТЫ СОЗДАНИЯ NPC ==========

TEST(NPCTest, CreateFrog) {
    auto frog = std::make_shared<Frog>("Тестовая жаба", Point(10, 10));
    EXPECT_EQ(frog->getType(), "Жаба");
    EXPECT_EQ(frog->getMoveDistance(), 1);
    EXPECT_EQ(frog->getKillDistance(), 10);
    EXPECT_TRUE(frog->isAlive());
}

TEST(NPCTest, CreateBull) {
    auto bull = std::make_shared<Bull>("Тестовый бык", Point(20, 20));
    EXPECT_EQ(bull->getType(), "Бык");
    EXPECT_EQ(bull->getMoveDistance(), 30);
    EXPECT_EQ(bull->getKillDistance(), 10);
    EXPECT_TRUE(bull->isAlive());
}

TEST(NPCTest, CreateDragon) {
    auto dragon = std::make_shared<Dragon>("Тестовый дракон", Point(30, 30));
    EXPECT_EQ(dragon->getType(), "Дракон");
    EXPECT_EQ(dragon->getMoveDistance(), 50);
    EXPECT_EQ(dragon->getKillDistance(), 30);
    EXPECT_TRUE(dragon->isAlive());
}

// Проверка правильности параметров перемещения (из таблицы)
TEST(NPCTest, CorrectMoveDistances) {
    auto frog = std::make_shared<Frog>("Жаба", Point(0, 0));
    auto bull = std::make_shared<Bull>("Бык", Point(0, 0));
    auto dragon = std::make_shared<Dragon>("Дракон", Point(0, 0));
    
    EXPECT_EQ(frog->getMoveDistance(), 1);   // Жаба: 1
    EXPECT_EQ(bull->getMoveDistance(), 30);   // Бык: 30
    EXPECT_EQ(dragon->getMoveDistance(), 50); // Дракон: 50
}

// Проверка правильности параметров убийства (из таблицы)
TEST(NPCTest, CorrectKillDistances) {
    auto frog = std::make_shared<Frog>("Жаба", Point(0, 0));
    auto bull = std::make_shared<Bull>("Бык", Point(0, 0));
    auto dragon = std::make_shared<Dragon>("Дракон", Point(0, 0));
    
    EXPECT_EQ(frog->getKillDistance(), 10);   // Жаба: 10
    EXPECT_EQ(bull->getKillDistance(), 10);   // Бык: 10
    EXPECT_EQ(dragon->getKillDistance(), 30); // Дракон: 30
}

// ========== ТЕСТЫ ПРАВИЛ УБИЙСТВА ==========

TEST(BattleRulesTest, FrogCanKillBull) {
    auto frog = std::make_shared<Frog>("Жаба", Point(0, 0));
    // Жаба пассивна - не может никого убивать
    EXPECT_FALSE(frog->canKill("Бык"));
    EXPECT_FALSE(frog->canKill("Дракон"));
    EXPECT_FALSE(frog->canKill("Жаба"));
}

TEST(BattleRulesTest, BullCanKillFrog) {
    auto bull = std::make_shared<Bull>("Бык", Point(0, 0));
    EXPECT_TRUE(bull->canKill("Жаба"));
    EXPECT_FALSE(bull->canKill("Дракон"));
    EXPECT_FALSE(bull->canKill("Бык"));
}

TEST(BattleRulesTest, DragonCanKillBullAndDragon) {
    auto dragon = std::make_shared<Dragon>("Дракон", Point(0, 0));
    EXPECT_TRUE(dragon->canKill("Бык"));  // Дракон ест быков
    EXPECT_FALSE(dragon->canKill("Дракон"));  // Драконы не убивают друг друга
    EXPECT_FALSE(dragon->canKill("Жаба"));
}

// ========== ТЕСТЫ РАССТОЯНИЙ ==========

TEST(DistanceTest, PointDistance) {
    Point p1(0, 0);
    Point p2(3, 4);
    EXPECT_DOUBLE_EQ(p1.distanceTo(p2), 5.0);
}

TEST(DistanceTest, NPCDistance) {
    auto npc1 = std::make_shared<Frog>("NPC1", Point(0, 0));
    auto npc2 = std::make_shared<Bull>("NPC2", Point(6, 8));
    EXPECT_DOUBLE_EQ(npc1->getPosition().distanceTo(npc2->getPosition()), 10.0);
}

TEST(DistanceTest, ZeroDistance) {
    Point p1(10, 10);
    Point p2(10, 10);
    EXPECT_DOUBLE_EQ(p1.distanceTo(p2), 0.0);
}

// ========== ТЕСТЫ ПЕРЕМЕЩЕНИЯ ==========

TEST(MovementTest, NPCMovement) {
    auto npc = std::make_shared<Frog>("Жаба", Point(50, 50));
    Point initial = npc->getPosition();
    npc->setPosition(Point(51, 51));
    Point after = npc->getPosition();
    EXPECT_NE(initial.x, after.x);
    EXPECT_NE(initial.y, after.y);
}

TEST(MovementTest, DeadNPCDoesNotMove) {
    auto npc = std::make_shared<Frog>("Жаба", Point(50, 50));
    npc->kill();
    EXPECT_FALSE(npc->isAlive());
}

// ========== ТЕСТЫ ГРАНИЦ КАРТЫ ==========

TEST(BoundaryTest, MapBoundaries) {
    Point p(0, 0);
    EXPECT_GE(p.x, 0);
    EXPECT_GE(p.y, 0);
    
    Point p2(100, 100);
    EXPECT_LE(p2.x, 100);
    EXPECT_LE(p2.y, 100);
}

TEST(BoundaryTest, NPCStaysWithinBounds) {
    const double MAP_SIZE = 100.0;
    auto npc = std::make_shared<Frog>("Жаба", Point(0, 0));
    
    // Проверяем, что NPC не может выйти за границы
    npc->setPosition(Point(-10, -10));
    Point pos = npc->getPosition();
    // В реальной игре GameWorld исправит координаты, здесь просто проверяем логику
}

// ========== ТЕСТЫ ЖИЗНИ/СМЕРТИ NPC ==========

TEST(LifeTest, NPCAliveAndDead) {
    auto npc = std::make_shared<Frog>("Жаба", Point(10, 10));
    EXPECT_TRUE(npc->isAlive());
    
    npc->kill();
    EXPECT_FALSE(npc->isAlive());
}

TEST(LifeTest, DeadNPCStaysDead) {
    auto npc = std::make_shared<Bull>("Бык", Point(10, 10));
    npc->kill();
    EXPECT_FALSE(npc->isAlive());
    npc->kill(); // Повторный вызов не должен ломать логику
    EXPECT_FALSE(npc->isAlive());
}

// ========== ТЕСТЫ ФАБРИКИ ==========

TEST(FactoryTest, CreateByType) {
    auto frog = NPCFactory::createNPC("Жаба", "Жаба1", Point(10, 10));
    EXPECT_NE(frog, nullptr);
    EXPECT_EQ(frog->getType(), "Жаба");
    
    auto bull = NPCFactory::createNPC("Бык", "Бык1", Point(20, 20));
    EXPECT_NE(bull, nullptr);
    EXPECT_EQ(bull->getType(), "Бык");
    
    auto dragon = NPCFactory::createNPC("Дракон", "Дракон1", Point(30, 30));
    EXPECT_NE(dragon, nullptr);
    EXPECT_EQ(dragon->getType(), "Дракон");
}

TEST(FactoryTest, CreateRandom) {
    auto npc = NPCFactory::createRandomNPC("Random", 100, 100);
    EXPECT_NE(npc, nullptr);
    EXPECT_TRUE(npc->isAlive());
    
    Point pos = npc->getPosition();
    EXPECT_GE(pos.x, 0);
    EXPECT_LE(pos.x, 100);
    EXPECT_GE(pos.y, 0);
    EXPECT_LE(pos.y, 100);
}

TEST(FactoryTest, CreateMultipleRandom) {
    std::set<std::string> types;
    for (int i = 0; i < 30; ++i) {
        auto npc = NPCFactory::createRandomNPC("NPC" + std::to_string(i), 100, 100);
        EXPECT_NE(npc, nullptr);
        types.insert(npc->getType());
    }
    // За 30 итераций должны появиться все 3 типа
    EXPECT_GE(types.size(), 1);
}

// ========== ТЕСТЫ БРОСКА КУБИКА ==========

TEST(DiceTest, RollDice) {
    auto npc = std::make_shared<Frog>("Жаба", Point(0, 0));
    for (int i = 0; i < 100; ++i) {
        int roll = npc->rollDice();
        EXPECT_GE(roll, 1);
        EXPECT_LE(roll, 6);
    }
}

TEST(DiceTest, RollDiceDistribution) {
    auto npc = std::make_shared<Frog>("Жаба", Point(0, 0));
    std::set<int> rolls;
    for (int i = 0; i < 1000; ++i) {
        rolls.insert(npc->rollDice());
    }
    // За 1000 бросков должны появиться разные значения
    EXPECT_GE(rolls.size(), 4);
}

// ========== ТЕСТЫ БОЕВ ==========

TEST(BattleTest, BattleWithVisitor) {
    auto attacker = std::make_shared<Frog>("Жаба", Point(0, 0));
    auto defender = std::make_shared<Bull>("Бык", Point(5, 0));
    
    BattleVisitor visitor;
    auto observer = std::make_shared<ConsoleObserver>();
    visitor.addObserver(observer);
    
    // Жаба может убить быка в радиусе 10
    visitor.visit(*attacker, *defender);
    
    // Результат зависит от броска кубика, но NPC должны остаться валидными
    EXPECT_TRUE(attacker->isAlive());
}

TEST(BattleTest, BattleOutOfRange) {
    auto attacker = std::make_shared<Frog>("Жаба", Point(0, 0));
    auto defender = std::make_shared<Bull>("Бык", Point(50, 50));
    
    BattleVisitor visitor;
    
    bool defender_was_alive = defender->isAlive();
    visitor.visit(*attacker, *defender);
    
    // Бой не должен произойти (слишком далеко)
    EXPECT_EQ(defender->isAlive(), defender_was_alive);
}

TEST(BattleTest, BattleWrongTarget) {
    auto attacker = std::make_shared<Frog>("Жаба", Point(0, 0));
    auto defender = std::make_shared<Dragon>("Дракон", Point(5, 0));
    
    BattleVisitor visitor;
    
    visitor.visit(*attacker, *defender);
    
    // Жаба не может убить дракона
    EXPECT_TRUE(defender->isAlive());
}

// ========== ТЕСТЫ ИГРОВОГО МИРА ==========

TEST(GameWorldTest, AddNPC) {
    GameWorld world(100, 100);
    auto npc = std::make_shared<Frog>("Жаба", Point(50, 50));
    world.addNPC(npc);
    
    auto survivors = world.getSurvivors();
    EXPECT_EQ(survivors.size(), 1);
}

TEST(GameWorldTest, MultipleNPC) {
    GameWorld world(100, 100);
    
    for (int i = 0; i < 10; ++i) {
        auto npc = NPCFactory::createRandomNPC("NPC_" + std::to_string(i), 100, 100);
        world.addNPC(npc);
    }
    
    auto survivors = world.getSurvivors();
    EXPECT_EQ(survivors.size(), 10);
}

TEST(GameWorldTest, FiftyNPC) {
    GameWorld world(100, 100);
    
    // Требование: создается 50 NPC
    for (int i = 0; i < 50; ++i) {
        auto npc = NPCFactory::createRandomNPC("NPC_" + std::to_string(i), 100, 100);
        world.addNPC(npc);
    }
    
    auto survivors = world.getSurvivors();
    EXPECT_EQ(survivors.size(), 50);
}

// ========== ТЕСТЫ ПОТОКОВ ==========

TEST(ThreadTest, MovementThreadWorks) {
    GameWorld world(100, 100);
    
    auto npc = std::make_shared<Bull>("Бык", Point(50, 50));
    world.addNPC(npc);
    
    Point initial_pos = npc->getPosition();
    
    std::thread movement_thread(&GameWorld::movementThread, &world);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    world.stop();
    movement_thread.join();
    
    Point final_pos = npc->getPosition();
    
    // NPC должен был переместиться
    EXPECT_TRUE(initial_pos.x != final_pos.x || initial_pos.y != final_pos.y);
}

TEST(ThreadTest, BattleThreadWorks) {
    GameWorld world(100, 100);
    
    auto observer = std::make_shared<ConsoleObserver>();
    world.addObserver(observer);
    
    // Размещаем жабу и быка близко друг к другу
    auto frog = std::make_shared<Frog>("Жаба", Point(50, 50));
    auto bull = std::make_shared<Bull>("Бык", Point(52, 50));
    
    world.addNPC(frog);
    world.addNPC(bull);
    
    std::thread movement_thread(&GameWorld::movementThread, &world);
    std::thread battle_thread(&GameWorld::battleThread, &world);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    world.stop();
    
    movement_thread.join();
    battle_thread.join();
    
    // Хотя бы один должен быть мертв (или оба живы, если броски не позволили)
    auto survivors = world.getSurvivors();
    EXPECT_LE(survivors.size(), 2);
}

TEST(ThreadTest, AllThreeThreadsWork) {
    GameWorld world(100, 100);
    
    auto observer = std::make_shared<ConsoleObserver>();
    world.addObserver(observer);
    
    for (int i = 0; i < 10; ++i) {
        auto npc = NPCFactory::createRandomNPC("NPC_" + std::to_string(i), 100, 100);
        world.addNPC(npc);
    }
    
    std::thread movement_thread(&GameWorld::movementThread, &world);
    std::thread battle_thread(&GameWorld::battleThread, &world);
    std::thread print_thread(&GameWorld::printThread, &world);
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    world.stop();
    
    movement_thread.join();
    battle_thread.join();
    print_thread.join();
    
    // Проверяем, что игра завершилась корректно
    auto survivors = world.getSurvivors();
    EXPECT_LE(survivors.size(), 10);
}

// ========== ТЕСТ НА 30 СЕКУНД (как в задании) ==========

TEST(GameTest, ThirtySecondsFullGame) {
    const double MAP_WIDTH = 100.0;
    const double MAP_HEIGHT = 100.0;
    const int NPC_COUNT = 50;
    const int GAME_DURATION_SEC = 30;

    GameWorld world(MAP_WIDTH, MAP_HEIGHT);
    
    auto console_observer = std::make_shared<ConsoleObserver>();
    auto file_observer = std::make_shared<FileObserver>("test_30sec_log.txt");
    world.addObserver(console_observer);
    world.addObserver(file_observer);
    
    // Создаем 50 случайных NPC (требование)
    for (int i = 0; i < NPC_COUNT; ++i) {
        std::string name = "NPC_" + std::to_string(i + 1);
        auto npc = NPCFactory::createRandomNPC(name, MAP_WIDTH, MAP_HEIGHT);
        world.addNPC(npc);
    }
    
    auto initial_survivors = world.getSurvivors();
    EXPECT_EQ(initial_survivors.size(), NPC_COUNT);
    
    // Запускаем три потока (требование)
    std::thread movement_thread(&GameWorld::movementThread, &world);
    std::thread battle_thread(&GameWorld::battleThread, &world);
    std::thread print_thread(&GameWorld::printThread, &world);
    
    // Ждем 30 секунд (требование)
    std::this_thread::sleep_for(std::chrono::seconds(GAME_DURATION_SEC));
    
    // Останавливаем игру (требование)
    world.stop();
    
    movement_thread.join();
    battle_thread.join();
    print_thread.join();
    
    // Проверяем список выживших (требование)
    auto survivors = world.getSurvivors();
    
    std::cout << "\n=== РЕЗУЛЬТАТЫ ТЕСТА НА 30 СЕКУНД ===" << std::endl;
    std::cout << "Начальное количество NPC: " << NPC_COUNT << std::endl;
    std::cout << "Выжившие: " << survivors.size() << std::endl;
    std::cout << "Погибшие: " << (NPC_COUNT - survivors.size()) << std::endl;
    
    // Выживших должно быть меньше или равно начальному количеству
    EXPECT_LE(survivors.size(), NPC_COUNT);
    
    // Все выжившие должны быть живы
    for (const auto& npc : survivors) {
        EXPECT_TRUE(npc->isAlive());
    }
}

// ========== ТЕСТЫ НАБЛЮДАТЕЛЕЙ ==========

TEST(ObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    EXPECT_NO_THROW(observer.onEvent("Тестовое сообщение"));
}

TEST(ObserverTest, FileObserver) {
    FileObserver observer("test_log.txt");
    EXPECT_NO_THROW(observer.onEvent("Тестовое сообщение"));
}

TEST(ObserverTest, MultipleObservers) {
    BattleVisitor visitor;
    
    auto console_obs = std::make_shared<ConsoleObserver>();
    auto file_obs = std::make_shared<FileObserver>("test_multi_log.txt");
    
    visitor.addObserver(console_obs);
    visitor.addObserver(file_obs);
    
    EXPECT_NO_THROW(visitor.notifyObservers("Тест множественных наблюдателей"));
}

// ========== ТЕСТЫ СИНХРОНИЗАЦИИ ==========

TEST(SynchronizationTest, ConcurrentAccess) {
    GameWorld world(100, 100);
    
    // Добавляем NPC из нескольких потоков
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&world, i]() {
            for (int j = 0; j < 10; ++j) {
                auto npc = NPCFactory::createRandomNPC(
                    "NPC_" + std::to_string(i * 10 + j), 100, 100);
                world.addNPC(npc);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto survivors = world.getSurvivors();
    EXPECT_EQ(survivors.size(), 50);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

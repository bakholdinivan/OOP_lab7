#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include "NPC.h"
#include "Dragon.h"
#include "Bull.h"
#include "Frog.h"
#include "NPCFactory.h"
#include "BattleVisitor.h"
#include "Observer.h"
#include "Editor.h"

//
TEST(NPCTest, DragonCreation) {
    Dragon dragon("Smaug", 100, 200);
    EXPECT_EQ(dragon.getName(), "Smaug");
    EXPECT_EQ(dragon.getX(), 100);
    EXPECT_EQ(dragon.getY(), 200);
    EXPECT_EQ(dragon.getType(), "Dragon");
    EXPECT_TRUE(dragon.isAlive());
}

TEST(NPCTest, BullCreation) {
    Bull bull("Ferdinand", 50, 75);
    EXPECT_EQ(bull.getName(), "Ferdinand");
    EXPECT_EQ(bull.getType(), "Bull");
    EXPECT_TRUE(bull.isAlive());
}

TEST(NPCTest, FrogCreation) {
    Frog frog("Kermit", 25, 30);
    EXPECT_EQ(frog.getName(), "Kermit");
    EXPECT_EQ(frog.getType(), "Frog");
    EXPECT_TRUE(frog.isAlive());
}

//
TEST(NPCTest, DistanceCalculationZero) {
    Dragon d1("D1", 100, 100);
    Dragon d2("D2", 100, 100);
    EXPECT_DOUBLE_EQ(d1.distanceTo(d2), 0.0);
}

TEST(NPCTest, DistanceCalculationHorizontal) {
    Dragon d1("D1", 0, 0);
    Bull b1("B1", 3, 0);
    EXPECT_DOUBLE_EQ(d1.distanceTo(b1), 3.0);
}

TEST(NPCTest, DistanceCalculationVertical) {
    Frog f1("F1", 0, 0);
    Frog f2("F2", 0, 4);
    EXPECT_DOUBLE_EQ(f1.distanceTo(f2), 4.0);
}

TEST(NPCTest, DistanceCalculationDiagonal) {
    Dragon d("D", 0, 0);
    Bull b("B", 3, 4);
    EXPECT_DOUBLE_EQ(d.distanceTo(b), 5.0);
}

//
TEST(NPCTest, KillNPC) {
    Dragon dragon("D", 100, 100);
    EXPECT_TRUE(dragon.isAlive());
    dragon.kill();
    EXPECT_FALSE(dragon.isAlive());
}

//
TEST(FactoryTest, CreateDragon) {
    auto npc = NPCFactory::createNPC("Dragon", "Test", 10, 20);
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Dragon");
    EXPECT_EQ(npc->getName(), "Test");
}

TEST(FactoryTest, CreateBull) {
    auto npc = NPCFactory::createNPC("Bull", "Test", 10, 20);
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Bull");
}

TEST(FactoryTest, CreateFrog) {
    auto npc = NPCFactory::createNPC("Frog", "Test", 10, 20);
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Frog");
}

TEST(FactoryTest, CreateInvalidType) {
    auto npc = NPCFactory::createNPC("InvalidType", "Test", 10, 20);
    EXPECT_EQ(npc, nullptr);
}

TEST(FactoryTest, LoadFromStringDragon) {
    auto npc = NPCFactory::loadFromString("Dragon TestDragon 100 200");
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Dragon");
    EXPECT_EQ(npc->getName(), "TestDragon");
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 200);
}

TEST(FactoryTest, LoadFromStringBull) {
    auto npc = NPCFactory::loadFromString("Bull BullName 50 75");
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Bull");
}

TEST(FactoryTest, LoadFromStringInvalid) {
    auto npc = NPCFactory::loadFromString("Invalid");
    EXPECT_EQ(npc, nullptr);
}

//
TEST(BattleTest, DragonKillsBull) {
    Dragon dragon("D", 0, 0);
    Bull bull("B", 0, 0);
    BattleVisitor visitor;
    
    dragon.accept(visitor, bull);
    
    EXPECT_TRUE(dragon.isAlive());
    EXPECT_FALSE(bull.isAlive());
}

TEST(BattleTest, BullKillsFrog) {
    Bull bull("B", 0, 0);
    Frog frog("F", 0, 0);
    BattleVisitor visitor;
    
    bull.accept(visitor, frog);
    
    EXPECT_TRUE(bull.isAlive());
    EXPECT_FALSE(frog.isAlive());
}

TEST(BattleTest, FrogSurvivesDragon) {
    Dragon dragon("D", 0, 0);
    Frog frog("F", 0, 0);
    BattleVisitor visitor;
    
    frog.accept(visitor, dragon);
    
    EXPECT_TRUE(dragon.isAlive());
    EXPECT_TRUE(frog.isAlive());
}

TEST(BattleTest, BullCannotKillDragon) {
    Bull bull("B", 0, 0);
    Dragon dragon("D", 0, 0);
    BattleVisitor visitor;
    
    bull.accept(visitor, dragon);
    
    EXPECT_TRUE(bull.isAlive());
    EXPECT_TRUE(dragon.isAlive());
}

TEST(BattleTest, TwoDragonsKillEachOther) {
    Dragon d1("D1", 0, 0);
    Dragon d2("D2", 0, 0);
    BattleVisitor visitor;
    
    d1.accept(visitor, d2);
    
    EXPECT_FALSE(d1.isAlive());
    EXPECT_FALSE(d2.isAlive());
}

TEST(BattleTest, TwoBullsKillEachOther) {
    Bull b1("B1", 0, 0);
    Bull b2("B2", 0, 0);
    BattleVisitor visitor;
    
    b1.accept(visitor, b2);
    
    EXPECT_FALSE(b1.isAlive());
    EXPECT_FALSE(b2.isAlive());
}

TEST(BattleTest, TwoFrogsDoNotFight) {
    Frog f1("F1", 0, 0);
    Frog f2("F2", 0, 0);
    BattleVisitor visitor;
    
    f1.accept(visitor, f2);
    
    EXPECT_TRUE(f1.isAlive());
    EXPECT_TRUE(f2.isAlive());
}

//
TEST(EditorTest, AddNPCValid) {
    Editor editor;
    auto npc = std::make_shared<Dragon>("Test", 100, 100);
    EXPECT_TRUE(editor.addNPC(npc));
    EXPECT_EQ(editor.getNPCCount(), 1);
}

TEST(EditorTest, AddNPCInvalidCoordinatesNegative) {
    Editor editor;
    auto npc = std::make_shared<Dragon>("Test", -10, 100);
    EXPECT_FALSE(editor.addNPC(npc));
    EXPECT_EQ(editor.getNPCCount(), 0);
}

TEST(EditorTest, AddNPCInvalidCoordinatesTooLarge) {
    Editor editor;
    auto npc = std::make_shared<Dragon>("Test", 600, 100);
    EXPECT_FALSE(editor.addNPC(npc));
    EXPECT_EQ(editor.getNPCCount(), 0);
}

TEST(EditorTest, AddNPCDuplicateName) {
    Editor editor;
    auto npc1 = std::make_shared<Dragon>("Same", 100, 100);
    auto npc2 = std::make_shared<Bull>("Same", 200, 200);
    
    EXPECT_TRUE(editor.addNPC(npc1));
    EXPECT_FALSE(editor.addNPC(npc2));
    EXPECT_EQ(editor.getNPCCount(), 1);
}

TEST(EditorTest, IsNameUniqueEmpty) {
    Editor editor;
    EXPECT_TRUE(editor.isNameUnique("AnyName"));
}

TEST(EditorTest, IsNameUniqueAfterAdd) {
    Editor editor;
    auto npc = std::make_shared<Dragon>("Test", 100, 100);
    editor.addNPC(npc);
    
    EXPECT_FALSE(editor.isNameUnique("Test"));
    EXPECT_TRUE(editor.isNameUnique("Other"));
}

TEST(EditorTest, SaveAndLoadFile) {
    Editor editor1;
    editor1.addNPC(std::make_shared<Dragon>("D1", 100, 150));
    editor1.addNPC(std::make_shared<Bull>("B1", 200, 250));
    
    EXPECT_TRUE(editor1.saveToFile("test_save.txt"));
    
    Editor editor2;
    EXPECT_TRUE(editor2.loadFromFile("test_save.txt"));
    EXPECT_EQ(editor2.getNPCCount(), 2);
}

TEST(EditorTest, SaveEmptyEditor) {
    Editor editor;
    EXPECT_TRUE(editor.saveToFile("test_empty.txt"));
}

TEST(EditorTest, LoadNonexistentFile) {
    Editor editor;
    EXPECT_FALSE(editor.loadFromFile("nonexistent_file_12345.txt"));
}

TEST(EditorTest, RemoveDeadNPCs) {
    Editor editor;
    auto d = std::make_shared<Dragon>("D", 100, 100);
    auto b = std::make_shared<Bull>("B", 200, 200);
    
    editor.addNPC(d);
    editor.addNPC(b);
    
    d->kill();
    
    EXPECT_EQ(editor.getNPCCount(), 2);
    editor.removeDeadNPCs();
    EXPECT_EQ(editor.getNPCCount(), 1);
}

TEST(EditorTest, BattleWithRange) {
    Editor editor;
    editor.addNPC(std::make_shared<Dragon>("D", 0, 0));
    editor.addNPC(std::make_shared<Bull>("B", 3, 4));
    
    BattleVisitor visitor;
    
    editor.startBattle(10.0, visitor);
    editor.removeDeadNPCs();
    
    EXPECT_EQ(editor.getNPCCount(), 1);  // Бык должен погибнуть
}

TEST(EditorTest, BattleOutOfRange) {
    Editor editor;
    editor.addNPC(std::make_shared<Dragon>("D", 0, 0));
    editor.addNPC(std::make_shared<Bull>("B", 100, 100));
    
    BattleVisitor visitor;
    
    editor.startBattle(10.0, visitor);  // Дистанция недостаточна
    editor.removeDeadNPCs();
    
    EXPECT_EQ(editor.getNPCCount(), 2);  // Оба живы
}

TEST(EditorTest, ClearEditor) {
    Editor editor;
    editor.addNPC(std::make_shared<Dragon>("D", 100, 100));
    editor.addNPC(std::make_shared<Bull>("B", 200, 200));
    
    EXPECT_EQ(editor.getNPCCount(), 2);
    editor.clear();
    EXPECT_EQ(editor.getNPCCount(), 0);
}

TEST(EditorTest, GetNPCValid) {
    Editor editor;
    auto npc = std::make_shared<Dragon>("D", 100, 100);
    editor.addNPC(npc);
    
    auto retrieved = editor.getNPC(0);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->getName(), "D");
}

TEST(EditorTest, GetNPCInvalidIndex) {
    Editor editor;
    auto npc = editor.getNPC(100);
    EXPECT_EQ(npc, nullptr);
}

// Тесты для Observer
TEST(ObserverTest, FileObserverCreation) {
    FileObserver observer("test_observer.log");
    observer.onKill("Attacker", "Victim");
    
    std::ifstream file("test_observer.log");
    EXPECT_TRUE(file.is_open());
    file.close();
}

// Дополнительные граничные тесты
TEST(EditorTest, AddNPCAtBoundary) {
    Editor editor;
    auto npc1 = std::make_shared<Dragon>("D1", 0, 0);
    auto npc2 = std::make_shared<Dragon>("D2", 500, 500);
    
    EXPECT_TRUE(editor.addNPC(npc1));
    EXPECT_TRUE(editor.addNPC(npc2));
}

TEST(EditorTest, AddNPCJustOverBoundary) {
    Editor editor;
    auto npc = std::make_shared<Dragon>("D", 500.1, 500.1);
    EXPECT_FALSE(editor.addNPC(npc));
}

TEST(BattleTest, MultipleNPCsBattle) {
    Editor editor;
    editor.addNPC(std::make_shared<Dragon>("D1", 0, 0));
    editor.addNPC(std::make_shared<Dragon>("D2", 1, 1));
    editor.addNPC(std::make_shared<Bull>("B1", 2, 2));
    editor.addNPC(std::make_shared<Frog>("F1", 3, 3));
    
    BattleVisitor visitor;
    editor.startBattle(10.0, visitor);
    
    // Драконы убьют друг друга, дракон убьет быка
    editor.removeDeadNPCs();
    EXPECT_GT(editor.getNPCCount(), 0);  // Жаба должна выжить
}

TEST(NPCTest, ToStringFormat) {
    Dragon dragon("TestDragon", 123, 456);
    std::string str = dragon.toString();
    EXPECT_NE(str.find("Dragon"), std::string::npos);
    EXPECT_NE(str.find("TestDragon"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

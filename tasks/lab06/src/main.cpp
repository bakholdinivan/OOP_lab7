#include <iostream>
#include <memory>
#include "Editor.h"
#include "NPCFactory.h"
#include "Observer.h"

void showMenu() {
    std::cout << "\n=== Balagur Fate 3 — Редактор подземелья ===" << std::endl;
    std::cout << "1. Добавить NPC" << std::endl;
    std::cout << "2. Показать всех NPC" << std::endl;
    std::cout << "3. Сохранить в файл" << std::endl;
    std::cout << "4. Загрузить из файла" << std::endl;
    std::cout << "5. Боевой режим" << std::endl;
    std::cout << "6. Удалить мёртвых NPC" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Ваш выбор: ";
}

int main() {
    Editor editor;
    BattleVisitor visitor;
    
    visitor.addObserver(std::make_shared<ConsoleObserver>());
    visitor.addObserver(std::make_shared<FileObserver>("log.txt"));
    
    int choice;
    
    while (true) {
        showMenu();
        std::cin >> choice;
        
        if (choice == 0) {
            std::cout << "Выход..." << std::endl;
            break;
        }
        
        switch (choice) {
            case 1: {
                std::string type, name;
                double x, y;
                std::cout << "Введите тип (Dragon/Bull/Frog): ";
                std::cin >> type;
                std::cout << "Введите имя: ";
                std::cin >> name;
                std::cout << "Введите координаты (x y): ";
                std::cin >> x >> y;
                auto npc = NPCFactory::createNPC(type, name, x, y);
                if (npc && editor.addNPC(npc)) {
                    std::cout << "NPC успешно добавлен!" << std::endl;
                } else {
                    std::cout << "Ошибка добавления NPC. Проверьте координаты (0-500) и уникальность имени." << std::endl;
                }
                break;
            }
            
            case 2: {
                editor.printAll();
                break;
            }
            
            case 3: {
                std::string filename;
                std::cout << "Введите имя файла: ";
                std::cin >> filename;
                if (editor.saveToFile(filename)) {
                    std::cout << "Сохранено успешно!" << std::endl;
                } else {
                    std::cout << "Ошибка сохранения." << std::endl;
                }
                break;
            }
            
            case 4: {
                std::string filename;
                std::cout << "Введите имя файла: ";
                std::cin >> filename;
                if (editor.loadFromFile(filename)) {
                    std::cout << "Загружено успешно!" << std::endl;
                } else {
                    std::cout << "Ошибка загрузки." << std::endl;
                }
                break;
            }
            
            case 5: {
                double range;
                std::cout << "Введите радиус боя: ";
                std::cin >> range;
                std::cout << "\nБой начался..." << std::endl;
                editor.startBattle(range, visitor);
                std::cout << "Бой завершён!" << std::endl;
                break;
            }
            
            case 6: {
                editor.removeDeadNPCs();
                std::cout << "Мёртвые NPC удалены." << std::endl;
                break;
            }
            
            default:
                std::cout << "Некорректный выбор!" << std::endl;
        }
    }
    
    return 0;
}

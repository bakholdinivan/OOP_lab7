#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

/**
 * Интерфейс Наблюдателя - паттерн Observer
 * Получает уведомления о событиях в игровом мире
 */
class Observer {
public:
    virtual ~Observer() = default;
    
    // Обработка события (например, убийство персонажа)
    virtual void notify(const std::string& event) = 0;
};

#endif // OBSERVER_H
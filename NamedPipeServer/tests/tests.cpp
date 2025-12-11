#include <cstdio>
#include <cstring>
#include <cstdlib>

// Подключаем заголовок. Путь уже настроен в CMake.
#include "testrunner.h"

#include "../common.h"

// --- Mock-функции (Ваш код без изменений) ---

struct employee create_employee(int id, const char* name, double hours) {
    struct employee e;
    e.num = id;
    strncpy(e.name, name, 9);
    e.name[9] = '\0';
    e.hours = hours;
    return e;
}

// Глобальная переменная для теста блокировок
int readers_count = 0;
int is_writing = 0;

int try_read_lock() {
    if (is_writing) return 0; // Fail
    readers_count++;
    return 1; // Success
}

int try_write_lock() {
    if (is_writing || readers_count > 0) return 0; // Fail
    is_writing = 1;
    return 1; // Success
}

void unlock_read() { readers_count--; }
void unlock_write() { is_writing = 0; }

// --- ТЕСТЫ (Обновленный синтаксис) ---

// Тест 1: Проверка структуры данных
TR_TEST(test_employee_struct) {
    struct employee e = create_employee(10, "Ivan", 40.5);

    // В новой версии TR_ASSERT требует указатель 't' (он создается макросом TR_TEST неявно)
    TR_ASSERT(t, e.num == 10);
    TR_ASSERT(t, strcmp(e.name, "Ivan") == 0);
    TR_ASSERT(t, e.hours == 40.5);
}

// Тест 2: Проверка логики читателей-писателей
TR_TEST(test_locking_logic) {
    readers_count = 0;
    is_writing = 0;

    TR_ASSERT(t, try_read_lock() == 1);
    TR_ASSERT(t, readers_count == 1);

    TR_ASSERT(t, try_write_lock() == 0);

    unlock_read();
    TR_ASSERT(t, readers_count == 0);

    TR_ASSERT(t, try_write_lock() == 1);
    TR_ASSERT(t, is_writing == 1);

    TR_ASSERT(t, try_read_lock() == 0);

    unlock_write();
    TR_ASSERT(t, is_writing == 0);
}

// --- MAIN (Обновленный API) ---

int main(int argc, char **argv) {
    // 1. Получаем конфигурацию
    auto config = trun::ITestingConfig::GetInstance();

    // 2. Создаем Runner
    trun::TestRunner runner(config);

    // 3. Регистрируем тесты
    // ("ИмяМодуля", "ИмяТеста", ФункцияТеста)
    runner.AddTest("DataStructs", "Employee", test_employee_struct);
    runner.AddTest("Concurrency", "Locks", test_locking_logic);

    // 4. Запускаем
    runner.Run(argc, argv);
    
    return 0;
}
#include <iostream>
#include <list>
#include <string>
#include <limits> // Для std::numeric_limits

class Task {
private:
    std::string description;
    std::string status;

public:
    Task(const std::string& desc, const std::string& stat) : description(desc), status(stat) {}

    std::string getDescription() const {
        return description;
    }

    std::string getStatus() const {
        return status;
    }

    void setDescription(const std::string& desc) {
        description = desc;
    }

    void setStatus(const std::string& stat) {
        status = stat;
    }
};

class TaskList {
private:
    std::list<Task> tasks;

public:
    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    void removeTask(int index) {
        auto it = tasks.begin();
        std::advance(it, index);
        tasks.erase(it);
    }

    void displayTasks(std::string msg) const {
        std::cout << "\033[2J\033[H";
        std::cout << msg;
        int index = 1;
        for (const auto& task : tasks) {
            std::cout << index << ". " << task.getDescription() << " - " << task.getStatus() << std::endl;
            index++;
        }
    }

    int getSize() const {
        return tasks.size();
    }

    Task& getTask(int index) {
        auto it = tasks.begin();
        std::advance(it, index);
        return *it;
    }
};

class Menu {
public:
    static void showMainMenu() {
        std::cout << "\nЧто бы вы хотели сделать?\n";
        std::cout << "1. Добавить новую задачу\n";
        std::cout << "2. Изменить статус задачи\n";
        std::cout << "3. Удалить задачу\n";
        std::cout << "4. Выйти из программы\n";
        std::cout << "Выберите действие (1-4): ";
    }

    static int getUserChoice() {
        int choice;
        std::cin >> choice;
        return choice;
    }
};

int main11() {
    system("chcp 1251 > nul");
    TaskList taskList;
    long long (*pmax)() = std::numeric_limits<std::streamsize>::max;

    // Добавляем несколько начальных задач для примера
    taskList.addTask(Task("Разработать интерфейс", "в очереди"));
    taskList.addTask(Task("Написать документацию", "в работе"));
    taskList.addTask(Task("Провести тестирование", "в очереди"));

    std::string msg = "Добро пожаловать в ваш todo лист!\n\n";

    // Основной цикл программы
    while (true) {
        
        taskList.displayTasks(msg + "Ваши текущие задачи:\n");

        Menu::showMainMenu();
        int choice = Menu::getUserChoice();
        switch (choice) {
        case 1: {
            std::string desc;
            std::cout << "Введите описание новой задачи: ";
            std::cin.ignore(pmax(), '\n'); // Очистка буфера ввода
            std::getline(std::cin, desc);
            taskList.addTask(Task(desc, "в очереди"));
            msg = "Новая задача добавлена!\n\n";
            break;
        }
        case 2: {
            std::cout << "Введите номер задачи, статус которой хотите изменить: ";
            int index;
            std::cin >> index;
            if (index < 1 || index > taskList.getSize()) {
                msg = "Неверный номер задачи.\n\n";
                std::cin.clear();
                std::cin.ignore(pmax(), '\n'); // Очистка буфера ввода
                break;
            }
            std::cout << "Введите новый статус для задачи (в очереди, в работе, выполнено): ";
            std::string newStatus;
            std::cin.ignore(pmax(), '\n'); // Очистка буфера ввода
            std::getline(std::cin, newStatus); // Считываем всю строку
            taskList.getTask(index - 1).setStatus(newStatus);
            msg = "Статус задачи успешно изменен!\n\n";
            break;
        }
        case 3: {
            std::cout << "Введите номер задачи, которую хотите удалить: ";
            int index;
            std::cin >> index;
            if (index < 1 || index > taskList.getSize()) {
                msg = "Неверный номер задачи.\n\n";
                std::cin.clear();
                std::cin.ignore(pmax(), '\n'); // Очистка буфера ввода
                break;
            }
            taskList.removeTask(index - 1);
            msg = "Задача успешно удалена!\n\n";
            break;
        }
        case 4:
            std::cout << "\033[2J\033[H";
            std::cout << "До свидания!\n\n";
            return 0; // Выходим из программы
        default:
            msg = "Неверный выбор.\n\n";
            std::cin.clear();
            std::cin.ignore(pmax(), '\n'); // Очистка буфера ввода
            break;
        }
    }

    return 0;
}

//Вот несколько идей, какую еще функциональность можно добавить в ваше приложение для управления задачами :
//Редактирование задачи : Дайте пользователю возможность редактировать описание или статус существующей задачи.
//Фильтрация задач : Позвольте пользователю фильтровать задачи по статусу или ключевому слову.
//Сортировка задач : Реализуйте возможность сортировки задач по различным критериям, таким как описание, статус, дата добавления и т.д.
//Сохранение и загрузка данных : Добавьте функциональность сохранения списка задач в файл и загрузки из файла.Это позволит пользователям сохранять свои задачи между запусками приложения.
//Приоритеты задач : Дайте возможность назначать приоритеты задачам и отображать их в списке.
//Дата и время : Позвольте пользователям добавлять дату и время к задачам и использовать это для сортировки и напоминаний.
//Метки и категории : Позвольте пользователям добавлять метки или категории к задачам для удобства организации и фильтрации.
//Оповещения и напоминания : Добавьте функциональность оповещения или напоминания пользователей о задачах, которые становятся активными в определенное время или дату.
//Многопользовательский режим : Реализуйте возможность работы с задачами для нескольких пользователей с возможностью совместного доступа и совместного редактирования.

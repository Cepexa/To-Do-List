#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <curses.h>
#include <codecvt>
#ifdef WIN32
#include <windows.h>
#endif 
void sizeTerm1(int& y, int& x) {
#ifdef WIN32
    // Получение дескриптора консоли
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Получение информации о консольном окне
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    // Ширина и высота окна
    x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif        
}

class Button2 {
public:
    Button2(int x, int y, int width, int height, std::wstring label)
        : x(x), y(y), width(width), height(height), label(label) {
        win = newwin(height, width, y, x);
    }
    ~Button2() {
        delwin(win);
    }
    void draw() const {
        box(win, 0, 0);

        wmove(win, height / 2, (width - label.size()) / 2);
        int j = (width - label.size()) / 2;

        for (const auto& cht : label)
        {
            wmove(win, height / 2, j++);
            waddch(win, cht);
        }

        wrefresh(win);
    }

    bool contains(int px, int py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

private:
    int x, y, width, height;
    std::wstring label;
    WINDOW* win;
};

class EditField1 {
public:
    EditField1(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {
        win = newwin(height, width, y, x);
    }
    ~EditField1() {
        delwin(win);
    }

    void printText() {
        int i = 1, j = 1;
        wmove(win, i, j);
        for (const auto& cht : text)
        {
            if (j == width - 1) {
                j = 1;
                if (++i == height - 1) {
                    text.pop_back();
                }
            }
            wmove(win, i, j++);
            waddch(win, cht);
        }
        if (j != width - 1) {
            waddch(win, ' ');
        }
        else if (i != height - 1) {
            wmove(win, i + 1, 1);
            waddch(win, ' ');
        }
        wmove(win, i, j);
        wrefresh(win);
    }

    void draw() {
        box(win, 0, 0);
        printText();
    }

    void refresh(int width, int height) {
        this->width = width;
        this->height = height;
        delwin(win);
        win = newwin(height, width, y, x);
        draw();
    }

    void handleInput() {
        int curX, curY;
        sizeTerm1(curY, curX);
        curs_set(1);
        nodelay(win, TRUE);
        while (true)
        {
            int ch = wgetch(win);
            int newX, newY;
            sizeTerm1(newY, newX);
            if (newY != curY || newX != curX) {
                refresh(newX - 2, newY - 10);
                break;
            }
            if (ch == -1) {

            }
            else if (ch == 8) {
                if (!text.empty())text.pop_back();
            }
            else if (ch == '\n')
            {
                break;
            }
            else {
                text.push_back(ch);
            }
            printText();

        }
        // Обработка ввода
    }

    bool contains(int px, int py) const {
        return px > x && px < x + width && py > y && py < y + height;
    }
    const std::wstring& getText() {
        return text;
    }
private:
    int x, y, width, height;
    WINDOW* win;
    std::wstring text;
};

class Task {
private:
    std::wstring topic;
    std::wstring description;
    std::wstring status;
    std::wstring date;

public:
    Task(const std::wstring& t, const std::wstring& d, const std::wstring& s, const std::wstring& dt)
        : topic(t), description(d), status(s), date(dt) {}

    // Методы для получения и изменения полей задачи
    std::wstring getTopic() const { return topic; }
    std::wstring getDescription() const { return description; }
    std::wstring getStatus() const { return status; }
    std::wstring getDate() const { return date; }

    void setTopic(const std::wstring& t) { topic = t; }
    void setDescription(const std::wstring& d) { description = d; }
    void setStatus(const std::wstring& s) { status = s; }
    void setDate(const std::wstring& dt) { date = dt; }
};

class ToDoList {
private:
    std::vector<Task> tasks;
    int currentTaskIndex;
    WINDOW* tasksFrame;

public:
    ToDoList() : currentTaskIndex(0) {}

    void addTask(const Task& task);
    void editTask();
    void deleteTask();
    void displayMenu();
    void displayTasksFrame();
    void run();
};

void ToDoList::addTask(const Task& task) {
    tasks.push_back(task);
    displayTasksFrame(); // Обновляем список задач после добавления
}

void ToDoList::editTask() {
    if (tasks.empty())
        return;

    // Получаем текущую задачу
    Task& task = tasks[currentTaskIndex];

    // Отображаем окно редактирования
    WINDOW* editWindow = newwin(10, 50, 5, 5);
    box(editWindow, 0, 0);
    mvwprintw(editWindow, 1, 2, "Edit Task");
    mvwprintw(editWindow, 3, 2, "Topic:");
    mvwprintw(editWindow, 4, 2, "Description:");
    mvwprintw(editWindow, 5, 2, "Status:");
    mvwprintw(editWindow, 6, 2, "Date:");
    wrefresh(editWindow);




    // Освобождаем ресурсы
    delwin(editWindow);

    displayTasksFrame(); // Обновляем список задач после редактирования
}

void ToDoList::deleteTask() {
    if (tasks.empty())
        return;

    tasks.erase(tasks.begin() + currentTaskIndex);
    if (currentTaskIndex >= tasks.size())
        currentTaskIndex = tasks.size() - 1;

    displayTasksFrame(); // Обновляем список задач после удаления
}

void ToDoList::displayMenu() {
    // Отображение меню и обработка ввода пользователя
}

void ToDoList::displayTasksFrame() {
    curs_set(0);
    
    int _y, _x;
    sizeTerm1(_y, _x);
    int maxY,maxX;
    getmaxyx(stdscr, maxY, maxX);
    if (_x != maxX || _y != maxY) {
        clear();
        if (_y > 20 && _x > 60 )
            resize_term(_y, _x);
    }
    if (tasksFrame != nullptr)
        delwin(tasksFrame);

    mvwprintw(stdscr, 6, 3, "Список задач:"); // Заголовок
    tasksFrame = newwin(20, 60, 7, 1); // Создание окна для списка задач
    box(tasksFrame, 0, 0); // Рамка вокруг окна

    // Вывод списка задач
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::wstring taskInfo = std::to_wstring(i+1)+L": " + tasks[i].getTopic() + L" - " + tasks[i].getStatus();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::string str = converter.to_bytes(taskInfo);
        mvwprintw(tasksFrame, i + 1, 2, str.c_str());
    }

    wrefresh(tasksFrame);
}

void ToDoList::run() {
    // Инициализация PDcurses
    system("chcp 1251 > nul");
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLUE); // Цвет "кнопки"
    init_pair(2, COLOR_BLUE, COLOR_RED); // Цвет "кнопки" при наведении
    noecho();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    Task t = Task(L"Тема", L"Описание", L"Статус",L"Дата");
    tasks.push_back(t);
    Button2 btn1( 1, 1, 20, 5, L"Добавить новую");
    Button2 btn2( 21,1, 20, 5, L"Редактировать");
    Button2 btn3( 41,1, 20, 5, L"Удалить");
    // Цикл обработки ввода пользователя
    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
        case KEY_UP:
            if (currentTaskIndex > 0)
                currentTaskIndex--;
            break;
        case KEY_DOWN:
            if (currentTaskIndex < tasks.size() - 1)
                currentTaskIndex++;
            break;
        case 'e':
            editTask();
            break;
        case 'd':
            deleteTask();
            break;
        case KEY_MOUSE:
            MEVENT event;
            if (nc_getmouse(&event) == OK) {
                int i = 15;
                if (btn1.contains(event.x, event.y)) {
                    int initial_y = 30, initial_x = 80;
                    int maxY, maxX;
                    getmaxyx(stdscr, maxY, maxX);
                    if (initial_y > maxY || initial_x > maxX) {
                        resize_term(initial_y, initial_x);
                    }
                    // Обработка события для кнопки 1
                    Button2 btnSave(1, 1, 20, 5, L"Сохранить и выйти"); 
                    EditField1* editField  = new EditField1(1, 10, maxX - 2, maxY - 10);
                    EditField1* editField2 = new EditField1(1, 6, 60, 4);
                    while (true) {
                        curs_set(0);
                        int _y, _x;
                        sizeTerm1(_y, _x);
                        if (_y != initial_y || initial_x != _x) {
                            clear();
                            if (_x > maxX && _y > maxY) resize_term(_y, _x);
                            editField->refresh(_x - 2, _y - 10);
                            initial_y = _y;
                            initial_x = _x;
                        };
                        // Отображение элементов
                        btnSave.draw();
                        editField->draw();
                        editField2->draw();
                        int ch = wgetch(stdscr);
                        if (ch == KEY_MOUSE) {
                            MEVENT event;
                            if (nc_getmouse(&event) == OK) {
                                int i = 15;
                                if (btnSave.contains(event.x, event.y)) {
                                    // Обработка события для кнопки 1
                                    Task t = Task(editField2->getText().c_str(), editField->getText().c_str(), L"В очереди", L"тест");
                                    tasks.push_back(t);

                                    break;
                                }
                                else if (editField->contains(event.x, event.y)) {
                                    editField->handleInput();
                                    // Обработка события для области редактирования
                                }                                
                                else if (editField2->contains(event.x, event.y)) {
                                    editField2->handleInput();
                                    // Обработка события для области редактирования
                                }
                            }
                        }
                        refresh();

                    }


                }
                else if (btn2.contains(event.x, event.y)) {
                    // Обработка события для кнопки 2

                }
                else if (btn3.contains(event.x, event.y)) {
                    // Обработка события для кнопки 3

                }

            }

            break;
        }
        btn1.draw();
        btn2.draw();
        btn3.draw();
        displayTasksFrame();
        refresh();
    }

    // Завершение PDcurses
    endwin();
}

int main() {
    // Создание объекта и запуск программы
    ToDoList toDoList;
    toDoList.run();

    return 0;
}

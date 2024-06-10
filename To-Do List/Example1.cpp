#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <curses.h>
#include <windows.h>
#include <list>
#include <vector>
#include <signal.h>
#include <functional>
#include "Example1.h"

void sizeTerm(int& y, int& x) {
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

class Button1 {
public:
    Button1(int x, int y, int width, int height, std::wstring label)
        : x(x), y(y), width(width), height(height), label(label) {
        win = newwin(height, width, y, x);
    }
    ~Button1() {
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

class EditField {
public:
    EditField(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {
        win = newwin(height, width, y, x);
    }
    ~EditField() {
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
        else if (i != height - 1){
            wmove(win, i+1, 1);
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
        this->width  = width;
        this->height = height;
        delwin(win);
        win = newwin(height, width, y, x);
        draw();
    }

    void handleInput() {
        int curX, curY;
        sizeTerm(curY, curX);
        curs_set(1);
        nodelay(win, TRUE);
        while (true)
        {
            int ch = wgetch(win);
            int newX, newY;
            sizeTerm(newY, newX);
            if (newY != curY || newX != curX) {
                refresh(newX - 2, newY - 10);
                break;
            }
            if (ch == -1) {

            }
            else if (ch == 8) {
                if(!text.empty())text.pop_back();
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

private:
    int x, y, width, height;
    WINDOW* win;
    std::wstring text;
};

int main33() {
    //setlocale(LC_ALL, "ru_RU.UTF-8");
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
    std::list<std::string> data;
    data.push_back("Привет мир");
    int initial_y = 30, initial_x = 80;
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    if (initial_y > maxY || initial_x > maxX) {
        resize_term(initial_y, initial_x);
    }
    Button1 button1( 1, 1,20, 5, L"Button 1");
    Button1 button2( 25,1, 20, 5, L"Button 2");
    Button1 button3( 49,1, 20, 5, L"Обновить");
    EditField* editField = new EditField( 1,10, maxX - 2, maxY - 10);

    while (true) {
        curs_set(0);
        int _y, _x;
        sizeTerm(_y, _x);
        if (_y != initial_y || initial_x != _x) {
            clear();
            if(_x > maxX &&_y>maxY) resize_term(_y, _x);
            editField->refresh(_x-2,_y- 10);
            initial_y = _y;
            initial_x = _x;
        };
        // Отображение элементов
        button1.draw();
        button2.draw();
        button3.draw();
        editField->draw();
        int ch = wgetch(stdscr);
        if (ch == KEY_MOUSE) {
            MEVENT event;
            if (nc_getmouse(&event) == OK) {
                int i = 15;
                if (button1.contains(event.x, event.y)) {
                    // Обработка события для кнопки 1
                attron(COLOR_PAIR(1));
                for (const auto& item : data) {
                    wchar_t* wstr = new wchar_t[item.size()];
                    mbstowcs(wstr, item.c_str(), item.size());
                    mvprintw(i, 0, "%ls", wstr);
                    i++;
                    delete[] wstr;
                }
                attroff(COLOR_PAIR(1));
                }
                else if (button2.contains(event.x, event.y)) {
                    // Обработка события для кнопки 2
                }
                else if (button3.contains(event.x, event.y)) {
                    // Обработка события для кнопки 3
                                    // Выводим информацию из списка

                }
                else if (editField->contains(event.x, event.y)) {
                    editField->handleInput();
                    // Обработка события для области редактирования
                }
            }
        }
        refresh();
    }
    delete editField;
    endwin();
    return 0;
}
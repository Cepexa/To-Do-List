#include <iostream>
#include <curses.h>

#if defined(_WIN32) || defined(_WIN64) 
#include <windows.h>
#define msleep(msec) Sleep(msec)
#else
#include <unistd.h>
#define msleep(msec) usleep(msec*1000)
#endif
#include <list>
#include <string>

#include <curses.h>
#include <string>

class Edit
{
public:
    Edit(int x, int y, int width, int height);
    ~Edit();
    void Prepare() {
        input_window = newwin(height, width, y, x);
        // �������� �����
        box(input_window, 0, 0);
        // ����������� ������� � ������ ����
        wmove(input_window, 1, 1);
    }
    void ListenMouse(int ch) {
        if (ch == KEY_MOUSE) {
            MEVENT event;
            nc_getmouse(&event);
            isCur = event.x >= x && event.x <= x + width &&
                event.y >= y && event.y <= y + height;
        }
    }
    void Show() {
        // �������� ��������� ������� �� "������"
        int ch;
        std::string text;
        if (isCur) {
            while ((ch = wgetch(input_window)) != '\n') {
                if (ch == 8 /*KEY_BACKSPACE */ && !text.empty()) {
                    text.erase(text.end() - 1);
                    wmove(input_window, 1, text.length() + 1);
                    waddch(input_window, ' ');
                    wmove(input_window, 1, text.length() + 1);
                }
                else if (isprint(ch)) {
                    text += static_cast<char>(ch);
                    waddch(input_window, ch);
                }
                wrefresh(input_window);
            }

        }
        else {

        }

    }

private:
    int x, y, width, height;
    WINDOW* input_window;
    bool isCur{ false };
};

Edit::Edit(int x, int y, int width, int height)
     :x(x), y(y), width(width), height(height)
{
    Prepare();
}

Edit::~Edit()
{
    delwin(input_window);
}

int main3() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // ������� ���� �����
    int window_width = 40;
    int window_height = 10;

    // ���������� ������ �������� ���� ����
    int window_x = (COLS - window_width) / 2;
    int window_y = (LINES - window_height) / 2;

    // �������� ���� �����
    WINDOW* input_window = newwin(window_height, window_width, window_y, window_x);

    // �������� �����
    box(input_window, 0, 0);

    // ����������� ������� � ������ ����
    wmove(input_window, 1, 1);

    // ���� ������
    int ch;
    std::string text;
    while ((ch = wgetch(input_window)) != '\n') {
        if (ch == 8 /*KEY_BACKSPACE */ && !text.empty()) {
            text.erase(text.end() - 1);
            wmove(input_window, 1, text.length() + 1);
            waddch(input_window, ' ');
            wmove(input_window, 1, text.length() + 1);
        }
        else if (ch != 8){
            text += static_cast<char>(ch);
            waddch(input_window, ch);
        }


        wrefresh(input_window);
    }

    // ����� ���������� ������
    mvprintw(LINES - 2, 0, "��������� �����: %s", text.c_str());
    refresh();

    getch();
    delwin(input_window);
    endwin();

    return 0;
}
int main2() {
    initscr();
    //noecho();
    cbreak();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    std::list<char> input_list;

    while (true) {
        int ch = getch();

        if (ch == KEY_MOUSE) {
            // ��������� ���������� ����
            MEVENT event;
            nc_getmouse(&event);
            int y = event.y;
            int x = event.x;

            // ���������� ������
            move(y, x);
            refresh();

            // ��������� ������
            ch = getch();

            // ��������� ������ � ������
            input_list.push_back(static_cast<char>(ch));
        }
        else if (ch == 'q') {
            // ����� �� ���������
            break;
        }

        // ... ��������� ������ ������� ...
    }

    endwin();
    return 0;
}
int main1()
{
    initscr();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    char str[100];
    addstr("Enter string: ");
    getstr(str); //�������� ������
    curs_set(0); //"�������" ������, ����� �� �������
    while (true)
    {
        //���������� �-���������� ��� ����� ������ ������, � ����� ��������� �, ���� ��� != 0
        unsigned x, y;
        getmaxyx(stdscr, x, y);
        for (; x; --x)
        {
            clear();
            mvaddstr(getmaxy(stdscr) / 2, x, str);
            refresh();
            msleep(200);
        }
    }

    endwin();
    return 0;
}
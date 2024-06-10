#include <iostream>
#include <string>
#include <curses.h>

class Blank
{
public:
    Blank(int x, int y, int width, int height);
    ~Blank();

private:
    WINDOW* win;
    int x, y, width, height;
};

Blank::Blank(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height)
{
    win = newwin(x, y, width, height);
}

Blank::~Blank()
{
    delwin(win);
} 

int main() {
    initscr(); // ������������� ������ curses
    noecho();  // ���������� ����������� �������� ��������

    // ����������� �������� ������
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // ����������� �������� ����
    int height = 10;
    int width = 30;
    int starty = (maxy - height) / 2; // ��������� �������� ������ ���� ����
    int startx = (maxx - width) / 2;  // ��������� �������� ������ ���� ����

    // �������� �������� ����
    WINDOW* background = newpad(maxy, maxx);
    init_pair(1, COLOR_BLUE, COLOR_BLUE);
    init_pair(2, COLOR_MAGENTA, COLOR_MAGENTA);
    wbkgd(background, COLOR_PAIR(1)); // ��������� �������� �����
    refresh();

    // ��������� �������� �� ������� ����
    box(background, 0, 0);
    mvwprintw(background, 1, 1, "Enter text (Window 1):");
    mvwprintw(background, 2, 1, "Enter text (Window 2):");
    prefresh(background, 0, 0, 0, 0, maxy - 1, maxx - 1);

    // �������� ���� ��� ������ ������� �����
    WINDOW* win1 = subwin(background, height - 2, width - 2, starty + 1, startx + 1);

    // ��������� ����� � ������ �������
    char input1[20];
    echo();
    mvwgetnstr(win1, 0, 0, input1, 20);

    // ����������� ���������� ������ � ������ �������
    mvwprintw(background, starty + 3, startx + 1, "You entered (Window 1): %s", input1);

    // �������� ���� ��� ������ ������� �����
    WINDOW* win2 = subwin(background, height - 2, width - 2, starty + height + 1, startx + 1);

    // ��������� ����� �� ������ �������
    char input2[20];
    mvwgetnstr(win2, 0, 0, input2, 20);

    // ����������� ���������� ������ �� ������ �������
    mvwprintw(background, starty + height + 3, startx + 1, "You entered (Window 2): %s", input2);

    // ���������� �������� ����
    prefresh(background, 0, 0, 0, 0, maxy - 1, maxx - 1);

    // �������� ������� ����� ������� ����� ���������
    getch();

    // ������� ������ � �����
    delwin(win1);
    delwin(win2);
    endwin();
    return 0;
}

#include <curses.h>
#include <string>

int main4() {
    initscr();
    noecho();
    cbreak();
    mousemask(ALL_MOUSE_EVENTS, NULL);
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
    bool editing = false;
    while (true) {
        ch = getch();

        // �������� ������� �� ������� �����
        if (ch == KEY_MOUSE) {
            MEVENT event;
            nc_getmouse(&event);
            if (event.y >= window_y && event.y < window_y + window_height &&
                event.x >= window_x && event.x < window_x + window_width) {
                editing = true;
                wmove(input_window, event.y - window_y, event.x - window_x);
            }
            else {
                editing = false;
            }
        }

        if (editing) {
            // �������������� ������ ������ �����
            if (ch == KEY_BACKSPACE && !text.empty()) {
                text.erase(text.end() - 1);
                wmove(input_window, 1, text.length() + 1);
                waddch(input_window, ' ');
                wmove(input_window, 1, text.length() + 1);
            }
            else if (isprint(ch)) {
                text += static_cast<char>(ch);
                waddch(input_window, ch);
            }
            else if (ch == '\n') {
                editing = false;
            }

            wrefresh(input_window);
        }
    }

    // ����� ���������� ������
    mvprintw(LINES - 2, 0, "��������� �����: %s", text.c_str());
    refresh();

    getch();
    delwin(input_window);
    endwin();

    return 0;
}
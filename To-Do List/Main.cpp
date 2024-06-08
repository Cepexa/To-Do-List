﻿#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <curses.h>
#include <windows.h>
#include <list>
#include <signal.h>
#include <functional>
class Edit
{
public:
    Edit(int x, int y, int width, int height);
    ~Edit();
    void Prepare() {
        input_window = newwin(height, width, y, x);
        // Создание рамки
        box(input_window, 0, 0);
        // Перемещение курсора в начало окна
        wmove(input_window, 1, 1);
    }
    bool ListenMouse(int ch) {
        if (ch == KEY_MOUSE) {
            MEVENT event;
            nc_getmouse(&event);
            isCur = event.x >= x && event.x <= x + width &&
                event.y >= y && event.y <= y + height;
        }
        return isCur;
    }
    void Show() {
        // Проверка наведения курсора на "кнопку"
        int ch;
        if (isCur) {
            curs_set(1); // показать курсор
            do { ch = wgetch(input_window); 
    
            if (ch == 8 /*KEY_BACKSPACE */ ) {
                if (text.empty()) {
                }
                else {

                // Удаление последнего символа
                text.erase(text.end() - 1);
                }
                // Перемещение курсора на позицию последнего символа (или на предыдущую строку, если символ был переходом на новую строку)
                int cursor_x = text.length() + 1;
                if (text.length() == 0) {
                     //Если строка пустая, переходим в начало первой строки
                    cursor_x = 1;
                    wmove(input_window, 1, cursor_x);
                    waddch(input_window, ' ');
                    //Перемещаем курсор обратно
                    wmove(input_window, 1, cursor_x);
                }
                else {
                    // Вставляем пробел, чтобы очистить символ
                    wmove(input_window, 1, cursor_x);
                    waddch(input_window, ' ');
                    //Перемещаем курсор обратно
                    wmove(input_window, 1, cursor_x);
                }
            }
            else if (ch != 8) {
                text += static_cast<char>(ch);
                //waddch(input_window, ch);
            }
            wrefresh(input_window);
            } 
            while (ch != '\n');
            isCur = false;
        }
        else {

        }

    }

private:
    int x, y, width, height;
    WINDOW* input_window;
    std::string text;
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

class Button
{
public:
    Button(int x,int y, int width, int height,const char* text);
    ~Button();
    void Prepare() {
        // Инициализация ncurses
    //noecho(); // Скрыть ввод символов
    // cbreak(); // Ввод символов без нажатия Enter
        start_color(); // Включение поддержки цвета

        // Определение цветов
        init_pair(1, COLOR_GREEN, COLOR_BLUE); // Цвет "кнопки"
        init_pair(2, COLOR_BLUE, COLOR_RED); // Цвет "кнопки" при наведении

        // Вывод "кнопки"
        attron(COLOR_PAIR(1));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                mvaddch(y + i, x + j, ' ');
            }
        }
    }
    void Show() {
        // Проверка наведения курсора на "кнопку"
        curs_set(0); // Скрыть курсор
        if (current == this) {
            // Выделить "кнопку"
            attron(COLOR_PAIR(2));
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    mvaddch(y + i, x + j, ' ');
                }
            }
            attroff(COLOR_PAIR(2));
            func();
        }
        else {
            // Вернуть "кнопке" обычный цвет
            attron(COLOR_PAIR(1));
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    mvaddch(y + i, x + j, ' ');
                }
            }
            attroff(COLOR_PAIR(1));
        }
        // Очищаем область кнопки
        mvhline(y, x, ' ', width);
        mvhline(y + height - 1, x, ' ', width);
        for (int i = 1; i < height - 1; i++) {
            mvaddch(y + i, x, '|');
            mvaddch(y + i, x + width - 1, '|');
        }
        // Выравнивание текста по центру
        int text_len = strlen(text);
        int text_x = x + (width - text_len) / 2;
        mvprintw(y + (height / 2), text_x, "%s", text);
    }
    void ListenMouse(int ch, std::function<void()>func) {
        if (ch == KEY_MOUSE) {
            MEVENT event;
            nc_getmouse(&event);
            if (event.x >= x && event.x <= x + width &&
                event.y >= y && event.y <= y + height)
            { 
                current = this;
                this->func = func;
            //switch (event.bstate) {
            //case BUTTON1_PRESSED:
            //    std::cout << "ЛКМ зажал на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //case BUTTON1_RELEASED:
            //    std::cout << "ЛКМ отпустил на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //case BUTTON1_CLICKED:
            //    std::cout << "Я клик ЛКМ на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //case BUTTON1_DOUBLE_CLICKED:
            //    std::cout << "Я Двойной клик ЛКМ на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //case BUTTON3_PRESSED:
            //    std::cout << "ПКМ зажал на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //case BUTTON3_RELEASED:
            //    std::cout << "ПКМ отпустил на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //case BUTTON3_CLICKED:
            //    std::cout << "Я клик ПКМ на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //case BUTTON3_DOUBLE_CLICKED:
            //    std::cout << "Я Двойной клик ПКМ на координате (" << event.x << "," << event.y << ") " << event.bstate << "\n";
            //    break;
            //default:
            //    std::cout << "Что это " << event.bstate << "\n";
            //}
            }

        }

    }
private:
    int x, y, width, height;
    const char* text;
    static Button* current;
    std::function<void()> func;
};
Button::Button(int x, int y, int width, int height, const char* text)
    :text(text),
     x(x),y(y), width(width), height(height){
    Prepare();
}
Button::~Button()
{
}
#ifdef WIN32_
void setMinSizeWindow() {

    // Получение дескриптора консоли
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Получение информации о консольном окне
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    // Ширина и высота окна
    int max_x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int max_y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    // Устанавливаем минимальный размер окна (например, 80x25)
    int min_x = 80;
    int min_y = 20;
    // Проверяем, если текущий размер окна меньше минимального,
    // меняем размер окна
    if (max_x < min_x) {
        resize_term(max_y, min_x);
    }
    else if (max_y < min_y) {
        resize_term(min_y, max_x);
    }
    else{
        resize_term(max_y, max_x);
            }
}
#endif

int main() {
    initscr();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    //noecho(); 
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    std::list<std::string> data;
    data.push_back("Hello world! Привет мир");
    Button btn1(1, 1, 30, 5,"label1");
    Button btn2(31, 1, 30, 5,"label2");    
    Edit edit(1, 12, 20, 5);    
            int initial_y, initial_x;
            getmaxyx(stdscr, initial_y, initial_x);
    while (true) {
        int _y, _x;
#ifdef WIN32
        // Получение дескриптора консоли
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // Получение информации о консольном окне
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        // Ширина и высота окна
        _x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        _y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif        
        if (_y != initial_y || initial_x != _x) {
            clear();
            resize_term(_y, _x);
            initial_y = _y;
            initial_x = _x;
            //signal(28, [](int) { clear(); });fro
            //clear();
        };
        int ch = getch();
        if (ch == -1) {
            curs_set(0);
            btn1.Show();
            btn2.Show();
            edit.Show();

        }
        else {
            clear();
            btn1.ListenMouse(ch, [&data]() {
                // Выводим информацию из списка
                int i = 11;
                for (const auto& item : data) {
                    wchar_t wstr[100];
                    mbstowcs(wstr, item.c_str(), item.size());
                    mvprintw(i, 0, "%ls", wstr);
                    i++;
                }});
            btn2.ListenMouse(ch, []() {});
            edit.ListenMouse(ch);
        }   
        refresh();
    }

    endwin();
    return 0;
}

//int main10() {
//    MSG msg;
//
//    // Создаем окно
//    HWND hWnd = CreateWindow(
//        L"MyWindowClass",  // Имя класса окна
//        L"My Window",       // Заголовок окна
//        WS_OVERLAPPEDWINDOW, // Стиль окна
//        CW_USEDEFAULT,       // Позиция по горизонтали (по умолчанию)
//        CW_USEDEFAULT,       // Позиция по вертикали (по умолчанию)
//        800,                // Ширина окна
//        600,                // Высота окна
//        NULL,               // Родительское окно
//        NULL,               // Меню
//        GetModuleHandle(NULL), // Дескриптор экземпляра
//        NULL);              // Дополнительные параметры
//
//      // Отображаем окно
//    ShowWindow(hWnd, SW_SHOW);
//    UpdateWindow(hWnd);
//
//    // Цикл обработки сообщений
//    while (true) {
//        // Проверяем очередь сообщений
//        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
//            // Обрабатываем сообщение, если оно есть
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//        else {
//            // Нет сообщений - выполняем другую работу, например:
//            //  - Обновляем графику
//            //  - Выполняем логику игры
//            //  - Ожидаем событий
//            //Sleep(10);
//        }
//
//        // Выход из цикла, если получено сообщение WM_QUIT
//        if (msg.message == WM_QUIT) {
//            break;
//        }
//    }
//
//    return 0;
//}
//using namespace std;
//
//int main9() {
//    system("chcp 1251 > nul");
//    // Заголовок консоли
//    cout << "Нажмите ЛКМ или ПКМ, чтобы получить координаты курсора мыши." << endl;
//    // Цикл, который работает постоянно
//    MSG msg;
//    do {
//        // Получение сообщений от Windows
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//        if (PeekMessage(&msg, GetConsoleWindow(), 0, 0, PM_NOREMOVE)) {
//            // Обработка сообщений
//            if (msg.message == WM_LBUTTONDOWN) {
//                // Обработка левой кнопки мыши
//                cout << "ЛКМ: ";
//                POINT cursorPos;
//                GetCursorPos(&cursorPos);
//                cout << "x = " << cursorPos.x << ", y = " << cursorPos.y << endl;
//            }
//            else if (msg.message == WM_RBUTTONDOWN) {
//                // Обработка правой кнопки мыши
//                cout << "ПКМ: ";
//                POINT cursorPos;
//                GetCursorPos(&cursorPos);
//                cout << "x = " << cursorPos.x << ", y = " << cursorPos.y << endl;
//            }
//        }
//    } while (msg.message != WM_QUIT);
//    return 0;
//}

//// Функция хука для обработки событий мыши
//LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
//    if (nCode == HC_ACTION) {
//        // Получение структуры с информацией о событии мыши
//        MOUSEHOOKSTRUCT* mouseHookStruct = (MOUSEHOOKSTRUCT*)lParam;
//
//        // Получение дескриптора окна консоли
//        HWND hConsoleWindow = GetConsoleWindow();
//
//        // Проверка, находится ли курсор мыши в консольном окне
//        POINT cursorPos;
//        GetCursorPos(&cursorPos);
//        if (WindowFromPoint(cursorPos) == hConsoleWindow) {
//            // Обработка нажатия левой кнопки мыши
//            if (wParam == WM_LBUTTONDOWN) {
//                std::cout << "Нажата левая кнопка мыши" << std::endl;
//            }
//            // Добавьте обработку других событий мыши по необходимости
//        }
//    }
//    // Продолжить обработку хука
//    return CallNextHookEx(NULL, nCode, wParam, lParam);
//}

//// Функция для запуска хука в отдельном потоке
//DWORD WINAPI HookThread(LPVOID lpParam) {
//    // Установка хука для отслеживания событий мыши
//    HHOOK hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, GetCurrentThreadId());
//    if (hHook == NULL) {
//        std::cerr << "Ошибка установки хука" << std::endl;
//        return 1;
//    }
//
//    // Основной цикл обработки сообщений
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    // Удаление хука
//    UnhookWindowsHookEx(hHook);
//
//    return 0;
//}

//int main7() {
//    system("chcp 1251 > nul");
//    // Запуск отдельного потока для хука
//    HANDLE hThread = CreateThread(NULL, 0, HookThread, NULL, 0, NULL);
//    if (hThread == NULL) {
//        std::cerr << "Ошибка создания потока" << std::endl;
//        return 1;
//    }
//
//    // Основной цикл обработки сообщений для главной программы
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    return 0;
//}

//// Регистрация сообщения
//const UINT WM_MOUSE_CLICK = RegisterWindowMessage(L"MyMouseClick");

//// Процедура окна
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    switch (message) {
//    case 49916:
//        // Обработка нажатия левой кнопки мыши
//        std::cout << "Нажата левая кнопка мыши" << std::endl;
//        break;
//    default:
//        return DefWindowProc(hwnd, message, wParam, lParam);
//    }
//    return 0;
//}

//// Функция для обработки событий мыши в отдельном потоке
//DWORD WINAPI MouseThread(LPVOID lpParam) {
//    HANDLE hEvent = (HANDLE)lpParam; // Событие для синхронизации
//    HWND hConsoleWindow = GetConsoleWindow();
//    POINT cursorPos;
//    while (true) {
//        GetCursorPos(&cursorPos);
//
//        // Отправка сообщения о нажатии левой кнопки мыши 
//        PostMessage(hConsoleWindow, WM_MOUSE_CLICK, 0, MAKELPARAM(cursorPos.x, cursorPos.y));
//
//        // Сигнализация главному потоку о событии
//        SetEvent(hEvent);
//
//        Sleep(100); // Уменьшите время ожидания для более плавной обработки
//    }
//    return 0;
//}

//int main6() {
//    // Регистрация класса окна
//    WNDCLASSEX wcex;
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = WindowProc;
//    wcex.cbClsExtra = 0;
//    wcex.cbWndExtra = 0;
//    wcex.hInstance = GetModuleHandle(NULL);
//    wcex.hIcon = NULL;
//    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wcex.lpszMenuName = NULL;
//    wcex.lpszClassName = L"MyWindowClass";
//    wcex.hIconSm = NULL;
//
//    if (!RegisterClassEx(&wcex)) {
//        std::cerr << "Ошибка регистрации класса окна" << std::endl;
//        return 1;
//    }
//
//    // Создание невидимого окна
//    HWND hwnd = CreateWindowEx(WS_EX_TRANSPARENT, L"MyWindowClass", L"",
//        WS_POPUP | WS_EX_TOOLWINDOW, 0, 0, 0, 0,
//        NULL, NULL, GetModuleHandle(NULL), NULL);
//
//    if (hwnd == NULL) {
//        std::cerr << "Ошибка создания окна" << std::endl;
//        return 1;
//    }
//
//    // Создание события для синхронизации
//    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//    if (hEvent == NULL) {
//        std::cerr << "Ошибка создания события" << std::endl;
//        return 1;
//    }
//
//    // Запуск отдельного потока для обработки событий мыши
//    HANDLE hThread = CreateThread(NULL, 0, MouseThread, (LPVOID)hEvent, 0, NULL);
//    if (hThread == NULL) {
//        std::cerr << "Ошибка создания потока" << std::endl;
//        return 1;
//    }
//
//    // Основной цикл обработки сообщений
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0)) {
//        // Проверка на наличие события
//        if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0) {
//            // Сброс события
//            ResetEvent(hEvent);
//        }
//
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    // Закрытие событий и потока
//    CloseHandle(hEvent);
//    CloseHandle(hThread);
//
//    return 0;
//}

//int main5() {
//    system("chcp 1251 > nul");
//    HWND hConsoleWindow = GetConsoleWindow();
//    MSG msg;
//
//    // Ожидание сообщений
//    while (GetMessage(&msg, NULL, 0, 0)) {
//        // Обработка сообщений
//        switch (msg.message) {
//        case WM_LBUTTONDOWN:
//            // Обработка нажатия левой кнопки мыши
//            std::cout << "Нажата левая кнопка мыши" << std::endl;
//            break;
//        case WM_RBUTTONDOWN:
//            // Обработка нажатия правой кнопки мыши
//            std::cout << "Нажата правая кнопка мыши" << std::endl;
//            break;
//        default:
//            // Обработка других сообщений
//            break;
//        }
//
//        //TranslateMessage(&msg);
//        //DispatchMessage(&msg);
//    }
//
//    return 0;
//}
//int main4() {
//    system("chcp 1251 > nul");
//    // Создание структуры INPUT для нажатия левой кнопки мыши
//    INPUT input;
//    input.type = INPUT_MOUSE;
//    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
//    input.mi.dx = 100; // Координата x
//    input.mi.dy = 100; // Координата y
//
//    // Отправка события
//    SendInput(1, &input, sizeof(INPUT));
//
//    // Пауза
//    Sleep(1000); // 1 секунда
//
//    // Создание структуры INPUT для отпускания левой кнопки мыши
//    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
//
//    // Отправка события
//    SendInput(1, &input, sizeof(INPUT));
//
//    return 0;
//}
//int main3() {
//    system("chcp 1251 > nul");
//    RECT windowRect;
//    GetWindowRect(GetConsoleWindow(), &windowRect);
//    int left = windowRect.left;
//    int top = windowRect.top;
//    int right = windowRect.right;
//    int bottom = windowRect.bottom;
//    printf("Координаты окна консоли:\n");
//    printf("Верхний левый угол: (%i,%i)\n",left, top);
//    printf("Нижний правый угол: (%i,%i)\n", right, bottom);
//    return 0;
//}
//
//int main2() {
//    system("chcp 1251 > nul");
//    initscr();
//    noecho();
//    cbreak();
//    curs_set(0);
//
//    // Получение координат курсора мыши
//    POINT cursorPos;
//    GetCursorPos(&cursorPos);
//
//    // Преобразование в координаты консоли 
//    // (требует дополнительного преобразования для PDCurses)
//    // ... 
//    printf("Привет, мир!\n");
//    //mvaddch(10,10,2222);
//    //mvprintw(10, 10, "Координаты курсора: (%d, %d)", cursorPos.x, cursorPos.y);
//    //refresh();
//    //getch();
//    endwin();
//
//    return 0;
//}
//

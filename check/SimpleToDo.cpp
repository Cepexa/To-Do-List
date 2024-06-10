#include <iostream>
#include <list>
#include <string>
#include <limits> // ��� std::numeric_limits

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
        std::cout << "\n��� �� �� ������ �������?\n";
        std::cout << "1. �������� ����� ������\n";
        std::cout << "2. �������� ������ ������\n";
        std::cout << "3. ������� ������\n";
        std::cout << "4. ����� �� ���������\n";
        std::cout << "�������� �������� (1-4): ";
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

    // ��������� ��������� ��������� ����� ��� �������
    taskList.addTask(Task("����������� ���������", "� �������"));
    taskList.addTask(Task("�������� ������������", "� ������"));
    taskList.addTask(Task("�������� ������������", "� �������"));

    std::string msg = "����� ���������� � ��� todo ����!\n\n";

    // �������� ���� ���������
    while (true) {
        
        taskList.displayTasks(msg + "���� ������� ������:\n");

        Menu::showMainMenu();
        int choice = Menu::getUserChoice();
        switch (choice) {
        case 1: {
            std::string desc;
            std::cout << "������� �������� ����� ������: ";
            std::cin.ignore(pmax(), '\n'); // ������� ������ �����
            std::getline(std::cin, desc);
            taskList.addTask(Task(desc, "� �������"));
            msg = "����� ������ ���������!\n\n";
            break;
        }
        case 2: {
            std::cout << "������� ����� ������, ������ ������� ������ ��������: ";
            int index;
            std::cin >> index;
            if (index < 1 || index > taskList.getSize()) {
                msg = "�������� ����� ������.\n\n";
                std::cin.clear();
                std::cin.ignore(pmax(), '\n'); // ������� ������ �����
                break;
            }
            std::cout << "������� ����� ������ ��� ������ (� �������, � ������, ���������): ";
            std::string newStatus;
            std::cin.ignore(pmax(), '\n'); // ������� ������ �����
            std::getline(std::cin, newStatus); // ��������� ��� ������
            taskList.getTask(index - 1).setStatus(newStatus);
            msg = "������ ������ ������� �������!\n\n";
            break;
        }
        case 3: {
            std::cout << "������� ����� ������, ������� ������ �������: ";
            int index;
            std::cin >> index;
            if (index < 1 || index > taskList.getSize()) {
                msg = "�������� ����� ������.\n\n";
                std::cin.clear();
                std::cin.ignore(pmax(), '\n'); // ������� ������ �����
                break;
            }
            taskList.removeTask(index - 1);
            msg = "������ ������� �������!\n\n";
            break;
        }
        case 4:
            std::cout << "\033[2J\033[H";
            std::cout << "�� ��������!\n\n";
            return 0; // ������� �� ���������
        default:
            msg = "�������� �����.\n\n";
            std::cin.clear();
            std::cin.ignore(pmax(), '\n'); // ������� ������ �����
            break;
        }
    }

    return 0;
}

//��� ��������� ����, ����� ��� ���������������� ����� �������� � ���� ���������� ��� ���������� �������� :
//�������������� ������ : ����� ������������ ����������� ������������� �������� ��� ������ ������������ ������.
//���������� ����� : ��������� ������������ ����������� ������ �� ������� ��� ��������� �����.
//���������� ����� : ���������� ����������� ���������� ����� �� ��������� ���������, ����� ��� ��������, ������, ���� ���������� � �.�.
//���������� � �������� ������ : �������� ���������������� ���������� ������ ����� � ���� � �������� �� �����.��� �������� ������������� ��������� ���� ������ ����� ��������� ����������.
//���������� ����� : ����� ����������� ��������� ���������� ������� � ���������� �� � ������.
//���� � ����� : ��������� ������������� ��������� ���� � ����� � ������� � ������������ ��� ��� ���������� � �����������.
//����� � ��������� : ��������� ������������� ��������� ����� ��� ��������� � ������� ��� �������� ����������� � ����������.
//���������� � ����������� : �������� ���������������� ���������� ��� ����������� ������������� � �������, ������� ���������� ��������� � ������������ ����� ��� ����.
//��������������������� ����� : ���������� ����������� ������ � �������� ��� ���������� ������������� � ������������ ����������� ������� � ����������� ��������������.

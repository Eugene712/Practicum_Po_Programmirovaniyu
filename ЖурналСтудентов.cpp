
// Программа: Журнал успеваемости студентов
// Описание: консольное приложение для учёта студентов и оценок.
// Возможности: добавление и удаление студентов, добавление оценок,
//              поиск, подсчёт среднего балла, сортировка,
//              сохранение в файл и загрузка из файла.

#include <iostream>   // ввод и вывод в консоль (cin, cout)
#include <fstream>    // работа с файлами (ifstream, ofstream)
#include <string>     // строки (string)
#include <vector>     // динамические массивы (vector)
#include <windows.h>

using namespace std;

// Структура "Студент": хранит все данные об одном студенте
struct Student {
    string name;          // ФИО студента
    int group;            // номер группы
    vector<int> grades;   // список оценок (от 2 до 5)
};

// Имя файла, в котором будут храниться данные
const string FILE_NAME = "journal.txt";


// Функция безопасного ввода целого числа.
// Повторяет запрос, пока пользователь не введёт число
// в диапазоне от minValue до maxValue.
// ------------------------------------------------------------
int readInt(const string& message, int minValue, int maxValue) {
    int value;
    while (true) {
        cout << message;
        cin >> value;

        if (cin.fail()) {
            // Пользователь ввёл не число (например, буквы)
            cin.clear();               // сбрасываем флаг ошибки
            cin.ignore(10000, '\n');   // выбрасываем неверный ввод
            cout << "Ошибка: пожалуйста, введите число.\n";
        }
        else if (value < minValue || value > maxValue) {
            cin.ignore(10000, '\n');
            cout << "Ошибка, введите число не менее " << minValue
                << "и не более " << maxValue << ".\n";
        }
        else {
            cin.ignore(10000, '\n');   // убираем символ конца строки
            return value;
        }
    }
}

// Функция считает средний балл студента.
// Если оценок нет, возвращает 0.

double getAverage(const Student& s) {
    if (s.grades.size() == 0) {
        return 0;
    }
    int sum = 0;
    for (int i = 0; i < s.grades.size(); i++) {
        sum += s.grades[i];
    }
    return (double)sum / s.grades.size();   // приводим к double, чтобы деление было дробным
}


// Функция выводит информацию об одном студенте

void printStudent(const Student& s, int number) {
    cout << number << ". " << s.name << " | Группа " << s.group << " | Оценки: ";

    if (s.grades.size() == 0) {
        cout << "Отсутсвуют ";
    }
    else {
        for (int i = 0; i < s.grades.size(); i++) {
            cout << s.grades[i] << " ";
        }
    }
    cout << "| Ср.балл: " << getAverage(s) << endl;
}


// Функция выводит всех студентов

void printAll(const vector<Student>& students) {
    if (students.size() == 0) {
        cout << "Список пуст\n";
        return;
    }
    cout << "\n--- Спиисок студентов ---\n";
    for (int i = 0; i < students.size(); i++) {
        printStudent(students[i], i + 1);
    }
}


// Функция добавляет нового студента в журнал

void addStudent(vector<Student>& students) {
    Student s;

    cout << "Введите полное имя студента: ";
    getline(cin, s.name);   // getline читает строку целиком, вместе с пробелами

    if (s.name.size() == 0) {
        cout << "Имя не может быть пустым.\n";
        return;
    }

    s.group = readInt("Введите номер группы (1-9999): ", 1, 9999);

    students.push_back(s);  // добавляем студента в конец списка
    cout << "Студент добавлен.\n";
}


// Функция добавляет оценку выбранному студенту

void addGrade(vector<Student>& students) {
    if (students.size() == 0) {
        cout << "Журнал пуст.\n";
        return;
    }
    printAll(students);

    int number = readInt("Введите номер студента: ", 1, students.size());
    int grade = readInt("Введите оценку (2-5): ", 2, 5);

    students[number - 1].grades.push_back(grade);
    cout << "Оценка добавлена.\n";
}


// Функция удаляет студента из журнала

void deleteStudent(vector<Student>& students) {
    if (students.size() == 0) {
        cout << "Журнал пуст.\n";
        return;
    }
    printAll(students);

    int number = readInt("Введите номер студента, которого необходимо удалить ", 1, students.size());

    // Сдвигаем всех студентов после удаляемого на одну позицию влево
    for (int i = number - 1; i < students.size() - 1; i++) {
        students[i] = students[i + 1];
    }
    students.pop_back();   // удаляем последний (лишний) элемент
    cout << "Студент удалён.\n";
}


// Функция ищет студентов, в ФИО которых встречается введённый текст

void findStudent(const vector<Student>& students) {
    string query;
    cout << "Введите имя студента для поиска ";
    getline(cin, query);

    bool found = false;   // флаг: нашли ли хоть кого-то

    for (int i = 0; i < students.size(); i++) {
        // find возвращает позицию подстроки или string::npos, если не нашёл
        if (students[i].name.find(query) != string::npos) {
            printStudent(students[i], i + 1);
            found = true;
        }
    }

    if (!found) {
        cout << "Никого не найдено\n";
    }
}


// Функция сортирует студентов по среднему баллу (по убыванию).
// Используется сортировка пузырьком.

void sortByAverage(vector<Student>& students) {
    for (int i = 0; i < students.size(); i++) {
        for (int j = 0; j < students.size() - 1 - i; j++) {
            // Если у левого балл меньше, чем у правого, меняем их местами
            if (getAverage(students[j]) < getAverage(students[j + 1])) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    cout << "Отсортировано по среднему баллу (начиная с наибольшего).\n";
}


// Функция выводит общую статистику по журналу

void showStatistics(const vector<Student>& students) {
    if (students.size() == 0) {
        cout << "Журнал пуст.\n";
        return;
    }

    int bestIndex = 0;        // индекс лучшего студента
    double totalAverage = 0;  // сумма средних баллов всех студентов

    for (int i = 0; i < students.size(); i++) {
        double avg = getAverage(students[i]);
        totalAverage += avg;
        if (avg > getAverage(students[bestIndex])) {
            bestIndex = i;
        }
    }

    cout << "\n--- Статистика ---\n";
    cout << "Все студенты: " << students.size() << endl;
    cout << "Средний балл всех студентов: " << totalAverage / students.size() << endl;
    cout << "Лучший студент: " << students[bestIndex].name
        << " (" << getAverage(students[bestIndex]) << ")\n";
}


// Функция сохраняет журнал в текстовый файл
// Формат: количество студентов, затем для каждого:
//   ФИО / группа / количество оценок / сами оценки

void saveToFile(const vector<Student>& students) {
    ofstream file(FILE_NAME);   // открываем файл для записи

    if (!file.is_open()) {
        cout << "Ошибка\n";
        return;
    }

    file << students.size() << endl;
    for (int i = 0; i < students.size(); i++) {
        file << students[i].name << endl;
        file << students[i].group << endl;
        file << students[i].grades.size() << endl;
        for (int j = 0; j < students[i].grades.size(); j++) {
            file << students[i].grades[j] << " ";
        }
        file << endl;
    }

    file.close();
    cout << "Сохранено" << FILE_NAME << endl;
}


// Функция загружает журнал из текстового файла

void loadFromFile(vector<Student>& students) {
    ifstream file(FILE_NAME);   // открываем файл для чтения

    if (!file.is_open()) {
        cout << "Файл не найден.\n";
        return;
    }

    students.clear();   // очищаем текущий список перед загрузкой

    int count;
    file >> count;
    file.ignore(10000, '\n');   // пропускаем конец строки после числа

    for (int i = 0; i < count; i++) {
        Student s;
        getline(file, s.name);
        file >> s.group;

        int gradesCount;
        file >> gradesCount;

        for (int j = 0; j < gradesCount; j++) {
            int g;
            file >> g;
            s.grades.push_back(g);
        }
        file.ignore(10000, '\n');

        students.push_back(s);
    }

    file.close();
    cout << "Загружено студентов:: " << students.size() << endl;
}


// Функция выводит главное меню

void printMenu() {
    cout << "\n===== Журнал студентов =====\n";
    cout << "1. Показать всех студентов\n";
    cout << "2. Добавить студентов\n";
    cout << "3. Добавить оценку студенту\n";
    cout << "4. Удалить студентаа\n";
    cout << "5. Найти студента в списке\n";
    cout << "6. Отсортировать студентов по среднему баллу\n";
    cout << "7. Показать статистикуs\n";
    cout << "8. Сохранить файл\n";
    cout << "9. Загрузить из файла\n";
    cout << "0. Выход\n";
}


// Главная функция: здесь работает меню программы

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector<Student> students;   // основной список студентов
    int choice;                 // выбранный пункт меню

    do {
        printMenu();
        choice = readInt("Выберете действие: ", 0, 9);

        switch (choice) {
        case 1: printAll(students); break;
        case 2: addStudent(students); break;
        case 3: addGrade(students); break;
        case 4: deleteStudent(students); break;
        case 5: findStudent(students); break;
        case 6: sortByAverage(students); break;
        case 7: showStatistics(students); break;
        case 8: saveToFile(students); break;
        case 9: loadFromFile(students); break;
        case 0: cout << "Спасибо, до свидания!\n"; break;
        }
    } while (choice != 0);   // повторяем, пока не выбран выход

    return 0;
}

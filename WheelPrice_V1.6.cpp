#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <limits>

using namespace std;

// СТРУКТУРА ДАНИХ
struct Auto {
    int id;
    string brand;
    string model;
    int year;
    double price;
};

// Глобальна або локальна колекція
vector<Auto> database;
const string FILE_NAME = "database.txt";

// --- ДОПОМІЖНІ ФУНКЦІЇ ДЛЯ РОБОТИ З ФАЙЛОМ ---

void saveToFile(const vector<Auto>& list) {
    ofstream outFile(FILE_NAME);
    for (const auto& a : list) {
        outFile << a.id << "|" << a.brand << "|" << a.model << "|" << a.year << "|" << a.price << endl;
    }
    outFile.close();
}

void loadFromFile(vector<Auto>& list) {
    list.clear();
    ifstream inFile(FILE_NAME);
    if (!inFile) return;

    string line;
    while (getline(inFile, line)) {
        // Проста логіка парсингу (можна вдосконалити)
        Auto a;
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);

        if (p1 != string::npos && p4 != string::npos) {
            a.id = stoi(line.substr(0, p1));
            a.brand = line.substr(p1 + 1, p2 - p1 - 1);
            a.model = line.substr(p2 + 1, p3 - p2 - 1);
            a.year = stoi(line.substr(p3 + 1, p4 - p3 - 1));
            a.price = stod(line.substr(p4 + 1));
            list.push_back(a);
        }
    }
    inFile.close();
}

// --- МОДУЛЬ 1: ВАЛІДАЦІЯ (Алгоритм Validation) ---
bool validation(int year, double price) {
    // КРОК 1 та 2 (C++ типізація гарантує int, перевіряємо значення)
    if (year <= 0) {
        cout << "Помилка: рік має бути додатнім числом" << endl;
        return false;
    }

    // КРОК 3: Поточний рік
    time_t t = time(0);
    tm* now = localtime(&t);
    int currentYear = now->tm_year + 1900;
    if (year > currentYear) {
        cout << "Помилка: рік не може бути в майбутньому" << endl;
        return false;
    }

    // КРОК 4 та 5
    if (price <= 0) {
        cout << "Помилка: ціна має бути додатною" << endl;
        return false;
    }

    return true; // КРОК 6
}

// --- МОДУЛЬ 2: СПИСОК (Алгоритм ListCheck) ---
void listCheck() {
    loadFromFile(database); // КРОК 1

    if (database.empty()) { // КРОК 2
        cout << "Список автомобілів порожній" << endl;
        return;
    }

    // КРОК 3
    cout << "\nID | Марка | Модель | Рік | Ціна/день" << endl;
    cout << "------------------------------------------" << endl;

    // КРОК 4
    for (const auto& a : database) {
        cout << a.id << " | " << a.brand << " | " << a.model << " | " 
             << a.year << " | " << fixed << setprecision(2) << a.price << " грн" << endl;
    }

    // КРОК 5
    cout << "------------------------------------------" << endl;
    cout << "Всього авто: " << database.size() << endl;
}

// --- МОДУЛЬ 3: ДОДАВАННЯ (Алгоритм AddAuto) ---
void addAuto() {
    Auto newAuto; // Створюємо тимчасовий об'єкт для нового авто
    cout << "--- ДОДАВАННЯ НОВОГО АВТО ---" << endl;
    cout << "Введіть ID: ";
    cin >> newAuto.id;
    // Очищуємо буфер після введення числа, щоб getline не "проскакував"
    cin.ignore(1000, '\n'); 
    cout << "Введіть бренд: ";
    getline(cin, newAuto.brand);
    cout << "Введіть модель: ";
    getline(cin, newAuto.model);
    cout << "Введіть рік: ";
    cin >> newAuto.year;
    cout << "Введіть ціну: ";
    cin >> newAuto.price;
    // Додаємо в загальну колекцію (вектор)
    database.push_back(newAuto);
    // Зберігаємо у файл, щоб дані не зникли
    saveToFile(database);
    cout << "\nАвто успішно додано. ID = " << newAuto.id << endl; 
}
// --- МОДУЛЬ 4: ВИДАЛЕННЯ (Алгоритм DeleteAuto) ---
void deleteAuto() {
    int idForDelete;
    cout << "Введіть ID для видалення: ";
    cin >> idForDelete; // КРОК 1

    if (idForDelete <= 0) { // КРОК 2
        cout << "Помилка: некоректний ID" << endl;
        return;
    }

    loadFromFile(database); // КРОК 3

    int foundIdx = -1; // КРОК 4
    for (int i = 0; i < database.size(); i++) {
        if (database[i].id == idForDelete) {
            foundIdx = i;
            break;
        }
    }

    if (foundIdx == -1) { // КРОК 5
        cout << "Помилка: автомобіль з ID " << idForDelete << " не знайдено" << endl;
        return;
    }

    database.erase(database.begin() + foundIdx); // КРОК 6
    saveToFile(database); // КРОК 7
    cout << "Автомобіль успішно видалено" << endl; // КРОК 8
}

// --- МОДУЛЬ 5: РОЗРАХУНОК (Алгоритм CalculatePrice) ---
struct CalcResult {
    double normalPrice;
    int discountPercent;
    double discountValue;
    double finalPrice;
};

CalcResult calculatePrice(double price, int days) {
    if (days <= 0) { // КРОК 1
        cout << "Помилка: кількість днів некоректна" << endl;
        return {0, 0, 0, 0};
    }

    double normalPrice = price * days; // КРОК 2
    int discountPercent = 0;

    // КРОК 3
    if (days > 30) discountPercent = 25;
    else if (days > 7) discountPercent = 7;

    // КРОК 4 та 5 (округлення до 2 знаків)
    double discountValue = round((normalPrice * discountPercent / 100.0) * 100.0) / 100.0;
    double finalPrice = round((normalPrice - discountValue) * 100.0) / 100.0;

    return {normalPrice, discountPercent, discountValue, finalPrice}; // КРОК 6
}

// --- МОДУЛЬ 6: ЗВІТ (Алгоритм ShowCalculation) ---
void showCalculation() {
    int autoId, days;
    cout << "Введіть ID автомобіля: "; cin >> autoId;
    cout << "Введіть кількість днів оренди: "; cin >> days; // КРОК 1

    loadFromFile(database);
    int foundIdx = -1; // КРОК 2
    for (int i = 0; i < database.size(); i++) {
        if (database[i].id == autoId) {
            foundIdx = i;
            break;
        }
    }

    if (foundIdx == -1) {
        cout << "Авто не знайдено" << endl;
        return;
    }

    // КРОК 3
    CalcResult res = calculatePrice(database[foundIdx].price, days);
    if (res.normalPrice == 0) return;

    // КРОК 4
    cout << "\n----------------------------------------------------" << endl;
    cout << "Автомобіль : " << database[foundIdx].brand << " " << database[foundIdx].model << endl;
    cout << "Кількість днів: " << days << endl;
    cout << "Базова вартість: " << res.normalPrice << " грн" << endl;

    if (res.discountPercent > 0) {
        cout << "Знижка (" << res.discountPercent << "%): -" << res.discountValue << " грн" << endl;
    } else {
        cout << "Знижка: не нараховується" << endl;
    }

    cout << "Сума до сплати: " << res.finalPrice << " грн" << endl;
    cout << "----------------------------------------------------" << endl;
}

// --- ГОЛОВНЕ МЕНЮ ---
int main() {
system("chcp 1251 > nul"); // Встановлює кодування Windows-1251 для терміналу
    setlocale(LC_ALL, "Ukrainian");
    int choice;
    loadFromFile(database);
    do {
        cout << "\n--- МЕНЮ ---" << endl;
        cout << "1. Додати авто" << endl;
        cout << "2. Список авто" << endl;
        cout << "3. Видалити авто" << endl;
        cout << "4. Розрахувати вартість оренди" << endl;
        cout << "0. Вихід" << endl;
        cout << "Вибір: "; cin >> choice;

        switch (choice) {
            case 1: addAuto(); break;
            case 2: listCheck(); break;
            case 3: deleteAuto(); break;
            case 4: showCalculation(); break;
        }
    } while (choice != 0);

    return 0;
}
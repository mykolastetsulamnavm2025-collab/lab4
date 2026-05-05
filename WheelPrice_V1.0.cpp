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

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Cell {
    map<string, int> products;
};

map<string, Cell> warehouse;

bool isValidAddress(const string& address) {
    setlocale(LC_ALL, "Russian");
    if (address.length() != 6) return false;

    char zone = address[0];
    string shelfStr = address.substr(1, 2);
    string sectionStr = address.substr(3, 2);
    char level = address[5];

    if (zone < 'A' || zone > 'G') return false;

    int shelf;
    try { shelf = stoi(shelfStr); }
    catch (...) { return false; }
    if (shelf < 1 || shelf > 15) return false;
)
    int section;
    try { section = stoi(sectionStr); }
    catch (...) { return false; }
    if (section < 1 || section > 11) return false;

    if (level < '1' || level > '3') return false;

    return true;
}

int getTotalQuantity(const Cell& cell) {
    setlocale(LC_ALL, "Russian");
    int total = 0;
    for (const auto& item : cell.products) {
        total += item.second;
    }
    return total;
}

void handleAdd(const vector<string>& tokens) {
    setlocale(LC_ALL, "Russian");
    if (tokens.size() != 4) {
        cout << "ќшибка: неверный формат команды ADD." << endl;
        return;
    }

    string product = tokens[1];
    int quantity;
    try { quantity = stoi(tokens[2]); }
    catch (...) {
        cout << "ќшибка: некорректное количество товара." << endl;
        return;
    }

    string address = tokens[3];
    if (!isValidAddress(address)) {
        cout << "ќшибка: некорректный адрес €чейки." << endl;
        return;
    }

    Cell& cell = warehouse[address];
    int currentTotal = getTotalQuantity(cell);

    if (currentTotal + quantity > 10) {
        cout << "ќшибка: превышена вместимость €чейки (макс. 10 единиц)." << endl;
        return;
    }

    cell.products[product] += quantity;
    cout << "ƒобавлено " << quantity << " единиц товара \"" << product << "\" в €чейку " << address << "." << endl;
}

void handleRemove(const vector<string>& tokens) {
    setlocale(LC_ALL, "Russian");
    if (tokens.size() != 4) {
        cout << "ќшибка: неверный формат команды REMOVE." << endl;
        return;
    }

    string product = tokens[1];
    int quantity;
    try { quantity = stoi(tokens[2]); }
    catch (...) {
        cout << "ќшибка: некорректное количество товара." << endl;
        return;
    }

    string address = tokens[3];
    if (!isValidAddress(address)) {
        cout << "ќшибка: некорректный адрес €чейки." << endl;
        return;
    }

    if (warehouse.find(address) == warehouse.end()) {
        cout << "ќшибка: €чейка пуста." << endl;
        return;
    }

    Cell& cell = warehouse[address];
    if (cell.products.find(product) == cell.products.end()) {
        cout << "ќшибка: товар отсутствует в €чейке." << endl;
        return;
    }

    if (cell.products[product] < quantity) {
        cout << "ќшибка: недостаточно товара дл€ списани€." << endl;
        return;
    }

    cell.products[product] -= quantity;
    if (cell.products[product] == 0) {
        cell.products.erase(product);
    }
    cout << "”далено " << quantity << " единиц товара \"" << product << "\" из €чейки " << address << "." << endl;
}

void handleInfo() {
    setlocale(LC_ALL, "Russian");
    int totalItems = 0;
    map<char, int> zoneItems;
    vector<string> occupiedCells, emptyCells;

    for (const auto& entry : warehouse) {
        const string& address = entry.first;
        const Cell& cell = entry.second;
        int total = getTotalQuantity(cell);

        if (total > 0) {
            occupiedCells.push_back(address);
            totalItems += total;
            zoneItems[address[0]] += total;
        } else {
            emptyCells.push_back(address);
        }
    }

    const int totalCapacity = 34650;
    double totalPercent = (static_cast<double>(totalItems) / totalCapacity * 100);
    cout << fixed << setprecision(2);
    cout << "ќбща€ загруженность склада: " << totalPercent << "%" << endl;

    const int zoneCapacity = 4950;
    cout << "«агруженность по зонам:" << endl;
    for (char zone = 'A'; zone <= 'G'; ++zone) {
        double percent = (static_cast<double>(zoneItems[zone]) / zoneCapacity) * 100;
        cout << "  «она " << zone << ": " << percent << "%" << endl;
    }

    cout << "—одержимое зан€тых €чеек:" << endl;
    for (const string& addr : occupiedCells) {
        const Cell& cell = warehouse[addr];
        cout << "  " << addr << ": ";
        for (const auto& item : cell.products) {
            cout << item.first << " (" << item.second << "), ";
        }
        cout << endl;
    }

    cout << "ѕустые €чейки:" << endl;
    for (const string& addr : emptyCells) {
        cout << "  " << addr << endl;
    }
}

vector<string> splitCommand(const string& line) {
    setlocale(LC_ALL, "Russian");
    vector<string> tokens;
    string token;
    for (char c : line) {
        if (c == ' ') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string line;
    while (getline(cin, line)) {
        vector<string> tokens = splitCommand(line);
        if (tokens.empty()) continue;

        string command = tokens[0];
        if (command == "ADD") {
            handleAdd(tokens);
        } else if (command == "REMOVE") {
            handleRemove(tokens);
        } else if (command == "INFO") {
            handleInfo();
        } else {
            cout << "ќшибка: неизвестна€ команда." << endl;
        }
    }
    return 0;
}

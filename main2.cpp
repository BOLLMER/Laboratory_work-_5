#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

struct Window {
    int total_time = 0;
    vector<string> tickets;
};

int main() {
    setlocale(LC_ALL, "Russian");
    int windows_count;
    cout << "¬ведите кол-во окон" << endl;
    cin >> windows_count;

    vector<pair<int, string>> visitors;
    int ticket_counter = 0;

    string command;
    while (cin >> command) {
        if (command == "ENQUEUE") {
            int time;
            cin >> time;
            ostringstream oss;
            oss << "T" << setw(3) << setfill('0') << ticket_counter;
            string ticket = oss.str();
            ticket_counter++;
            visitors.emplace_back(time, ticket);
            cout << ticket << endl;
        } else if (command == "DISTRIBUTE") {
            break;
        } else {
            cerr << "Unknown command" << endl;
            return 1;
        }
    }

    vector<Window> windows(windows_count);

    for (const auto& visitor : visitors) {
        int min_time = numeric_limits<int>::max();
        int selected_window = 0;
        for (int i = 0; i < windows_count; ++i) {
            if (windows[i].total_time < min_time) {
                min_time = windows[i].total_time;
                selected_window = i;
            }
        }
        windows[selected_window].total_time += visitor.first;
        windows[selected_window].tickets.push_back(visitor.second);
    }

    for (int i = 0; i < windows_count; ++i) {
        cout << "ќкно " << (i + 1) << " (" << windows[i].total_time << " минут): ";
        for (size_t j = 0; j < windows[i].tickets.size(); ++j) {
            if (j != 0) {
                cout << ", ";
            }
            cout << windows[i].tickets[j];
        }
        cout << endl;
    }

    return 0;
}

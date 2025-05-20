#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

vector<string> split(const string &s)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, ' '))
    {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

int main()
{
    int N;
    cin >> N;
    cin.ignore();

    map<string, string> regions;

    for (int i = 0; i < N; ++i)
    {
        string line;
        getline(cin, line);
        vector<string> parts = split(line);

        if (parts.empty()) continue;

        string command = parts[0];

        if (command == "CHANGE")
        {
            if (parts.size() < 3)
            {
                cerr << "Incorrect" << endl;
                continue;
            }
            string region = parts[1];
            string new_center = parts[2];
            if (regions.find(region) != regions.end())
            {
                string old_center = regions[region];
                regions[region] = new_center;
                cout << "Region " << region << " has changed its administrative center from "<< old_center << " to " << new_center << endl;
            }
            else
            {
                regions[region] = new_center;
                cout << "New region " << region << " with administrative center " << new_center << endl;
            }
        }

        else if (command == "RENAME")
        {
            if (parts.size() < 3)
            {
                cerr << "Incorrect" << endl;
                continue;
            }
            string old_region = parts[1];
            string new_region = parts[2];
            if (regions.find(old_region) != regions.end() && regions.find(new_region) == regions.end() && old_region != new_region)
            {
                string center = regions[old_region];
                regions.erase(old_region);
                regions[new_region] = center;
                cout << old_region << " has been renamed to " << new_region << endl;
            }
            else
            {
                cerr << "Incorrect" << endl;
            }
        }

        else if (command == "ABOUT")
        {
            if (parts.size() < 2)
            {
                cerr << "Incorrect" << endl;
                continue;
            }
            string region = parts[1];
            if (regions.find(region) != regions.end())
            {
                cout << region << " has administrative center " << regions[region] << endl;
            }
            else
            {
                cerr << "Incorrect" << endl;
            }
        }

        else if (command == "ALL")
        {
            for (const auto &pair : regions)
            {
                cout << pair.first << " - " << pair.second << endl;
            }
        }

        else
        {
            cerr << "Incorrect" << endl;
        }
    }

    return 0;
}

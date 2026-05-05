#include "Prompt.h"

#include <iostream>
#include <string>

using namespace std;

void Prompt::printMsg(string msg) { cout << msg; }
void Prompt::input(string msg, int& val) {
    printMsg(msg);
    cin >> val;
}
void Prompt::input(string msg, double& val) {
    printMsg(msg);
    cin >> val;
}
void Prompt::input(string msg, string& val) {
    printMsg(msg);
    getline(cin, val);
    if (val.length() == 0) {
        getline(cin, val);
    }
}

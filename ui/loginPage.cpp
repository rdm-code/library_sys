#include "loginPage.h"
#include "Prompt.h"
using namespace std;

bool loginPage(AuthService& auth) {
    string uname;
    string pswd;

    cout << endl;
    cout << setfill('_') << setw(70) << "" << endl;
    cout << setfill(' ') << right << setw(50) << "ANNA UNIV LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << setfill('_') << setw(70) << "" << endl;

    Prompt::input("Enter Username: ", uname);
    Prompt::input("Enter Password: ", pswd);

    return auth.login(uname, pswd);
}

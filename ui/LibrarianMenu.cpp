#include "LibrarianMenu.h"

#include <iomanip>
#include <iostream>

#include "Prompt.h"
#include "SharedUI.h"

using namespace std;

void LibrarianMenu::run(AppContainer& container) {
    int option = 0;

    cout << endl;
    cout << setfill('-') << setw(70) << "" << endl;
    cout << setfill(' ') << right << setw(55) << "ANNA UNIVERSITY LIBRARY MANAGEMENT SYSTEM"
         << endl;
    cout << setfill('-') << setw(70) << "" << endl;
    cout << setfill(' ') << right << setw(60) << "WHERE QUALITY LIBRARY SERVICES CANNOT BEMATCHED"
         << endl;
    cout << setfill('-') << setw(70) << "" << endl;
    cout << setfill(' ') << right << setw(40) << "Main Menu" << endl;
    cout << setfill('-') << setw(70) << "" << endl;

    cout << setfill(' ') << left << setw(40) << "1. Add Book Record" << setw(30)
         << "6. Return a Book" << endl
         << setw(40) << "2. Modify Book Record" << setw(30) << "7. Display all owing Members"
         << endl
         << setw(40) << "3. Show All Books" << setw(30) << "8. Show Borrower Records" << endl
         << setw(40) << "4. Search Book Record" << setw(30) << "9. Exit Application" << endl
         << setw(40) << "5. Issue Book" << endl
         << endl;

    Prompt::input("Please Enter your choice (1-9): ", option);

    if (option == 1) {
        container.getBookService().addBook();
    } else if (option == 2) {
        modifyBookUI(container.getBookService());
    } else if (option == 3) {
        container.getBookService().listBooks();
    } else if (option == 4) {
        searchBookUI(container.getBookService());
    } else if (option == 5) {
        issueBookUI(container.getUserService(), container.getBookService(),
                    container.getBorrowRecordService());
    } else if (option == 6) {
        returnBookUI(container.getUserService(), container.getBookService(),
                     container.getBorrowRecordService());
    } else if (option == 7) {
        showOwingMembersUI(container.getUserService(), container.getBookService(),
                     container.getBorrowRecordService());
    } else if (option == 8) {
        container.getBorrowRecordService().listBorrowRecords();
    } else if (option == 9) {
        container.getAuthService().logout();
    }
}

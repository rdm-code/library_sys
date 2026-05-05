#include "StudentMenu.h"

#include <iomanip>
#include <iostream>

#include "Prompt.h"
#include "SharedUI.h"

using namespace std;

void StudentMenu::run(AppContainer& container) {
    int option = 0;

    cout << endl;
    cout << setfill('-') << setw(70) << "" << endl;
    cout << setfill(' ') << setw(60) << "********************Welcome Member********************"
         << endl;
    cout << setfill('-') << setw(70) << "" << endl;
    cout << setfill(' ') << setw(50) << ">>Please choose an option<<" << endl;
    cout << setfill('-') << setw(70) << "" << endl;
    cout << setfill(' ') << endl;
    cout << "1. View Book List" << endl;
    cout << "2. Search for a Book" << endl;
    cout << "3. View Borrowed Books" << endl;
    cout << "4. Exit Application" << endl;
    Prompt::input("\nEnter your choice(1-4): ", option);

    if (option == 1) {
        container.getBookService().listBooks();
    } else if (option == 2) {
        searchBookUI(container.getBookService());
    } else if (option == 3) {
        const auto& auth = container.getAuthService();
        auto& borrowedBs = container.getBorrowRecordService().getBorrowedRecordsIf(
            [&](const BorrowRecord& br) { return br.getBorrowerId() == auth.getUser().getId(); });
        if (borrowedBs.empty()) {
            cout << "\nNo borrowed books found." << endl;
        } else {
            const auto& books = container.getBookService().findBooks(getIsbnList(borrowedBs));
            for (const auto& br : books) {
                br.display();
            }
        }
    }else if (option == 4){
        container.getAuthService().logout();
    }
    else {
        cout << "Invalid option!!!" << endl;
    }
}

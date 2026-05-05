#include "AdminMenu.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "Prompt.h"
#include "SharedUI.h"

using namespace std;

void AdminMenu::run(AppContainer& container) {
    auto& auth = container.getAuthService();
    auto& uService = container.getUserService();
    auto& bService = container.getBookService();
    auto& brService = container.getBorrowRecordService();

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
         << "9. Add a new Member" << endl
    
         << setw(40) << "2. Modify Book Record" << setw(30) << "10. Modify Member Record" << endl
    
         << setw(40) << "3. Delete Book Record" << setw(30) << "11. Delete Member Record" << endl
    
         << setw(40) << "4. Search Book Record" << setw(30) << "12. Display all owing Members" << endl
    
         << setw(40) << "5. Issue Book" << setw(30) << "13. Show All Books" << endl
    
         << setw(40) << "6. Show Borrow Records" << setw(30) << "14. Show Available Books" << endl
    
         << setw(40) << "7. Return a Book" << setw(30) << "15. Exit Application" << endl
    
         << setw(40) << "8. Show All Users" << endl
    
         << endl;

         Prompt::input("Please Enter your choice (1-15): ", option);
         
         if (option == 1) {
             bService.addBook();
         } 
         else if (option == 2) {
             modifyBookUI(bService);
         } 
         else if (option == 3) {
             const Book* book = searchBookUI(bService);
         
             if (book == nullptr) {
                 return;
             }
         
             Book updated = *book;
             bService.deleteBook(updated);
         
             cout << "[+] Successfully deleted!" << endl;
         } 
         else if (option == 4) {
             searchBookUI(bService);
         } 
         else if (option == 5) {
             issueBookUI(container.getUserService(),
                         container.getBookService(),
                         container.getBorrowRecordService());
         } 
         else if (option == 6) {
             container.getBorrowRecordService().listBorrowRecords();
         } 
         else if (option == 7) {
             returnBookUI(container.getUserService(),
                          container.getBookService(),
                          container.getBorrowRecordService());
         } 
         else if (option == 8) {
             uService.listMembers();
         } 
         else if (option == 9) {
             uService.addMember();
         } 
         else if (option == 10) {
             string key;
             Prompt::input("Enter username: ", key);
         
             const User* user = uService.findUser(key);
         
             if (user == nullptr) {
                 cout << "[X] User not found!!!" << endl;
                 return;
             }
         
             User updated = *user;
         
             cout << "\nCurrent Record:\n" << updated << endl;
         
             vector<string> fields = {
                 "Full Name",
                 "Username",
                 "Password",
                 "Role (1-ADMIN / 2-Librarian / 3-Member)"
             };
         
             string input;
         
             for (int i = 0; i < fields.size(); ++i) {
                 cout << "Enter new " << fields[i] << " (leave empty to skip): ";
         
                 if (i == 0)
                     getline(cin >> ws, input);
                 else
                     getline(cin, input);
         
                 if (input.empty()) continue;
         
                 switch (i) {
                     case 0:
                         updated.setFullname(input);
                         break;
                     case 1:
                         updated.setUsername(input);
                         break;
                     case 2:
                         updated.setPassword(input);
                         break;
                     case 3:
                         updated.setRole((Role)stoi(input));
                         break;
                 }
             }
         
             uService.updateUser(updated);
         
             cout << "[✓] User updated successfully\n";
         } 
         else if (option == 11) {
             string key;
             Prompt::input("Enter username: ", key);
         
             const User* user = uService.findUser(key);
         
             if (user == nullptr) {
                 cout << "[X] User not found!!!" << endl;
                 return;
             }
         
             User updated = *user;
         
             uService.deleteUser(updated);
         
             cout << "[+] Successfully deleted!" << endl;
         } 
         else if (option == 12) {
             showOwingMembersUI(container.getUserService(),
                          container.getBookService(),
                          container.getBorrowRecordService());
         } 
         else if (option == 13) {
             bService.listBooks();
         } 
         else if (option == 14) {
             bService.listAvailableBooks();
         } 
         else if (option == 15) {
             auth.logout();
         }
}

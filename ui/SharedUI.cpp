#include "SharedUI.h"

#include <vector>

#include "../models/Book.h"
#include "../models/BorrowRecord.h"
#include "Prompt.h"

using namespace std;

static bool askRetry() {
    int choice = 1;
    Prompt::input("Do you want to try again? 1-Yes / 0-No: ", choice);
    return choice != 0;
}

void deleteUserUI(UserService& uService) {
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

void modifyBookUI(BookService& bService) {
    const Book* book = searchBookUI(bService);

    if (book == nullptr) {
        return;
    }

    Book updated = *book;

    // field labels
    vector<string> fields = {"ISBN", "Title", "Author", "No. of Copies", "Max Days"};

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
                updated.setIsbn(input);
                break;
            case 1:
                updated.setTitle(input);
                break;
            case 2:
                updated.setAuthor(input);
                break;
            case 3:
                updated.setNoOfCopies(stoi(input));
                break;
            case 4:
                updated.setMaxBorrowDays(stoi(input));
                break;
        }
    }

    bService.updateBook(updated);

    cout << "[✓] Book updated successfully\n";
}

const Book* searchBookUI(BookService& bService) {
    string key;
    Prompt::input("Enter the isbn, title or author name: ", key);

    const Book* book = bService.findBook(key);

    if (book == nullptr) {
        cout << "[X] Book not found!!!" << endl;
    } else {
        cout << "\n" << (*book) << endl;
    }

    return book;
}

const User* searchUserUI(UserService& uService) {
    string key;
    Prompt::input("Enter username: ", key);

    const User* user = uService.findUser(key);

    if (user == nullptr) {
        cout << "[X] User not found!!!" << endl;
    } else {
        cout << "\n" << (*user) << endl;
    }

    return user;
}

void issueBookUI(UserService& uService, BookService& bService, BorrowRecordService& brService) {
    const User* userPtr;
    const Book* bookPtr;

    do {
        userPtr = searchUserUI(uService);
    } while (userPtr == nullptr && askRetry());

    if (userPtr == nullptr) return;

    do {
        bookPtr = searchBookUI(bService);
    } while (bookPtr == nullptr && askRetry());

    if (bookPtr == nullptr) return;

    User user = *userPtr;
    Book book = *bookPtr;

    // Check if book is available
    if (!book.isAvailable()) {
        cout << "[X] Book is not available!" << endl;
        return;
    }

    // Create borrow record
    BorrowRecord br(user, book);
    brService.addBorrowRecord(br);

    // Decrement available count
    // book.setAvailable(book.getAvailable() - 1);
    --book;

    // Update book in repository
    bService.updateBook(book);

    cout << "[+] Book issued successfully!" << endl;
}

void returnBookUI(UserService& uService, BookService& bService, BorrowRecordService& brService) {
    const User* userPtr;

    do {
        userPtr = searchUserUI(uService);
        int isYes = 1;

        if (userPtr == nullptr) {
            Prompt::input("Do you want to try again? 1-Yes/ 0-No: ", isYes);
            if (isYes == 0) {
                return;
            }
        }
    } while (userPtr == nullptr);

    User user = *userPtr;

    auto borrowedRecords = brService.getBorrowedRecordsIf([&](const BorrowRecord& br) {
        return !br.isReturned() && br.getBorrowerId() == user.getId();
    });

    if (borrowedRecords.empty()) {
        cout << "[X] No borrowed books found!" << endl;
        return;
    }

    std::vector<std::string> isbns;
    for (const auto& br : borrowedRecords) {
        isbns.push_back(br.getIsbn());
    }

    auto books = bService.findBooks(isbns);
    if (books.empty()) {
        cout << "[X] No borrowed books found!" << endl;
        return;
    }

    Book book;

    int opt = 0;
    do {
        cout << "\nSelect a book to return: " << endl;
        for (size_t i = 0; i < books.size(); i++) {
            auto& b = books.at(i);
            cout << i + 1 << ". " << b.getTitle() << " (ISBN: " << b.getIsbn() << ")" << endl;
        }

        Prompt::input("Enter the book number: ", opt);

        try {
            book = books.at(opt - 1);
        } catch (const std::out_of_range&) {
            cout << "[X] Invalid book number!" << endl;
            continue;
        }
        break;
    } while (true);

    brService.returnBook(book.getIsbn(), user.getId());
    bService.updateBook(++book);

    cout << "[+] Successfully returned the book!" << endl;
}

void showOwingMembersUI(UserService& uService, BookService& bService,
                        BorrowRecordService& brService) {
    auto& borrowedRecords = brService.getOwingRecords();

    if (borrowedRecords.empty()) {
        cout << "\n[X] No owing Members1!!" << endl;
        return;
    }

    vector<BorrowRecord> owingRecords;

    for (auto record : borrowedRecords) {
        const auto& bookPtr = bService.findBook(record.getIsbn());
        if (bookPtr == nullptr) {
            continue;
        }
        record.setBook(*bookPtr);

        const auto& userPtr = uService.findUser(record.getBorrowerId());
        if (userPtr == nullptr) {
            continue;
        }

        record.setUser(*userPtr);

        owingRecords.push_back(record);
    }

    if (owingRecords.empty()) {
        cout << "\n[X] No owing Members2!!" << endl;
        return;
    }

    cout << "\n[+] Owing Members:" << endl;
    cout << left << setw(20) << "Name" << setw(20) << "ISBN" << setw(45) << "Title" << setw(20)
         << "Due Date" << endl;
    for (const auto& record : owingRecords) {
        cout << left << setw(20) << record.getUser().getFullname() << setw(20) << record.getIsbn()
             << setw(45) << record.getBook().getTitle() << setw(20) << record.getDueDateStr()
             << endl;
    }

    // const auto& ids = getIdList(borrowedRecords);
    // const auto& users = uService.findUsers(ids);

    // if(users.empty()){
    //     cout << "\n[X] No borrowing members found!" << endl;
    //     return;
    // }

    // cout << "\n[+] Owing Members:" << endl;
    // for (size_t i = 0; i < users.size(); i++){
    //     auto& user = users.at(i);
    //     cout << i+1 << ". " << user.getName() << " (ID: " << user.getId() << ")" << endl;
    // }
}

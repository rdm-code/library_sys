#include "BookService.h"

#include <iomanip>
#include <iostream>

using namespace std;

BookService::BookService(JsonRepo<Book>& r) : repo(r) {}

void BookService::addBook() {
    Book b;
    b.read();
    repo.add(b);
}

const Book* BookService::findBook(const Book& key) const {
    return repo.find(key);  // find with ISBN
}

const Book* BookService::findBook(const std::string& key) const {
    const Book* res = repo.find(key);  // find with isbn

    return res;
}

const std::vector<Book> BookService::findBooks(const std::vector<std::string>& isbns) const {
    std::vector<Book> res;
    for (const auto& isbn : isbns) {
        const Book* b = repo.find(isbn);
        if (b != nullptr) {
            res.push_back(*b);
        }
    }
    return res;
}

void BookService::updateBook(const Book& b) const { repo.update(b); }
void BookService::deleteBook(const Book& b) const { repo.remove(b); }

void BookService::listBooks() {
    const auto& Books = repo.getAll();
    cout << endl
         << left << setw(20) << "ISBN" << setw(45) << "Title" << setw(20) << "Author" << setw(20)
         << "Max borrow Days" << setw(20) << "No.of Copies" << setw(20) << "Available" << endl;
    
    for (const auto& b : Books) {
        b.display();
        // std::cout << b << std::endl;
    }
}

void BookService::listAvailableBooks() {
    const auto& Books = repo.getAll();
    cout << endl
         << left << setw(20) << "ISBN" << setw(45) << "Title" << setw(20) << "Author" << setw(20)
         << "Max borrow Days" << setw(20) << "No.of Copies" << setw(20) << "Available" << endl;
    for (const auto& b : Books) {
        if (b.isAvailable()) {
            b.display();
        }
    }
}

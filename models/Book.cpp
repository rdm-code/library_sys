#include "Book.h"

#include <iomanip>
#include <iostream>

#include "../application/Helper.h"
#include "../ui/Prompt.h"

using namespace std;

Book::Book() {}

Book::Book(std::string id, std::string isbn, std::string title, std::string author, int noOfCopies,
           int avail, int maxDays) {
    setId(id);
    setIsbn(isbn);
    setTitle(title);
    setAuthor(author);
    setNoOfCopies(noOfCopies);
    setAvailable(avail);
    setMaxBorrowDays(maxDays);
}

void Book::read() {
    id = generateUUID();
    Prompt::input("Enter isbn: ", isbn);
    Prompt::input("Enter title: ", title);
    Prompt::input("Enter author: ", author);
    Prompt::input("Enter No. of Copies: ", noOfCopies);
    Prompt::input("Enter maximum borrowing days: ", maxBorrowDays);
    available = noOfCopies;
}

void Book::set(const Book& b) {
    setIsbn(b.isbn);
    setTitle(b.title);
    setAuthor(b.author);
    setNoOfCopies(b.noOfCopies);
    setMaxBorrowDays(b.maxBorrowDays);
    setAvailable(b.available);
}

void Book::setIsbn(std::string isbn) { this->isbn = isbn; }

std::string Book::getIsbn() const { return isbn; }

void Book::setId(std::string id) { this->id = id; }

std::string Book::getId() const { return id; }

void Book::setTitle(std::string title) { this->title = title; }

std::string Book::getTitle() const { return title; }

void Book::setAuthor(std::string author) { this->author = author; }

std::string Book::getAuthor() const { return author; }

void Book::setNoOfCopies(int noOfCopies) { this->noOfCopies = noOfCopies; }
void Book::setAvailable(int avail) { 
    if(avail >= noOfCopies)
        this->available = noOfCopies;
    else
        this->available = avail; }

int Book::getNoOfCopies() const { return noOfCopies; }

int Book::getAvailable() const { return available; }

bool Book::isAvailable() const { return available > 0; }

void Book::setMaxBorrowDays(int max) { maxBorrowDays = max; }

int Book::getMaxBorrowDays() const { return maxBorrowDays; }

Book& Book::operator++() {
    ++available;
    return *this;
}

Book& Book::operator--() {
    if(available > 0)
        --available;
    return *this;
}

void Book::display() const {
    // std::cout << this;
    cout << left << setw(20) << isbn << setw(45) << title << setw(20) << author << setw(20)
         << maxBorrowDays << setw(20) << noOfCopies << setw(20) << available << endl;
}

bool operator==(const Book& b1, const Book& b2) { return b1.id == b2.id; }

bool operator==(const Book& b1, const std::string& key) {
    return b1.isbn == key || b1.title == key || b1.author == key;
}

std::ostream& operator<<(std::ostream& out, const Book& b) {
    out << "ISBN: " << b.isbn << std::endl;
    out << "Title: " << b.title << std::endl;
    out << "Author: " << b.author << std::endl;
    out << "Number of copies: " << b.noOfCopies << std::endl;
    out << "Available no. of copies: " << b.available << std::endl;
    out << "Max borrow days: " << b.maxBorrowDays << std::endl;

    return out;
}

Book::~Book() {}

void from_json(const nlohmann::json& j, Book& b) {
    j.at("id").get_to(b.id);
    j.at("isbn").get_to(b.isbn);
    j.at("author").get_to(b.author);
    j.at("title").get_to(b.title);
    j.at("maxBorrowDays").get_to(b.maxBorrowDays);
    j.at("available").get_to(b.available);
    j.at("noOfCopies").get_to(b.noOfCopies);
}

void to_json(nlohmann::json& j, const Book& b) {
    j = {
        {"id", b.id},
        {"isbn", b.isbn},
        {"author", b.author},
        {"title", b.title},
        {"maxBorrowDays", b.maxBorrowDays},
        {"available", b.available},
        {"noOfCopies", b.noOfCopies},
    };
}

#include "BorrowRecord.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../application/Helper.h"
#include "Book.h"

using namespace std;

using chronoSysClk = std::chrono::system_clock;

std::string BorrowRecord::timePointToStr(const ChronoSysClk::time_point& tp) {
    std::ostringstream oss;
    std::time_t t = ChronoSysClk::to_time_t(tp);
    std::tm* tm = std::localtime(&t);
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

ChronoSysClk::time_point BorrowRecord::strToTimePoint(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::invalid_argument("Invalid date format. Expected: YYYY-MM-DD HH:MM:SS");
    }
    return ChronoSysClk::from_time_t(std::mktime(&tm));
}

BorrowRecord::BorrowRecord() {};

BorrowRecord::BorrowRecord(std::string id, std::string& bId, std::string& isbn,
                           chronoSysClk::time_point& borrowedDate,
                           chronoSysClk::time_point& dueDate,
                           const std::optional<ChronoSysClk::time_point>& rDate) {
    setId(id);
    setBorrowerId(bId);
    setIsbn(isbn);
    setBorrowedDate(borrowedDate);
    setDueDate(dueDate);
    if (rDate.has_value()) {
        markReturned(*rDate);
    }
}

BorrowRecord::BorrowRecord(std::string id, std::string& bId, std::string& isbn,
                           chronoSysClk::time_point& borrowedDate,
                           const std::optional<ChronoSysClk::time_point>& rDate) {
    setId(id);
    setBorrowerId(bId);
    setIsbn(isbn);
    setBorrowedDate(borrowedDate);
    if (rDate.has_value()) {
        markReturned(*rDate);
    }
}

BorrowRecord::BorrowRecord(User& u, Book& b) {
    user = u;
    book = b;

    id = generateUUID();
    setBorrowerId(u.id);
    setIsbn(b.isbn);

    auto t = chronoSysClk::now();
    setBorrowedDate(t);

    auto dDate = t + std::chrono::hours(24 * b.maxBorrowDays);
    setDueDate(dDate);
}

void BorrowRecord::read() {
    // TODO: to be implemented
}

void BorrowRecord::set(const BorrowRecord& br) {
    setId(br.id);
    setIsbn(br.isbn);
    setBorrowedDate(br.borrowedDate);
    setDueDate(br.dueDate);
    if (br.returnedDate.has_value()) {
        markReturned(br.returnedDate.value());
    }
}

void BorrowRecord::setUser(User u){
    user = u;
}
const User& BorrowRecord::getUser() const { return user; }

void BorrowRecord::setBook(Book b){
    book = b;
}

const Book& BorrowRecord::getBook() const { return book;}

// ID
void BorrowRecord::setId(std::string id) { this->id = id; }
std::string BorrowRecord::getId() const { return id; }

// Borrower ID
void BorrowRecord::setBorrowerId(std::string& bId) { this->borrowerId = bId; }
std::string BorrowRecord::getBorrowerId() const { return borrowerId; }

// ISBN
void BorrowRecord::setIsbn(std::string isbn) { this->isbn = isbn; }
std::string BorrowRecord::getIsbn() const { return isbn; }

// Borrow Date
void BorrowRecord::setBorrowedDate(const chronoSysClk::time_point& bDate) { borrowedDate = bDate; }
void BorrowRecord::setBorrowedDate(const std::string& bDate) {
    setBorrowedDate(strToTimePoint(bDate));
}
chronoSysClk::time_point& BorrowRecord::getBorrowedDate() { return borrowedDate; }
std::string BorrowRecord::getBorrowedDateStr() const { return timePointToStr(borrowedDate); }

// Due Date
void BorrowRecord::setDueDate(const chronoSysClk::time_point& dDate) {
    if (dDate < borrowedDate) {
        throw std::invalid_argument("Return date cannot be before borrowed date.");
    }

    dueDate = dDate;
}

void BorrowRecord::setDueDate(const std::string& dDate) {
    setDueDate(strToTimePoint(dDate));  // reuse validation in the overload above
}

const chronoSysClk::time_point& BorrowRecord::getDueDate() const { return dueDate; }

std::string BorrowRecord::getDueDateStr() const { return timePointToStr(dueDate); }

bool BorrowRecord::isOverdue() const { return !isReturned() && ChronoSysClk::now() > dueDate; }

// Return Date
void BorrowRecord::markReturned(const ChronoSysClk::time_point& rDate) {
    if (rDate < borrowedDate) {
        throw std::invalid_argument("Return date cannot be before borrowed date.");
    }
    returnedDate = rDate;
}
std::optional<ChronoSysClk::time_point> BorrowRecord::getReturnedDate() const {
    return returnedDate;
}
std::string BorrowRecord::getReturnedDateStr() const {
    return returnedDate ? timePointToStr(*returnedDate) : "Not returned yet";
}

bool BorrowRecord::isReturned() const { return returnedDate.has_value(); }

void BorrowRecord::display() const {
    // TODO
    cout << left << setw(20) << isbn << setw(40) << borrowerId << setw(20)
         << getBorrowedDateStr() << setw(20) << getDueDateStr() << setw(20)
         << getReturnedDateStr() << endl;

    // std::cout << (*this) << std::endl;
}

// JSON
void from_json(const nlohmann::json& j, BorrowRecord& b) {
    j.at("id").get_to(b.id);
    j.at("isbn").get_to(b.isbn);
    j.at("borrowerId").get_to(b.borrowerId);

    std::string bDate;
    std::string dDate;

    j.at("borrowedDate").get_to(bDate);
    j.at("dueDate").get_to(dDate);

    b.setBorrowedDate(bDate);
    b.setDueDate(dDate);
    if (j.contains("returnedDate") && !j.at("returnedDate").is_null()) {
        b.markReturned(BorrowRecord::strToTimePoint(j.at("returnedDate").get<std::string>()));
    }
    // b.markReturned(b.strToTimePoint(rDate));
}
void to_json(nlohmann::json& j, const BorrowRecord& b) {
    j = {
        {"id", b.id},
        {"isbn", b.isbn},
        {"borrowerId", b.borrowerId},
        {"borrowedDate", b.getBorrowedDateStr()},
        {"dueDate", b.getDueDateStr()},
        {"returnedDate",
         b.isReturned() ? nlohmann::json(b.getReturnedDateStr()) : nlohmann::json(nullptr)},
    };
}

bool operator==(const BorrowRecord b1, const BorrowRecord b2) { return b1.id == b2.id; }

std::ostream& operator<<(std::ostream& out, const BorrowRecord& br) {
    out << "Id: " << br.id << std::endl;
    out << "Borrower Id: " << br.borrowerId << std::endl;
    out << "Book Id: " << br.isbn << std::endl;
    out << "Issue Date: " << br.getBorrowedDateStr() << std::endl;
    out << "Due Date: " << br.getDueDateStr() << std::endl;
    out << "Returned Date: " << br.getReturnedDateStr() << std::endl;

    return out;
}

BorrowRecord::~BorrowRecord() {}

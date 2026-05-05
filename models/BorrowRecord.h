#pragma once
#include <chrono>
#include <nlohmann/json.hpp>
#include <string>

#include "Book.h"
#include "User.h"

using ChronoSysClk = std::chrono::system_clock;

class BorrowRecord {
   private:
    std::string id;
    std::string borrowerId;
    std::string isbn;
    ChronoSysClk::time_point borrowedDate;
    ChronoSysClk::time_point dueDate;
    std::optional<ChronoSysClk::time_point> returnedDate;
    Book book;
    User user;

    static std::string timePointToStr(const ChronoSysClk::time_point& tp);
    static ChronoSysClk::time_point strToTimePoint(const std::string& str);

   public:
    BorrowRecord();
    BorrowRecord(std::string id, std::string& bId, std::string& isbn,
                 ChronoSysClk::time_point& borrowedDate, ChronoSysClk::time_point& dueDate,
                 const std::optional<ChronoSysClk::time_point>& returnedDate = std::nullopt);
    BorrowRecord(std::string id, std::string& bId, std::string& isbn,
                 ChronoSysClk::time_point& dueDate,
                 const std::optional<ChronoSysClk::time_point>& returnedDate = std::nullopt);
    BorrowRecord(User& m, Book& book);

    void read();

    void set(const BorrowRecord& br);
    
    void setUser( User br);
    const User& getUser()const;
    
    void setBook( Book br);
    const Book& getBook() const;
    
    void setId(std::string id);
    std::string getId() const;

    void setBorrowerId(std::string& bId);
    std::string getBorrowerId() const;

    void setIsbn(std::string isbn);
    std::string getIsbn() const;

    void setBorrowedDate(const ChronoSysClk::time_point& bDate);
    void setBorrowedDate(const std::string& bDate);
    ChronoSysClk::time_point& getBorrowedDate();
    std::string getBorrowedDateStr() const;

    void setDueDate(const ChronoSysClk::time_point& rDate);
    void setDueDate(const std::string& bDate);
    const ChronoSysClk::time_point& getDueDate() const;
    std::string getDueDateStr() const;
    bool isOverdue() const;

    void markReturned(const ChronoSysClk::time_point& rDate = ChronoSysClk::now());
    std::optional<ChronoSysClk::time_point> getReturnedDate() const;
    std::string getReturnedDateStr() const;
    bool isReturned() const;

    void display() const;

    friend void from_json(const nlohmann::json& j, BorrowRecord& b);
    friend void to_json(nlohmann::json& j, const BorrowRecord& b);
    friend bool operator==(const BorrowRecord b1, const BorrowRecord b2);
    friend std::ostream& operator<<(std::ostream& out, const BorrowRecord& br);
    ~BorrowRecord();
};

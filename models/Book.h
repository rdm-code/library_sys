#pragma once
#include <nlohmann/json.hpp>
#include <string>

class Book {
   private:
    std::string id;
    std::string isbn;
    std::string title;
    std::string author;
    int noOfCopies;
    int maxBorrowDays;
    int available;

   public:
   Book();
    Book(std::string id, std::string isbn, std::string title, std::string author, int noOfCopies,int avail,
         int maxBorrowDays);
    
    void read();
    
    void set(const Book& b);
    
    void setIsbn(std::string isbn);
    std::string getIsbn() const;
    void setId(std::string id);
    std::string getId() const;
    void setTitle(std::string title);
    std::string getTitle() const;
    void setAuthor(std::string author);
    std::string getAuthor() const;
    void setNoOfCopies(int noOfCopies);
    int getNoOfCopies() const;
    void setAvailable(int avail);
    int getAvailable() const;
    bool isAvailable() const;
    void setMaxBorrowDays(int maxBorrowDays);
    int getMaxBorrowDays() const;
    void display() const;

    Book& operator++();
    Book& operator--();
    
    friend std::ostream& operator<<(std::ostream& out, const Book& b);
    
    /** 
     * @param const Book& b1,const Book& b2
     * @Description Compares by isbn 
     */
    friend bool operator==(const Book& b1,const Book& b2);
    
    /** 
     * @param const Book& b1,const string& title
     * @Description Compares by title 
     */
    friend bool operator==(const Book& b1, const std::string& title);
    
    ~Book();

    friend class BorrowRecord;

    friend  void from_json(const nlohmann::json& j, Book& b);
    friend  void to_json(nlohmann::json& j, const Book& b);
};

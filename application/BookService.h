#pragma once

#include "../models/Book.h"
#include "../repos/JsonRepo.h"

#include <functional>
#include <vector>

class BookService {
   private:
    JsonRepo<Book>& repo;

   public:
    BookService(JsonRepo<Book>& r);

    void addBook();
    const Book* findBook(const Book&) const;
    const Book* findBook(const std::string&) const;
    const std::vector<Book> findBooks(const std::vector<std::string>& isbns) const;
    
    
    void updateBook(const Book&) const;
    void deleteBook(const Book&) const;
    void listBooks();
    void listAvailableBooks();
};

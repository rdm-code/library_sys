#include "AppContainer.h"

#include "../Constants.h"
#include "../infrastructure/JSONLoader.h"

AppContainer::AppContainer()
    : userRepo(USERS_PATH, EventType::USER_CREATED, EventType::USER_UPDATED,
               EventType::USER_UPDATED),

      bookRepo(BOOKS_PATH, EventType::BOOK_CREATED, EventType::BOOK_UPDATED,
               EventType::BOOK_UPDATED),
      borrowRecordRepo(BORROW_RECORD_PATH, EventType::BORROW_RECORD_CREATED,
                       EventType::BORROW_RECORD_UPDATED, EventType::BORROW_RECORD_UPDATED),

      authService(userRepo),
      userService(userRepo),
      bookService(bookRepo),
      borrowRecordService(borrowRecordRepo) {
    JSONLoader::getInstance().loadFromDirectory("./data");

    userRepo.load();
    bookRepo.load();
    borrowRecordRepo.load();
}

// Getters
AuthService& AppContainer::getAuthService() { return authService; }

UserService& AppContainer::getUserService() { return userService; }

BookService& AppContainer::getBookService() { return bookService; }

BorrowRecordService& AppContainer::getBorrowRecordService() { return borrowRecordService; }

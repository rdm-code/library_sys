#pragma  once 

#include "../models/User.h"
#include "../models/Book.h"
#include "../models/BorrowRecord.h"
#include "../repos/JsonRepo.h"
#include "../application/AuthService.h"
#include "../application/UserService.h"
#include "../application/BookService.h"
#include "../application/BorrowRecordService.h"

class AppContainer {
private:
    // Repositories
    JsonRepo<User> userRepo;
    JsonRepo<Book> bookRepo;
    JsonRepo<BorrowRecord> borrowRecordRepo;

    // Services
    AuthService authService;
    UserService userService;
    BookService bookService;
    BorrowRecordService borrowRecordService;

public:
    AppContainer();

    AuthService& getAuthService();
    UserService& getUserService();
    BookService& getBookService();
    BorrowRecordService& getBorrowRecordService();
};
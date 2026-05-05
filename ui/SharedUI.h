#include "../models/User.h"
#include "../application/UserService.h"
#include "../application/BookService.h"
#include "../application/BorrowRecordService.h"

void deleteUserUI(UserService& uService);
void modifyBookUI(BookService& bService);
void issueBookUI(UserService& uService,BookService& bService,BorrowRecordService&);
void returnBookUI(UserService& uService, BookService& bService, BorrowRecordService& brService);
void showOwingMembersUI(UserService& uService, BookService& bService, BorrowRecordService& brService);

const Book* searchBookUI(BookService& bService);
const User* searchUserUI(UserService& uService);
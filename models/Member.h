#pragma once

#include "User.h"

class Member: public User{
    
    public:
    Member();
    Member(std::string id, std::string fname, std::string uname, std::string pass, Role role);
    
    friend class BorrowRecord;
};

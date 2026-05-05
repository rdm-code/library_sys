#pragma once
#include "../models/User.h"
#include "../repos/JsonRepo.h"

class UserService {
private:
    JsonRepo<User>& repo;

public:
    UserService(JsonRepo<User>& r) ;

    void addMember();
    
    const User* findUser(const User& key) const;    
    const User* findUser(const std::string& key)const;
    
    void updateUser(const User& b) const;
    void deleteUser(const User& b) const;
    
    void listMembers();
};
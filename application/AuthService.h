#pragma once
#include <string>

#include "../repos/JsonRepo.h"
#include "../models/User.h"

// class Auth {
//    private:
//     User user;
//     Auth();
//     void memberMenu(int &option);
//     void adminMenu(int &option);
//     void librarianMenu(int &option);
//     void addBook();
//     void addMember();
//     void add();

//    public:
//     Auth(const Auth&) = delete;
//     Auth& operator=(const Auth&) = delete;

//     static Auth& getInstance();

//     void displayMenu(int& opt);

//     bool login(std::string uname,std::string psw);
// };

class AuthService {
   private:
    JsonRepo<User>& userRepo;
    User user;
    bool authenticated = false;

   public:
    AuthService(JsonRepo<User>& repo);
    bool login(const std::string& uname, const std::string& psw);
    const User& getUser() const;

    bool isAdmin() const;
    bool isLibrarian() const;
    bool isMember() const;
    bool isAuthenticated() const;
    void logout();
};

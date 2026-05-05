
#include "AuthService.h"
#include "../models/User.h"

using namespace std;

AuthService::AuthService(JsonRepo<User>& repo) : userRepo(repo) {}

const User& AuthService::getUser()const{
    return user;
}

bool AuthService::login(const std::string& uname, const std::string& psw) {
    auto users = userRepo.getAll();

    for (auto& u : users) {
        if (u.getUsername() == uname && u.getPassword() == psw) {
            user = u;
            authenticated = true;
            EventBus::getInstance().publish({EventType::USER_LOGGED_IN, u});

            return true;
        }
    }

    EventBus::getInstance().publish({EventType::USER_LOGIN_FAILED, {{"username", uname}}});

    return false;
}

bool AuthService::isAuthenticated() const{
    return authenticated;
}


void AuthService::logout() {
    authenticated = false;
    EventBus::getInstance().publish({EventType::USER_LOGGED_OUT, user});

    user = User();  // Reset user
}



// bool AuthService::login(std::string uname, std::string psw) {
//     std::cout << "Login" << std::endl;
//     try {
//         User u("398283", "Reatile M", "rdm", "12345678", Role::ADMIN);
//         user = u;
//     } catch (const char* msg) {
//         std::cout << msg << std::endl;
//     }
//     return true;
// }

// void AuthService::displayMenu(int& opt) {
//     std::cout << "Display" << std::endl;
//     auto& auth = AuthService::getInstance();
//     auto& jloader = JSONLoader::getInstance();

//     if (user.getRole() == Role::ADMIN) {
//         while (opt != 11) {
//             adminMenu(opt);
//             if (opt == 7) {
//                 addMember();
//                 UsersRepo::getInstance().display();
//                 jloader.printAll();
//             }
//         }
//     } else if (user.getRole() == Role::LIBRARIAN) {
//         librarianMenu(opt);
//     } else if (user.getRole() == Role::MEMBER) {
//         memberMenu(opt);
//     }
// }

// void AuthService::addMember() {
//     User u;
//     u.read();
//     UsersRepo::getInstance().add(std::move(u));
// }

// 
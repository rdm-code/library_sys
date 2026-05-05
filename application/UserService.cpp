#include "UserService.h"
#include <iostream>
#include <iomanip>
using namespace std;

UserService::UserService(JsonRepo<User>& r) : repo(r) {}

void UserService::addMember() {
    User u;
    u.read();  
    repo.add(u);
}

const User* UserService::findUser(const User& key) const {
    return repo.find(key); // find with ISBN
}

const User* UserService::findUser(const std::string& key) const {
    const User* res =  repo.find(key); //find with isbn
    
    return res;
}

void UserService::updateUser(const User& b)const {
    repo.update(b);
}

void UserService::deleteUser(const User& b) const{
    repo.remove(b);
}

void UserService::listMembers() {
    const auto& users = repo.getAll();
    cout << left << setw(20) << "Name" << setw(20) << "Username"<< setw(20) << "Role" << endl;
    for (const auto& u : users) {
        u.display();
        // std::cout << u << std::endl;
    }
}

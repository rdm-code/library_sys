#include "User.h"

#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

#include "../application/Helper.h"
#include "../ui/Prompt.h"

std::string roleToStr(Role r) {
    switch (r) {
        case Role::ADMIN:
            return "Admin";
        case Role::LIBRARIAN:
            return "Librarian";
        case Role::STUDENT:
            return "STUDENT";
        default:
            throw "Invalid role";
    }
}

User::User() {}

User::User(std::string id, std::string fname, std::string uname, std::string pass, Role role) {
    setId(id);
    setFullname(fname);
    setUsername(uname);
    setPassword(pass);

    setRole(role);
}

void User::read() {
    id = generateUUID();
    Prompt::input("Enter fullname: ", fullname);
    Prompt::input("Enter username: ", username);
    Prompt::input("Enter password: ", password);
    int r = 0;
    Prompt::input("Enter Role(1-Admin/2-Librarian/3-Student): ", r);
    role = (Role)r;
}

void User::set(const User& u) {
    fullname = u.fullname;
    username = u.username;
    password = u.password;
    role = u.role;
}

void User::setId(std::string id) { this->id = id; }

void User::setRole(Role r) {
    std::cout << r << std::endl;
    if (r != Role::ADMIN && r != Role::LIBRARIAN && r != Role::STUDENT) {
        throw std::invalid_argument("Invalid role");
    }
    role = r;
}

void User::setFullname(std::string f) { fullname = f; }

void User::setUsername(std::string u) { username = u; }

void User::setPassword(std::string p) { password = p; }

std::string User::getId() const { return id; }
std::string User::getFullname() const { return fullname; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
const Role& User::getRole() const { return role; }

void User::display() const {
    cout << left << setw(20) << fullname << setw(20) << username << setw(20) << roleToStr(role)
         << endl;
    // std::cout << this;
}

bool operator==(const User& u1, const User& u2) { return u1.id == u2.id; }
bool operator==(const User& u, const std::string& key) { return u.id == key || u.username == key; }

std::ostream& operator<<(std::ostream& out, const User& u) {
    out << left << setw(20) << "Id" << ": "<< setw(20) << u.id << std::endl;
    out << left << setw(20) << "Fullname"<< ": " << setw(20) << u.fullname << std::endl;
    out << left << setw(20) << "Username"<< ": " << setw(20) << u.username << std::endl;
    out << left << setw(20) << "Role" << ": "<< setw(20) << roleToStr(u.role) << std::endl;

    return out;
}

void from_json(const nlohmann::json& j, User& u) {
    j.at("id").get_to(u.id);
    j.at("username").get_to(u.username);
    j.at("fullname").get_to(u.fullname);
    j.at("password").get_to(u.password);
    j.at("role").get_to(u.role);
}

void to_json(nlohmann::json& j, const User& u) {
    j = {
        {"id", u.id},
        {"fullname", u.fullname},
        {"username", u.username},
        {"password", u.password},
        {"role", u.role},
    };
}

User::~User() {}

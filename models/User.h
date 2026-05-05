#pragma once
#include <nlohmann/json.hpp>
#include <string>

enum Role { ADMIN = 1, LIBRARIAN, STUDENT };

std::string roleToStr(Role r);

class User {
   protected:
    std::string id;
    std::string fullname;
    std::string username;
    std::string password;
    Role role;

   public:
    User();
    User(std::string id, std::string fname, std::string uname, std::string pass, Role role);
    void read();
    void set(const User& u);
    void setId(std::string id);
    void setRole(Role r);
    void setFullname(std::string f);
    void setUsername(std::string u);
    void setPassword(std::string p);

    std::string getId() const;
    std::string getFullname() const;
    std::string getUsername() const;
    std::string getPassword() const;
    const Role& getRole() const;

    void display() const;

    friend std::ostream& operator<<(std::ostream& out, const User& u);
    friend bool operator==(const User&, const User&);
    friend bool operator==(const User& u, const std::string& uname);
    
    friend void from_json(const nlohmann::json& j, User& u);
    friend void to_json(nlohmann::json& j, const User& u);
    friend class BorrowRecord;
    
    ~User();
};

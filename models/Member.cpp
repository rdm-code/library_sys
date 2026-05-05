#include "Member.h"

Member::Member() {}
Member::Member(std::string id, std::string fname, std::string uname, std::string pass, Role role)
    : User(id, fname, uname, pass, role) {}

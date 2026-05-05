// #include "UsersRepo.h"

// #include "../Constants.h"
// #include "JsonRepo.h"
// #include <iostream>

// UsersRepo::UsersRepo() { path = JSONLoader::getInstance().getFullKey(USERS_PATH); }

// UsersRepo& UsersRepo::getInstance() {
//     static UsersRepo instance;
//     return instance;
// }

// std::string UsersRepo::getPath() { return path; }

// void UsersRepo::load() {
//     data.clear();
//     nlohmann::json d = JSONLoader::getInstance().get(path);

//     for (auto& j : d) {
//         auto u = j.get<User>();
//         add(u);
//     }
// }

// void UsersRepo::add(const User& u) {
//     data.emplace_back(std::move(u));
//     nlohmann::json j = data;
//     auto& loader = JSONLoader::getInstance();
//     loader.set(path, j);
//     loader.saveChanges();
// }

// void UsersRepo::update(const User& u) {
//     for (auto& user : data) {
//         if (user == u) {
//             user.set(u);
//             JSONLoader::getInstance().markDirty(path);
//         }
//     }
// }

// void UsersRepo::display() {
//     for (auto& u : data) {
//         std::cout << u << std::endl;
//     }
// }

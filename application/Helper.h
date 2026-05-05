#pragma once
#include <nlohmann/json.hpp>
#include "../models/BorrowRecord.h"

nlohmann::json loadFromFile(const std::string& filename);

void split(const std::string& str, char delimiter, std::vector<std::string>& out);

std::string generateUUID();

void adminMenu(int&);

bool contains(const std::string& s, const std::string& sub);

std::string getFullFilepath(const std::string&);

std::vector<std::string> getIsbnList(const std::vector<BorrowRecord>& borrowedBs);

// std::vector<std::string> getIdList(const std::vector<BorrowRecord>& v);

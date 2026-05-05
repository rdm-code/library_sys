#include "Helper.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <vector>

#include "../models/BorrowRecord.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

json loadJson(const std::string& filename) {
    fs::directory_entry entry(filename);

    if (entry.path().extension() != ".json") throw "Not json file.";

    std::ifstream file(entry.path());

    if (!file.is_open()) {
        throw "[File Error] Cannot open:";
    }

    json j;

    file >> j;

    if (!j.is_object()) {
        throw "[Validation Error] Not a JSON object";
    }

    return j;
}

void split(const std::string& str, char delimiter, std::vector<std::string>& out) {
    size_t start = 0;

    for (size_t i = 0; i <= str.size(); ++i) {
        if (i == str.size() || str[i] == delimiter) {
            if (i > start) {
                out.emplace_back(str.begin() + start, str.begin() + i);
            }
            start = i + 1;
        }
    }
}

std::string generateUUID() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<> dist(0, 15);

    const char* hex = "0123456789abcdef";

    std::stringstream ss;

    int groups[] = {8, 4, 4, 4, 12};

    for (int i = 0; i < 5; i++) {
        if (i > 0) ss << "-";
        for (int j = 0; j < groups[i]; j++) {
            ss << hex[dist(gen)];
        }
    }

    return ss.str();
}

bool contains(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

std::string getFullFilepath(const std::string& path) {
    std::error_code ec;
    auto p = fs::weakly_canonical(path, ec);

    if (ec) {
        std::cerr << ec.message() << std::endl;
        return path;
    }

    return p.string();
}

std::vector<std::string> getIsbnList(const std::vector<BorrowRecord>& borrowedBs) {
    std::vector<std::string> isbnList;
    std::transform(borrowedBs.begin(), borrowedBs.end(), std::back_inserter(isbnList),
                   [&](const auto& br) { return br.getIsbn(); });
    return isbnList;
}

// template<typename T>
// std::vector<std::string> getIdList(const std::vector<BorrowRecord>& v) {
//     std::vector<std::string> idList;
//     std::transform(v.begin(), v.end(), std::back_inserter(idList),
//                    [&](const auto& br) { return br.borrowerId(); });
//     return idList;
// }
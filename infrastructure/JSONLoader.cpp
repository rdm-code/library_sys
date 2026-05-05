#include "JSONLoader.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../application/Helper.h"
#include "../core/EventBus.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

// Constructor
JSONLoader::JSONLoader() {
    // Subscribe to FILE_CHANGED so any watcher-detected change auto-reloads
    EventBus::getInstance().subscribe(EventType::FILE_CHANGED, [this](const Event& e) {
        // std::cout << "in JsonLoader callback" << std::endl;

        auto payload = e.payload;
        const auto path = payload["path"].get<std::string>();

        if (path != "") reloadFile(path);
    });

    std::cout << "JSONLoader initialized\n";
}

// Singleton instance
JSONLoader& JSONLoader::getInstance() {
    static JSONLoader instance;
    return instance;
}

// Load JSON files
void JSONLoader::loadFromDirectory(const std::string& folderPath) {
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        std::cerr << "Invalid directory: " << folderPath << std::endl;
        return;
    }
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;
        try {
            std::ifstream file(entry.path());
            if (!file.is_open()) {
                std::cerr << "Failed to open: " << entry.path() << std::endl;
                continue;
            }
            json j;
            file >> j;

            // j = {{"version", 0}, {"data", j}};

            std::string key = fs::canonical(entry.path()).string();
            data[key] = {j, false};
        } catch (const std::exception& e) {
            std::cerr << "Error loading " << entry.path() << ": " << e.what() << std::endl;
        }
    }
}

void JSONLoader::reloadFile(const std::string& path) {
    // std::cout << "in JsonLoader" << std::endl;

    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "[JSONLoader] Cannot reopen: " << path << std::endl;
            return;
        }
        json j;
        file >> j;

        // j = {{"version", 0}, {"data", j}};

        std::string key = fs::canonical(path).string();
        data[key] = {j, false};  // fresh from disk — not dirty

        std::cout << "[JSONLoader] Reloaded: " << key << std::endl;
        EventBus::getInstance().publish({EventType::DATA_RELOADED, {{"path", key}}});
    } catch (const std::exception& e) {
        std::cerr << "[JSONLoader] Reload failed for " << path << ": " << e.what() << std::endl;
    }
}

void JSONLoader::set(const std::string& key, const nlohmann::json j) { data[key] = {j, true}; }

std::string JSONLoader::getFullKey(const std::string& str) {
    for (auto& [key, value] : data) {
        if (contains(key, str)) return key;
    }
    return str;
}

void JSONLoader::markDirty(const std::string& key) { data[key].dirty = true; }

json JSONLoader::get(const std::string& key) {
    if (data.find(key) == data.end()) {
        std::cerr << "[JSONLoader] Key not found: " << key << std::endl;
        return json::object();
    }
    return data[key].data;
}

// Get all data
const std::map<std::string, JSONEntry>& JSONLoader::getAll() const { return data; }

void JSONLoader::saveChanges() {
    for (auto& [path, entry] : data) {
        if (!entry.dirty) continue;

        std::string tempPath = path + ".tmp";

        std::ofstream out(tempPath);
        if (!out.is_open()) {
            std::cerr << "Write failed: " << tempPath << "\n";
            continue;
        }

        out << entry.data.dump(4);
        out.close();

        std::filesystem::rename(tempPath, path);

        entry.dirty = false;
    }
}

// Print all
void JSONLoader::printAll() const {
    for (const auto& j : data) {
        std::cout << j.first << " : " << j.second.data.dump(4) << std::endl;
    }
}

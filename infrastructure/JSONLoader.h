#pragma once
#include <utility>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

struct JSONEntry{
    nlohmann::json data;
    bool dirty;
};

class JSONLoader {
private:
    std::map<std::string, JSONEntry> data;

    // Private constructor
    JSONLoader();

public:
    // Delete copy operations
    JSONLoader(const JSONLoader&) = delete;
    JSONLoader& operator=(const JSONLoader&) = delete;
    
    // Singleton access
    static JSONLoader& getInstance();

    // Load JSON files
    void loadFromDirectory(const std::string& folderPath);
    
    void reloadFile(const std::string& path);

    void set(const std::string& key,const nlohmann::json data);
    
    std::string getFullKey(const std::string &str);
    
    void markDirty(const std::string& key);
    
    nlohmann::json get(const std::string& key);    
    
    /** Save changes */
    void saveChanges();
    
    // Access data
    const std::map<std::string, JSONEntry>& getAll() const;

    void printAll() const;
};
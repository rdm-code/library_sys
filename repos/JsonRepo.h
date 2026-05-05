#pragma once

#include <algorithm>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <iostream>

#include "../application/Helper.h"
#include "../core/EventBus.h"
#include "../infrastructure/JSONLoader.h"

template <typename T>
class JsonRepo {
   private:
    std::vector<T> data;
    std::string path;
    int version = 0;

    EventType createEvent;
    EventType updateEvent;
    EventType deleteEvent;

   public:
    JsonRepo(const std::string& p, EventType cEvent, EventType uEvent, EventType dEvent)
        : createEvent(cEvent), updateEvent(uEvent), deleteEvent(dEvent) {
        path = getFullFilepath(p);
        
        EventBus::getInstance().subscribe(EventType::DATA_RELOADED,
                [this](const Event& e) {
                    // std::cout << "in JsonRepo callback" << std::endl;
                    
                    auto payload = e.payload;            
                    auto reloadedPath = payload["path"].get<std::string>(); 
                    
                    // std::cout << "Path: " << path << " ReloadedPath: " << reloadedPath << std::endl;
                    
                    if (reloadedPath == path) {
                        reload();
                    }
                });
    }

    void load() {
        // std::cout << "in JsonRepo load" << std::endl;

        auto j = JSONLoader::getInstance().get(path);

        version = j.value("version", 0);
        
        data.clear();
        for (auto& item : j["data"]) {
            data.push_back(item.get<T>());
        }
    }

    void reload() {
        // std::cout << "in JsonRepo reload" << std::endl;
        auto latest = JSONLoader::getInstance().get(path);
        int diskVersion = latest.value("version", 0);
    
        if (diskVersion == version) return; // nothing actually changed
    
        std::vector<T> diskData;
        for (auto& item : latest["data"]) {
            diskData.push_back(item.get<T>());
        }
    
        // Merge: keep local items that aren't on disk yet (pending adds)
        for (auto& local : data) {
            if (!existsIn(diskData, local)) {
                diskData.push_back(local);
            }
        }
    
        data = diskData;
        version = diskVersion;
        std::cout << "[JsonRepo] Reloaded and merged from disk\n";
    }
    
    void add(const T& item) {
        auto latest = JSONLoader::getInstance().get(path);
        int fileVersion = latest.value("version", 0);
    
        if (fileVersion != version) {
            handleConflict(latest);  // sync data and version with disk
        }
    
        data.push_back(item);
        version++;
        persist();
        EventBus::getInstance().publish({createEvent, {{"item", nlohmann::json(item)}}});
    }
    
    void handleConflict(const nlohmann::json& latest) {
        std::vector<T> newData;

        for (auto& item : latest["data"]) {
            newData.push_back(item.get<T>());
        }

        // simple merge: append missing items
        for (auto& local : data) {
            if (!existsIn(newData, local)) {
                newData.push_back(local);
            }
        }

        data = newData;
        version = latest.value("version", version);
    }

    void update(const T& obj) {
        auto latest = JSONLoader::getInstance().get(path);
        int fileVersion = latest.value("version", 0);
    
        if (fileVersion != version) handleConflict(latest);
    
        for (auto& item : data) {
            if (item == obj) item.set(obj);
        }
        version++;
        persist();
        EventBus::getInstance().publish({updateEvent, {{"item", nlohmann::json(obj)}}});
    }
    
    void remove(const T& obj) {
        auto latest = JSONLoader::getInstance().get(path);
        int fileVersion = latest.value("version", 0);
    
        if (fileVersion != version) handleConflict(latest);
    
        data.erase(
            std::remove_if(data.begin(), data.end(), [&](const T& item) { return item == obj; }),
            data.end());
        version++;
        persist();
        EventBus::getInstance().publish({deleteEvent, {{"item", nlohmann::json(obj)}}});
    }

    template <typename U>
    const T* find(const U& key) {
        for (const auto& item : data) {
            if (item == key) {
                return &item;
            }
        }
        return nullptr;  // not found
    }

    template <typename Predicate>
    const T* findIf(Predicate pred) {
        for (const auto& item : data) {
            if (pred(item)) {
                return &item;
            }
        }
        return nullptr;
    }

    const std::vector<T>& getAll() const { return data; }

   private:
    void persist() {
        nlohmann::json j;
        j["version"] = version;
        j["data"] = data;

        JSONLoader::getInstance().set(path, j);
        JSONLoader::getInstance().saveChanges();
    }

    bool existsIn(const std::vector<T>& list, const T& item) {
        for (auto& d : list) {
            if (d == item) {
                return true;
            }
        }
        return false;
    }
};

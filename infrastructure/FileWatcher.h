#pragma once
#include <filesystem>
#include <map>
#include <string>

using PathStr = std::string;

class FileWatcher {
   public:
    static FileWatcher& getInstance() {
        static FileWatcher instance;
        return instance;
    }

    void watchDirectory(const PathStr& dir);
    void poll();

   private:
    FileWatcher() = default;
    std::map<PathStr, std::filesystem::file_time_type> files;
};
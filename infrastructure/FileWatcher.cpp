#include "FileWatcher.h"

#include <filesystem>

#include "../core/EventBus.h"

namespace fs = std::filesystem;

void FileWatcher::watchDirectory(const PathStr& dir) {
    if (!fs::exists(dir) || !fs::is_directory(dir)) return;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;
        
        files[fs::canonical(entry.path()).string()] = fs::last_write_time(entry.path());
    }
}

void FileWatcher::poll() {
    for (auto& [path, knownTime] : files) {
        std::error_code ec;
        auto currentTime = fs::last_write_time(path, ec);
        if (ec) continue;  // file may have been deleted mid-poll

        if (currentTime != knownTime) {
            knownTime = currentTime;
            
            // std::cout << "Publishing..." << path << std::endl;
            EventBus::getInstance().publish({EventType::FILE_CHANGED, {{"path", path}}});
            // std::cout << "Event published " << path << std::endl;
            
        }
    }
}

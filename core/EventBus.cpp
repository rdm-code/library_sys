#include "EventBus.h"

EventBus& EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

void EventBus::subscribe(EventType type, std::function<void(const Event&)> callback) {
    listeners[type].push_back(callback);
}

void EventBus::publish(const Event& event) {
    for (auto& cb : listeners[event.type]) {
        cb(event);
    }
}

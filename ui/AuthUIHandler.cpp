#include "AuthUIHandler.h"

#include <iostream>

#include "../core/EventBus.h"

void registerAuthUIHandlers() {
    EventBus::getInstance().subscribe(EventType::USER_LOGIN_FAILED, [](const Event& e) {
        std::cout << "[❌] Invalid username or password.\n";
    });

    EventBus::getInstance().subscribe(EventType::USER_LOGGED_IN, [](const Event& e) {
        std::cout << "[+] Login successful!" << std::endl;
    });
}

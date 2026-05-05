#include <iostream>

#include "Constants.h"
#include "application/AuthService.h"
#include "core/AppContainer.h"
#include "ui/Menu.h"
#include "ui/AuthUIHandler.h"
#include "ui/loginPage.h"
#include "infrastructure/FileWatcher.h"

using namespace std;

int main() {    
    // 1. Load initial data
    JSONLoader::getInstance().loadFromDirectory(ROOT_PATH);
    
    // 2. Register the same directory with the watcher
    FileWatcher::getInstance().watchDirectory(ROOT_PATH);
    
    auto& loader = JSONLoader::getInstance();
    loader.loadFromDirectory(ROOT_PATH);
    
    registerAuthUIHandlers();
    
    AppContainer container;
    
    auto& auth = container.getAuthService();
    // auto& userService = container.getUserService();
    // auto& bookService = container.getBookService();
    // auto& borrowRecordService = container.getBorrowRecordService();
    
    while(!loginPage(auth)){
        cout << "Please try again!!!" << endl;
    }
    
    while(true){
        Menu::show(container);
        FileWatcher::getInstance().poll();
        if (!auth.isAuthenticated()) {
            break;
        }
    }
    
    std::cout << "Cleaning up before exit...\n";  
    loader.saveChanges();
}
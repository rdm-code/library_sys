#pragma once
#include <iostream>
#include "../application/UserService.h"
#include "../application/BookService.h"
using namespace std;

#pragma once

#include "../application/AuthService.h"
#include "../core/AppContainer.h"

class Menu {
public:
    static void show(AppContainer &);
};
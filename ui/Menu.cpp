#include "Menu.h"
#include "AdminMenu.h"
#include "LibrarianMenu.h"
#include "StudentMenu.h"

void Menu::show(AppContainer& container) {
    auto& auth = container.getAuthService();
    if (!auth.isAuthenticated()) return;

    auto& role = auth.getUser().getRole();

    switch (role) {
        case Role::ADMIN:
            AdminMenu::run(container);
            break;

        case Role::LIBRARIAN:
            LibrarianMenu::run(container);
            break;

        case Role::STUDENT:
            StudentMenu::run(container);
            break;
    }
}

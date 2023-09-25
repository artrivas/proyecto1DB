
#include <iostream>

#include "Menu.hpp"
#include "DataManager.hpp"

#include "./records/Player.hpp"


int main() {
    Parser parser;
    DataManager<Player, int> data_manager;
    Menu<Player, int> menu(data_manager, parser);

    menu.display_main();

    return 0;
}
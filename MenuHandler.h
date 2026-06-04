#pragma once
#include "VaultManager.h"

class MenuHandler {
private:
    VaultManager vault;
    void display_menu();
    void add_entry();
    void find_entry();
    void delete_entry();
    void list_all();
    void change_password();

public:
    MenuHandler();
    void request_master_password();
    void run();
};

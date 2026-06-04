#include "MenuHandler.h"
#include <iostream>
#include <string>
#include <algorithm>

MenuHandler::MenuHandler() {}

void MenuHandler::request_master_password() {
    int attempts = 0;
    const int max_attempts = 3;

    while (attempts < max_attempts) {
        std::cout << "Введите мастер-пароль: ";
        std::string pwd;
        std::getline(std::cin, pwd);

        if (pwd.empty()) {
            std::cout << "Пароль не может быть пустым.\n";
            continue;
        }

        // Нормализация пароля до 16 байт (128 бит)
        std::array<unsigned char, 16> key;
        key.fill(0);
        for (size_t i = 0; i < pwd.size() && i < 16; ++i) {
            key[i] = static_cast<unsigned char>(pwd[i]);
        }

        // Зануляем строку пароля в памяти
        std::fill(pwd.begin(), pwd.end(), '\0');

        vault.set_session_key(key);

        if (vault.load_from_file()) {
            std::cout << "Хранилище разблокировано.\n";
            return;
        }

        attempts++;
        if (attempts < max_attempts) {
            std::cout << "Неверный мастер-пароль! Осталось попыток: " 
                      << (max_attempts - attempts) << "\n";
        }
    }

    std::cout << "Превышено количество попыток. Выход.\n";
    exit(0);
}

void MenuHandler::display_menu() {
    std::cout << "\n===== Менеджер паролей =====\n"
              << "1. Добавить запись\n"
              << "2. Найти запись\n"
              << "3. Удалить запись\n"
              << "4. Вывести все записи\n"
              << "5. Сменить пароль\n"
              << "0. Выход\n"
              << "============================\n"
              << "Выберите действие: ";
}

void MenuHandler::add_entry() {
    std::string service;
    std::cout << "Введите название сервиса: ";
    std::getline(std::cin, service);
    if (service.empty()) {
        std::cout << "Название сервиса не может быть пустым.\n";
        return;
    }

    // Проверяем, существует ли сервис
    std::vector<int> matches = vault.find_all_by_service(service);
    if (!matches.empty()) {
        auto& entries = vault.get_all();
        std::cout << "\nСервис \"" << service << "\" уже существует ("
                  << matches.size() << " аккаунт(ов)):\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " 
                      << entries[matches[i]].get_login()
                      << " (Обновлено: " << entries[matches[i]].get_date() << ")\n";
        }
        std::cout << "\nВыберите действие:\n"
                  << "  a) Добавить ещё один аккаунт\n"
                  << "  b) Сменить пароль существующего аккаунта\n"
                  << "  c) Отмена\n"
                  << "Ваш выбор: ";
        
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "b" || choice == "B") {
            // Смена пароля для выбранного аккаунта
            int target_index;
            if (matches.size() == 1) {
                target_index = matches[0];
            } else {
                std::cout << "Введите номер аккаунта: ";
                std::string num_str;
                std::getline(std::cin, num_str);
                int num = std::stoi(num_str);
                if (num < 1 || num > (int)matches.size()) {
                    std::cout << "Неверный номер.\n";
                    return;
                }
                target_index = matches[num - 1];
            }
            std::string new_password;
            std::cout << "Введите новый пароль: ";
            std::getline(std::cin, new_password);
            if (new_password.empty()) {
                std::cout << "Пароль не может быть пустым.\n";
                return;
            }
            entries[target_index].set_password(new_password);
            vault.save_to_file();
            std::cout << "Пароль успешно изменён!\n";
            return;
        } else if (choice != "a" && choice != "A") {
            std::cout << "Отмена.\n";
            return;
        }
        // Если выбрали "a" — продолжаем добавление нового аккаунта
    }

    std::string login, password;
    std::cout << "Введите логин: ";
    std::getline(std::cin, login);
    if (login.empty()) {
        std::cout << "Логин не может быть пустым.\n";
        return;
    }
    std::cout << "Введите пароль: ";
    std::getline(std::cin, password);
    if (password.empty()) {
        std::cout << "Пароль не может быть пустым.\n";
        return;
    }

    vault.add_entry(Entry(service, login, password));
    vault.save_to_file();
    std::cout << "Запись успешно добавлена!\n";
}

void MenuHandler::find_entry() {
    std::string service;
    std::cout << "Введите название сервиса для поиска: ";
    std::getline(std::cin, service);

    std::vector<int> matches = vault.find_all_by_service(service);
    if (matches.empty()) {
        std::cout << "Запись не найдена.\n";
        return;
    }

    auto& entries = vault.get_all();
    if (matches.size() == 1) {
        int idx = matches[0];
        std::cout << "\nСервис: " << entries[idx].get_service() << "\n"
                  << "Логин: " << entries[idx].get_login() << "\n"
                  << "Пароль: " << entries[idx].get_password() << "\n"
                  << "Дата изменения: " << entries[idx].get_date() << "\n";
    } else {
        std::cout << "\nНайдено " << matches.size() << " аккаунтов для \"" << service << "\":\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            int idx = matches[i];
            std::cout << "\n--- Аккаунт " << (i + 1) << " ---\n"
                      << "Логин: " << entries[idx].get_login() << "\n"
                      << "Пароль: " << entries[idx].get_password() << "\n"
                      << "Дата изменения: " << entries[idx].get_date() << "\n";
        }
    }
}

void MenuHandler::delete_entry() {
    std::string service;
    std::cout << "Введите название сервиса для удаления: ";
    std::getline(std::cin, service);

    std::vector<int> matches = vault.find_all_by_service(service);
    if (matches.empty()) {
        std::cout << "Запись не найдена.\n";
        return;
    }

    auto& entries = vault.get_all();
    int target_index;

    if (matches.size() == 1) {
        target_index = matches[0];
        std::cout << "Удалить запись \"" << entries[target_index].get_service()
                  << "\" (логин: " << entries[target_index].get_login() << ")? (y/n): ";
    } else {
        std::cout << "\nНайдено " << matches.size() << " аккаунтов для \"" << service << "\":\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            int idx = matches[i];
            std::cout << "  " << (i + 1) << ". " << entries[idx].get_login()
                      << " (Обновлено: " << entries[idx].get_date() << ")\n";
        }
        std::cout << "Введите номер аккаунта для удаления: ";
        std::string num_str;
        std::getline(std::cin, num_str);
        int num = std::stoi(num_str);
        if (num < 1 || num > (int)matches.size()) {
            std::cout << "Неверный номер.\n";
            return;
        }
        target_index = matches[num - 1];
        std::cout << "Удалить аккаунт \"" << entries[target_index].get_login() << "\"? (y/n): ";
    }

    std::string confirm;
    std::getline(std::cin, confirm);
    if (confirm == "y" || confirm == "Y") {
        vault.remove_entry(target_index);
        vault.save_to_file();
        std::cout << "Запись успешно удалена!\n";
    } else {
        std::cout << "Удаление отменено.\n";
    }
}

void MenuHandler::list_all() {
    auto& entries = vault.get_all();
    if (entries.empty()) {
        std::cout << "Хранилище пусто.\n";
        return;
    }
    std::cout << "\nСохраненные записи (" << entries.size() << "):\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << entries[i].get_service()
                  << " — " << entries[i].get_login()
                  << " (Обновлено: " << entries[i].get_date() << ")\n";
    }
}

void MenuHandler::change_password() {
    std::string service;
    std::cout << "Введите название сервиса: ";
    std::getline(std::cin, service);

    std::vector<int> matches = vault.find_all_by_service(service);
    if (matches.empty()) {
        std::cout << "Запись не найдена.\n";
        return;
    }

    auto& entries = vault.get_all();
    int target_index;

    if (matches.size() == 1) {
        target_index = matches[0];
        std::cout << "Аккаунт: " << entries[target_index].get_login() << "\n";
    } else {
        std::cout << "\nНайдено " << matches.size() << " аккаунтов для \"" << service << "\":\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            int idx = matches[i];
            std::cout << "  " << (i + 1) << ". " << entries[idx].get_login()
                      << " (Обновлено: " << entries[idx].get_date() << ")\n";
        }
        std::cout << "Введите номер аккаунта: ";
        std::string num_str;
        std::getline(std::cin, num_str);
        int num = std::stoi(num_str);
        if (num < 1 || num > (int)matches.size()) {
            std::cout << "Неверный номер.\n";
            return;
        }
        target_index = matches[num - 1];
    }

    std::string new_password;
    std::cout << "Введите новый пароль: ";
    std::getline(std::cin, new_password);
    if (new_password.empty()) {
        std::cout << "Пароль не может быть пустым.\n";
        return;
    }

    entries[target_index].set_password(new_password);
    vault.save_to_file();
    std::cout << "Пароль успешно изменён!\n";
}

void MenuHandler::run() {
    request_master_password();

    std::string choice;
    while (true) {
        display_menu();
        std::getline(std::cin, choice);

        if (choice == "1") {
            add_entry();
        } else if (choice == "2") {
            find_entry();
        } else if (choice == "3") {
            delete_entry();
        } else if (choice == "4") {
            list_all();
        } else if (choice == "5") {
            change_password();
        } else if (choice == "0") {
            std::cout << "Выход из программы.\n";
            break;
        } else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

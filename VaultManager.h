#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "Entry.h"
#include <fstream>
#include <iomanip>
#include <sstream>

class VaultManager
{
private:
    std::vector<Entry> entries;
public:
    VaultManager() {};
    ~VaultManager() {};

    void add_entry(const Entry& entry) {entries.push_back(entry);}
    void remove_entry(int index) {
        if (index >= 0 && index < entries.size()) {
            entries.erase(entries.begin() + index);
        }
    }

    std::vector<Entry>& get_all() {return entries;}
    Entry* find_info(std::string service) {
        for (int i = 0; i < entries.size(); i++) {
            if (entries[i].get_service() == service) {
                return &entries[i];
            }
        }
        return 0;
    }

    std::string string_to_hex(const std::string& text) {
        std::stringstream hex_string;
        for (int i = 0; i < text.size(); i++) {
            hex_string << std::hex << std::uppercase << std::setw(2) 
            << std::setfill('0') << (int)(unsigned char)text[i];
        }
        return hex_string.str();
    }

    std::string hex_to_string(const std::string& hex_string) {
    std::stringstream text;
    for (int i = 0; i < hex_string.size(); i += 2) {
        std::string sub = hex_string.substr(i, 2);
        unsigned char ch = std::stoi(sub, 0, 16);
        text << ch;
    }
    return text.str();
}

    void save_to_file() {
        std::ofstream out("passwords.txt");
        if (out.is_open()){
            for (int i = 0; i < entries.size(); i++) { 
                out << string_to_hex(entries[i].get_service()) << std::endl 
                << string_to_hex(entries[i].get_login()) << std::endl
                << string_to_hex(entries[i].get_password()) << std::endl
                << string_to_hex(entries[i].get_date()) << std::endl;
            }
        }
    }

    void load_from_file() {
        std::ifstream in("passwords.txt");
        if (in.is_open()){
            std::string service_hex, login_hex, password_hex, date_hex;

            entries.clear();

            while (std::getline(in, service_hex) && std::getline(in, login_hex) &&
            std::getline(in, password_hex) && std::getline(in, date_hex)) {
                
                std::string service = hex_to_string(service_hex);
                std::string login = hex_to_string(login_hex);
                std::string password = hex_to_string(password_hex);
                std::string date = hex_to_string(date_hex);

                add_entry(Entry(service, login, password, date));
            }
        }
    }
};
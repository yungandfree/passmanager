#include "VaultManager.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

const std::string VaultManager::CHECK_PHRASE = "PASSMANAGER_OK";

VaultManager::VaultManager() {
    session_key.fill(0);
}

VaultManager::~VaultManager() {
    std::fill(session_key.begin(), session_key.end(), 0);
    entries.clear();
}

void VaultManager::set_session_key(const std::array<unsigned char, 16>& key) {
    session_key = key;
}

void VaultManager::add_entry(const Entry& entry) {
    entries.push_back(entry);
}

void VaultManager::remove_entry(int index) {
    if (index >= 0 && index < (int)entries.size()) {
        entries.erase(entries.begin() + index);
    }
}

std::vector<Entry>& VaultManager::get_all() {
    return entries;
}

std::vector<int> VaultManager::find_all_by_service(const std::string& service) {
    std::vector<int> indices;
    for (int i = 0; i < (int)entries.size(); i++) {
        if (entries[i].get_service() == service) {
            indices.push_back(i);
        }
    }
    return indices;
}

std::string VaultManager::vector_to_hex(const std::vector<unsigned char>& vec) {
    std::stringstream hex_string;
    for (size_t i = 0; i < vec.size(); i++) {
        hex_string << std::hex << std::uppercase << std::setw(2) 
        << std::setfill('0') << (int)vec[i];
    }
    return hex_string.str();
}

std::vector<unsigned char> VaultManager::hex_to_vector(const std::string& hex_string) {
    std::vector<unsigned char> vec;
    for (size_t i = 0; i < hex_string.size(); i += 2) {
        std::string sub = hex_string.substr(i, 2);
        unsigned char ch = static_cast<unsigned char>(std::stoi(sub, 0, 16));
        vec.push_back(ch);
    }
    return vec;
}

void VaultManager::save_to_file() {
    std::ofstream out("passwords.txt");
    if (out.is_open()) {
        std::vector<unsigned char> enc_check = cipher.encrypt(CHECK_PHRASE, session_key);
        out << vector_to_hex(enc_check) << std::endl;

        for (size_t i = 0; i < entries.size(); i++) {
            std::vector<unsigned char> enc_service  = cipher.encrypt(entries[i].get_service(), session_key);
            std::vector<unsigned char> enc_login    = cipher.encrypt(entries[i].get_login(), session_key);
            std::vector<unsigned char> enc_password  = cipher.encrypt(entries[i].get_password(), session_key);
            std::vector<unsigned char> enc_date      = cipher.encrypt(entries[i].get_date(), session_key);

            out << vector_to_hex(enc_service) << std::endl
                << vector_to_hex(enc_login) << std::endl
                << vector_to_hex(enc_password) << std::endl
                << vector_to_hex(enc_date) << std::endl;
        }
    }
}

bool VaultManager::load_from_file() {
    std::ifstream in("passwords.txt");
    if (!in.is_open()) {
        return true;
    }

    std::string check_hex;
    if (!std::getline(in, check_hex) || check_hex.empty()) {
        return true;
    }

    std::vector<unsigned char> enc_check = hex_to_vector(check_hex);
    std::string decrypted_check = cipher.decrypt(enc_check, session_key);

    if (decrypted_check != CHECK_PHRASE) {
        return false;
    }

    entries.clear();
    std::string service_hex, login_hex, password_hex, date_hex;

    while (std::getline(in, service_hex) && std::getline(in, login_hex) &&
           std::getline(in, password_hex) && std::getline(in, date_hex)) {

        std::string service  = cipher.decrypt(hex_to_vector(service_hex), session_key);
        std::string login    = cipher.decrypt(hex_to_vector(login_hex), session_key);
        std::string password = cipher.decrypt(hex_to_vector(password_hex), session_key);
        std::string date     = cipher.decrypt(hex_to_vector(date_hex), session_key);

        add_entry(Entry(service, login, password, date));
    }

    return true;
}

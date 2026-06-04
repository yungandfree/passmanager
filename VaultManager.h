#pragma once
#include <string>
#include <vector>
#include <array>
#include "Entry.h"
#include "Cipher.h"

class VaultManager
{
private:
    static const std::string CHECK_PHRASE;
    Cipher cipher;
    std::vector<Entry> entries;
    std::array<unsigned char, 16> session_key;

    std::string vector_to_hex(const std::vector<unsigned char>& vec);
    std::vector<unsigned char> hex_to_vector(const std::string& hex_string);

public:
    VaultManager();
    ~VaultManager();

    void set_session_key(const std::array<unsigned char, 16>& key);
    void add_entry(const Entry& entry);
    void remove_entry(int index);
    std::vector<Entry>& get_all();
    std::vector<int> find_all_by_service(const std::string& service);

    void save_to_file();
    bool load_from_file();
};
#pragma once
#include <string>
#include <vector>
#include <array>

class Cipher {
private:
    static const int SALT_LEN = 4;
    int Nb = 4;
    int Nk = 4;
    int Nr = 10;

    std::vector<unsigned char> w;

    unsigned char xtimes(unsigned char b);
    unsigned char multiply(unsigned char a, unsigned char b);
    
    static const unsigned char sbox[256];
    static const unsigned char rsbox[256];
    static const unsigned char rcon[11];

    void RotWord(std::array<unsigned char, 4>& word);
    void SubWord(std::array<unsigned char, 4>& word);
    void SubBytes(unsigned char state[4][4]);
    void InvSubBytes(unsigned char state[4][4]);
    void ShiftRows(unsigned char state[4][4]);
    void InvShiftRows(unsigned char state[4][4]);
    void MixColumns(unsigned char state[4][4]);
    void InvMixColumns(unsigned char state[4][4]);
    void AddRoundKey(unsigned char state[4][4], int round);
    std::vector<unsigned char> add_padding(const std::vector<unsigned char>& data);
    std::vector<unsigned char> generate_salt();

public: 
    Cipher();
    ~Cipher();
    void KeyExpansion(std::array<unsigned char, 16> key);
    std::vector<unsigned char> encrypt(const std::string& data, const std::array<unsigned char, 16>& key);
    std::string decrypt(const std::vector<unsigned char>& encryptedtext, const std::array<unsigned char, 16>& key);
};

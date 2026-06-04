#pragma once
#include <string>

class Entry
{
private:
    std::string service; 
    std::string login; 
    std::string password;
    std::string date;

    std::string current_time_to_string();

public:
    Entry(std::string service, std::string login, std::string password);
    Entry(std::string service, std::string login, std::string password, std::string date);
    ~Entry();
    
    std::string get_service() const;
    std::string get_login() const;
    std::string get_password() const;
    std::string get_date() const;

    void set_service(std::string new_service);
    void set_login(std::string new_login);
    void set_password(std::string new_password);
};

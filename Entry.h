#pragma once
#include <string>
#include <ctime>

class Entry
{
private:
    std::string service; 
    std::string login; 
    std::string password;
    std::time_t date;
public:
    Entry(std::string service, std::string login, std::string password):
    service(service), login(login), password(password), date(std::time(0))
    {};
    
    ~Entry() {};
    
    std::string get_service() {return service;}
    std::string get_login() {return login;}
    std::string get_password() {return password;}
    std::time_t get_date() {return date;}

    void set_service(std::string new_service) {service = new_service;}
    void set_login(std::string new_login) {login = new_login;}
    void set_password(std::string new_password) {password = new_password; date = std::time(0);}    
};

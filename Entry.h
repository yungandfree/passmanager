#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

class Entry
{
private:
    std::string service; 
    std::string login; 
    std::string password;
    std::string date;

    std::string current_time_to_string() {
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(now, "%d.%m.%Y %H:%M:%S");
        return ss.str();
    }

public:
    Entry(std::string service, std::string login, std::string password):
    service(service), login(login), password(password), date(current_time_to_string())
    {};

    Entry(std::string service, std::string login, std::string password, std::string date):
    service(service), login(login), password(password), date(date)
    {};
    
    ~Entry() {};
    
    std::string get_service() const {return service;}
    std::string get_login() const {return login;}
    std::string get_password() const {return password;}
    std::string get_date() const {return date;}

    void set_service(std::string new_service) {service = new_service;}
    void set_login(std::string new_login) {login = new_login;}
    void set_password(std::string new_password) {password = new_password; date = current_time_to_string();}    
};

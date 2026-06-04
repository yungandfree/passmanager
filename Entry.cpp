#include "Entry.h"
#include <ctime>
#include <sstream>
#include <iomanip>

std::string Entry::current_time_to_string() {
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(now, "%d.%m.%Y %H:%M:%S");
    return ss.str();
}

Entry::Entry(std::string service, std::string login, std::string password):
    service(service), login(login), password(password), date(current_time_to_string())
{}

Entry::Entry(std::string service, std::string login, std::string password, std::string date):
    service(service), login(login), password(password), date(date)
{}

Entry::~Entry() {}

std::string Entry::get_service() const { return service; }
std::string Entry::get_login() const { return login; }
std::string Entry::get_password() const { return password; }
std::string Entry::get_date() const { return date; }

void Entry::set_service(std::string new_service) { service = new_service; }
void Entry::set_login(std::string new_login) { login = new_login; }
void Entry::set_password(std::string new_password) { 
    password = new_password; 
    date = current_time_to_string();
}

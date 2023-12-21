#include <vector>
#include <string>
#include "fmt/ranges.h"
#include <iostream>
#include <sstream>
#include "Passwords.h"
#include "UserInterface.h"
#include <algorithm>

std::vector<std::string> categories;

std::vector<Password> Password::castToPasswords(const std::vector<std::string> &strings){
        auto passwords = std::vector<Password>();

        for (const auto &s : strings){
            std::string element;
            auto ss = std::stringstream(s);
            auto password = Password();

            std::getline(ss, element, ',');
            password.category = element;

            std::getline(ss, element, ',');
            password.name = element;

            std::getline(ss, element, ',');
            password.password = element;

            std::getline(ss, element, ',');
            password.website = element == "NO_WEBSITE" ? "" : element;

            std::getline(ss, element, ',');
            password.login = element == "NO_LOGIN" ? "" : element;

            passwords.push_back(password);
        }
        return passwords;
    }

void Password::editName(Password &toEdit){

    auto newName = std::string();
    fmt::println("Wprowadź nową nazwę:");
    std::getline(std::cin, newName);

    toEdit.name = newName;
    fmt::println("Nazwa hasła została zmieniona.");
}

void Password::editPasswordText(Password &toEdit){

    auto newPassText = std::string();
    fmt::println("Wprowadź nowy tekst hasła (musi być jednym słowem, inne słowa niż pierwszę będą ignorowane):");
    std::cin >> newPassText;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    toEdit.password = newPassText;
    fmt::println("Tekst hasła został zmieniony.");

}

void Password::editCategory(Password &toEdit){

    fmt::println("Wybierz nową kategorię");
    auto action = int();
    auto categoriesSize = categories.size();
    do {
        for (int i = 0; i < categories.size(); i++)
            fmt::println("[{}]: {}", i + 1, categories.at(i));
        fmt::println("[{}]: Dodać nową kategorię", categoriesSize + 1);

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action < 1 || action > categoriesSize + 1)
            fmt::println("Wybrana niepoprawna kategoria.");
        else if (action == categoriesSize + 1)
            UI::addCategory();
        else{
            toEdit.category = categories.at(action - 1);
            fmt::println("Kategoria została zmieniona");
        }
    }
    while (action < 1 || action > categoriesSize + 1);
}

void Password::editLogin(Password &toEdit){

    auto newLogin = std::string();
    fmt::println("Wprowadź nowy login:");
    std::getline(std::cin, newLogin);

    toEdit.login = newLogin;
    fmt::println("Login został zmieniony.");
}

void Password::editWebsite(Password &toEdit){

    auto newWebsite = std::string();
    fmt::println("Wprowadź nową stronę:");
    std::cin >> newWebsite;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    toEdit.website = newWebsite;
    fmt::println("Strona została zmieniona.");
}

bool Password::editPassword(std::vector<Password> &passwords){
    auto action = int();
    auto counterChanges = 0;

    do {
        fmt::println("Wybierz hasło, które chcesz edytować:");
        for (int i = 0; i < passwords.size(); i++) {
            auto strForm = passwords.at(i).toString();
            fmt::println("[{}]: {}", i + 1, strForm);
        }
        fmt::println("[{}]: Wrócić do pliku", passwords.size() + 1);

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action < 1 || action > passwords.size() + 1)
            fmt::println("Wybrane złe hasło.");

        else if (action != passwords.size() + 1) {

            Password &toEdit = passwords.at(action - 1);
            auto parameterToEdit = std::string();
            fmt::println("Wpisz nazwę parametru, który chcesz zmienić.");
            fmt::println("(Możliwe parametry: \"nazwa\", \"haslo\", \"kategoria\", \"login\", \"strona\")");
            fmt::println("Lub wpisz \"back\" żeby wybrać inne hasło do edytowania.");

            std::cin >> parameterToEdit;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (parameterToEdit == "back") {
                return false;
            }
            else if (parameterToEdit == "nazwa") {
                editName(toEdit);
                counterChanges++;
            } else if (parameterToEdit == "haslo") {
                editPasswordText(toEdit);
                counterChanges++;
            }
            else if (parameterToEdit == "kategoria") {
                editCategory(toEdit);
                counterChanges++;
            } else if (parameterToEdit == "login"){
                editLogin(toEdit);
                counterChanges++;
            }
            else if (parameterToEdit == "strona"){
                editWebsite(toEdit);
                counterChanges++;
            }
            else
                fmt::println("Wybrany niepoprawny parametr.");

        }
    }
    while (action != passwords.size() + 1);

    if (counterChanges != 0) {
        fmt::println("Czy chcesz zapisać zmiany?");
        fmt::println("[1]: tak");
        fmt::println("[2]: nie");

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action == 1)
            return true;
        else return false;
    }
    else return false;


}

int Password::evaluatePasswordStrength(const std::string &password){
    int strength = 0;
    if (password.size() >= 7)
        strength++;
    else
        strength--;

    bool hasUpperCase = false;
    for (const char &c : password)
        if (std::isupper(c)){
            hasUpperCase = true;
            break;
        }
    if (hasUpperCase)
        strength++;

    bool hasLowerCase = false;
    for (const char &c : password)
        if (std::islower(c)){
            hasLowerCase = true;
            break;
        }

    if (hasLowerCase)
        strength++;

    bool hasSpecial = false;
    std::string specialChars = "!?@#$%^&*+-_[]{}()\\/";
    for (const char &c : password)
        if (specialChars.find(c) != std::string::npos){
            hasSpecial = true;
            break;
        }

    if (hasSpecial)
        strength++;

    return strength;
}


[[nodiscard]] std::string Password::toString() const{
    return this -> category + "," + this -> name + ","
           + this -> password + ","
           + ((this -> website.empty()) ? "NO_WEBSITE" : this -> website) + ","
           + (this -> login.empty() ? "NO_LOGIN" : this -> login);
}


bool Password::operator==(const Password &another) const{
    return
            this -> password == another.password
            && this -> name == another.name
            && this -> category == another.category
            && this -> login == another.login
            && this -> website == another.website;
}
void findCategories(){
    auto stream = std::fstream("categories.txt", std::ios::in);
    if (!stream.is_open()) {
        categories.emplace_back("WORK");
        categories.emplace_back("EDUCATION");
    }
    else {
        auto line = std::string();
        while (std::getline(stream, line))
            categories.emplace_back(line);
    }
    stream.close();
}

void saveCategories(){
    auto stream = std::fstream("categories.txt", std::ios::out);
    for (const auto &str : categories)
        stream << str << '\n';

    stream.close();
}
#include <fmt/ranges.h>
#include <iostream>
#include "Files.h"
#include "Operations.h"
#include <algorithm>
#include "UserInterface.h"



void Operations::addPassword(std::vector<Password> &passwords) {
    auto newPassword = Password();
    auto text = std::string();
    auto action = int();

    bool success = false;
    while (!success) {
        fmt::println("Wybierz kategorię:");
        for (int i = 0; i < categories.size(); i++)
            fmt::println("[{}]: {}", i + 1, categories.at(i));

        fmt::println("[{}]: Dodać nową kategorię", categories.size() + 1);
        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action <= 0 || action > categories.size() + 1)
            fmt::println("Wybrana nieistniająca kategoria.");
        else if (action == categories.size() + 1)
            UI::addCategory();
        else {
            newPassword.category = categories.at(action - 1);
            success = true;
        }
    }


    success = false;
    while (!success) {
        fmt::println("Wprowadź nazwę hasła:");
        std::getline(std::cin, text);

        if (text.find(',') != std::string::npos)
            fmt::println("Wprowadzona nawa zawiera przecienk \',\', "
                         "Wprowadzenie takiej nazwy spowoduje niepoprawne działanie programu.");
        else success = true;
    }
    newPassword.name = text;

    success = false;
    while (!success) {


        fmt::println("Wprowadź hasło (musi być jednym słowem, inne słowa niż pierwsze będą ignorowane)");
        fmt::println("Jeżeli chcesz, żeby ptogram wygenerował hasło dla Ciebie, wpisz \"random\"");
        std::cin >> text;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (text == "random") {
            auto length = int();
            auto hasLowerCase = bool(), hasUpperCase = bool(), hasSpecial = bool();

            UI::getParametersForRandomPassword(length, hasUpperCase, hasLowerCase, hasSpecial);
            text = File::generateRandomPassword(length, hasUpperCase, hasLowerCase, hasSpecial);
        }
        if (text.find(',') != std::string::npos){
            fmt::println("Hasło zawiera przecinek \',\'."
                         " Wprowadzenie takiego hasła spowoduje niepoprawne działanie programu.");
            continue;
        }
        if (text == "categories.txt"){
            fmt::println("Plik o takiej nazwie nie może zostać stworzony, "
                         "to spowoduje błędnę działanie programu.");
            continue;
        }

       bool alreadyExists = false;

        for (const Password &temp : passwords)
            if (temp.password == text) {
                alreadyExists = true;
                break;
            }
        if (alreadyExists){
            fmt::println("Wprowadzone hasło już istnieje w tym pliku, wybierz inne.");
            continue;
        }

        int passwordStrength = Password::evaluatePasswordStrength(text);
        switch (passwordStrength) {
            case 0:
                fmt::println("Twoje hasło jest bardzo niebezpieczne. Zaleca się wprowadzić inne hasło.");
                break;
            case 1:
                fmt::println("Twoje hasło jest mało bezpieczne. Zaleca się wprowadzić inne hasło");
                break;
            case 2:
                fmt::println("Twoje hasło jest bezpieczne.");
                break;
            default:
                fmt::println("Twoje hasło jest bardzo bezpieczne.");
        }
        fmt::println("Czy chcesz zatwierdzić hasło, czy zapisać inne?");
        fmt::println("[1]: Zatwierdzić");
        fmt::println("[2]: Wprowadzić inne hasło");

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action == 1)
            success = true;
    }
    newPassword.password = text;


    success = false;
    while (!success) {
        fmt::println("Wprowadź stronę internetową"
                     " tego hasła (jeżeli nie chcesz dodawać strone, wpisz \"NO\")");

        std::getline(std::cin, text);

        if (text.find(',') != std::string::npos)
            fmt::println("Strona zawiera przecinek \',\'."
                         " Wprowadzenie takiej strony spowoduje niepoprawne działanie programu.");
        else success = true;
    }

    if (text != "NO")
        newPassword.website = text;

    else newPassword.website = "";


    success = false;
    while (!success){
        fmt::println("Wprowadź login do"
                     " tego hasła (jeżeli nie chcesz dodawać logina, wpisz \"NO\")");

        std::getline(std::cin, text);
        if (text.find(',') != std::string::npos)
            fmt::println("Login zawiera przecinek \',\'."
                         " Wprowadzenie takiego logina spowoduje niepoprawne działanie programu.");
        else success = true;
    }

    if (text != "NO")
        newPassword.login = text;
    else newPassword.login = "";

    passwords.push_back(newPassword);

    fmt::println("Hasło zostało dodane.");
}

void Operations::removePasswords(std::vector<Password> &passwords, const std::vector<Password> &toRemove){

    passwords.erase(
            std::remove_if(passwords.begin(), passwords.end(),
                           [&toRemove](const Password& p) {
                                       return std::find(toRemove.begin(), toRemove.end(),p)
                                                != toRemove.end();
                                   }), passwords.end());
}


bool Operations::comparatorByParameters(const Password &one, const Password &another,
                                  const std::vector<std::string> &parameters){
    for (const std::string &parameter : parameters){
        if (parameter == "nazwa") {
            if (one.name != another.name)
                return one.name < another.name;
        }
        else if (parameter == "kategoria"){
            if (one.category != another.category)
                return one.category < another.category;
        }
        else if (parameter == "hasło"){
            if (one.password != another.password)
                return one.password < another.password;
        }
        else if (parameter == "login"){
            if (one.login != another.login)
                return one.login < another.login;
        }
        else if (parameter == "strona"){
            if (one.website != another.website)
                return one.website < another.website;
        }
    }
    return false;
}



std::vector<const Password*> Operations::findMatches(const std::vector<Password> &passwords,
                                               const std::map<std::string, std::string> &parameters){
    auto passwordsThatMatch = std::vector<const Password*>();
    for (const auto &pass : passwords){
        bool matches = true;
        for (const auto &[option, text] : parameters)
            if (option == "nazwa"){
                if (text != pass.name){
                    matches = false;
                    break;
                }
            }
            else if (option == "kategoria"){
                if (text != pass.category){
                    matches = false;
                    break;
                }
            }
            else if (option == "hasło"){
                if (text != pass.password){
                    matches = false;
                    break;
                }
            }
            else if (option == "login"){
                if (text != pass.login){
                    matches = false;
                    break;
                }
            }
            else if (option == "strona"){
                if (text != pass.website){
                    matches = false;
                    break;
                }
            }
        if (matches)
            passwordsThatMatch.push_back(&pass);
    }
    return passwordsThatMatch;
}

int Operations::generateKeyForEncoding(const std::string &password){
    int key = 0;
    for (int i = 0; i < password.size(); i++)
        key = (i % 2 == 0) ? key + password[i] / 2 : key - password[i] / 2;

    if (password.size() % 2 != 0)
        key += password[0] / 2;

    return key == 0 ? 5 : key;
}

void Operations::encodeOrDecode(std::string &data, int key, bool encode){
    for (auto &c : data) {
        if (c == ' ') continue;
        c = encode ?
                (c + key == '\n' ? c + key + 1 : c + key)
            :   (c - key == '\n' ? c - key - 1 : c - key);
    }
}
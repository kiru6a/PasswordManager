#include <fmt/ranges.h>
#include <iostream>
#include <algorithm>
#include "UserInterface.h"
#include "Passwords.h"
#include "Files.h"
#include "Operations.h"

void UI::addCategory(){
    fmt::println("Wprowadź nową kategorię:");
    auto success = false;
    auto newCategory = std::string();
    while (!success) {
        std::getline(std::cin, newCategory);
        success = true;
        if (newCategory.find(',') != std::string::npos){
            fmt::println("Kategoria zawiera przecinek,"
                         " wykorzystywania takiej kategorii spowoduje błędne działanie programu.");
            success = false;
        }
    }
    categories.emplace_back(newCategory);
    fmt::println("Kategoria została dodana");
}

std::string UI::getCategoryToBeRemoved(){
    auto toRemove = std::string();
    auto action = int();
    do{
        fmt::println("Wybierz kategorię, którą chcesz usunąć:");
        for (auto i = 0; i < categories.size(); i++)
            fmt::println("[{}]: {}", i + 1, categories.at(i));
        fmt::println("[{}]: Anulować operację", categories.size() + 1);

        std::cin >> action;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    while (action < 1 || action > categories.size() + 1);

    if (action == categories.size() + 1)
        toRemove.clear();
    else
        toRemove = categories.at(action - 1);

    return toRemove;
}

std::vector<Password> UI::getPasswordsToBeRemoved(const std::vector<Password> &passwords){
    auto action = -1;
    auto toRemove = std::vector<Password>();
    auto passwordsSize = passwords.size();


    while (action != passwordsSize + 1 && action != passwordsSize + 2) {

        fmt::println("========================");
        fmt::println("Wybrane hasła do usunięcia:");
        for (const auto &pass : toRemove)
            fmt::println("{}", pass.toString());
        fmt::println("========================");

        fmt::println("Wybierz hasło(-a), które chcesz usunąć:");
        for (int i = 0; i < passwordsSize; i++) {
            const auto &str = passwords.at(i).toString();

            fmt::println("[{}]: {}", i + 1, str);
        }
        fmt::println("[{}]: Anulować i wrócić", passwordsSize + 1);
        fmt::println("[{}]: Zatwierdzić i usunąć", passwordsSize + 2);

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action < 1 || action > passwordsSize + 2)
            fmt::println("Wybrane niepoprawne hasło.");
        else if (action != passwordsSize + 1 && action != passwordsSize + 2) {
            const auto &candidate = passwords.at(action - 1);

            if (std::ranges::find(toRemove, candidate) == toRemove.end())
                toRemove.push_back(candidate);
        }
    }
    if (action == passwordsSize + 1)
        toRemove.clear();
    return toRemove;
}


void UI::displayActionsForFile(const std::string &filePath){
    auto action = int();

    while (action != 12) {

        fmt::println("Plik -> {}", filePath);
        auto stringRepresentation = File::readFromFile(filePath);
        auto passwordRepresentation = Password::castToPasswords(stringRepresentation);

        fmt::println("Wybierz opcje:");
        fmt::println("[1]: Pokazać zawartości");
        fmt::println("[2]: Zaszyfrować plik");
        fmt::println("[3]: Odszyfrować plik");
        fmt::println("[4]: Odszukać hasła");
        fmt::println("[5]: Posortować hasła");
        fmt::println("[6]: Dodać hasło");
        fmt::println("[7]: Usunąć hasło");
        fmt::println("[8]: Edytować hasło");
        fmt::println("[9]: Pokazać kategorię haseł");
        fmt::println("[10]: Dodać kategorię");
        fmt::println("[11]: Usunąć kategorię");
        fmt::println("[12]: Wrócić na początkowy ekran");

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action < 1 || action > 12){
            fmt::println("Wybrana niepoprawna opcja.");
            continue;
        }
        else if (action == 12)
            return;
        else if (action == 1) {
            fmt::println("==================================");
            for (const std::string &s: stringRepresentation)
                fmt::println("{}", s);
            fmt::println("==================================");
        }
        else if (action == 2) {
            auto password = std::string();

            fmt::println("Wprowadź hasło dla zaszyfrowania:");
            std::cin >> password;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            int key = Operations::generateKeyForEncoding(password);

            for (auto &str: stringRepresentation)
                Operations::encodeOrDecode(str, key, true);

            fmt::println("Plik został zaszyfrowany.");
            File::saveToFile(filePath, stringRepresentation);
        }
        else if (action == 3) {
            auto password = std::string();

            fmt::println("Wprowadź hasło dla odszyfrowania:");
            std::cin >> password;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            int key = Operations::generateKeyForEncoding(password);

            for (auto &str: stringRepresentation)
                Operations::encodeOrDecode(str, key, false);

            fmt::println("Plik został odszyfrowany.");
            File::saveToFile(filePath, stringRepresentation);
        }
        else if (action == 4) {
            const auto availableParameters =
                    std::vector<std::string>{"nazwa", "hasło", "kategoria", "login", "strona"};
            fmt::println("==================================");
            auto chosenParameters = UI::getParametersForSearching(availableParameters);

            if (!chosenParameters.empty()) {
                auto passwordsThatMatch =
                        Operations::findMatches(passwordRepresentation, chosenParameters);
                if (passwordsThatMatch.empty())
                    fmt::println("Brak wyników wyszukiwania.");
                else {
                    fmt::println("==================================");
                    fmt::println("Wyniki wyszukiwania:");
                    for (const auto pass: passwordsThatMatch)
                        fmt::println("{}", pass->toString());
                    fmt::println("==================================");
                }
            }
        }
        else if (action == 5) {
            const auto availableParameters =
                    std::vector<std::string>{"nazwa", "hasło", "kategoria", "login", "strona"};
            auto chosenParameters = std::vector<std::string>();

            fmt::println("==================================");
            bool proceed = UI::getParametersForSorting(chosenParameters, availableParameters);

            if (proceed) {
                std::ranges::sort(passwordRepresentation,
                                  [chosenParameters](const Password &one, const Password &another) -> bool {
                                      return Operations::comparatorByParameters(one,
                                                                                another, chosenParameters);
                                  });
                UI::updateStringRepresentation(stringRepresentation, passwordRepresentation);
                File::saveToFile(filePath, stringRepresentation);
                fmt::println("Hasła zostały posortowane");
                fmt::println("==================================");


            }
        }
        else if (action == 6) {
            fmt::println("==================================");
            Operations::addPassword(passwordRepresentation);
            stringRepresentation.push_back(passwordRepresentation.back().toString());
            File::saveToFile(filePath, stringRepresentation);
        }
        else if (action == 7) {
            const auto &toRemove =
                    UI::getPasswordsToBeRemoved(passwordRepresentation);

            if (!toRemove.empty()) {
                bool acceptedOrDeclined = false;
                while (!acceptedOrDeclined) {

                    fmt::println("============UWAGA============");
                    fmt::println("Zamierzasz usunąć następujące hasła:");

                    for (const auto &pass: toRemove)
                        fmt::println("{}", pass.toString());

                    fmt::println("Czy chcesz kontynuować?");
                    fmt::println("[1]: Anulować operację");
                    fmt::println("[2]: Zatwierdzić i usunąć");

                    std::cin >> action;

                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (action == 1 || action == 2) acceptedOrDeclined = true;
                }
                if (action == 1)
                    fmt::println("Operacja została anulowana");
                else {
                    Operations::removePasswords(passwordRepresentation, toRemove);
                    UI::updateStringRepresentation(stringRepresentation, passwordRepresentation);
                    File::saveToFile(filePath, stringRepresentation);
                }
            }
        }
        else if (action == 8) {
            fmt::println("==================================");
            bool saveChanges = Password::editPassword(passwordRepresentation);

            if (saveChanges) {
                fmt::println("Zmiany zostały zapisane.");
                UI::updateStringRepresentation(stringRepresentation, passwordRepresentation);
                File::saveToFile(filePath, stringRepresentation);
            }
            else
                fmt::println("Zmiany zostały usunięte.");
        }
        else if (action == 9){
            fmt::println("==================================");
            fmt::println("Kategorie haseł:");
            for (const auto &cat : categories)
                fmt::println("{}", cat);
            fmt::println("==================================");
        }
        else if (action == 10) {
            fmt::println("==================================");
            UI::addCategory();
        }
        else if (action == 11){
            fmt::println("==================================");
            if (categories.size() == 1){
                fmt::println("Ostatnia kategoria nie może być usunięta.");
                continue;
            }
            auto toRemove = UI::getCategoryToBeRemoved();

            if (toRemove.empty()) {
                fmt::println("Operacja anulowana");
                continue;
            }

            auto matchesPointers = Operations::findMatches(passwordRepresentation,
                                                           std::map<std::string, std::string>{{"kategoria", toRemove}});



            if (!matchesPointers.empty()) {
                fmt::println("============UWAGA============");

                fmt::println("Usuwając tę kategorię, usuniesz również następujące hasła:");
                for (const auto pass: matchesPointers)
                    fmt::println("{}", pass->toString());

                fmt::println("Czy chcesz kontynuować?");
                fmt::println("[1]: kontynuować");
                fmt::println("[2]: anulować");

                std::cin >> action;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else action = 1;

            if (action == 1){
                categories.erase(
                        std::remove(categories.begin(), categories.end(), toRemove),
                        categories.end());
                auto matches = std::vector<Password>();
                for (const auto &pass : matchesPointers)
                    matches.emplace_back(*pass);
                Operations::removePasswords(passwordRepresentation, matches);
                UI::updateStringRepresentation(stringRepresentation, passwordRepresentation);
                File::saveToFile(filePath, stringRepresentation);
                fmt::println("Kategoria zostałą usunięta");
            }
            else
                fmt::println("Operacja anulowana.");
        }
    }
}


void UI::updateStringRepresentation(std::vector<std::string> &str, const std::vector<Password> &pass){
    str.clear();
    for (const auto &p : pass)
        str.emplace_back(p.toString());
}


void UI::getParametersForRandomPassword(int &length, bool &hasUpperCase,
                                          bool &hasLowerCase, bool &hasSpecial){
    bool success = false;
    auto action = std::string();


    while (!success) {
        fmt::println("Wybierz długość hasła (min.5)");
        std::cin >> length;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (length < 5)
            fmt::println("Hasło o takiej długości nie może być wygenerowane.");

        else success = true;
    }

    success = false;

    while (!success) {
        fmt::println("Czy chesz, żeby hasło zawierało duże litery?(\"tak\"/\"nie\")");
        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        success = true;
        if (action == "nie")
            hasUpperCase = false;
        else if (action == "tak")
            hasUpperCase = true;
        else {
            fmt::println("Wprowadzona niepoprawna komenda.");
            success = false;
        }
    }
    success = false;
    while (!success) {
        fmt::println("Czy chesz, żeby hasło zawierało małe litery?(\"tak\"/\"nie\")");
        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        success = true;
        if (action == "nie")
            hasLowerCase = false;
        else if (action == "tak")
            hasLowerCase = true;
        else {
            fmt::println("Wprowadzona niepoprawna komenda.");
            success = false;
        }
    }
    success = false;

    while (!success) {
        fmt::println("Czy chesz, żeby hasło zawierało znaki specjalne?(\"tak\"/\"nie\")");
        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        success = true;
        if (action == "nie")
            hasSpecial = false;
        else if (action == "tak")
            hasSpecial = true;
        else {
            fmt::println("Wprowadzona niepoprawna komenda.");
            success = false;
        }
    }

    if (!hasLowerCase && !hasUpperCase && !hasSpecial){
        fmt::println("Hasło o takich parametrych nie może być wygenerowane, hasło musi co najmniej zawierać\n"
                     "małe litery. Zostanie wygenerowane hasło zawierające tylko małe litery");
        hasLowerCase = true;
    }

}


bool UI::getParametersForSorting(std::vector<std::string> &chosenParameters,
                                         const std::vector<std::string> &availableParameters){

    fmt::println("Wybierz parametry według których chcesz posortować hasła z pliku");

    auto action = 0;
    while (action != availableParameters.size() + 1 && action != availableParameters.size() + 2){

        fmt::println("Wybrane parametry: {}", chosenParameters);

        for (int i = 0; i < availableParameters.size(); i++)
            fmt::println("[{}]: {}", i + 1, availableParameters.at(i));
        fmt::println("[{}]: Anulować i wrócić do pliku", availableParameters.size() + 1);
        fmt::println("[{}]: Posortować według wybranych parametrów", availableParameters.size() + 2);

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action > 0 && action < availableParameters.size() + 1){
            const std::string &chosen = availableParameters.at(action - 1);
            bool notAddedYet =
                    std::ranges::find(chosenParameters, chosen) == chosenParameters.end();

            if (notAddedYet)
                chosenParameters.push_back(chosen);
            else
                chosenParameters.erase(std::remove(chosenParameters.begin(),
                                                   chosenParameters.end(),
                                                   chosen),
                                       chosenParameters.end());
        }
        else if (action != availableParameters.size() + 1 && action != availableParameters.size() + 2)
            fmt::println("Wybrany błędny parametr.");
    }
    if (action == availableParameters.size() + 1)
        return false;
    else if (action == availableParameters.size() + 2)
        return true;
    else {
        fmt::println("Operacja anulowana");
        return false;
    }

}

std::map<std::string, std::string> UI::getParametersForSearching(
        const std::vector<std::string> &availableParameters){

    auto chosenParameters = std::map<std::string, std::string>();
    auto action = 0;
    fmt::println("Wybierz parametry, według których chcesz dokonać wyszukiwania");

    while (action != availableParameters.size() + 1 && action != availableParameters.size() + 2){
        fmt::println("Wybrane parametry: {}", chosenParameters);

        for (int i = 0; i < availableParameters.size(); i++)
            fmt::println("[{}]: {}", i + 1, availableParameters.at(i));

        fmt::println("[{}]: Wrócić do pliku", availableParameters.size() + 1);
        fmt::println("[{}]: Wyszukać hasła według wybranych parametrów",
                     availableParameters.size() + 2);

        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (action > 0 && action < availableParameters.size() + 1){
            const std::string &chosenOption = availableParameters.at(action - 1);

            bool notAddedYet = !chosenParameters.contains(chosenOption);

            if (notAddedYet){
                auto chosenText = std::string();
                if (chosenOption != "kategoria") {
                    fmt::println("Wprowadź tekst, który będzie wykorzystywany podczas wyszukiwania");
                    std::getline(std::cin, chosenText);
                }
                else{
                    auto chosenProperly = false;
                    while (!chosenProperly) {
                        fmt::println("Wybierz kategorię:");
                        for (auto i = 0; i < categories.size(); i++)
                            fmt::println("[{}]: {}", i + 1, categories.at(i));

                        std::cin >> action;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


                        if (action < 1 || action > categories.size()) {
                            fmt::println("Wybrana niepoprawna kategoria");
                            continue;
                        }
                        else{
                            chosenText = categories.at(action - 1);
                            chosenProperly = true;
                        }
                    }
                }
                chosenParameters.insert({chosenOption, chosenText});
            }

            else
                chosenParameters.erase(chosenOption);
        }
    }

    if (action == availableParameters.size() + 1)
        chosenParameters.clear();

    else if (action != availableParameters.size() + 2){
        fmt::println("Operacja anulowana.");
        chosenParameters.clear();
    }
    return chosenParameters;
}
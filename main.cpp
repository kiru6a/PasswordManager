#include <fmt/ranges.h>
#include <iostream>
#include "Passwords.h"
#include "Files.h"
#include "UserInterface.h"


bool launchingScreen() {
    fmt::println("Wybierz opcje");
    fmt::println("[1]: Wybrać istniejący plik");
    fmt::println("[2]: Utworzyć plik w katalogu programu");
    fmt::println("[3]: Zamknąć program");
    auto action = int();

    std::cin >> action;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (action < 1 || action > 3) {
        fmt::println("Wybrana niepoprawna komenda.");
        return true;
    }
    switch(action){
        case 3:
            return false;
        case 2:
            fmt::println("========================");
            File::createFile();
            return true;
        default:
            fmt::println("========================");

            auto filePath = std::string();
            bool fileChosen = File::chooseFile(filePath);
            if (fileChosen) {
                fmt::println("========================");
                UI::displayActionsForFile(filePath);
                return true;
            }
            else
                return true;
    }
}


int main() {
    fmt::println("============UWAGA============");
    fmt::println("Program nie oferuje możliwości działania na hasłach, które zawierają\n"
                 "przecinki \',\'. Podczas próby wprowadzenia takiego hasło, zostaniesz\n"
                 "powiadomiony i poproszony o wybranie innego hasła.");
    fmt::println("===========================");
    fmt::println("W sytuacjach, gdy program oczekuje od Ciebie jednego słowa,\n"
                 "dokładnie jedno słowo zostanie wczytane. Jeżeli w takiej sytuacji wprowadzisz\n"
                 "więcej niż jedno słowo, inne słowa niż pierwsze będą ignorowane.");
    fmt::println("==================================");
    findCategories();

    bool keepRunning = launchingScreen();

    while (keepRunning)
        keepRunning = launchingScreen();

    saveCategories();
    fmt::println("Program został zamknięty.");
    return 0;
}
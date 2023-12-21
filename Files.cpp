#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <fmt/ranges.h>
#include <algorithm>
#include <random>
#include "Files.h"
#include "Operations.h"
#include "UserInterface.h"


void File::saveToFile(const std::string &path, std::vector <std::string> strings) {
        auto out = std::ofstream(path);
        //if (!out)
        while (strings.size() < 30)
            strings.emplace_back("");

        // Get the current system time
        auto currentTime = std::chrono::system_clock::now();

        // Convert the current time to a time_t object
        std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);

        // Convert the time_t object to a tm struct
        std::tm *currentTimeStruct = std::localtime(&currentTimeT);

        // Extract hour, minute, and second from the tm struct
        int day = currentTimeStruct->tm_mday;
        int month = currentTimeStruct->tm_mon + 1;
        int year = currentTimeStruct->tm_year + 1900;
        int hour = currentTimeStruct->tm_hour;
        int minute = currentTimeStruct->tm_min;
        int second = currentTimeStruct->tm_sec;

        int counter = 1;
        for (const std::string &string: strings) {
            if (counter == 5)
                out << (day < 10 ? "0" : "") + std::to_string(day);
            else if (counter == 10)
                out << (month < 10 ? "0" : "") + std::to_string(month);
            else if (counter == 15)
                out << year;
            else if (counter == 20)
                out << (hour < 10 ? "0" : "") + std::to_string(hour);
            else if (counter == 25)
                out << (minute < 10 ? "0" : "") + std::to_string(minute);
            else if (counter == 30)
                out << (second < 10 ? "0" : "") + std::to_string(second);
            out << string << "\n";

            counter++;
        }

        out.close();
    }

std::vector<std::string> File::readFromFile(const std::string &path) {
    auto in = std::ifstream(path);
    auto result = std::vector<std::string>();

    if (!in)
        return result;


    auto line = std::string();
    auto counter = 1;

    while (std::getline(in, line)) {
        if (line.empty() || (counter % 5 == 0) && line.size() <= 4)
            break;
        else if (counter % 5 == 0 && counter != 15 && counter < 31  && line.size() >= 2)
            result.push_back(line.substr(2));
        else if (counter == 15)
            result.push_back(line.substr(4));
        else
            result.push_back(line);
        counter++;
    }
    in.close();

    return result;
}

std::string File::generateRandomPassword(int length, const bool &hasUpperCase,
                                          const bool &hasLowerCase, const bool &hasSpecial) {

    const std::string upperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lowerCase = "abcdefghijklmnopqrstuvwxyz";
    const std::string specialChars = "!?@#$%^&*+-_[]{}()\\/";

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    auto password = std::string();
    while (password.size() < length){
        if (hasUpperCase)
            password += upperCase[rand() % upperCase.length()];
        if (hasLowerCase)
            password += lowerCase[rand() % lowerCase.length()];
        if (hasSpecial)
            password += specialChars[rand() % specialChars.length()];
    }
    fmt::println("=====================");
    fmt::println("Wygenerowane hasło: {}", password);
    fmt::println("=====================");
    return password;
}


void File::createFile() {

    auto name = std::string();
    fmt::println("Wprowadź nazwę pliku (nazwa musi być jednym słowem, inne niż pierwsze będą ignorowane):");
    fmt::println("============UWAGA============");
    fmt::println("Dla możliwości dalszego działania na utworzonym pliku,\n"
                 "musisz nadać mu rozszerzenie \"txt\".");
    std::cin >> name;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto passwords = std::vector<Password>();
    bool proceed = addPasswordsWhileCreatingFile(passwords);


    if (proceed) {
        fmt::println("Wybierz hasło główne do pliku (będzie ono wykorzystane do szyfrowania pliku).");
        fmt::println("Hasło musi być jednym słowem, słowa inne niż pierwsze będą ignorowane.");
        auto mainPassword = std::string();
        std::cin >> mainPassword;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto encoded = std::vector<std::string>();
        auto key = Operations::generateKeyForEncoding(mainPassword);
        for (const Password &password: passwords) {
            auto str = password.toString();
            Operations::encodeOrDecode(str, key, true);
            encoded.push_back(str);
        }

        File::saveToFile(name, encoded);
        fmt::println("Plik został utworzony.");
    }
}

bool File::addPasswordsWhileCreatingFile(std::vector<Password> &passwords) {
    auto name = std::string();
    auto password = Password();
    auto text = std::string();
    auto action = int();

    fmt::println("[1]: Dodać hasło");
    fmt::println("[2]: Kontynuować tworzenie pliku");
    fmt::println("[3]: Anulować i wrócić na początkowy ekran");
    std::cin >> action;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (action != 3 && action != 2) {
        if (action == 1) {
            Operations::addPassword(passwords);
        }
        else
            fmt::println("Wprowadzona niepoprawna komenda.");
        fmt::println("[1]: Dodać hasło");
        fmt::println("[2]: Kontynuować tworzenie pliku");
        fmt::println("[3]: Anulować i wrócić na początkowy ekran");
        std::cin >> action;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (action == 2)
        return true;
    else return false;
}

namespace fs = std::filesystem;

std::string File::chooseFromWorkingDirectory() {
    fs::path workingDir = fs::current_path();
    auto files = std::vector<fs::path>();

    for (const auto& entry : fs::recursive_directory_iterator(workingDir))
        if (entry.is_regular_file() && entry.path().extension().string() == ".txt")
            files.push_back(entry.path());

    if (files.empty()){
        fmt::println("W katalogu programu nie ma żadnego pliku.");
        return "NOT_CHOSEN";
    }


    auto action = int();
    auto path = std::string();

    for (auto i = 0; i < files.size(); i++)
        std::cout << "[" << (i + 1) << "]:" << files.at(i).filename() << std::endl;

    fmt::println("Wybierz plik podając odwopiedni numer:");
    std::cin >> action;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (action >= 1 && action <= files.size()){
        auto chosenFilePath = files.at(action - 1);
        path = chosenFilePath.string();
    }
    else{
        fmt::println("Wprowadzony niepoprawny numer.");
        path = "NOT_CHOSEN";
    }
    return path;
}


bool File::chooseFile(std::string &filePath) {
    auto action = int();

    fmt::println("[1]: Wybrać plik z katalogu programu");
    fmt::println("[2]: Podać absolutną ścieżkę do pliku");
    fmt::println("[3]: Wrócić na początkowy ekran");

    std::cin >> action;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    if (action == 1) {
        filePath = File::chooseFromWorkingDirectory();
        if (filePath == "NOT_CHOSEN") {
            filePath.clear();
            return false;
        }
        else return true;
    }
    else if (action == 2){
        fmt::println("Podaj absolutną ścieżkę do pliku.");
        std::cin >> filePath;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::filesystem::path path = filePath;
        if (!(fs::exists(path) && fs::is_regular_file(path))){
            fmt::println("Podany plik nie istneje lub ścieżka jest niepoprawna.");
            filePath.clear();
            return false;
        }
        else return true;
    }
    else return false;
}
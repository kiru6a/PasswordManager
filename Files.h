#pragma once
#include <string>
#include <vector>
#include "Passwords.h"

/**
 * This class provides methods for reading from and writing to files,
 * as well as functions for operations on passwords inside files.
 */
struct File{

    /**
     * @brief Saves a vector of strings to a file.
     * This function saves the vector of strings to the specified file.
     * @param path The path to the file where the data will be saved.
     * @param strings The data to be saved.
     * @note If the file already exists, its contents will be overwritten.
     */
    static void saveToFile(const std::string &path, std::vector<std::string> strings);


    /**
     * @brief Reads strings from a file and returns them as a vector.
     * This function reads strings from the specified file path and returns them as a vector.
     * @param path The path to the file to be read.
     * @return A vector of strings read from the file.
     * @note If the file doesn't exist, an empty vector will be returned
     */
    static std::vector<std::string> readFromFile(const std::string &path);


    /**
     * This function generates a random password based on the provided parameters.
     * @param length The length of the password to be generated.
     * @param hasUpperCase Whether the generated password has to contain upper case letters.
     * @param hasLowerCase Whether the generated password has to contain lower case letters.
     * @param hasSpecial Whether the generated password has to contain special characters.
     * @return The generated password.
     */
    static std::string generateRandomPassword(int length, const bool &hasUpperCase,
                                              const bool &hasLowerCase, const bool &hasSpecial);


    /**
     * Prompts the user to choose a file from the working directory.
     * @return The path to the chosen file it was chosen properly, "NOT_CHOSEN" otherwise.
     */
    static std::string chooseFromWorkingDirectory();


    /**
     * Prompts the user to choose a file.
     * @param filePath A reference to a string, where the path to the chosen file will be stored.
     * @return true if the file was chosen properly, false otherwise.
     */
    static bool chooseFile(std::string &filePath);


    /**
     * @brief Creates a file
     * This function gets the information needed for creation a file from the user and creates it.
     */
    static void createFile();


    /**
     * This function gives the user an opportunity to fill the file which
     * is being created with passwords.
     * It also lets the user to return to the launching screen without creating the file.
     * @param passwords A reference to a vector of passwords, where the newly created passwords will be added.
     * @return true if the user chose to create the file, false otherwise.
     * @note This function is meant to be called bo the createFile() function only.
     */
    static bool addPasswordsWhileCreatingFile(std::vector<Password> &passwords);
};
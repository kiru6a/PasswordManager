#pragma once
#include <map>
#include <vector>
#include <fstream>

extern std::vector<std::string> categories;

/**
 * Fills the vector "categories" with strings from a file "categories.txt"
 * @note If the file doesn't exist yet, meaning this function
 * is called during the first launching of the program, "categories" will be filled with default values.
 */
void findCategories();

/**
 * Saves categories from the vector "categories" to a file "categories.txt".
 */
void saveCategories();

/**
 * This structure represents a password with categories such as name,
 * password, category, website and login. It also provides static functions for editing passwords and
 * casting strings to passwords.
 */
struct Password{

    std::string name;
    std::string password;
    std::string category;
    std::string website;
    std::string login;

    /**
     * Converts a vector of strings to a vector of Password objects.
     *
     * @param strings A vector of strings to be converted.
     * @return The vector of password objects.
     */
    static std::vector<Password> castToPasswords(const std::vector<std::string> &strings);


    /**
     * Edits the name field of the passed Password object.
     * @param toEdit A reference to the Password object to be edited.
     */
    static void editName(Password &toEdit);

    /**
     * Edits the password field of the passed Password object.
     * @param toEdit A reference to the Password object to be edited.
     */
    static void editPasswordText(Password &toEdit);


    /**
     * Edits the category field of the passed Password object.
     * @param toEdit A reference to the Password object to be edited.
     */
    static void editCategory(Password &toEdit);

    /**
     * Edits the login field of the passed Password object.
     * @param toEdit A reference to the Password object to be edited.
     */
    static void editLogin(Password &toEdit);

    /**
     * Edits the website field of the passed Password object.
     * @param toEdit A reference to the Password object to be edited.
     */
    static void editWebsite(Password &toEdit);

    /**
     * @brief Edits passwords in a way chosen by the user.
     * This function asks the user which password should be modified,
     * and then calls the corresponding function for further handling.
     * The function also asks the user if the changes should be saved.
     * @param passwords The vector of Password objects the user should choose from.
     * @return true if the changes to passwords should be saved, false otherwise.
     * @note The function will ask the user if changes should be saved
     * only if at least one change has been made, otherwise it will always return false.
     */
    static bool editPassword(std::vector<Password> &passwords);

    /**
     * This function evaluates the strength of the provided password.
     * @param password The password to be evaluated.
     * @return The password strength level:
     *                      0 - Very dangerous
     *                      1 - Dangerous
     *                      2 - Safe
     *                      3 - Very save
     */
    static int evaluatePasswordStrength(const std::string &password);

    /**
     * @return A string representation of this password
     */
    [[nodiscard]] std::string toString() const;

    /**
     * Checks if two passwords are equal.
     * @param another The password to be compared with this password.
     * @return true if each object field of the two passwords matches, false otherwise.
     */
    bool operator==(const Password &another) const;
};
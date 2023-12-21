#include <string>
#include <vector>
#include "Passwords.h"

/**
 * This structure provides static functions that are used for getting data from the user.
 */
struct UI{
    static void addCategory();


    /**
     * Prompts the user to choose a password category to be removed.
     * @return The category to be removed.
     */
    static std::string getCategoryToBeRemoved();

    /**
     * Prompts the user to choose passwords to be removed.
     * @param passwords The available passwords to choose from.
     * @return A vector of passwords that should be removed.
     */
    static std::vector<Password> getPasswordsToBeRemoved(const std::vector<Password> &passwords);

    /**
     * @brief Displays available actions for a file.
     * This function displays available actions for the file and
     * calls the corresponding functions to execute them.
     * @param filePath The path to the file, which the actions are displayed for.
     * @note The available actions are: printing the file, encoding, decoding,
     * searching for passwords, sorting passwords, adding and removing and editing passwords,
     * printing available password categories, adding and removing password categories.
     */
    static void displayActionsForFile(const std::string &filePath);

    static void updateStringRepresentation(std::vector<std::string> &str, const std::vector<Password> &pass);


    /**
     * @brief Retrieves parameters for generating a random password.
     * This function retrieves the parameters required to generate a random password from the user.
     * The caller should provide references to the variables that will store the retrieved parameters.
     * @param length The desired length of the password.
     * @param hasUpperCase Indicates, whether the password should contain upper case letters.
     * @param hasLowerCase Indicates, whether the password should contain lower case letters.
     * @param hasSpecial Indicates, whether the password should contain special characters.
     * @note If the user sets hasUpperCase, hasLowerCase and hasSpecial to false,
     * the hasLowerCase variable will be automatically set to true.
     */
    static void getParametersForRandomPassword(int &length, bool &hasUpperCase,
                                              bool &hasLowerCase, bool &hasSpecial);

    /**
     * @brief Gets parameters for sorting from the available parameters list
     * This function asks the user to choose parameters for sorting passwords
     * from the provided list of available parameters
     * @param chosenParameters A reference to the vector that will store the chosen parameters for sorting.
     * @param availableParameters The vector of available parameters to choose from.
     * @return true if the user successfully chooses parameters
     * and chooses to sort the passwords, false otherwise.
     */
    static bool getParametersForSorting(std::vector<std::string> &chosenParameters,
                                             const std::vector<std::string> &availableParameters);

    /**
     * This function prompts the user to choose parameters
     * for searching a passwords from the provided list of available parameters and asks
     * them to choose the corresponding text.
     * @param availableParameters The vector of available parameters to choose from.
     * @return A map containing the selected parameters for searching,
     * where the keys represent the chosen parameters and the values represent text that should be searched for.
     */
    static std::map<std::string, std::string> getParametersForSearching(
            const std::vector<std::string> &availableParameters);
};

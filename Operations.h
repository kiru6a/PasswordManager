#include <vector>
#include "Passwords.h"

/**
 * This structure provides functions for operations of passwords inside files.
 */
struct Operations{

    /**
     * @brief Adds a password to a vector of passwords.
     * This function creates a password based on parameters provided by the user,
     * and adds it to the provided vector of passwords.
     * @param passwords The vector of passwords to which the new password will be added.
     */
    static void addPassword(std::vector<Password> &passwords);

    /**
    * @brief Removes passwords from a vector of passwords.
    * This function asks the user to choose passwords from the provided
    * vector of passwords and removes them
    * @param passwords The vector of passwords from which a password should be removed
    * @return true if the password was successfully removed, false otherwise.
    */
    static void removePasswords(std::vector<Password> &passwords, const std::vector<Password> &toRemove);

    /**
     * @brief Comparator function for sorting passwords based on specified parameters.
     * @param one The first Password object to compare
     * @param another The second Password object to compare
     * @param parameters The vector of parameters used for comparison
     * @return true if "one" is considered less than "another" based on the specified parameters, false otherwise.
     * @note The "parameters" vector should contain only parameters, which are Password member functions,
     * e.g. name, password, category, login or website
     */
    static bool comparatorByParameters(const Password &one, const Password &another,
                                       const std::vector<std::string> &parameters);


    /**
    * @brief Finds password that match specified parameters.
    * @param passwords The vector of Password objects to search within.
    * @param parameters The map of parameters and their corresponding values to match against.
    * @return A vector of const Password pointers that represent the matching passwords.
    */
    static std::vector<const Password*> findMatches(const std::vector<Password> &passwords,
                                                   const std::map<std::string, std::string> &parameters);


    static int generateKeyForEncoding(const std::string &password);


    /**
    * Encodes or decode the provided string.
    * @param data The string to be encoded or decoded.
    * @param key The key used for decoding or encoding
    * @param encode Indicates whether the data should encoded or decoded.
    * true if it should be decoded, false otherwise.
    */
    static void encodeOrDecode(std::string &data, int key, bool encode);
};
#include "utils.h"

std::string 
removeSpaces(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}
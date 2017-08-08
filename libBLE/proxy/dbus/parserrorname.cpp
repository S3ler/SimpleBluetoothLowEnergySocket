//
// Created by bele on 06.08.17.
//

#include "parserrorname.h"
std::string parseErrorName(const char *fullErrorName) {
    std::string fullErrorNameAsString(fullErrorName);
    return fullErrorNameAsString.substr(fullErrorNameAsString.rfind('.') + 1);
}

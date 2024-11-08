#include "dictionary.h"

Dictionary::Dictionary() {}

Dictionary::~Dictionary() {}

void
Dictionary::init(const std::string &filename) {
    loadFromFile(filename);
}


bool 
Dictionary::exists(const std::string &key) const {
    return m_data_map.find(key) != m_data_map.end();
}

void 
Dictionary::loadFromFile(const std::string &filename) {
    std::ifstream infile(filename);
    std::string line;
    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    while (std::getline(infile, line)) {
        std::cout << "add word: " << line << std::endl;
        m_data_map[line] = true;
    }
}
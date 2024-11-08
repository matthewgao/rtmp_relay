#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

class Dictionary {
public:
    Dictionary();
    ~Dictionary();

    bool exists(const std::string &key) const;
    void init(const std::string &filename);

private:
    std::unordered_map<std::string, bool> m_data_map;
    void loadFromFile(const std::string &filename);
};
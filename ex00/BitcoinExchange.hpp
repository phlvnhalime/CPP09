#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <map>

class BitcoinExchange
{
    private:
        std::map<std::string, float> _data;
    public:
        BitcoinExchange();
        ~BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& copy);
        BitcoinExchange& operator=(const BitcoinExchange& copy);
        void parseData(const std::string& filename);
        void parseInput(const std::string& filename);
        void processInput();
        void printData();
};

#endif
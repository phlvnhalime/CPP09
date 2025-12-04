#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cstdlib>

class BitcoinExchange
{
    private:
        std::map<std::string, float> _loadData;

        // Helper functions
        void _loadDataBase(const std::string& filename);
        int _isValidData(const std::string& date);
        void _findExactDate(const std::string& date, float value);
    public:
    BitcoinExchange();
    ~BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& copy);
    BitcoinExchange& operator=(const BitcoinExchange& copy);

    void processInput();
};

#endif
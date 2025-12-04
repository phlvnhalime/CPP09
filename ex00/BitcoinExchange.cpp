#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
    _loadDataBase("data.csv");
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy)
{
    *this = copy;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& copy)
{
    if (this != &copy)
    {
        _loadData = copy._loadData;
    }
    return *this;
}

void BitcoinExchange::_loadDataBase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    std::string line;
    std::getline(file, line);// pass the first line
    while(std::getline(file, line))
    {
        if(line.empty()) continue;
        
        size_t pos = line.find(',');
        if(pos == std::string::npos) continue;

        std::string date = line.substr(0, pos);
        std::string rateString = line.substr(pos + 1);
        _loadData[date] = std::strtod(rateString.c_str(), NULL); // convert the initial portion of the string pointed to by nptr to double, float, and long double representation
    }
    file.close();
}

int BitcoinExchange::_isValidData(const std::string& date)
{
    if(date.length() != 10)
        return 0;
    if(date[4] != '-' || date[7] != '-')
        return 0;
    for(int i = 0; i < 10; i++)
    {
        if(i != 4 && i != 7)
        {
            if(!isdigit(date[i]))
                return 0;
        }
    }
    int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());
    if(year < 2009 || year > 2025)
        return 0;
    if(month < 1 || month > 12)
        return 0;
    if(day < 1 || day > 31)
        return 0;
    return 1;
}
/*
    Find the first non-space character in the string
    Find the last non-space character in the string
    Return the substring between the first and last non-space characters!
    Be carefull - - A B C - - -> A B C 
                    |   |
                    |   |
                    |   ----> Last
                    |  !(Last - First = 5 - 3 = 2) But there are 3 characters in the substring not 2
                    ---> First

*/
std::string BitcoinExchange::_trim(const std::string& str){
    size_t start = str.find_first_not_of(" \t");
    if(start == std::string::npos)
        return "";
    size_t end = str.find_last_not_of(" \t");
    if(end == std::string::npos)
        return str.substr(start);
    return str.substr(start, end - start + 1);
}


void BitcoinExchange::_findExactDate(const std::string& date, float value)
{
    if(_loadData.empty())
    {
        std::cout << "Error: database is empty" << std::endl;
        return;
    }
    // Find the biggest date before the given date
    std::map<std::string, float>::iterator it = _loadData.upper_bound(date);
    // upper_bound is a binary search function that returns an iterator to the first element greater than the given value
    if(it == _loadData.begin())
    {
        std::cout << "Error: date too early" << std::endl;
        return;
    }
    it--; // (O(logn)) time complexity to find the exact date in the data base
    float res = value * it->second;
    // std::cout << std::fixed << std::setprecision(2); // to print the result with 2 decimal places
    std::cout << date << " => " << value << " = " << res << std::endl;
}

void BitcoinExchange::processInput(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    std::string line;
    std::getline(file, line); // pass the first line (header)
    
    while(std::getline(file, line))
    {
        if(line.empty())
            continue;
        
        size_t pos = line.find('|');
        if(pos == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        std::string date = _trim(line.substr(0, pos));
        std::string fileValue = _trim(line.substr(pos + 1));

        if(!_isValidData(date))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if(fileValue.empty())
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        char *end;
        float value = std::strtod(fileValue.c_str(), &end);
        if(end == fileValue.c_str() || *end != '\0')
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        if(value < 0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if(value > 1000)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        // To find the exact date in the data base
        _findExactDate(date, value);
    }
    file.close();
}
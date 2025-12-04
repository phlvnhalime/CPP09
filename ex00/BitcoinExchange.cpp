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
        std::stringstream ss(line);
        std::string date, rateString;
        if(std::getline(ss, date, ',') && std::getline(ss, rateString))
        {
            _loadData[date] = std::strtof(rateString.c_str(), NULL); // change the string to float
        }
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


void BitcoinExchange::_findExactDate(const std::string& date, float value)
{
    std::map<std::string, float>::iterator it = _loadData.upper_bound(date);
    if(it == _loadData.begin())
        std::cout << "Error: date too early" << std::endl;
    else{
        it--;
        float res = value * it->second;
        std::cout << date << " => " << value << " = " << res << std::endl;
    }
}

void BitcoinExchange::processInput()
{
    std::ifstream file("input.txt");
    if(!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }
    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty())
            continue;
        std::stringstream ss(line);
        std::string date, sep, fileValue;

        ss >> date >> sep >> fileValue;
        if(sep != "|" || !_isValidData(date))
        {
            std::cerr << "Error: Invalid input ! Please check the input file." << std::endl;
            continue;
        }

        float value = std::strtof(fileValue.c_str(), NULL);
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
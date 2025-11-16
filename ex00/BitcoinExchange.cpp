#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() : _data()
{
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
    if(this != &copy)
    {
        _data = copy._data;
    }
    return *this;
}

void BitcoinExchange::parseData(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::string line;
    while(getline(file, line))
    {
        std::stringstream ss(line);
        std::string date;
        std::getline(ss, date, ',');
        float value;
        ss >> value;
        _data[date] = value;
    }
    file.close();
}

void BitcoinExchange::parseInput(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::string line;
    while(getline(file, line))
    {
        std::stringstream ss(line);
        std::string date;
        std::getline(ss, date, '|');
        float value;
        ss >> value;
        if (value < 0 || value > 1000)
        {
            throw std::runtime_error("Value is out of range: " + line);
        }
        if(date.empty() || value == 0)
        {
            throw std::runtime_error("Invalid date or value: " + line);
        }
        std::map<std::string, float>::iterator it = _data.find(date);
        if(it == _data.end())
        {
            throw std::runtime_error("Date not found: " + date);
        }
        std::cout << date << " => " << value << " = " << value * it->second << std::endl;
    }
    file.close();
}

void BitcoinExchange::processInput()
{
    std::string filename = "input.txt";
    std::ifstream file(filename);
    if(!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::string line;
    while(getline(file, line))
    {
        std::stringstream ss(line);
        std::string date;
        std::getline(ss, date, '|');
        float value;
        ss >> value;
        if(value < 0 || value > 1000)
        {
            throw std::runtime_error("Value is out of range: " + line);
        }
        if(date.empty() || value == 0)
        {
            throw std::runtime_error("Invalid date or value: " + line);
        }
    }
}

void BitcoinExchange::printData()
{
    std::map<std::string, float>::iterator it = _data.begin();
    while(it != _data.end())
    {
        std::cout << it->first << " => " << it->second << std::endl;
        it++;
    }
}
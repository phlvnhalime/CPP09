#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    BitcoinExchange exchange;
    exchange.processInput(std::string(argv[1]));
    return 0;
}
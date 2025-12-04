#include "RPN.hpp"

RPN::RPN()
{
}

RPN::~RPN()
{
}

RPN::RPN(const RPN& copy)
{
    *this = copy;
}

RPN& RPN::operator=(const RPN& copy)
{
    if(this != &copy)
    {
        _stack = copy._stack;
    }
    return *this;
}

static bool isOperator(char character)
{
    return character == '+' || character == '-' || character == '*' || character == '/';
}

void RPN::run(const std::string &expression)
{
    if(expression.empty())
    {
        std::cerr << "Error" << std::endl;
        return;
    }
    for(size_t i = 0; i < expression.length(); i++)
    {
        char character = expression[i];

        if(character == ' ') continue;

        else if(isdigit(character))
        {
            _stack.push(character - '0');
        }

        else if(isOperator(character))
        {
            if(_stack.size() < 2)
            {
                std::cerr << "Error" << std::endl;
            }
            int a = _stack.top();
            _stack.pop();
            int b = _stack.top();
            _stack.pop();
            int res;
            if(character == '+') res = b + a;
            else if(character == '-') res = b - a;
            else if(character == '*') res = b * a;
            else if(character == '/'){
                if(a == 0) {
                    std::cerr << "Error" << std::endl;
                    return;
                }
                res = b / a;
            }
            _stack.push(res);
        }
        else
        {
            std::cerr << "Error" << std::endl;
            return;
        }
    }
    if(_stack.size() != 1)
    {
        std::cerr << "Error" << std::endl;
        return;
    }
    else {
        std::cout << _stack.top() << std::endl;
    }
}
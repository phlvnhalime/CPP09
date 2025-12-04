#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>
#include <cstdlib>

class RPN
{
    private:
        std::stack<int> _stack;
    public:
        RPN();
        ~RPN();
        RPN(const RPN& copy);
        RPN& operator=(const RPN& copy);

        void run(const std::string &expression);
};

#endif
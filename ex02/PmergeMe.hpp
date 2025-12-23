#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <ctime>
#include <cstdlib>
#include <cctype>

class PmergeMe
{
    private:
        std::vector<int> _vec;
        std::deque<int> _deq;
        double _timeVec;
        double _timeDeq;

        // Data manipulation
        void addNumber(int number);
        void sortVec(std::vector<int>& vec);
        void sortDeq(std::deque<int>& deq);
        void printContainer();

        // Ford-Johnson helper functions
        std::vector<int> generateJacobsthal(int n);
        std::vector<int> getInsertionOrder(int n);
        int binarySearchInsertPos(std::vector<int>& sorted, int value, int end);
        int binarySearchInsertPosDeq(std::deque<int>& sorted, int value, int end);

    public:

        // Program Structure and constructor
        PmergeMe();
        ~PmergeMe();
        PmergeMe(const PmergeMe& copy);
        PmergeMe& operator=(const PmergeMe& copy);

        // Public interface
        void run(int argc, char **argv);

        // Error handling
        static void stdError(const std::string& message);
        static const std::string ERROR_INVALID_ARGUMENTS_SHORT;
        static const std::string ERROR_INVALID_NUMBER_SHORT;
        static const std::string ERROR_INVALID_RANGE_SHORT;


};

#endif
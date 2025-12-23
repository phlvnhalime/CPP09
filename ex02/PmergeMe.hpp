#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>
#include <cstdlib>

class PmergeMe
{
    private:
        std::vector<int> _vec;
        std::deque<int> _deq;



    public:
        PmergeMe();
        ~PmergeMe();
        PmergeMe(const PmergeMe& copy);
        PmergeMe& operator=(const PmergeMe& copy);

        void addNumber(int number);
        void sortVec();
        void sortDeq();
        void printVec();
        void printDeq();
        static void stdError(const std::string& message);

        static const std::string ERROR_INVALID_ARGUMENTS_SHORT;
        static const std::string ERROR_INVALID_NUMBER_SHORT;
        static const std::string ERROR_INVALID_SEQUENCE_SHORT;
        static const std::string ERROR_INVALID_DUPLICATE_SHORT;
        static const std::string ERROR_INVALID_RANGE_SHORT;

        // to calculate the time taken by the sorting algorithms
        void calculateTime();
        void calculateTimeVec();
        void calculateTimeDeq();
        void printTimeVec(const std::string& time);
        void printTimeDeq(const std::string& time);
};

#endif
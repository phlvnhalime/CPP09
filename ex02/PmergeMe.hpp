#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>
#include <cstdlib>

class PmergeMe
{
    private:
        std::vector<int> vec;
        std::deque<int> deq;
    public:
        PmergeMe(void);
        PmergeMe(const PmergeMe& other);
        ~PmergeMe(void);
        PmergeMe& operator=(const PmergeMe& other);
        void addNumber(int number);
        void sort(void);
        void print(void) const;
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:36:49 by hpehliva          #+#    #+#             */
/*   Updated: 2025/12/23 13:05:45 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

// Static const definitions
const std::string PmergeMe::ERROR_INVALID_ARGUMENTS_SHORT = "Error: invalid arguments";
const std::string PmergeMe::ERROR_INVALID_NUMBER_SHORT = "Error: invalid number";
const std::string PmergeMe::ERROR_INVALID_SEQUENCE_SHORT = "Error: invalid sequence";
const std::string PmergeMe::ERROR_INVALID_DUPLICATE_SHORT = "Error: invalid duplicate";
const std::string PmergeMe::ERROR_INVALID_RANGE_SHORT = "Error: invalid range";

PmergeMe::PmergeMe()
{
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
{
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if(this != &other)
    {
        this->_vec = other._vec;
        this->_deq = other._deq;
    }
    return *this;
}

void PmergeMe::run(int argc, char **argv)
{
    // Parse and validate arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        for (size_t j = 0; j < arg.length(); j++)
        {
            if (!std::isdigit(arg[j]))
                stdError(ERROR_INVALID_NUMBER_SHORT);
        }
        int num = std::atoi(argv[i]);
        if (num < 0)
            stdError(ERROR_INVALID_RANGE_SHORT);
        addNumber(num);
    }

    // Print before
    std::cout << "Before: ";
    printVec();

    // Sort both containers
    sortVec(_vec);
    sortDeq(_deq);

    // Print after
    std::cout << "After:  ";
    printVec();

    // Print time for vector
    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " << _timeVec << " us" << std::endl;
    
    // Print time for deque
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque  : " << _timeDeq << " us" << std::endl;
}

void PmergeMe::addNumber(int number)
{
    _vec.push_back(number);
    _deq.push_back(number);
}

void PmergeMe::sortVec(std::vector<int>& vec)
{
    clock_t start = clock();
    
    // Base case: 0 or 1 elements are already sorted
    if (vec.size() <= 1)
    {
        clock_t end = clock();
        this->_timeVec = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
        return;
    }

    // Step 1: Pair and find winners (larger) and losers (smaller)
    std::vector<int> winners;
    std::vector<int> losers;
    bool hasStraggler = (vec.size() % 2 != 0);
    int straggler = 0;
    
    if (hasStraggler)
        straggler = vec.back();
    
    for (size_t i = 0; i + 1 < vec.size(); i += 2)
    {
        if (vec[i] > vec[i + 1])
        {
            winners.push_back(vec[i]);
            losers.push_back(vec[i + 1]);
        }
        else
        {
            winners.push_back(vec[i + 1]);
            losers.push_back(vec[i]);
        }
    }

    // Step 2: Recursive call on winners
    sortVec(winners);

    // Step 3: Build main chain - start with first loser (paired with smallest winner)
    // Find index of smallest winner's paired loser
    std::vector<int> sortedLosers;
    for (size_t i = 0; i < winners.size(); i++)
    {
        for (size_t j = 0; j + 1 < vec.size(); j += 2)
        {
            int w = (vec[j] > vec[j + 1]) ? vec[j] : vec[j + 1];
            int l = (vec[j] > vec[j + 1]) ? vec[j + 1] : vec[j];
            if (w == winners[i])
            {
                sortedLosers.push_back(l);
                break;
            }
        }
    }

    // Main chain starts with first loser, then all winners
    std::vector<int> mainChain;
    if (!sortedLosers.empty())
        mainChain.push_back(sortedLosers[0]);
    for (size_t i = 0; i < winners.size(); i++)
        mainChain.push_back(winners[i]);

    // Step 4: Binary search insertion using Jacobsthal sequence
    std::vector<int> insertOrder = getInsertionOrder(sortedLosers.size());
    
    for (size_t i = 0; i < insertOrder.size(); i++)
    {
        int idx = insertOrder[i];
        if (idx >= (int)sortedLosers.size() || idx == 0)
            continue;
        
        int valueToInsert = sortedLosers[idx];
        // Find position of corresponding winner to limit search
        int winnerPos = 0;
        for (size_t j = 0; j < mainChain.size(); j++)
        {
            if (mainChain[j] == winners[idx])
            {
                winnerPos = j;
                break;
            }
        }
        int pos = binarySearchInsertPos(mainChain, valueToInsert, winnerPos);
        mainChain.insert(mainChain.begin() + pos, valueToInsert);
    }

    // Insert straggler if exists
    if (hasStraggler)
    {
        int pos = binarySearchInsertPos(mainChain, straggler, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, straggler);
    }

    vec = mainChain;
    
    clock_t end = clock();
    this->_timeVec = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
}

void PmergeMe::sortDeq(std::deque<int>& deq)
{
    clock_t start = clock();
    
    // Base case: 0 or 1 elements are already sorted
    if (deq.size() <= 1)
    {
        clock_t end = clock();
        this->_timeDeq = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
        return;
    }

    // Step 1: Pair and find winners (larger) and losers (smaller)
    std::deque<int> winners;
    std::deque<int> losers;
    bool hasStraggler = (deq.size() % 2 != 0);
    int straggler = 0;
    
    if (hasStraggler)
        straggler = deq.back();
    
    for (size_t i = 0; i + 1 < deq.size(); i += 2)
    {
        if (deq[i] > deq[i + 1])
        {
            winners.push_back(deq[i]);
            losers.push_back(deq[i + 1]);
        }
        else
        {
            winners.push_back(deq[i + 1]);
            losers.push_back(deq[i]);
        }
    }

    // Step 2: Recursive call on winners
    sortDeq(winners);

    // Step 3: Build main chain - reorder losers to match sorted winners
    std::deque<int> sortedLosers;
    for (size_t i = 0; i < winners.size(); i++)
    {
        for (size_t j = 0; j + 1 < deq.size(); j += 2)
        {
            int w = (deq[j] > deq[j + 1]) ? deq[j] : deq[j + 1];
            int l = (deq[j] > deq[j + 1]) ? deq[j + 1] : deq[j];
            if (w == winners[i])
            {
                sortedLosers.push_back(l);
                break;
            }
        }
    }

    // Main chain starts with first loser, then all winners
    std::deque<int> mainChain;
    if (!sortedLosers.empty())
        mainChain.push_back(sortedLosers[0]);
    for (size_t i = 0; i < winners.size(); i++)
        mainChain.push_back(winners[i]);

    // Step 4: Binary search insertion using Jacobsthal sequence
    std::vector<int> insertOrder = getInsertionOrder(sortedLosers.size());
    
    for (size_t i = 0; i < insertOrder.size(); i++)
    {
        int idx = insertOrder[i];
        if (idx >= (int)sortedLosers.size() || idx == 0)
            continue;
        
        int valueToInsert = sortedLosers[idx];
        // Find position of corresponding winner to limit search
        int winnerPos = 0;
        for (size_t j = 0; j < mainChain.size(); j++)
        {
            if (mainChain[j] == winners[idx])
            {
                winnerPos = j;
                break;
            }
        }
        int pos = binarySearchInsertPosDeq(mainChain, valueToInsert, winnerPos);
        mainChain.insert(mainChain.begin() + pos, valueToInsert);
    }

    // Insert straggler if exists
    if (hasStraggler)
    {
        int pos = binarySearchInsertPosDeq(mainChain, straggler, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, straggler);
    }

    deq = mainChain;
    
    clock_t end = clock();
    this->_timeDeq = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
}

void PmergeMe::printVec()
{
    for(size_t i = 0; i < _vec.size(); i++)
    {
        if(i == _vec.size() - 1)
            std::cout << _vec[i] << std::endl;
        else
            std::cout << _vec[i] << " ";
    }
}

void PmergeMe::printDeq()
{
    for(size_t i = 0; i < _deq.size(); i++)
    {
        if(i == _deq.size() - 1)
            std::cout << _deq[i] << std::endl;
        else
            std::cout << _deq[i] << " ";
    }
}

void PmergeMe::stdError(const std::string& message)
{
    std::cerr << message << std::endl;
    exit(1);
}

void PmergeMe::calculateTime()
{
    calculateTimeVec();
    calculateTimeDeq();
}

void PmergeMe::calculateTimeVec()
{
    std::time_t start = std::time(nullptr);
    sortVec(_vec);
    std::time_t end = std::time(nullptr);
    printTimeVec(std::to_string(end - start));
}

void PmergeMe::calculateTimeDeq()
{
    std::time_t start = std::time(nullptr);
    sortDeq(_deq);
    std::time_t end = std::time(nullptr);
    printTimeDeq(std::to_string(end - start));
}


void PmergeMe::printTimeVec(const std::string& time)
{
    std::cout << "Vector time: " << time << " seconds" << std::endl;
}

void PmergeMe::printTimeDeq(const std::string& time)
{
    std::cout << "Deque time: " << time << " seconds" << std::endl;
}

// Generate Jacobsthal sequence up to n elements
// J(0) = 0, J(1) = 1, J(n) = J(n-1) + 2*J(n-2)
std::vector<int> PmergeMe::generateJacobsthal(int n)
{
    std::vector<int> jacobsthal;
    jacobsthal.push_back(0);
    if (n <= 0)
        return jacobsthal;
    jacobsthal.push_back(1);
    
    while (jacobsthal.back() < n)
    {
        int next = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
        jacobsthal.push_back(next);
    }
    return jacobsthal;
}

// Get insertion order based on Jacobsthal sequence
// Order: 1, 3, 2, 5, 4, 11, 10, 9, 8, 7, 6, ...
std::vector<int> PmergeMe::getInsertionOrder(int n)
{
    std::vector<int> order;
    std::vector<int> jacobsthal = generateJacobsthal(n);
    
    for (size_t i = 1; i < jacobsthal.size(); i++)
    {
        int current = jacobsthal[i];
        int prev = jacobsthal[i - 1];
        
        // Add from current down to prev+1
        for (int j = current; j > prev; j--)
        {
            if (j < n)
                order.push_back(j);
        }
    }
    return order;
}

// Binary search to find insertion position in sorted vector
int PmergeMe::binarySearchInsertPos(std::vector<int>& sorted, int value, int end)
{
    int left = 0;
    int right = end;
    
    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (sorted[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

// Binary search to find insertion position in sorted deque
int PmergeMe::binarySearchInsertPosDeq(std::deque<int>& sorted, int value, int end)
{
    int left = 0;
    int right = end;
    
    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (sorted[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}
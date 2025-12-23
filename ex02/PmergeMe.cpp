/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:36:49 by hpehliva          #+#    #+#             */
/*   Updated: 2025/12/23 15:46:11 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

// Static const definitions
const std::string PmergeMe::ERROR_INVALID_ARGUMENTS_SHORT = "Error: invalid arguments";
const std::string PmergeMe::ERROR_INVALID_NUMBER_SHORT = "Error: invalid number";
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

/*
** run() - Main entry point for the Ford-Johnson merge-insert sort
** 
** Example with input: ./PmergeMe 3 5 9 7 4 4 4
** 
** 1. Parse arguments: validates each is a positive integer
**    - "3" -> valid, add to both containers
**    - "5" -> valid, add to both containers
**    - ... and so on
**    - _vec = [3, 5, 9, 7, 4, 4, 4]
**    - _deq = [3, 5, 9, 7, 4, 4, 4]
** 
** 2. Print "Before: 3 5 9 7 4 4 4"
** 3. Sort both containers using Ford-Johnson algorithm
** 4. Print "After:  3 4 4 4 5 7 9"
** 5. Print timing for each container
*/
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
    printContainer();

    // Sort both containers using Ford-Johnson algorithm
    sortVec(_vec);
    sortDeq(_deq);

    // Print sorted result
    std::cout << "After:  ";
    printContainer();

    // Print time for vector
    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " << _timeVec << " us" << std::endl;
    
    // Print time for deque
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque  : " << _timeDeq << " us" << std::endl;
}

/*
** addNumber() - Add number to both containers
** Both containers will have identical elements for comparison purposes
*/
void PmergeMe::addNumber(int number)
{
    _vec.push_back(number);
    _deq.push_back(number);
}

/*
** sortVec() - Ford-Johnson merge-insert sort algorithm for vector
** 
** Example: vec = [3, 5, 9, 7, 4, 4, 4]
** 
** STEP 1 - PAIRING:
**   Size=7 (odd), so straggler = 4 (last element)
**   Pairs: (3,5), (9,7), (4,4)
**   Winners (larger):  [5, 9, 4]
**   Losers (smaller):  [3, 7, 4]
** 
** STEP 2 - RECURSIVE SORT ON WINNERS:
**   sortVec([5, 9, 4]) is called recursively
**     -> Size=3 (odd), straggler=4
**     -> Pairs: (5,9)
**     -> Winners: [9], Losers: [5]
**     -> sortVec([9]) -> base case, returns [9]
**     -> mainChain = [5, 9], insert straggler 4 -> [4, 5, 9]
**   Winners after sort: [4, 5, 9]
** 
** STEP 3 - REORDER LOSERS TO MATCH SORTED WINNERS:
**   Winner 4 was paired with loser 4 -> sortedLosers[0] = 4
**   Winner 5 was paired with loser 3 -> sortedLosers[1] = 3
**   Winner 9 was paired with loser 7 -> sortedLosers[2] = 7
**   sortedLosers = [4, 3, 7]
** 
**   Build mainChain: first loser + all winners
**   mainChain = [4, 4, 5, 9]
** 
** STEP 4 - INSERT REMAINING LOSERS USING JACOBSTHAL ORDER:
**   insertOrder for n=3: [1, 2]
**   
**   Insert loser[1]=3: binary search up to winner[1]=5's position
**     mainChain = [3, 4, 4, 5, 9]
**   
**   Insert loser[2]=7: binary search up to winner[2]=9's position
**     mainChain = [3, 4, 4, 5, 7, 9]
** 
** STEP 5 - INSERT STRAGGLER:
**   Insert 4: mainChain = [3, 4, 4, 4, 5, 7, 9]
** 
** Final result: [3, 4, 4, 4, 5, 7, 9]
*/
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

    // ========== STEP 1: PAIRING ==========
    // Pair adjacent elements, separate into winners (larger) and losers (smaller)
    // Example: [3,5,9,7,4,4,4] -> pairs (3,5),(9,7),(4,4), straggler=4
    std::vector<int> winners;
    std::vector<int> losers;
    bool hasStraggler = (vec.size() % 2 != 0);
    int straggler = 0;
    
    // If odd number of elements, last one waits for insertion at the end
    if (hasStraggler)
        straggler = vec.back();
    
    // Compare pairs: larger goes to winners, smaller goes to losers
    // Example: (3,5)->w:5,l:3  (9,7)->w:9,l:7  (4,4)->w:4,l:4
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

    // ========== STEP 2: RECURSIVE SORT ==========
    // Recursively sort winners until we reach base case (size <= 1)
    // Example: winners [5,9,4] -> recursive call -> returns [4,5,9]
    sortVec(winners);

    // ========== STEP 3: BUILD MAIN CHAIN ==========
    // Reorder losers to match their corresponding sorted winners
    // Example: sorted winners [4,5,9] -> find their original pairs
    //   4 was paired with 4, 5 was paired with 3, 9 was paired with 7
    //   sortedLosers = [4, 3, 7]
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

    // Main chain = first loser (already smaller than first winner) + all sorted winners
    // Example: mainChain = [4] + [4,5,9] = [4, 4, 5, 9]
    std::vector<int> mainChain;
    if (!sortedLosers.empty())
        mainChain.push_back(sortedLosers[0]);
    for (size_t i = 0; i < winners.size(); i++)
        mainChain.push_back(winners[i]);

    // ========== STEP 4: JACOBSTHAL INSERTION ==========
    // Insert remaining losers using Jacobsthal sequence order for optimal comparisons
    // Jacobsthal order: 1, 3, 2, 5, 4, 11, 10, 9, 8, 7, 6, ...
    // Example: for 3 losers, insertOrder = [1, 2] (index 0 already in mainChain)
    std::vector<int> insertOrder = getInsertionOrder(sortedLosers.size());
    
    for (size_t i = 0; i < insertOrder.size(); i++)
    {
        int idx = insertOrder[i];
        if (idx >= (int)sortedLosers.size() || idx == 0)
            continue;
        
        int valueToInsert = sortedLosers[idx];
        
        // Binary search is limited to position of corresponding winner
        // (loser is always smaller than its paired winner)
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

    // ========== STEP 5: INSERT STRAGGLER ==========
    // If we had odd elements, insert the straggler now
    // Example: insert 4 into [3,4,4,5,7,9] -> [3,4,4,4,5,7,9]
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

void PmergeMe::printContainer()
{
    for(size_t i = 0; i < _vec.size(); i++)
    {
        if(i == _vec.size() - 1)
            std::cout << _vec[i] << std::endl;
        else
            std::cout << _vec[i] << " ";
    }
}

void PmergeMe::stdError(const std::string& message)
{
    std::cerr << message << std::endl;
    exit(1);
}

/*
** generateJacobsthal() - Generate Jacobsthal sequence
** 
** Jacobsthal sequence: J(0)=0, J(1)=1, J(n) = J(n-1) + 2*J(n-2)
** Sequence: 0, 1, 1, 3, 5, 11, 21, 43, 85, 171, ...
** 
** Example: n=7
**   J(0) = 0
**   J(1) = 1
**   J(2) = 1 + 2*0 = 1
**   J(3) = 1 + 2*1 = 3
**   J(4) = 3 + 2*1 = 5
**   J(5) = 5 + 2*3 = 11 (> 7, stop)
**   Returns: [0, 1, 3, 5, 11]
** 
** Why Jacobsthal? It minimizes the number of comparisons needed
** for binary search insertions in Ford-Johnson algorithm.
*/
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

/*
** getInsertionOrder() - Get optimal insertion order using Jacobsthal sequence
** 
** Instead of inserting losers in order 1,2,3,4,5...
** We use Jacobsthal-based order: 1, 3, 2, 5, 4, 11, 10, 9, 8, 7, 6, ...
** 
** Algorithm: For each Jacobsthal pair [prev, current], insert indices
** from current down to prev+1 (descending order)
** 
** Example: n=7 (we have 7 losers to insert, index 0 already inserted)
**   Jacobsthal: [0, 1, 3, 5, 11]
**   
**   i=1: current=1, prev=0 -> add 1 (if < 7) -> [1]
**   i=2: current=3, prev=1 -> add 3,2 (if < 7) -> [1, 3, 2]
**   i=3: current=5, prev=3 -> add 5,4 (if < 7) -> [1, 3, 2, 5, 4]
**   i=4: current=11, prev=5 -> add 11,10,9,8,7,6 (if < 7) -> [1, 3, 2, 5, 4, 6]
**   
**   Result: [1, 3, 2, 5, 4, 6]
** 
** Example: n=3 (for our example [3,5,9,7,4,4,4])
**   Jacobsthal: [0, 1, 3]
**   i=1: current=1, prev=0 -> add 1 -> [1]
**   i=2: current=3, prev=1 -> add 3 (skip, >=3), add 2 -> [1, 2]
**   Result: [1, 2]
*/
std::vector<int> PmergeMe::getInsertionOrder(int n)
{
    std::vector<int> order;
    std::vector<int> jacobsthal = generateJacobsthal(n);
    
    for (size_t i = 1; i < jacobsthal.size(); i++)
    {
        int current = jacobsthal[i];
        int prev = jacobsthal[i - 1];
        
        // Add from current down to prev+1 (descending)
        for (int j = current; j > prev; j--)
        {
            if (j < n)
                order.push_back(j);
        }
    }
    return order;
}

/*
** binarySearchInsertPos() - Find correct position to insert value
** 
** Uses binary search to find the first position where sorted[pos] >= value
** Search is limited to [0, end) because loser is always < its paired winner
** 
** Example: Insert 3 into mainChain [4, 4, 5, 9], end=2 (position of winner 5)
**   Search range: [0, 2) -> elements [4, 4]
**   left=0, right=2
**   mid=1, sorted[1]=4 >= 3 -> right=1
**   mid=0, sorted[0]=4 >= 3 -> right=0
**   left=right=0 -> return 0
**   Insert at position 0: [3, 4, 4, 5, 9]
** 
** Example: Insert 7 into mainChain [3, 4, 4, 5, 9], end=4 (position of winner 9)
**   Search range: [0, 4) -> elements [3, 4, 4, 5]
**   left=0, right=4
**   mid=2, sorted[2]=4 < 7 -> left=3
**   mid=3, sorted[3]=5 < 7 -> left=4
**   left=right=4 -> return 4
**   Insert at position 4: [3, 4, 4, 5, 7, 9]
*/
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

/*
** binarySearchInsertPosDeq() - Same as above but for std::deque
** (Separate function because different container types)
*/
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
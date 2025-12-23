/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:36:49 by hpehliva          #+#    #+#             */
/*   Updated: 2025/12/23 12:29:32 by hpehliva         ###   ########.fr       */
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

void PmergeMe::addNumber(int number)
{
    _vec.push_back(number);
    _deq.push_back(number);
}

void PmergeMe::sortVec()
{
    std::sort(_vec.begin(), _vec.end());
}

void PmergeMe::sortDeq()
{
    std::sort(_deq.begin(), _deq.end());
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
    sortVec();
    std::time_t end = std::time(nullptr);
}

void PmergeMe::calculateTimeDeq()
{
    std::time_t start = std::time(nullptr);
    sortDeq();
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
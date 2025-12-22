/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:14:22 by hpehliva          #+#    #+#             */
/*   Updated: 2025/12/22 21:14:23 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
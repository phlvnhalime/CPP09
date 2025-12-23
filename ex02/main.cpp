/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:14:16 by hpehliva          #+#    #+#             */
/*   Updated: 2025/12/23 12:09:39 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if(argc < 2)
        PmergeMe::stdError(PmergeMe::ERROR_INVALID_ARGUMENTS_SHORT);
    for(int i = 1; i < argc; i++)
    {
        if(argv[i] == NULL)
        {
            PmergeMe::stdError(PmergeMe::ERROR_INVALID_NUMBER_SHORT);
        }
    }
    PmergeMe pmergeMe;
    for(int i = 1; i < argc; i++)
    {
        pmergeMe.addNumber(atoi(argv[i]));
    }
    pmergeMe.sortVec();
    pmergeMe.sortDeq();
    pmergeMe.printVec();
    pmergeMe.printDeq();
    return 0;
}
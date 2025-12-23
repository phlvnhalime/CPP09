/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:14:16 by hpehliva          #+#    #+#             */
/*   Updated: 2025/12/23 12:47:37 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
        PmergeMe::stdError(PmergeMe::ERROR_INVALID_ARGUMENTS_SHORT);
    
    PmergeMe pmergeMe;
    pmergeMe.run(argc, argv);
    return 0;
}
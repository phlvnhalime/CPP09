/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:36:49 by hpehliva          #+#    #+#             */
/*   Updated: 2025/12/22 21:36:54 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe(void): vec(), deq()
{
}

PmergeMe::PmergeMe(const PmergeMe& other): vec(other.vec), deq(other.deq)
{
}

PmergeMe::~PmergeMe(void)
{
}
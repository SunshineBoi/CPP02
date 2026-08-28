/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 16:10:48 by kong              #+#    #+#             */
/*   Updated: 2026/08/28 16:50:25 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

const int Fixed::frac = 8;

Fixed::Fixed() : fp(0)
{
	std::cout
		<< "Default constructor called"
		<< std::endl;
}

Fixed::Fixed(const Fixed& other)
{
	std::cout
		<< "Copy constructor called"
		<< std::endl;
	fp = other.getRawBits();
}

Fixed& Fixed::operator=(const Fixed& other)
{
	std::cout
		<< "Copy assignment operator called"
		<< std::endl;
	if (this != &other)
	{
		fp = other.getRawBits();
	}
	return (*this);
}

Fixed::~Fixed()
{
	std::cout
		<< "Destructor called"
		<< std::endl;
}

int	Fixed::getRawBits( void ) const
{
	std::cout
		<< "getRawBits member function called"
		<< std::endl;
	return (fp);
}

void	Fixed::setRawBits( int const raw )
{
	fp = raw;
	std::cout
		<< "setRawBits member function called"
		<< std::endl;
}

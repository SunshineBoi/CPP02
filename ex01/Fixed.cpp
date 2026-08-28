/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 16:10:48 by kong              #+#    #+#             */
/*   Updated: 2026/08/28 19:00:35 by kong             ###   ########.fr       */
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

Fixed::Fixed(const int n) : fp(n << frac)
{
	// shifting into integer bits
	std::cout
		<< "Int constructor called"
		<< std::endl;
}

Fixed::Fixed(const float n)
{
	std::cout
		<< "Float constructor called"
		<< std::endl;
	fp = roundf(n * (1 << frac));  // bit shift equiv to  2 ^ frac
}

Fixed::Fixed(const Fixed& other)
{
	std::cout
		<< "Copy constructor called"
		<< std::endl;
	*this = other;
}

Fixed& Fixed::operator=(const Fixed& other)
{
	std::cout
		<< "Copy assignment operator called"
		<< std::endl;
	if (this != &other)
	{
		fp = other.fp;
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

float	Fixed::toFloat( void ) const
{
	return (static_cast<float>(fp) / (1 << frac));
}

int		Fixed::toInt( void ) const
{
	return (fp >> frac);
}

std::ostream& operator<<(std::ostream& out, const Fixed& val)
{
	out << val.toFloat();

	return (out);
}

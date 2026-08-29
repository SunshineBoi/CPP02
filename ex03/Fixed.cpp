/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 16:10:48 by kong              #+#    #+#             */
/*   Updated: 2026/08/29 14:23:47 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

const int Fixed::frac = 8;

Fixed& Fixed::min(Fixed& a, Fixed& b)
{
	if (a < b)
		return (a);
	return (b);
}

const Fixed& Fixed::min(const Fixed& a, const Fixed& b)
{
	// const object can only invoke const-qualified method
	// a.operator<(b) has to guarantees it doesnt modify `a`.
	// hence, the operator itself has to be a const-method 
	if (a < b)
		return (a);
	return (b);
}

Fixed& Fixed::max(Fixed& a, Fixed& b)
{
	if (a > b)
		return (a);
	return (b);
}

const Fixed& Fixed::max(const Fixed& a, const Fixed& b)
{
	if (a > b)
		return (a);
	return (b);
}

Fixed::Fixed() : fp(0)
{
	// std::cout
	// 	<< "Default constructor called"
	// 	<< std::endl;
}

Fixed::Fixed(const int n) : fp(n << frac)
{
	// shifting into integer bits
	// std::cout
	// 	<< "Int constructor called"
	// 	<< std::endl;
}

Fixed::Fixed(const float n)
{
	// std::cout
	// 	<< "Float constructor called"
	// 	<< std::endl;
	fp = roundf(n * (1 << frac));  // bit shift equiv to  2 ^ frac
	// for example:
	// 0.01 * 256 = 2.56 -> rounded to 3.
	// we shall see binary representation of 3.
}

Fixed::Fixed(const Fixed& other)
{
	// std::cout
	// 	<< "Copy constructor called"
	// 	<< std::endl;
	*this = other;
}

Fixed& Fixed::operator=(const Fixed& other)
{
	// std::cout
	// 	<< "Copy assignment operator called"
	// 	<< std::endl;
	if (this != &other)
	{
		fp = other.fp;
	}
	return (*this);
}

Fixed::~Fixed()
{
	// std::cout
	// 	<< "Destructor called"
	// 	<< std::endl;
}

int	Fixed::getRawBits( void ) const
{
	// std::cout
	// 	<< "getRawBits member function called"
	// 	<< std::endl;
	return (fp);
}

void	Fixed::setRawBits( int const raw )
{
	fp = raw;
	// std::cout
	// 	<< "setRawBits member function called"
	// 	<< std::endl;
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

bool	Fixed::operator==(const Fixed& other) const
{
	return (this->fp == other.fp);
}

bool	Fixed::operator!=(const Fixed& other) const
{
	return (this->fp != other.fp);
}

bool	Fixed::operator>=(const Fixed& other) const
{
	return (this->fp >= other.fp);
}

bool	Fixed::operator<=(const Fixed& other) const
{
	return (this->fp <= other.fp);
}

bool	Fixed::operator>(const Fixed& other) const
{
	return (this->fp > other.fp);
}

bool	Fixed::operator<(const Fixed& other) const
{
	return (this->fp < other.fp);
}

Fixed	Fixed::operator+(const Fixed& other) const
{
	Fixed	ret;

	// this is in 256-based ald
	ret.setRawBits(this->fp + other.fp);
	return (ret);
}

Fixed	Fixed::operator-(const Fixed& other) const
{
	Fixed	ret;

	// this is in 256-based ald
	ret.setRawBits(this->fp - other.fp);
	return (ret);
}

Fixed	Fixed::operator*(const Fixed& other) const
{
	float	temp;

	temp = this->toFloat() * other.toFloat();
	Fixed ret( temp );

	return (ret);
}

Fixed	Fixed::operator/(const Fixed& other) const
{
	float	temp;

	if (other.getRawBits() == 0)
	{
		int x = 1;

		x /= other.getRawBits();
	}
	temp = this->toFloat() / other.toFloat();
	Fixed ret( temp );

	return (ret);
}

Fixed&	Fixed::operator++()
{
	fp += 1;
	return (*this);
}

// post increment has to return old val
Fixed	Fixed::operator++(int)
{
	Fixed	old( *this );
	fp += 1;
	return (old);
}

Fixed&	Fixed::operator--()
{
	fp -= 1;
	return (*this);
}

Fixed	Fixed::operator--(int)
{
	Fixed	old( *this );
	fp -= 1;
	return (old);
}

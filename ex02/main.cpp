/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 15:37:45 by kong              #+#    #+#             */
/*   Updated: 2026/08/29 11:08:22 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

static void testConstructors( void )
{
	std::cout << "\n--- constructors ---" << std::endl;

	Fixed	def;
	Fixed	fromInt( 42 );
	Fixed	fromFloat( 42.42f );
	Fixed	fromNegFloat( -13.37f );
	Fixed	copy( fromFloat );
	Fixed	assigned;

	assigned = fromInt;

	std::cout << "def          = " << def          << std::endl;
	std::cout << "fromInt      = " << fromInt      << std::endl;
	std::cout << "fromFloat    = " << fromFloat    << std::endl;
	std::cout << "fromNegFloat = " << fromNegFloat << std::endl;
	std::cout << "copy         = " << copy         << std::endl;
	std::cout << "assigned     = " << assigned     << std::endl;
}

static void testComparisons( void )
{
	std::cout << "\n--- comparisons ---" << std::endl;

	Fixed	a( 5.5f );
	Fixed	b( 5.5f );
	Fixed	c( 2.25f );

	std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
	std::cout << "a == b : " << (a == b) << std::endl;
	std::cout << "a != c : " << (a != c) << std::endl;
	std::cout << "a > c  : " << (a > c)  << std::endl;
	std::cout << "c < a  : " << (c < a)  << std::endl;
	std::cout << "a >= b : " << (a >= b) << std::endl;
	std::cout << "a <= b : " << (a <= b) << std::endl;

	const Fixed	ca( 1.0f );
	const Fixed	cb( 2.0f );

	std::cout << "const ca < cb : " << (ca < cb) << std::endl;
}

static void testArithmetic( void )
{
	std::cout << "\n--- arithmetic ---" << std::endl;

	Fixed	a( 10.5f );
	Fixed	b( 3.25f );

	std::cout << "a = " << a << ", b = " << b << std::endl;
	std::cout << "a + b = " << (a + b) << std::endl;
	std::cout << "a - b = " << (a - b) << std::endl;
	std::cout << "a * b = " << (a * b) << std::endl;
	std::cout << "a / b = " << (a / b) << std::endl;

	Fixed	neg( -4.5f );

	std::cout << "a + neg = " << (a + neg) << std::endl;
	std::cout << "neg * neg = " << (neg * neg) << std::endl;
}

static void testIncrementDecrement( void )
{
	std::cout << "\n--- increment / decrement ---" << std::endl;

	Fixed	a( 5.0f );

	std::cout << "start      a = " << a << std::endl;
	std::cout << "++a  returns " << (++a) << " | a is now " << a << std::endl;
	std::cout << "a++  returns " << (a++) << " | a is now " << a << std::endl;
	std::cout << "--a  returns " << (--a) << " | a is now " << a << std::endl;
	std::cout << "a--  returns " << (a--) << " | a is now " << a << std::endl;

	Fixed	edge;

	edge.setRawBits( 255 );
	std::cout << "\nedge case: raw 255 (" << edge << ") ++ -> ";
	++edge;
	std::cout << edge << " (raw " << edge.getRawBits() << ")" << std::endl;

	edge.setRawBits( 0 );
	std::cout << "edge case: raw 0 (" << edge << ") -- -> ";
	--edge;
	std::cout << edge << " (raw " << edge.getRawBits() << ")" << std::endl;
}

static void testMinMax( void )
{
	std::cout << "\n--- min / max ---" << std::endl;

	Fixed		a( 4.2f );
	Fixed		b( 7.7f );
	const Fixed	ca( 1.5f );
	const Fixed	cb( -2.5f );

	std::cout << "min(a, b)   = " << Fixed::min( a, b )   << std::endl;
	std::cout << "max(a, b)   = " << Fixed::max( a, b )   << std::endl;
	std::cout << "min(ca, cb) = " << Fixed::min( ca, cb ) << std::endl;
	std::cout << "max(ca, cb) = " << Fixed::max( ca, cb ) << std::endl;
	std::cout << "min(a, cb)  = " << Fixed::min( a, cb )  << std::endl;

	Fixed::max( a, b ).setRawBits( 0 );
	std::cout << "after resetting the max through the returned reference: a = "
		<< a << ", b = " << b << std::endl;
}

static void testDivideByZero( void )
{
	std::cout << "\n--- divide by zero ---" << std::endl;

	Fixed	pos( 10.0f );
	Fixed	neg( -10.0f );
	Fixed	zero( 0.0f );

	std::cout << "pos / zero  = " << (pos / zero)  << std::endl;
	std::cout << "neg / zero  = " << (neg / zero)  << std::endl;
	std::cout << "zero / zero = " << (zero / zero) << std::endl;

	std::cout << "raw bits of pos / zero  : " << (pos / zero).getRawBits()  << std::endl;
	std::cout << "raw bits of zero / zero : " << (zero / zero).getRawBits() << std::endl;
}

int main(int ac, char **av)
{
	(void)av;
	if (ac == 2)
	{
		testConstructors();
		testComparisons();
		testArithmetic();
		testIncrementDecrement();
		testMinMax();
	}
	testDivideByZero();
	return (0);
}

/*
int main( void )
{
	Fixed a;
	Fixed const b( Fixed( 5.05f ) * Fixed( 2 ) );

	std::cout << a << std::endl;
	std::cout << ++a << std::endl;
	std::cout << a << std::endl;
	std::cout << a++ << std::endl;
	std::cout << a << std::endl;

	std::cout << b << std::endl;

	std::cout << Fixed::max( a, b ) << std::endl;
	return (0);
}
*/
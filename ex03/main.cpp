/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 15:00:00 by kong              #+#    #+#             */
/*   Updated: 2026/08/29 15:00:00 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"

static void check( const char* label, Point const a, Point const b, Point const c,
	Point const point, bool expected )
{
	bool	result = bsp( a, b, c, point );

	std::cout << label << " -> " << (result ? "inside" : "outside")
		<< "  [" << (result == expected ? "OK" : "FAIL") << "]" << std::endl;
}

int main( void )
{
	Point	a( 0.0f, 0.0f );
	Point	b( 4.0f, 0.0f );
	Point	c( 0.0f, 4.0f );

	std::cout << "triangle: a(0,0) b(4,0) c(0,4)" << std::endl << std::endl;

	check( "clearly inside      (1, 1)   ", a, b, c, Point( 1.0f, 1.0f ), true );
	check( "clearly outside     (5, 5)   ", a, b, c, Point( 5.0f, 5.0f ), false );
	check( "outside, negative   (-1, -1) ", a, b, c, Point( -1.0f, -1.0f ), false );
	check( "on edge a-b         (2, 0)   ", a, b, c, Point( 2.0f, 0.0f ), false );
	check( "on edge a-c         (0, 2)   ", a, b, c, Point( 0.0f, 2.0f ), false );
	check( "on hypotenuse b-c   (2, 2)   ", a, b, c, Point( 2.0f, 2.0f ), false );
	check( "exactly on vertex a (0, 0)   ", a, b, c, Point( 0.0f, 0.0f ), false );
	check( "just inside hyp.    (1.9,1.9)", a, b, c, Point( 1.9f, 1.9f ), true );
	check( "near origin, inside (0.1,0.1)", a, b, c, Point( 0.1f, 0.1f ), true );

	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 15:37:45 by kong              #+#    #+#             */
/*   Updated: 2026/08/28 17:03:10 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

int main(void)
{
	Fixed	a;
	Fixed	b(a);
	Fixed	c;

	c = b;

	std::cout << a.getRawBits() << std::endl;
	std::cout << b.getRawBits() << std::endl;
	std::cout << c.getRawBits() << std::endl;
	return (0);
}

/*
Default constructor called			(a)
Copy constructor called				(b(a))
getRawBits member function called	=results of copy ctor
Default constructor called			(c)
Copy assignment operator called		(c = b)
getRawBits member function called	=results of assign ctor
getRawBits member function called
0
getRawBits member function called
0
getRawBits member function called
0
Destructor called
Destructor called
Destructor called
*/


/*
Lower 8 bits represents the fractional part of real value.
0.5 
decimal -> 5 in the range of 0 - 9;
binary -> 128 in the range of 0 - 255;
both representing the floating number 0.5

Resolution of 0.1 means every increment step is 1.1, 1.2 ...
similarly, with 8 bits representing fractional part, we have 1/256 ~= 0.00390625 as the increment step.
anything in between gets rounded off.
0/256 = 0.00000000
1/256 = 0.00390625
2/256 = 0.00781250
3/256 = 0.01171875
...
255/256 = 0.99609375

More fractional bits means finer precision and vice versa. Basically, the more bits (larger range) to be divisioned,
the finer the steps are. Think of this, how many parts we can cut a decimal 1 into?
range of 10 -> divide into 10 sections
range of 256 -> divide into 1/256 sections
the larger it is, we can create finer compartment.

Why multiply by 2^frac ? 
in Decimal , shifting one bit (decimal) is literally divide / multiply by 10.
same case, for Binary, shifting a bit is to take (base) ^ (number of bits representation)
Therefore, allowing bit-shifting operation, and example:
Concretely, for 5.5:
5.5 × 256 = 1408   →  store fp = 1408
Check it in binary: 1408 = 0b101_10000000
Decipher:
101 	| 10000000
^whole=5  ^ fractional bits: 10000000 = 128/256 = 0.5
Split right at bit #8, and you can literally read off 5 and .5 from the two halves.
*why divide by 256? exactly the 'available compartment range' noted above.
*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 11:52:26 by kong              #+#    #+#             */
/*   Updated: 2026/08/29 14:17:00 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Point.hpp"

static Fixed	cross(Fixed ux, Fixed uy, Fixed vx, Fixed vy)
{
	// u and v are vectors
	// taking cross product returns perpendicular vector (z-vector)
	// depending on who cross who, z-vector will point up or down,
		// indicating positive or negative (left or right)
	Fixed ret ((ux * vy) - (uy * vx));
	return (ret);	
}

bool bsp( Point const a, Point const b, Point const c, Point const point )
{
	bool	is_neg = 0;
	bool	is_pos = 0;

	Fixed z1 = cross(
		b.getX() - a.getX(),
		b.getY() - a.getY(),
		point.getX() - a.getX(),
		point.getY() - a.getY()
	);
	Fixed z2 = cross(
		c.getX() - b.getX(),
		c.getY() - b.getY(),
		point.getX() - b.getX(),
		point.getY() - b.getY()
	);
	Fixed z3 = cross(
		a.getX() - c.getX(),
		a.getY() - c.getY(),
		point.getX() - c.getX(),
		point.getY() - c.getY()
	);
	is_neg = (z1.getRawBits() < 0 && z2.getRawBits() < 0 && z3.getRawBits() < 0);
	is_pos = (z1.getRawBits() > 0 && z2.getRawBits() > 0 && z3.getRawBits() > 0);

	return ((is_neg || is_pos));
}

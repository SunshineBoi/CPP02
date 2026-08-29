/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 11:26:07 by kong              #+#    #+#             */
/*   Updated: 2026/08/29 12:47:36 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_HPP
# define POINT_HPP

# include "Fixed.hpp"

class Point
{
private:
	Fixed const x;
	Fixed const y;

public:
	Point();
	Point(const float a, const float b);
	Point(const Point& other);
	Point& operator=(const Point& other);
	~Point();

	Fixed	getX( void ) const;
	Fixed	getY( void ) const;
};

bool	bsp( Point const a, Point const b, Point const c, Point const point );

#endif

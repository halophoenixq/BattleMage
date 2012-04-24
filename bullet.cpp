/*#################################################################

BattleMage bullet.cpp
Copyright (C) 2012 Quentin Hsu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

To contact the author, email: haloqa@gmail.com

##################################################################*/

#include "bullet.h"
#include <QRadialGradient>
#include <QPainter>

bullet::bullet() : QGraphicsEllipseItem()
{
	type = 1; //first type of bullet
	setRect(-10.0,-10.0,20.0,20.0); //set bullet size
	QPointF temp(370, 370); // initialize a position
	set_current(temp); 
	speed = 0;
	//color bullet red gradient
	QRadialGradient rGrad( 0.0, 0.0, 10.0, 0.0, 0.0);
    rGrad.setColorAt(0.0, QColor(255,255,255));
    rGrad.setColorAt(0.5, QColor(200,100,20));
    rGrad.setColorAt(1.0, QColor(255,0,0,0));
    setBrush(QBrush(rGrad));
    setPen(QPen(Qt::NoPen)); //no outline
	active = false;
}

void bullet::advance(int phase)
{
	if(phase = 1)
	{
		setPoint(current, current.x() - speed, current.y()); //move left based off speed
		setPos(current);
		if(current.x() < 0) //delete bullet if off screen
		{
			active = false;
		}
	}
}

//setters and getters
void bullet::set_current(int x, int y)
{
	QPointF temp(x, y);
	current = temp;
}

void bullet::set_current(double x, double y)
{
	QPointF temp(x, y);
	current = temp;
}

void bullet::set_current(QPointF temp)
{
	current = temp;
}

QPointF bullet::get_current()
{
	return current;
}

void bullet::set_active(bool x)
{
	active = x;
}

bool bullet::get_active()
{
	return active;
}

void bullet::setPoint(QPointF& current, int x, int y) //function to set QPointF's easier
{
	current.setX(x);
	current.setY(y);
}

void bullet::setPoint(QPointF& current, double x, double y)
{
	current.setX(x);
	current.setY(y);
}

void bullet::set_speed(double x)
{
	speed = x;
}

double bullet::get_speed()
{
	return speed;
}

void bullet::set_type(int i)
{
	type = i;
}

int bullet::get_type()
{
	return type;
}
/*#################################################################

BattleMage tracker.cpp
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

#include "tracker.h"
#include <QRadialGradient>
#include <QPainter>

tracker::tracker() : aimer()
{
	type = 3;
	setRect(-30.0,-30.0,60.0,60.0); //set size
	QPointF temp(370, 370);
	set_current(temp);
	speed = 0;
	//color the bullet
	QRadialGradient rGrad( 0.0, 0.0, 30.0, 0.0, 0.0);
    rGrad.setColorAt(0.0, QColor(255,255,255));
    rGrad.setColorAt(0.7, QColor(20,200,100));
    rGrad.setColorAt(1.0, QColor(0,255,0,0));
    setBrush(QBrush(rGrad));
    setPen(QPen(Qt::NoPen));
}

void tracker::set_direction(QPointF target)
{
	slope = (target.y()-current.y())/(target.x()-current.x()); //adjust slope between target and bullet
}

QPointF tracker::get_target() //getter
{
	return target;
}

void tracker::set_target(QPointF x) //setter 
{
	target = x;
}

void tracker::advance(int phase) 
{
	if(phase = 1)
	{
		if (current.x() - target.x() > 40) //if bullet's x coordinate not too close (avoid infinity) adjust targetting
		{
			set_direction(target);
		}
		double newx = current.x();
		double newy = current.y();
		newx -= speed;
		newy -= slope*speed; //update coordinates
		setPoint(current, newx, newy);
		setPos(current);
		if(current.x() < 0)
		{
			active = false;
		}
	}
}
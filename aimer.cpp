/*#################################################################

BattleMage aimer.cpp
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

#include "aimer.h"
#include <QRadialGradient>
#include <QPainter>

aimer::aimer() : bullet()
{
	type = 2; //bullet type
	setRect(-20.0,-20.0,40.0,40.0); //set bullet size
	QPointF temp(370, 370);
	set_current(temp);
	speed = 0;
	//color bullet
	QRadialGradient rGrad( 0.0, 0.0, 20.0, 0.0, 0.0);
    rGrad.setColorAt(0.0, QColor(255,255,255));
    rGrad.setColorAt(0.7, QColor(20,100,200));
    rGrad.setColorAt(1.0, QColor(0,0,255,0));
    setBrush(QBrush(rGrad));
    setPen(QPen(Qt::NoPen));
}

void aimer::set_direction(QPointF target)
{
	slope = (target.y()-current.y())/(target.x()-current.x()); //get slope between target and bullet
}

void aimer::advance(int phase) 
{
	if(phase = 1)
	{
		double newx = current.x();
		double newy = current.y();
		newx -= speed;
		newy -= slope*speed; //move y based on speed
		setPoint(current, newx, newy);
		setPos(current);
		if(current.x() < 0) //inactivate bullet if it reaches the edge
		{
			active = false;
		}
	}
}
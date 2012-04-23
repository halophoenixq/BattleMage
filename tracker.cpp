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
/*#################################################################

BattleMage turrets.h
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

#pragma once
#include "aimer.h"
#include <QGraphicsPixmapItem>

class turret
{
protected:
	QPointF current; //current turret location
	QGraphicsPixmapItem *item; //Pixmap item pointer
	int fire_rate; //turret fire rate, not implemented in current version of game
	int health; //turret health
	bool active;
public:
	turret(); //constructor
	virtual ~turret(); //destructor

	//getters and setters
	void set_current(QPointF);
	QPointF get_current();

	void set_item(QGraphicsPixmapItem*);
	QGraphicsPixmapItem* get_item();

	void set_fire_rate(int);
	int get_fire_rate();

	void set_health(int);
	int get_health();

	void set_active(bool);
	bool get_active();

	void setPoint(QPointF&, int, int); //set current point
	void setPoint(QPointF&, double, double);
};
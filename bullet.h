/*#################################################################

BattleMage bullet.h
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

#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsEllipseItem>
#include <QBrush>

class bullet : public QGraphicsEllipseItem
{
private:
	QBrush color; //paint the bullet
protected:
	QPointF current; //current position
	double speed; //bullet speed
	bool active;
	int type; //bullet type
public:
	bullet();
	virtual ~bullet(){}

	virtual void advance(int); //advance the bullet

	//setters and getters
	void set_current(int x, int y);
	void set_current(double x, double y);
	void set_current(QPointF current);
	QPointF get_current();

	void set_active(bool x);
	bool get_active();

	void set_speed(double x);
	double get_speed();

	void set_type(int i);
	int get_type();

	void setPoint(QPointF&, int, int); //set current point
	void setPoint(QPointF&, double, double);
};

#endif
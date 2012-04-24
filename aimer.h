/*#################################################################

BattleMage aimer.h
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
#include "bullet.h"
#include <QBrush>

class aimer : public bullet
{
protected:
	double slope; //slope to see how to move the aimer
	QBrush color; //brush to color the bullet
public:
	aimer();
	virtual ~aimer(){}

	void set_direction(QPointF target); //update slope towards target
	virtual void advance(int phase); //move bullet
 
};
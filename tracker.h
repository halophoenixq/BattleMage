/*#################################################################

BattleMage tracker.h
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

#ifndef TRACKER_H
#define TRACKER_H
#include "aimer.h"

class tracker : public aimer
{
protected:
	QPointF target; //copy of target that is updated in main program
public:
	tracker();
	virtual ~tracker(){}

	void set_direction(QPointF target); //adjust slope
	virtual void advance(int phase); //move bullet
 
	//getters and setters
	QPointF get_target();
	void set_target(QPointF x);
};

#endif
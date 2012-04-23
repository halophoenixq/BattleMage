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
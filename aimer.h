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
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
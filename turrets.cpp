#include "turrets.h"

turret::turret()
{
	setPoint(current, 0, 0); //set current position
	health = 0;
}

turret::~turret()
{
	delete item;
}

//getters  and setters
void turret::set_fire_rate(int x)
{
	fire_rate = x;
}

int turret::get_fire_rate()
{
	return fire_rate;
}

void turret::set_health(int x)
{
	health = x;
}

int turret::get_health()
{
	return health;
}

void turret::set_current(QPointF x)
{
	current = x;
}

QPointF turret::get_current()
{
	return current;
}

void turret::set_item(QGraphicsPixmapItem* x)
{
	item = x;
}

QGraphicsPixmapItem* turret::get_item()
{
	return item;
}

void turret::set_active(bool x)
{
	active = x;
}

bool turret::get_active()
{
	return active;
}

void turret::setPoint(QPointF& current, int x, int y) //function to set QPointF's easier
{
	current.setX(x);
	current.setY(y);
}

void turret::setPoint(QPointF& current, double x, double y)
{
	current.setX(x);
	current.setY(y);
}
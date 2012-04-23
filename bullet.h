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
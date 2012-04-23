#include "map.h"
#include "window.h"
#include <QApplication>
#include <QTimer>
#include <QGridLayout>
#include <QWidget>
#include <QGridLayout>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Window *window = new Window; //main game
	window->resize(640, 540); //adjust windows size
	window->show(); //display
	
	return app.exec();
}

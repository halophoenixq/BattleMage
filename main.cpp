/*#################################################################

BattleMage main.cpp
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

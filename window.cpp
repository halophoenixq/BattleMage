#include "window.h"
#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QTimer>
#include <QRadialGradient>
#include <QDockWidget>
#include <QStyle>
#include <QFont>


//Window Class
Window::Window()
{
	//menu bar
	setMenuBar();
	
	//central widget
	game = new Map(1); //construct first stage of game
	game->setSceneRect(0.0, 0.0, 640.0, 360.0);	//set game screen sizes

	timer = new QTimer(); //create a timer, not active yet
    connect(timer, SIGNAL(timeout()), game, SLOT(advance())); //connect timeouts to scene updates
	connect(game, SIGNAL(turret_battle()), this, SLOT(turrets())); //connect stage advance signal to turrets
	connect(game, SIGNAL(boss_battle()), this, SLOT(boss())); //moves to boss battle
    connect(game, SIGNAL(dead()), this, SLOT(die())); //connects stage death to die function here
	view = new QGraphicsView(game); //allocate memory for graphicsview
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //turn off main window scroll bars
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setCentralWidget(view); //set graphicsview as central widget

	//dock widgets
	setDockWidgets(); //set dockwidget function
	game->set_toggle(towerClick->blue); //update game's tower spawn variable

	//prevent shortcuts except quit
	disconnect(actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionFirst, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionTurrets, SIGNAL(triggered()), this, SLOT(turrets()));
	disconnect(actionBoss, SIGNAL(triggered()), this, SLOT(boss()));
	//connect restart shortcut to start game
	connect(actionRestart, SIGNAL(triggered()), this, SLOT(deleteInstructions()));
	instructions(); //display instructions
}

Window::~Window()
{
	//delete dynamic variables
	delete file;
	delete stages;
	delete timer;
	delete view;
}

void Window::restart() //restart game
{
	timer->stop(); //stop the timer
	delete timer; //remake timer
	delete game; //clear old Map

	game = new Map(1); //make new map
	game->setSceneRect(0.0, 0.0, 640.0, 360.0);	

	timer = new QTimer(); 
    connect(timer, SIGNAL(timeout()), game, SLOT(advance())); //reconnect signals
	connect(game, SIGNAL(turret_battle()), this, SLOT(turrets()));
	connect(game, SIGNAL(boss_battle()), this, SLOT(boss())); //moves to boss battle
	connect(game, SIGNAL(dead()), this, SLOT(die()));

	view->setScene(game); //change qgraphicsview's scene
	//delete dockwidgets
	removeDockWidget(info);
	removeDockWidget(towerInfo);
	delete info; //remake stats
	delete towerInfo; //remake button
	setDockWidgets(); //remake dockwidgets
	game->set_toggle(towerClick->blue); //update tower activation

	//display stage name
	tempTimer = new QTimer();
	QString x("First Stage");
	connect(tempTimer, SIGNAL(timeout()), this, SLOT(deleteDisplay())); //connect tempTimer to manage display time
	//prevent shortcuts except quit until stage name screen is over
	disconnect(actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionFirst, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionTurrets, SIGNAL(triggered()), this, SLOT(turrets()));
	disconnect(actionBoss, SIGNAL(triggered()), this, SLOT(boss()));
	setDisplay(x); //display the stage name
	tempTimer->start(1500); //start tempTimer, game timer is in deleteDisplay function
}

void Window::turrets()
{
	timer->stop();
	//copy game stats to transfer to new scene
	int temp_score = game->get_score();
	int temp_lives = game->get_lives();
	int temp_kills = game->get_kills();
	int temp_tower = game->get_max_towers();
	//mage location
	QPointF temp_current_mage = game->get_current_mage();

	delete game; //clear old Map
	game = new Map(2); //make turret stage
	game->setSceneRect(0.0, 0.0, 640.0, 360.0);	

	delete timer; //remake timer
	timer = new QTimer(); 
    connect(timer, SIGNAL(timeout()), game, SLOT(advance())); //reconnect signals
	connect(game, SIGNAL(turret_battle()), this, SLOT(turrets()));
	connect(game, SIGNAL(boss_battle()), this, SLOT(boss())); //moves to boss battle
	connect(game, SIGNAL(dead()), this, SLOT(die()));

	view->setScene(game); //change qgraphicsview's scene
	
	//remake dockwidgets
	removeDockWidget(info);
	removeDockWidget(towerInfo);
	delete info; //remake stats
	delete towerInfo; //remake button
	setDockWidgets();

	//transfer backup stats to new scene
	game->set_current_mage(temp_current_mage);
	game->set_score(temp_score);
	game->set_lives(temp_lives);
	game->set_kills(temp_kills);
	game->set_max_towers(temp_tower);
	game->set_toggle(towerClick->blue);

	//display stage name
	tempTimer = new QTimer();
	QString x("Turret Battle");
	connect(tempTimer, SIGNAL(timeout()), this, SLOT(deleteDisplay()));
	disconnect(actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionFirst, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionTurrets, SIGNAL(triggered()), this, SLOT(turrets()));
	disconnect(actionBoss, SIGNAL(triggered()), this, SLOT(boss()));
	setDisplay(x);
	tempTimer->start(1500);
}

void Window::boss()
{
	//same as turret() function, changed to load boss scene
	timer->stop();
	int temp_score = game->get_score();
	int temp_lives = game->get_lives();
	int temp_kills = game->get_kills();
	int temp_tower = game->get_max_towers();
	QPointF temp_current_mage = game->get_current_mage();
	delete game; //clear old Map
	game = new Map(3); //make boss map
	game->setSceneRect(0.0, 0.0, 640.0, 360.0);	
	delete timer; //remake timer
	timer = new QTimer(); 
    connect(timer, SIGNAL(timeout()), game, SLOT(advance())); //reconnect signals
	connect(game, SIGNAL(turret_battle()), this, SLOT(turrets()));
	connect(game, SIGNAL(boss_battle()), this, SLOT(boss())); //moves to boss battle
	connect(game, SIGNAL(dead()), this, SLOT(die()));
	view->setScene(game); //change qgraphicsview's scene
	removeDockWidget(info);
	removeDockWidget(towerInfo);
	delete info; //remake stats
	delete towerInfo; //remake button
	setDockWidgets();
	game->set_current_mage(temp_current_mage);
	game->set_score(temp_score);
	game->set_lives(temp_lives);
	game->set_kills(temp_kills);
	game->set_max_towers(temp_tower);
	game->set_toggle(towerClick->blue);

	//display stage name
	tempTimer = new QTimer();
	QString x("Boss Battle");
	connect(tempTimer, SIGNAL(timeout()), this, SLOT(deleteDisplay()));
	disconnect(actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionFirst, SIGNAL(triggered()), this, SLOT(restart()));
	disconnect(actionTurrets, SIGNAL(triggered()), this, SLOT(turrets()));
	disconnect(actionBoss, SIGNAL(triggered()), this, SLOT(boss()));
	setDisplay(x);
	tempTimer->start(1500);
}

void Window::die()
{
	//loads game over screen
	timer->stop();
	delete timer;
	timer = new QTimer;
	int score_ = game->get_score(); //backup score
	delete game;
	game = new Map(4, score_); //load dead scene
	connect(game, SIGNAL(turret_battle()), this, SLOT(turrets()));
	connect(game, SIGNAL(boss_battle()), this, SLOT(boss())); //moves to boss battle
	connect(game, SIGNAL(dead()), this, SLOT(die()));
	connect(game, SIGNAL(restart()), this, SLOT(restart())); //restart game
	connect(game, SIGNAL(end()), qApp, SLOT(quit())); //quit application
	view->setScene(game); //change qgraphicsview's scene
	removeDockWidget(info);
	removeDockWidget(towerInfo);
	delete info; //remake stats
	delete towerInfo; //remake button
	setDockWidgets();
	game->set_toggle(towerClick->blue);
}

void Window::setDisplay(QString x)
{
	display_screen = new QGraphicsScene; //create temporary new graphics scene
	QFont f("Arial", 20, QFont::Bold); //set font
	QGraphicsTextItem *temp = display_screen->addText(x, f); //create graphicstextitem to display stage name
	temp->setPos(250, 360/2); //position center of screen
	view->setScene(display_screen); //display screen in central widget
	view->show();
}

void Window::instructions()
{
	//same as setDisplay, but displays instructions
	display_screen = new QGraphicsScene;
	QString x("Controls:\nArrow Keys - Move\nSpace - Fire\nTo activate towers that block 15 hits for you, click the tower button.\nWhen tower button says True, click on the screen to spawn your tower at that location.\nYou only have 3 towers to use for the whole game\n\nPress CTRL + R to begin playing!");
	QFont f("Arial", 12);
	QGraphicsTextItem *temp = display_screen->addText(x, f);
	temp->setPos(0,0);
	view->setScene(display_screen);
	view->show();
}

void Window::deleteDisplay()
{
	tempTimer->stop(); //stop temp timer 
	display_screen->clear(); //remove items from graphicsscene
	delete display_screen;
	delete tempTimer;
	//reconnect shortcuts
	connect(actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
	connect(actionFirst, SIGNAL(triggered()), this, SLOT(restart()));
	connect(actionTurrets, SIGNAL(triggered()), this, SLOT(turrets()));
	connect(actionBoss, SIGNAL(triggered()), this, SLOT(boss()));

	view->setScene(game); //reload paused map
	timer->start(10); //start game scene
}

void Window::deleteInstructions()
{
	//same as deleteDisplay but used for deleting instructions
	display_screen->clear();
	delete display_screen;
	disconnect(actionRestart, SIGNAL(triggered()), this, SLOT(deleteInstructions())); //reset different shortcut connection
	connect(actionRestart, SIGNAL(triggered()), this, SLOT(restart()));
	connect(actionFirst, SIGNAL(triggered()), this, SLOT(restart()));
	connect(actionTurrets, SIGNAL(triggered()), this, SLOT(turrets()));
	connect(actionBoss, SIGNAL(triggered()), this, SLOT(boss()));

	view->setScene(game);
	timer->start(10);
}

void Window::setMenuBar()
{
	//file menu
	file = new QMenu;
	file = menuBar()->addMenu("&File"); //File Menu
	
	actionRestart = new QAction("&Restart", this); //Restart action
	actionQuit = new QAction("&Quit", this); //Quit action
	
	actionRestart->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R)); //shortcut for actionbutton
	connect(actionRestart, SIGNAL(triggered()), this, SLOT(restart())); //connect to restart scene
	actionQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q)); //shortcut for actionbutton
	connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit())); //connect to quit app
	
	file->addAction(actionRestart); // add action to menu
	file->addAction(actionQuit); // add action to menu
	menuBar()->addMenu(file); //add menu to menubar

	//stages menu
	stages = new QMenu;
	stages = menuBar()->addMenu("&Stages"); //stages menu

	actionFirst = new QAction("&First Stage", this); // first stage
	actionTurrets = new QAction("&Second Stage", this); //skip to turret battle
	actionBoss = new QAction("&Boss", this); //skip to boss battle

	actionFirst->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F)); //shortcut for first stage
	connect(actionFirst, SIGNAL(triggered()), this, SLOT(restart()));
	actionTurrets->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T)); //shortcut for turret battle
	connect(actionTurrets, SIGNAL(triggered()), this, SLOT(turrets()));
	actionBoss->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B)); //shortcut for boss battle
	connect(actionBoss, SIGNAL(triggered()), this, SLOT(boss()));

	//add shortcuts to stages menu
	stages->addAction(actionFirst);
	stages->addAction(actionTurrets);
	stages->addAction(actionBoss);
	menuBar()->addMenu(stages);
}

void Window::setDockWidgets()
{
	//stats widget
	info = new QDockWidget("Stats", this);
	stats = new statsWidget;
	//connect score and lives to the game
	connect(game, SIGNAL(scoreChanged(int)), this, SLOT(update_score(int))); 
	connect(game, SIGNAL(livesChanged(int)), this, SLOT(update_lives(int)));
	connect(game, SIGNAL(killsChanged(int)), this, SLOT(update_kills(int)));
	info->setWidget(stats); //set dockwidget
	addDockWidget(Qt::BottomDockWidgetArea, info); //add dockwidget to bottom of main window

	//click widget
	towerInfo = new QDockWidget("Towers", this);
	towerClick = new clickWidget;
	//connect tower toggle
	connect(towerClick->towerOn, SIGNAL(clicked()), this, SLOT(update_click()));
	connect(towerClick->towerOn, SIGNAL(clicked()), game, SLOT(tower_toggle()));
	towerInfo->setWidget(towerClick);
	addDockWidget(Qt::TopDockWidgetArea, towerInfo); //add dockwidget to top of main window

	//setStyleSheet("QDockWidget#info{subcontrol-position: top left; subcontrol-origin: margin; position: absolute; top: 0px; left: 0px; bottom: 0px; width: 14px;}");
	//file->addAction(info->toggleViewAction());
}

//public slots to update labels in dockwidgets
void Window::update_score(int x)
{
	QString temp;
	(stats->score)->setText(temp.number(x));
}
void Window::update_lives(int x)
{
	QString temp;
	(stats->lives)->setText(temp.number(x));
}
void Window::update_kills(int x)
{
	QString temp;
	(stats->kills)->setText(temp.number(x));
}
void Window::update_click()
{
	towerClick->blue = !(towerClick->blue);
	if(towerClick->blue == true)
	{
		(towerClick->label)->setText("True");
	}
	else
	{
		(towerClick->label)->setText("False");
	}
}
//end Window Class



//Dockwidgets
//statsWidget
statsWidget::statsWidget(QWidget *parent)
     : QWidget(parent)
{
	QFont f("Arial", 14);
	
	//create lives label
	QLabel *label = new QLabel;
	label->setText("Lives");
	f.setBold(true);
	label->setFont(f);
	//create lives number
	lives = new QLabel;
	lives->setText("10");
	f.setPointSize(12);
	f.setBold(false);
	lives->setFont(f);
	//add lives to the layout
	layout = new QGridLayout;
	layout->addWidget(label, 0, 0);
	layout->addWidget(lives, 1, 0);

	//create score label
	label = new QLabel;
	label->setText("Score");
	f.setBold(true);
	f.setPointSize(14);
	label->setFont(f);
	//create score number
	score = new QLabel;
	score->setText("0");
	f.setBold(false);
	f.setPointSize(12);
	score->setFont(f);
	//add score to layout
	layout->addWidget(label, 0, 1);
	layout->addWidget(score, 1, 1);

	//create kills label
	label = new QLabel;
	label->setText("Kills");
	f.setBold(true);
	f.setPointSize(14);
	label->setFont(f);
	//create kills number
	kills = new QLabel;
	kills->setText("0");
	f.setBold(false);
	f.setPointSize(12);
	kills->setFont(f);
	//add kills to layout
	layout->addWidget(label, 0, 2);
	layout->addWidget(kills, 1, 2);

	setLayout(layout);
}

statsWidget::~statsWidget()
{
	delete lives;
	delete score;
	delete kills;
	delete layout;
}
//end StatsWidget


//clickWidget
clickWidget::clickWidget(QWidget *parent) : QWidget(parent)
{
	blue = false; //set initial tower placement to false
	//create pushbutton
	towerOn = new QPushButton("Tower", this);
	towerOn->setFont(QFont("Arial", 10));
    towerOn->setFixedSize(180, 40);
	//create bool label
	label = new QLabel;
	label->setText("False");
	QFont f("Arial", 12);
	f.setBold(true);
	label->setFont(f);
	//make layout
	layout = new QGridLayout;
	layout->addWidget(towerOn, 0, 0);
	layout->addWidget(label, 0, 1);
	//set layout
	setLayout(layout);
}

clickWidget::~clickWidget()
{
	delete towerOn;
	delete label;
	delete layout;
}
//end ClickWidget
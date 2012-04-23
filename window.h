#include "map.h"
#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

class statsWidget : public QWidget //widget customized for dock widget
{
	Q_OBJECT
public:
	statsWidget(QWidget *parent = 0); //constructor
	~statsWidget(); //destructor
	QLabel *score; //label for score number
	QLabel *lives; //label for lives number
	QLabel *kills; //label for kills number
	QGridLayout *layout; //layout of labels
};

class clickWidget : public QWidget //widget for dock widget that holds a push button to activate tower placement
{
	Q_OBJECT
public:
	clickWidget(QWidget *parent = 0); //constructor
	~clickWidget(); //destructor
	QPushButton *towerOn; //pushbutton for activation
	QLabel *label; //label to display if tower placement is active
	bool blue; //bool to store if tower placement is active
	QGridLayout *layout; //layout of buttons and labels
};


class Window : public QMainWindow //mainwindow of the program
{
	Q_OBJECT
private:
	QTimer *timer; //timer to control timeouts
	QTimer *tempTimer;
	Map *game; //graphicsscene for the game
	QGraphicsScene *display_screen; //display text
	QGraphicsView *view; //graphicsview to display the game
	
	QMenu *file; //file menu object
	QAction *actionRestart; //restart game
	QAction *actionQuit; //quit game
	QMenu *stages; //stages menu object
	QAction *actionFirst; //first stage
	QAction *actionTurrets; //second stage
	QAction *actionBoss; //third stage

	QDockWidget *info; //stats widget dock
	QDockWidget *towerInfo; //tower toggle
	statsWidget *stats; //stats widget to add to info
	clickWidget *towerClick; //click widget to add to towerInfo
public:
	Window(); //constructor
	~Window(); //destructor
	void setDisplay(QString x); //function to display a text screen, used for stage names
	void instructions(); //display instructions, used for start of game
	void setMenuBar(); //set top bar with shortcut keys and file and stages
	void setDockWidgets(); //set the two widgets above and below the graphicsscene
public slots:
	void restart(); //restart game, new scene
	void deleteDisplay(); //complements setDisplay function to manage timer and tempTimer and signals
	void deleteInstructions(); //modified version of deleteDisplay to work with instructions
	void die(); //change map to dead_screen
	void boss(); //change map to boss_stage
	void turrets(); //change map to turret_stage
	void update_score(int x); //update score number label in stats widget
	void update_lives(int x); //update lives number label in stats widget
	void update_kills(int x); //update kills number label in stats widget
	void update_click(); //update click label in click widget
};


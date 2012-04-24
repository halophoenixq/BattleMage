/*#################################################################

BattleMage map.h
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

#ifndef MAP_H
#define MAP_H

#include "tracker.h"
#include "turrets.h"
#include <QWidget>
#include <QPointF>
#include <QPoint>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSound>
#include <QString>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <vector>

struct fireball
{
	QGraphicsPixmapItem* item; //image of fireball
	QPointF current; //fireball location
	bool active; //bool to see if fireball is active
};

struct monster //handles all 5 types of monsters
{
	int type; //monster type
	QGraphicsPixmapItem* item; //image of monster
	QPointF current; //monster location
	int health; //monster's hp
	double speed; //monster's speed
	bool active; //spawn variable
	bool dead; //dead variable
	int count; //rounds dead to see how long to display ash balls
};


class Map : public QGraphicsScene
{
	Q_OBJECT

private:
	QPointF current_mage; //mage position
	QGraphicsPixmapItem* character; //mage image item
	bool dead_check; //check if character is dead within the advance function to avoid segnmentation fault

	std::vector<fireball> fireballs; //current fired fireballs
	
	std::vector<bullet*> bullets; //projectiles being shot at player

	std::vector<turret*> turrets; //towers placed on screen to block things

	std::vector<monster> monsters; //current spawned monsters
	
	//itme spawn control
	int fireball_rate; //fireball spawn rate
	int fireball_spawn; //current fireball spawn position
	int monster_rate; //control spawn rate
	int monster_spawn; //current spawn position
	int bullet_rate; //projectile control
	int bullet_spawn;
	int aimer_rate;
	int aimer_spawn;
	int tracker_rate;
	int tracker_spawn; //end projectile control
	int turret_rate; //controls enemy turret teleportation rate
	int turret_move; //current moving state of enemy turret
	bool boss_up; //control when boss is moving up and when boss is moving down

	int max_towers; //keep track of towers spawned and is used to prevent further spawning
	int tower_num; //keep track of how many towers spawned
	
	bool turret_done; //see if second stage has been beaten
	bool toggle; //allow or disallow tower placement

	int scene; //number of which scene map currently is displaying
	int score; //player's score
	int lives; //player's lives
	int kills; //player's kills

	//control multidirectional key movement
	bool KeyUp;
	bool KeyDown;
	bool KeyLeft;
	bool KeyRight;
	bool KeySpace;

	QSound *will; //music

	//text items for the dead screen
	QGraphicsTextItem *game_over;
	QGraphicsTextItem *score_display;
	QGraphicsTextItem *exit_text;
public:
	Map(int x); //constructor for stages
	Map(int x, int score_); //constructor for dead_scene
    ~Map(); //destructor

	//basic useful functions
	void moveUp(QPointF&, double); //move current point right by int amount
	void moveDown(QPointF&, double); //move current point left by int amount
	void moveRight(QPointF&, double); //move current point right by int amount
	void moveLeft(QPointF&, double); //move current point left by int amount
	void setPoint(QPointF&, int, int); //set current point
	void setPoint(QPointF&, double, double); //overloaded function
	void clean(); //create a new scene for game restart
	
	//screen constructors
	void first_stage(); //initializations for first stage
	void second_stage(); //initializations for the second stage
	void third_stage(); //initializations for third stage
	void dead_screen(int); //initializations for dead screen

	//spawners
	void spawn_bullet(); //code to spawn a regular bullet
	void spawn_aimer(); //spawn targetted bullet
	void spawn_tracker(); //spawn tracking bullet
	void spawn_monster(int); //spawn a monster
	void spawn_tower(QPointF); //spawn a tower
	
	void collision(); //collision detection for all items
	void delete_inactive(); //clean up vectors

	void keyinput(); //looks at key bools and moves character/fires fireballs accordingly

	//getters and setters
	QPointF get_current_mage();
	void set_current_mage(QPointF);
	int get_score();
	void set_score(int);
	int get_lives();
	void set_lives(int);
	int get_kills();
	void set_kills(int);
	bool get_toggle();
	void set_toggle(bool);
	int get_max_towers();
	void set_max_towers(int);

public slots:
	void advance(); //update screen
	void tower_toggle(); //change tower placement bool variable
signals:
	void scoreChanged(int value); //score changed
	void livesChanged(int value); //lives changed
	void killsChanged(int value); //kills changed
	void turret_battle(); //advance to turret battle
	void boss_battle(); //advance to boss battle
	void dead(); //character dead, or has won the game
	void restart(); //restart app
	void end(); //quit app
protected:
	void keyPressEvent(QKeyEvent *event); //keyboard input
	void keyReleaseEvent(QKeyEvent *event); //for multikey input
	void mousePressEvent(QGraphicsSceneMouseEvent *event); //mouse click event
};

#endif
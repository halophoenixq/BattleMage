/*#################################################################

BattleMage map.cpp
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
#include <QPainter>
#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QSound>
#include <ctime>
#include <cstdlib>


Map::Map(int x) : QGraphicsScene() //constructor for chosen scene
{
	//set keyinputs to false
	KeyUp = false;
	KeyDown = false;
	KeyLeft = false;
	KeyRight = false;
	KeySpace = false;

	toggle = false; //tower placement false

	//choose correct scene to construct
	if(x == 1)
	{
		first_stage();
	}
	else if (x == 2)
	{
		second_stage();
	}
	else if (x == 3)
	{
		third_stage();
	}
}

Map::Map(int x, int score_) : QGraphicsScene() //constructor for the dead scene
{
	KeyUp = false;
	KeyDown = false;
	KeyLeft = false;
	KeyRight = false;
	KeySpace = false;
	if(x == 4)
	{
		dead_screen(score_);
	}
}

Map::~Map() //destructor
{
	delete will;
}

void Map::advance()
{
	if(scene == 4) //don't advance if in game over screen
	{
		return;
	}

	keyinput(); //update character position based off keyboard input
	if(dead_check == true) //if screen was first made, update the stats
	{
		emit livesChanged(lives);
		emit scoreChanged(score);
		emit killsChanged(kills);
	}

	//update character location in scene
	character->setPos(current_mage);
	dead_check = false; //set dead to false to signal that scene not in first run through anymore

	if(fireball_spawn < fireball_rate) //maintain fireball spawn rate
	{
		fireball_spawn++;
	}

	monster_rate = rand()%1000 + 100; //randomize spawn rate
	
	//spawn bullets
	if(aimer_spawn < aimer_rate) //maintain aimer spawn rate
	{
		aimer_spawn++;
	}
	else
	{
		spawn_aimer();
	}
	if(scene == 3) //if dragon scene, spawn bullet
	{
		if(bullet_spawn < bullet_rate)
		{
			bullet_spawn++;
		}
		else
		{
			spawn_bullet();
		}
		for(int i = 0; i < bullets.size(); i ++)
		{
			if(bullets[i]->get_type() != 3) //find all the tracker bullets
			{
				continue;
			}
			(dynamic_cast<tracker*>(bullets[i]))->set_target(current_mage); //update tracker targetting and slope
		}
		if(tracker_spawn < tracker_rate) //maintain tracker spawn rate
		{
			tracker_spawn++;
		}
		else
		{
			spawn_tracker();
		}
	}

	if (scene == 1) //spawn monsters for scene one
	{
		//monster spawning
		if(monster_spawn < monster_rate) //manage spawn rate
		{
			monster_spawn++;
		}
		else
		{
			//create monster
			spawn_monster(rand()%3+1);
		}
	}

	//fireball movement
	for(int i = 0; i < fireballs.size(); i++)
	{
		if(fireballs[i].active == true) //move fireballs that are active
		{
			if(fireballs[i].current.x() > 640) //inactive fireball that's off screen
			{
				fireballs[i].active = false;
				continue;
			}
			moveRight(fireballs[i].current, 10); //move fireball right
			fireballs[i].item->setPos(fireballs[i].current); //set position on scene
		}
	}
	
	if (scene == 1)
	{
		//monster move
		for(int i = 0; i < monsters.size(); i++)
		{
			if(monsters[i].active == true) //move monsters that are active
			{
				if(monsters[i].current.x() <= -5) //inactivate monster
				{
					monsters[i].active = false;
					/*lives--; //lose lives
					emit livesChanged(lives);
					if(lives <= 0)
					{
						will->stop();
						emit dead(); //send signal if player lost
						dead_check = true;
						return;
					}*/
					continue;
				}
			
				moveLeft(monsters[i].current, monsters[i].speed); //move monster left
				monsters[i].item->setPos(monsters[i].current); //set position on scene
			}
		}
	}
	else if (scene == 2)
	{
		//monster move
		if(turret_move < turret_rate) //control turret movement
		{
			turret_move++;
		}
		else
		{
			for(int i = 0; i < monsters.size(); i++)
			{
				if(monsters[i].active == true) //move monsters that are active
				{
					switch(i)
					{
					case 0:
						{
							setPoint(monsters[i].current, rand()%100 + 452, rand()%(320/3)); //randomly teleport monster
							monsters[i].item->setPos(monsters[i].current); //set position on scene
							break;
						}
					case 1:
						{
							setPoint(monsters[i].current, rand()%100 + 452, rand()%(320/3)+(320/3)); //randomly teleport monster
							monsters[i].item->setPos(monsters[i].current); //set position on scene
							break;
						}
					case 2:
						{
							setPoint(monsters[i].current, rand()%100 + 452, rand()%(320/3)+(640/3)); //randomly teleport monster
							monsters[i].item->setPos(monsters[i].current); //set position on scene
							break;
						}
					}
				}
			}
			turret_move = 0; //reset movement control
		}
	}
	else if (scene == 3 && !monsters.empty()) //move dragon boss
	{
		if(monsters[0].current.y() + 80 == 0) //adjust dragon direction
		{
			boss_up = false;
		}
		else if(monsters[0].current.y() > 80)
		{
			boss_up = true;
		}

		if(boss_up == true) //move up or down according to variable
		{
			moveUp(monsters[0].current, monsters[0].speed);
		}
		else if(boss_up == false)
		{
			moveDown(monsters[0].current, monsters[0].speed);
		}
		monsters[0].item->setPos(monsters[0].current);
	}

	//projectile move
	QGraphicsScene::advance();

	//collision testing
	collision();

	//check lives
	if(dead_check == true)
	{
		return;
	}
	//object removal
	delete_inactive();

	//QGraphicsScene::advance();
	update();

	if(scene == 2 && turret_done == true) //if killed all turrets, advance to boss
	{
		emit boss_battle();
		return;
	}
	if(scene == 1 && kills > 15) //if killed 15 monsters, advance to turret
	{
		emit turret_battle();
	}
}

void Map::first_stage()
{
	scene = 1;
	//delete all items
	clear();
	//play music
	will = new QSound("sounds/will.wav");
	will->play();
	//clear vectors
	monsters.clear();
	fireballs.clear();
	bullets.clear();
	//refresh mage
	setPoint(current_mage, 0, 0);
	QPixmap temp("images/blackmage.png");
	character = addPixmap(temp);
	character->setZValue(1); //make mage load on top of other images
	dead_check = true; //activate dead so if scene was recreated for death, advance function will return
	//reset spawns
	fireball_rate = 20;
	fireball_spawn = 20;
	monster_rate = rand()%500 + 100; //generate random spawn rate
	monster_spawn = 0;
	aimer_rate = rand()%100 + 100;
	aimer_spawn = 0;
	//setup tower control
	max_towers = 0;
	tower_num = 0;
	turret_done = false;
	//reset stats
	score = 0;
	emit scoreChanged(score);
	lives = 10;
	emit livesChanged(lives);
	kills = 0;
	emit killsChanged(kills);
	//reset random seed
	srand(time(NULL));
}

void Map::second_stage()
{
	scene = 2;
	//delete all items
	clear();
	//play music
	will = new QSound("sounds/wings.wav");
	will->play();
	//clear vectors
	monsters.clear();
	fireballs.clear();
	bullets.clear();
	//refresh mage
	setPoint(current_mage, 0, 0);
	QPixmap temp("images/blackmage.png");
	character = addPixmap(temp);
	character->setZValue(1);
	dead_check = true;
	//reset spawns
	fireball_rate = 20;
	fireball_spawn = 20;
	aimer_rate = rand()%20 + 20; //turret bullets
	aimer_spawn = 0;
	turret_rate = 100; //turret movement rate
	turret_move = 0;
	turret_done = false;
	//setup tower control
	max_towers = 0;
	tower_num = 0;
	turret_done = false;
	//reset stats
	score = 0;
	emit scoreChanged(score);
	lives = 10;
	emit livesChanged(lives);
	kills = 0;
	emit killsChanged(kills);
	//spawn 3 turrets
	for(int i = 0; i < 3; i++)
	{
		spawn_monster(5);
	}
	//reset random seed
	srand(time(NULL));
}

void Map::third_stage()
{
	scene = 3;
	//delete all items
	clear();
	//play music
	will = new QSound("sounds/standtall.wav");
	will->play();
	//clear vectors
	monsters.clear();
	fireballs.clear();
	bullets.clear();
	//refresh mage
	setPoint(current_mage, 0, 0);
	QPixmap temp("images/blackmage.png");
	character = addPixmap(temp);
	character->setZValue(1);
	spawn_monster(4);
	dead_check = true;
	boss_up = true; //boss starts by moving up
	//reset spawn
	bullet_rate = 20;
	bullet_spawn = 20;
	fireball_rate = 20;
	fireball_spawn = 20;
	aimer_rate = rand()%100 + 100;
	aimer_spawn = 0;
	tracker_rate = rand()%200 + 500;
	tracker_spawn = 500;
	//setup tower control
	max_towers = 0;
	tower_num = 0;
	turret_done = false;
	//reset stats
	score = 0;
	emit scoreChanged(score);
	lives = 10;
	emit livesChanged(lives);
	kills = 0;
	emit killsChanged(kills);
	//reset random seed
	srand(time(NULL));
}

void Map::dead_screen(int score_)
{
	scene = 4;
	//delete all items
	clear();
	//play music
	will = new QSound("sounds/krone.wav");
	will->play();
	//clear vectors
	monsters.clear();
	fireballs.clear();
	bullets.clear();
	//Game Over
	QFont f("Arial", 14, QFont::Bold);
	game_over = addText("Game Over", f);
	game_over->setPos(280, 360/2 - 40);
	//display Score
	f.setBold(false);
	QString num("Score: ");
	num.append(QString::number(score_));
	score_display = addText(num, f);
	score_display->setPos(280,360/2 - 20);
	//display instructions to continue
	num.clear();
	num.append("Click File to choose options");
    exit_text = addText(num, QFont("Arial", 12, QFont::Bold));
	exit_text->setPos(230, 360/2+10);
	//reset stats
	score = 0;
	emit scoreChanged(score);
	lives = 10;
	emit livesChanged(lives);
	kills = 0;
	emit killsChanged(kills);
}

void Map::moveUp(QPointF& current, double speed) //move things up
{
	current.setY(current.y() - speed);
}

void Map::moveDown(QPointF& current, double speed) //move things down
{
	current.setY(current.y() + speed);
}

void Map::moveRight(QPointF& current, double speed) //move things right
{
	current.setX(current.x() + speed);
}

void Map::moveLeft(QPointF& current, double speed) //move things left
{
	current.setX(current.x() - speed);
}

void Map::setPoint(QPointF& current, int x, int y) //function to set QPointF's easier
{
	current.setX(x);
	current.setY(y);
}

void Map::setPoint(QPointF& current, double x, double y) //overloaded function
{
	current.setX(x);
	current.setY(y);
}

void Map::spawn_bullet()
{
	if(scene == 3) //only boss stage has this bullet
	{
		//randomly generate coordiante
		double x;
		double y;
		int random = rand()%3;
		if(random == 0) //generate coordinate on left wing
		{
			x = rand()%112+3;
			y = 45+1.5*x;
		}
		else if(random == 1) //generate coordinate on right wing
		{
			x = rand()%6 + 260;
			y = 135 + 15*(265-x);
		}
		else if(random == 2) //generate coordinate on tail
		{
			x = rand()%92 + 35;
			y = 340 + 0.75*(35 - x);
		}

		bullet *shot = new bullet; //create bullet
		shot->set_active(true);
		QPointF shot_pos;

		setPoint(shot_pos, monsters[0].current.x() + x, monsters[0].current.y() + y); //create position on wing
		shot->set_speed(2); //set speed
	
		shot->set_current(shot_pos); //set position
	
		addItem(shot); //add bullet to scene
		bullets.push_back(shot); //add bullet to vector
		bullet_spawn = 0; //reset bullet current spawn position
	}
}

void Map::spawn_aimer()
{
	if(scene == 2) //turret small aimers
	{
		for(int i = 0; i < monsters.size(); i++)
		{
			if(monsters[i].active == false) //skip spawning on dead turrets
			{
				continue;
			}
			else
			{
				aimer *shot = new aimer; //new bullet
				shot->set_active(true);
				QPointF shot_pos;
				//color bullet purple gradient
				QRadialGradient rGrad( 0.0, 0.0, 10.0, 0.0, 0.0);
				rGrad.setColorAt(0.0, QColor(255,255,255));
				rGrad.setColorAt(0.5, QColor(197,82,255));
				rGrad.setColorAt(1.0, QColor(143,0,214,0));
				//resize bullet
				shot->setRect(-5.0,-5.0,10.0,10.0);
				shot->setBrush(QBrush(rGrad)); //paint the bullet
				setPoint(shot_pos, monsters[i].current.x() + 31, monsters[i].current.y() + 31); //set bullet position
				shot->set_speed(1); //set bullet speed
		
				shot->set_current(shot_pos); //set position in scene
				shot->set_direction(current_mage); //target mage
	
				addItem(shot); //add bullet to scene
				bullets.push_back(shot); //add bullet to vector
				aimer_spawn = 0; //reset bullet current spawn
			}
		}
	}
	else if(scene == 3 || scene == 1) //dragon's aimer bullet
	{
		aimer *shot = new aimer; //new bullet
		shot->set_active(true);
		QPointF shot_pos;
		if(scene == 3) //set position at dragons mouth
		{
			setPoint(shot_pos, monsters[0].current.x() + 125, monsters[0].current.y() + 180);
			shot->set_speed(5);
		}
		else //randomly generate position for bullet
		{
			setPoint(shot_pos, 640, rand()%320);
			shot->set_speed(3);
		}
		shot->set_current(shot_pos); //set bullet position
		shot->set_direction(current_mage); //target mage
	
		addItem(shot); //add bullet to scene
		bullets.push_back(shot); //add bullet to vector
		aimer_spawn = 0; //reset bullet current spawn position
	}
}

void Map::spawn_tracker()
{
	tracker *shot = new tracker; //new tracker
	shot->set_active(true);
	QPointF shot_pos;

	setPoint(shot_pos, monsters[0].current.x() + 35, monsters[0].current.y() + 356); //set position at dragons tail
	shot->set_speed(.5); //set shot speed
	
	shot->set_current(shot_pos); //set bullet position on screen
	shot->set_target(current_mage); //target mage
	
	addItem(shot); //add bullet to scene
	bullets.push_back(shot); //add bullet to vector
	tracker_spawn = 0; //reset bullet current spawn position
}

void Map::spawn_monster(int type)
{
	monster cute; //create monster
	cute.active = true; //activate
	cute.dead = false; //not dead
	cute.type = type; //choose random monster type
	QPixmap temp; //image to load
	switch(type)
	{
	case 1: //fast mob
		{
			cute.health = 1;
			temp.load("images/monster1");
			cute.speed = 3;
			break;
		}
	case 2: //slow stronger mob
		{
			cute.health = 5;
			temp.load("images/monster2");
			cute.speed = 1;
			break;
		}
	case 3: //tank mob
		{
			cute.health = 20;
			temp.load("images/monster3");
			cute.speed = .5;
			break;
		}
	case 4: //ice dragon 
		{
			cute.health = 200 + rand()%100; //random health from 200-299
			temp.load("images/icedragon");
			cute.speed = .5;
			setPoint(cute.current, 343, 0); //set defined location for dragon to start out
			cute.item = addPixmap(temp); //add pixmap to scene and keep pointer to the pixmap
			cute.item->setPos(cute.current); //set location
			monsters.push_back(cute); //add monster to vector
			return; //exit function
		}
	case 5: //turrets
		{
			cute.health = 30;
			temp.load("images/locust");
			cute.speed = 0;
			setPoint(cute.current, rand()%100 + 452, rand()%320); //spawn in random set area
			cute.item = addPixmap(temp);
			cute.item->setPos(cute.current);
			monsters.push_back(cute);
			monster_spawn = 0;
			return; //exit function

		}
	}

	setPoint(cute.current, 640, rand()%320); //random spawn point
	cute.item = addPixmap(temp); //add pixmap to map and keep track with pointer
	cute.item->setPos(cute.current); //set position
	monsters.push_back(cute); //add monster to vector
	monster_spawn = 0; //reset monster spawn position
}

void Map::spawn_tower(QPointF location)
{
	if(location.x() > 300) //prevent spawning walls past what players can move in
	{
		return;
	}
	turret *tower = new turret; //create tower
	setPoint(location, location.x()-20, location.y()-25); //center the location of where mouse was clicked
	tower->set_current(location); //set tower at location
	QPixmap temp("images/wall");
	tower->set_health(15); //set health
	tower->set_active(true);
	tower->set_item(addPixmap(temp)); //add tower to scene
	tower->get_item()->setPos(tower->get_current()); //set position
	turrets.push_back(tower); //add tower to vector
	tower_num++; //increament tower count
	max_towers++;
}

void Map::collision()
{
	for(int i = 0; i < fireballs.size(); i++) //fireball hit monster
	{
		for(int j = 0; j < monsters.size(); j++ )
		{
			if(monsters[j].active == false) //if monster is dead, pass by
			{
				continue;
			}
			if(fireballs[i].item->collidesWithItem(monsters[j].item)) //if collide with monster
			{
				monsters[j].health--;
				fireballs[i].active = false;
				score++; //add to score
				emit scoreChanged(score);
				
				if(monsters[j].health == 0) //killed monster, set initial dead phase
				{
					fireballs[i].active = false; //set fireball for deletion
					monsters[j].active = false;
					monsters[j].dead = true; //enter monster dead phase
					monsters[j].count = 0;
					QPixmap dead_temp("images/dead.png"); //set monster as ash ball
					monsters[j].item->setPixmap(dead_temp);
					kills++; //increment skills
					emit killsChanged(kills);
				}
			}
		}
	}
	for(int i = 0; i < bullets.size(); i++) //bullets hit character or tower
	{
		if(bullets[i]->collidesWithItem(character))
		{
			bullets[i]->set_active(false); //set bullet for deletion
			lives--; //decrease health
			emit livesChanged(lives);
			if(lives <= 0)
			{
				will->stop();
				emit dead(); //send signal if player lost
				dead_check = true;
				return;
			}
		}
		else
		{
			for(int j = 0; j < turrets.size(); j++) //check if bullets hit towers
			{
				if(bullets[i]->collidesWithItem(turrets[j]->get_item()))
				{
					bullets[i]->set_active(false);
					turrets[j]->set_health(turrets[j]->get_health() - 1);
					if(turrets[j]->get_health() == 0)
					{
						turrets[j]->set_active(false);
					}
					break;
				}
			}
		}		
	}
	for(int i = 0; i < monsters.size(); i++) //if monster hits character
	{
		if(monsters[i].item->collidesWithItem(character))
		{
			monsters[i].active = false;
			monsters[i].dead = true;
			monsters[i].count = 100;
			lives--;
			if(lives <= 0)
			{
				will->stop();
				emit dead(); //send signal if player lost
				dead_check = true;
				return;
			}
		}
		else
		{
			for(int j = 0; j < turrets.size(); j++) //check if monsters hit towers
			{
				if(monsters[i].item->collidesWithItem(turrets[j]->get_item()))
				{
					monsters[i].active = false;
					monsters[i].dead = true;
					monsters[i].count = 100;
					turrets[j]->set_health(turrets[j]->get_health() - 1);
					if(turrets[j]->get_health() == 0)
					{
						turrets[j]->set_active(false);
					}
					break;
				}
			}
		}
	}
}

void Map::delete_inactive()
{
	for(int i = 0; i < fireballs.size(); i++) //remove inactive fireballs
	{
		if(fireballs[i].active == false)
		{
			removeItem(fireballs[i].item);
			fireballs.erase(fireballs.begin()+i);
		}
	}
	for(int i = 0; i < monsters.size(); i++) //remove inactive monsters
	{
		if(monsters[i].dead == true)
		{
			if(monsters[i].count < 100) //leave monster if initial dead phase not over yet
			{
				monsters[i].count++;
			}
			else
			{
				monsters[i].dead = false;
			}
		}
		if(monsters[i].dead == false && monsters[i].active == false) //delete monster
		{
			if(monsters[i].type == 4) //if killed boss, go to gameover screen
			{
				emit dead();
				return;
			}
			removeItem(monsters[i].item);
			monsters.erase(monsters.begin()+i);
		}
	}
	if(scene == 2) //if killed all turrets
	{
		if(monsters.empty())
		{
			turret_done = true;
			return;
		}
	}
	for(int i = 0; i < bullets.size(); i++) //remove inactive bullets
	{
		if(bullets[i]->get_active() == false)
		{
			removeItem(bullets[i]);
			delete bullets[i];
			bullets.erase(bullets.begin()+i);
		}
	}
	for(int i = 0; i < turrets.size(); i++) //remove inactive towers
	{
		if(turrets[i]->get_active() == false)
		{
			removeItem(turrets[i]->get_item());
			delete turrets[i]->get_item();
			turrets.erase(turrets.begin()+i);
		}
	}

}

void Map::keyPressEvent(QKeyEvent *event)
{
 	switch(event->key()) 
	{
  	  	case Qt::Key_Up:
		//case Qt::Key_W:
			KeyUp = true;
			break;
  		case Qt::Key_Down:
		//case Qt::Key_S:
			KeyDown = true;
			break;
		case Qt::Key_Left:
		//case Qt::Key_A:
			KeyLeft = true;
			break;
  		case Qt::Key_Right:
		//case Qt::Key_D:
			KeyRight = true;
			break;
		case Qt::Key_Space:
			KeySpace = true;
			break;
	}
}

void Map::keyReleaseEvent(QKeyEvent *event)
{
  if (!event->isAutoRepeat())
  {
	switch(event->key())
	{
  		case Qt::Key_Up:
			KeyUp = false;
			break;
  		case Qt::Key_Down:
			KeyDown = false;
			break;
		case Qt::Key_Left:
			KeyLeft = false;
			break;
  		case Qt::Key_Right:
			KeyRight = false;
			break;
		case Qt::Key_Space:
			KeySpace = false;
			break;
	}
  }
}

void Map::keyinput()
{
	if(KeyUp == true) //move mage up
	{
		if(current_mage.y() > 1)
		{
			current_mage.setY(current_mage.y() - 3);
		}
	}
	if(KeyDown == true) //move mage down
	{
		if(current_mage.y() < 319)
		{
			current_mage.setY(current_mage.y() + 3);
		}
	}
	if(KeyLeft == true) //move mage left
	{
		if(current_mage.x() > 1)
		{
			current_mage.setX(current_mage.x() - 3);
		}
	}
	if(KeyRight == true) //move mage left
	{
		if(current_mage.x() < 301)
		{
			current_mage.setX(current_mage.x() + 3);
		}
	}
	if(KeySpace == true) //fire fireball
	{
		if(fireball_spawn < fireball_rate)
		{
			fireball_spawn++;
		}
		else
		{
			fireball temp;
			temp.active = true;
			setPoint(temp.current, current_mage.x()+20, current_mage.y() + 10);
			QPixmap image("images/fireball.png");
			temp.item = addPixmap(image);
			temp.item->setPos(temp.current);
			fireballs.push_back(temp);
			fireball_spawn = 0;
		}
	}
	
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(toggle == true) //if allowed to place towers
	{
		if(event->button() != Qt::LeftButton) //only mouse command that works is left click
		{
			return;
		}
	
		else
		{
			if(max_towers < 3) //make sure 3 towers haven't been placed already
			{
				spawn_tower(event->buttonDownScenePos(Qt::LeftButton)); //spawn tower at the mouse location
			}
			if (tower_num > 2) //turn off toggle if too many towers
			{
				toggle = false;
			}
		}
	}
	QGraphicsScene::mousePressEvent(event);
}

void Map::tower_toggle() //change toggle to opposite bool
{
	toggle = !toggle;
}

//getters and setters
QPointF Map::get_current_mage()
{
	return current_mage;
}
void Map::set_current_mage(QPointF x)
{
	current_mage = x;
}
int Map::get_score()
{
	return score;
}
void Map::set_score(int x)
{
	score = x;
	emit scoreChanged(score);
}
int Map::get_lives()
{
	return lives;
}
void Map::set_lives(int x)
{
	lives = x;
	emit livesChanged(lives);
}
int Map::get_kills()
{
	return kills;
}
void Map::set_kills(int x)
{
	kills = x;
	emit killsChanged(kills);
}
bool Map::get_toggle()
{
	return toggle;
}
void Map::set_toggle(bool x)
{
	toggle = x;
}
int Map::get_max_towers()
{
	return max_towers;
}
void Map::set_max_towers(int x)
{
	max_towers = x;
}
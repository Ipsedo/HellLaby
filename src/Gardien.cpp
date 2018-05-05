#include "Gardien.h"
#include "Dijkstra.h"
#include "Chasseur.h"
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <math.h>
#include <algorithm>

Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele) {
	dir = false;
	isProtector = true;
	cpt = (int) 100.f * (float) std::rand() / RAND_MAX;
	downSeuil = 30.f;
	upSeuil = 50.f;
	activeFireBall = false;
	max_life = _life;
}

void Gardien::toucher(){
	_life = std::max(_life-50,0);
	if (_life==0) {
		message ("Mort");
		rester_au_sol();
	}else{
		tomber();
	}
}
void Gardien::regen(int pv){
	_life = std::min(pv+_life,100);
}


std::pair<int, int> randomDir(Environnement* l, std::pair<int, int> pos) {
	std::vector<std::pair<int, int> > dir;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			int newX = pos.first + i;
			int newY = pos.second + j;
			if (!(i == 0 && j == 0) && l->data(newX, newY) != FULL) {
				dir.push_back(std::make_pair(i, j));
			}
		}
	}
	int ran = dir.size() * (float) std::rand() / RAND_MAX;
	return dir[ran];
}

void Gardien::update (void) {

	if (cpt == 0) {
		float score = 0.f;
		for (int i = 1; i < _l->_nguards; i++) {
				int dist = distDij(_l, _l->_guards[i]);
				score += dist;
		}
		score /= _l->_nguards;
		score += 10.f * (float) std::rand() / RAND_MAX; // ajout de 10pts de score random
		if (score < downSeuil)
			isProtector = false;
		if (score > upSeuil)
			isProtector = true;
	}

	bool willUpdate = (int)_x% 10 == (int)(_l->scale/2) && (int)_y%10 == (int)(_l->scale/2);

	if (willUpdate) {
		if (this->isSeing() && !activeFireBall){
			activeFireBall = true;
			float dx = _l->_guards[0]->_x - _x;
			float dy = _l->_guards[0]->_y - _y;
			_angle = -360 * atan2(dy, dx) / (M_PI * 2.) + 90;
			this -> fire(0.);
		}

		int oldX = _x / Environnement::scale;
		int oldY = _y / Environnement::scale;
		((Labyrinthe*) _l)->setdata(oldX, oldY, EMPTY);

		if (isProtector) {
			int dist = distDij(_l, this);
			if (dist < 2 || (float) std::rand() / RAND_MAX < 0.2) {
				// Pret du trésor ou mouvement pas précis pour dijkstra
				auto dir = randomDir(_l, std::make_pair(oldX, oldY));
				dirx = dir.first;
				diry = dir.second;
			} else {
				// Dijkstra
				auto a = dijkstra(_l,this);
				dirx = a.first;
				diry = a.second;
			}
		} else {
			// Aléatoire
			if (_l->data(oldX + dirx, oldY + diry) != EMPTY) {
				auto dir = randomDir(_l, std::make_pair(oldX, oldY));
				dirx = dir.first;
				diry = dir.second;
			}
		}
		 _angle = 360 * atan2(diry, dirx) / (M_PI * 2.) - 90;
		int futureX = oldX + dirx;
		int futureY = oldY + diry;
		((Labyrinthe*) _l)->setdata(futureX, futureY, GARDIEN);
	}
	move(dirx, diry);

	cpt++;
	cpt %= 1000;

}

bool Gardien::move (double dx, double dy) {
	_x += dx;
	_y += dy;

	return true;
}

void Gardien::fire (int angle_vertical) {
	float coeffPrecision = 5.f;
	float precision = (1.f - _life / max_life) * coeffPrecision;

	float angleH;
	float angleV;
	float randomHorizontal = (float) std::rand() / RAND_MAX;
	float randomVertical = (float) std::rand() / RAND_MAX;
	if ((float) std::rand() / RAND_MAX > 0.5) {
		angleH = _angle + precision * randomHorizontal;
	} else {
		angleH = _angle - precision * randomHorizontal;
	}
	if ((float) std::rand() / RAND_MAX > 0.5) {
		angleV = angle_vertical + precision * randomVertical;
	} else {
		angleV = angle_vertical - precision * randomVertical;
	}
	_fb->init(_x, _y , Environnement::scale, angleV, angleH);
}

bool Gardien::process_fireball (float dx, float dy) {
	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb->get_x()) / Environnement::scale;
	float	y = (_y - _fb->get_y()) / Environnement::scale;
	float	dist2 = x*x + y*y;
	// on bouge que dans le vide!
	int currX = (int)((_fb->get_x() + dx) / Environnement::scale);
	int currY = (int)((_fb->get_y() + dy) / Environnement::scale);

	int chasseurX = _l->_guards[0]->_x / Environnement::scale;
	int chasseurY = _l->_guards[0]->_y / Environnement::scale;
	if (chasseurX == currX && chasseurY == currY) {
		Chasseur* c = dynamic_cast<Chasseur*>(_l->_guards[0]);
		c->toucher();
	}

	int currCase = _l->data(currX, currY);
	if (FULL != currCase) {
		// il y a la place.
		return true;
	}

	activeFireBall = false;
	return false;
}

float sign(float f) {
	if (f > 0.f) return 1.f;
	else if (f < 0.f) return -1.f;
	else return 0.f;
}

bool Gardien::isSeing() {
	Mover* toSee = _l->_guards[0];

	int x0 = _x / Environnement::scale;
	int y0 = _y / Environnement::scale;

	int x1 = toSee->_x / Environnement::scale;
	int y1 = toSee->_y / Environnement::scale;

	// https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm
	float dx = x1 - x0;
	float dy = y1 - y0;
	float step;
	if (abs(dx) > abs(dy)) {
		step = abs(dx);
	} else {
		step = abs(dy);
	}
	float x = x0;
	float y = y0;
	float xIncr = dx / step;
	float yIncr = dy / step;
	for (int i = 0; i < step; i++) {
		x += xIncr;
		y += yIncr;
		if (_l->data(x, y) != EMPTY)
			return false;
	}
	return true;
}

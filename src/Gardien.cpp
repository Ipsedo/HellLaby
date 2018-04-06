#include "Gardien.h"
#include "Dijkstra.h"
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <math.h>

Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele) {
	dir = false;
	isProtector = true;
	cpt = (int) 100.f * (float) std::rand() / RAND_MAX;
	downSeuil = 30.f;
	upSeuil = 50.f;
	activeFireBall = false;
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
	if (!activeFireBall && (((float) std::rand()) / RAND_MAX) < 0.1 ){
		activeFireBall = true;
		this -> fire(this->_angle);
	}



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
		int oldX = _x / Environnement::scale;
		int oldY = _y / Environnement::scale;
		((Labyrinthe*) _l)->setdata(oldX, oldY, EMPTY);

		if (isProtector) {
			int dist = distDij(_l, this);
			if (dist < 2) {
				// Aléatoire
				auto dir = randomDir(_l, std::make_pair(oldX, oldY));
				dirx = dir.first;
				diry = dir.second;
			} else {
				// Dijsktra
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
	cpt %= 100;

}

bool Gardien::move (double dx, double dy) {
	_x += dx;
	_y += dy;

	return true;
}

void Gardien::fire (int angle_vertical) {
	_fb->init(_x, _y , Environnement::scale, angle_vertical, _angle);
}

bool Gardien::process_fireball (float dx, float dy) {
	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb->get_x()) / Environnement::scale;
	float	y = (_y - _fb->get_y()) / Environnement::scale;
	float	dist2 = x*x + y*y;
	// on bouge que dans le vide!
	if (EMPTY == _l->data((int)((_fb->get_x() + dx) / Environnement::scale),
	(int)((_fb->get_y() + dy) / Environnement::scale)))
	{
		message ("[MOB] Woooshh ..... %d", (int) dist2);
		// il y a la place.
		return true;
	}
	// collision...
	// calculer la distance maximum en ligne droite.
	//float	dmax2 = (_l->width ())*(_l->width ()) + (_l->height())*(_l->height());
	// faire exploser la boule de feu avec un bruit fonction de la distance.
	//_wall_hit -> play (1. - dist2/dmax2);
	activeFireBall = false;
	message ("[MOB] Booom...");
	return false;
}

bool Gariden::isSeing() {
	Mover* toSee = _l->_guards[0];
	/*dx = x2 - x1
dy = y2 - y1
for x from x1 to x2 {
  y = y1 + dy * (x - x1) / dx
  plot(x, y)
}*/
	int currX = _x / Environnement::scale;
	int currY = _y / Environnement::scale;

	int targetX = toSee->_x / Environnement::scale;
	int targetY = toSee->_y / Environnement::scale;

	int dx = targetX - currX;
	int dy = targetY - currY;

	for (int i = currX; i < targetX; i++) {
		int y = currY + dy * (i - currX) / dx;
	}
	return false;
}

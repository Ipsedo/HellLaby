#include "Gardien.h"
#include "Dijkstra.h"
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

		message("%d",this->isSeing());
		if (this->isSeing()){
			activeFireBall = true;
			float dx = _l->_guards[0]->_x - _x;
			float dy = _l->_guards[0]->_y - _y;
			_angle = 360 * atan2(dy, dx) / (M_PI * 2.) - 90;
			this -> fire(0.);
		}

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
	cpt %= 1000;

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
	if (FULL != _l->data((int)((_fb->get_x() + dx) / Environnement::scale),
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
	message ("[MOB] Booom...%d,%d", (int)((_fb->get_x() + dx) / Environnement::scale),
	(int)((_fb->get_y() + dy) / Environnement::scale));
	return false;
}

bool Gardien::plotLineLow(int x0, int y0, int x1, int y1) {
  int dx = x1 - x0;
  int dy = y1 - y0;
  int yi = 1;
  if (dy < 0) {
    yi = -1;
    dy = -dy;
  }
  int D = 2*dy - dx;
  int y = y0;

	if (x0 < x1) {
  	for (int x = x0; x < x1; x++) {
			if (_l->data(x, y) != EMPTY)
				return false;
    	if (D > 0) {
       	y = y + yi;
       	D = D - 2*dx;
    	}
    	D = D + 2*dy;
		}
	} else {
		for (int x = x0; x > x1; x--) {
			if (_l->data(x, y) != EMPTY)
				return false;
    	if (D > 0) {
       	y = y + yi;
       	D = D - 2*dx;
    	}
    	D = D + 2*dy;
		}
	}
	return true;
}

bool Gardien::plotLineHigh(int x0, int y0, int x1, int y1) {
	int dx = x1 - x0;
  int dy = y1 - y0;
  int xi = 1;
  if (dx < 0) {
    xi = -1;
    dx = -dx;
  }
  int D = 2*dx - dy;
  int x = x0;

	if (y0 < y1) {
  	for (int y = y0; y < y1; y++) {
			if (_l->data(x, y) != EMPTY)
				return false;
    	if (D > 0) {
       	x = x + xi;
       	D = D - 2*dy;
    	}
    	D = D + 2*dx;
		}
	} else {
		for (int y = y0; y > y1; y--) {
			if (_l->data(x, y) != EMPTY)
				return false;
    	if (D > 0) {
       	x = x + xi;
       	D = D - 2*dy;
    	}
    	D = D + 2*dx;
		}
	}
	return true;
}

bool Gardien::isSeing() {
	Mover* toSee = _l->_guards[0];
	/*
	https://en.wikipedia.org/wiki/Line_drawing_algorithm
	dx = x2 - x1
	dy = y2 - y1
	for x from x1 to x2 {
  	y = y1 + dy * (x - x1) / dx
  	plot(x, y)
	}
	*/
	int x0 = _x / Environnement::scale;
	int y0 = _y / Environnement::scale;

	int x1 = toSee->_x / Environnement::scale;
	int y1 = toSee->_y / Environnement::scale;

	/*int dx = targetX - currX;
	int dy = targetY - currY;

	if (currX < targetX) {
		for (int i = currX; i < targetX; i++) {
			int y = currY + dy * (i - currX) / dx;
			if (_l->data(i, y) != EMPTY)
				return false;
		}
	} else {
		for (int i = currX; i > targetX; i--) {
			int y = currY + dy * (i - currX) / dx;
			if (_l->data(i, y) != EMPTY)
				return false;
		}
	}*/
  if (abs(y1 - y0) < abs(x1 - x0)) {
    if (x0 > x1) {
      return plotLineLow(x1, y1, x0, y0);
    } else {
      return plotLineLow(x0, y0, x1, y1);
    }
  } else {
    if (y0 > y1) {
      return plotLineHigh(x1, y1, x0, y0);
    } else {
      return plotLineHigh(x0, y0, x1, y1);
    }
  }
}

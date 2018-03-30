#include "Gardien.h"
#include "Dijkstra.h"
#include <cstdlib>
#include <iostream>
#include <math.h>

Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele) {
	dir = false;
	haveHitWall = true; // On veut calculer une première fois l'angle de déplacement
	isProtector = true;
	cpt = (int) 99.f * (float) std::rand() / RAND_MAX;
	downSeuil = 3.f;
	upSeuil = 5.f;
}

void Gardien::update (void) {

	// if (cpt == 0) {
	// 	float score = 0.f;
	// 	for (int i = 1; i < _l->_nguards; i++) {
	// 			score += distDij(_l, _l->_guards[i]);
	// 	}
	// 	score /= _l->_nguards;
	// 	score += 10.f * std::rand() / RAND_MAX; // ajout de 10pts de score random
	// 	if (score < downSeuil * _l->_nguards)
	// 		isProtector = false;
	// 	if (score > upSeuil * _l->_nguards)
	// 		isProtector = true;
	// }
	//
	// if(!dir){
	// 	bool willUpdate = (int)_x% 10 == (int)(_l->scale/2) && (int)_y%10 == (int)(_l->scale/2);
	// 	if(isProtector && willUpdate){
	// 		// Le gardien est en mode protection
	// 		// TODO stopper le mouvement si on est proche du trésor
	// 		auto a = dijkstra(_l,this);
	// 		dirx = a.first;
	// 		diry = a.second;
	// 		_angle = 360 * atan2(diry, dirx) / (M_PI * 2.) - 90;
	// 		if(dirx == 0 && diry == 0) dir = true;
	// 	} else if (!isProtector && haveHitWall) {
	// 		// TODO faire comportement quand guardien "voit" chasseur
	// 		double randAngle = 2. * M_PI * (double) std::rand() / RAND_MAX;
	// 		dirx = cos(randAngle);
	// 		diry = sin(randAngle);
	// 		_angle = 360 * randAngle / (M_PI * 2.) - 90.;
	// 		haveHitWall = false;
	// 	}
	// 	move(dirx, diry);
	// }
	// cpt++;
	// cpt %= 100;
	bool willUpdate = (int)_x% 10 == (int)(_l->scale/2) && (int)_y%10 == (int)(_l->scale/2);
	if (willUpdate) {
		auto a = dijkstra(_l,this);
			dirx = a.first;
			diry = a.second;
	}
	move(dirx, diry);


}

bool Gardien::move (double dx, double dy) {
	// ((Labyrinthe*)_l)->setdata((int) _x / _l->scale,(int) _y / _l->scale, EMPTY);
	std::cout << dx << " " << dy << '\n';
	float x = _x + dx;
	float y = _y + dy;
	int posX = (int) x / _l->scale;
	int posY = (int) y / _l->scale;

	char tmp = _l->data(posX, posY);
	// ((Labyrinthe*) _l)->setdata((int) _x / _l->scale,(int) _y / _l->scale, GARDIEN);
	if (tmp != EMPTY) {
		// haveHitWall = true;
		return false;
	}

	_x = x;
	_y = y;


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
	message ("[MOB] Booom...");
	return false;
}

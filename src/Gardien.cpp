#include "Gardien.h"
#include "Dijkstra.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>

Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele) {
	_l = l;
	std::srand(std::time(nullptr));
}

void Gardien::update (void) {
	// double dx = (double) 2. * std::rand() / RAND_MAX - 1.;
	// double dy = (double) 2. * std::rand() / RAND_MAX - 1.;
	// _angle++;



	double a = 2. * M_PI * _angle / 360. + 90.;
	double dx = cos(a);
	double dy = sin(a);

	float x = _x + dx;
	float y = _y + dy;
	int oldX = _x / _l->scale;
	int oldY = _y / _l->scale;
	int posX = (int) x / _l->scale;
	int posY = (int) y / _l->scale;
	if ((oldX != posX || oldY != posY) && _l->data(posX, posY) == FULL)
		_angle = 360. * (double) std::rand() / RAND_MAX;
	else
		this->move(dx, dy);
	if (0.01 > (double) std::rand() / RAND_MAX)
		fire(0.);
}

bool Gardien::move (double dx, double dy) {
	/*float x = _x + dx;
	float y = _y + dy;
	int oldX = _x / _l->scale;
	int oldY = _y / _l->scale;
	int posX = (int) x / _l->scale;
	int posY = (int) y / _l->scale;
	if ((oldX != posX || oldY != posY) && _l->data(posX, posY) == FULL)
		return false;
	_x = x;
	_y = y;*/
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
	message ("[MOB] Booom...");
	return false;
}

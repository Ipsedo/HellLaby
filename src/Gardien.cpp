#include "Gardien.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>

Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele) {
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

}

bool Gardien::process_fireball (float dx, float dy) {
	return false;
}

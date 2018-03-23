#include "Gardien.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

Gardien::Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele) {
	std::srand(std::time(nullptr));
}

void Gardien::update (void) {
	double dx = (double) 0.5 * std::rand() / RAND_MAX - 0.25;
	double dy = (double) 0.5 * std::rand() / RAND_MAX - 0.25;
	this->move(dx, dy);
}

bool Gardien::move (double dx, double dy) {
	double x = _x + dx * (double) _l->scale;
	double y = _y + dy * (double) _l->scale;
	int posX = x / _l->scale;
	int posY = y / _l->scale;
	std::cout << posX << " " << posY << std::endl;
	if (_l->data(posX, posY) == 1)
		return false;
	_x = x;
	_y = y;
	return true;
}

void Gardien::fire (int angle_vertical) {

}

bool Gardien::process_fireball (float dx, float dy) {
	return false;
}
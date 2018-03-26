#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"

class Labyrinthe;

class Gardien : public Mover {
private:
	bool dir;
	int dirx;
	int diry;
	bool isProtector;
public:
	Gardien (Labyrinthe* l, const char* modele);/* : Mover (120, 80, l, modele)
	{}*/

	// mon gardien pense tr�s mal!
	void update (void);// {};
	// et ne bouge pas!
	bool move (double dx, double dy);// { return false; }
	// ne sait pas tirer sur un ennemi.
	void fire (int angle_vertical);// {}
	// quand a faire bouger la boule de feu...
	bool process_fireball (float dx, float dy);// { return false; }
};

#endif

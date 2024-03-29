#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"

class Labyrinthe;

class Gardien : public Mover {
private:
	bool dir;
	double dirx;
	double diry;
	bool isProtector;
	float downSeuil;
	float upSeuil;
	int cpt;
	bool haveHitWall;
	bool activeFireBall;
	int max_life;
	bool dead;
public:
	Gardien (Labyrinthe* l, const char* modele);/* : Mover (120, 80, l, modele)
	{}*/

	void toucher();
	void regen(int pv);
	// mon gardien pense tr�s mal!
	void update (void);// {};
	// et ne bouge pas!
	bool move (double dx, double dy);// { return false; }
	// ne sait pas tirer sur un ennemi.
	void fire (int angle_vertical);// {}
	// quand a faire bouger la boule de feu...
	bool process_fireball (float dx, float dy);// { return false; }

	bool isSeing();
};

#endif

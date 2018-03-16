#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"

class Labyrinthe : public Environnement {
private:

public:
	Labyrinthe (char*);
	int width ();	// retourne la largeur du labyrinthe.
	int height ();	// retourne la longueur du labyrinthe.
	char data (int i, int j);
};

#endif
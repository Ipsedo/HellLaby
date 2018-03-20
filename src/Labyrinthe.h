#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"


class Labyrinthe : public Environnement {
private:
	int lab_w;
	int lab_h;
	char**	_data;

public:
	Labyrinthe (char*);
	int width () { return lab_w;}	// retourne la largeur du labyrinthe.
	int height () { return lab_h;}	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return _data [i][j];
	}	// retourne la case (i, j).
};

#endif

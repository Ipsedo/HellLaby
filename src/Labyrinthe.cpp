#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"

Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.

Environnement* Environnement::init (char* filename)
{
	return new Labyrinthe (filename);
}

Labyrinthe::Labyrinthe (char* filename)
{
	_walls = new Wall [1];
	_nwall = 1;
	_walls[0] = {0, 0, 0, 10, 0};

	_picts = new Wall [1];
	_npicts = 0;
	_picts[0] = _walls[0];

	_boxes = new Box [1];		// les caisses.
	_nboxes = 1;	// leur nombre.
	_boxes[0] = {0, 0, 0};

	_treasor._x = _treasor._y = 0;	// le trésor.

	_guards = new Mover* [1];	// les gardes (tableau de pointeurs de gardiens).
	_guards[0] = new Chasseur(this);
	_nguards = 1;
}

int Labyrinthe::width () { return 1;}	// retourne la largeur du labyrinthe.
int Labyrinthe::height () { return 10;}	// retourne la longueur du labyrinthe.
char Labyrinthe::data (int i, int j)
{
		return 0;
}
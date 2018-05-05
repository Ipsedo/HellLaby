#include "Chasseur.h"

using namespace std;
/*
 *	Tente un deplacement.
 */

bool Chasseur::move_aux (double dx, double dy)
{
	/*if (EMPTY == _l -> data ((int)((_x + dx) / Environnement::scale),
							 (int)((_y + dy) / Environnement::scale)))
	{
		_x += dx;
		_y += dy;
		return true;
	}*/
	int posX = (_x + dx) / Environnement::scale;
	int posY = (_y + dy) / Environnement::scale;
	int oldX = _x / Environnement::scale;
	int oldY = _y / Environnement::scale;
	if ((posX != oldX || posY != oldY) && _l->data(posX, posY) == FULL)
		return false;

	_x += dx;
	_y += dy;
	return true;
}

/*
 *	Constructeur.
 */

Chasseur::Chasseur (Labyrinthe* l) : Mover (100, 80, l, 0)
{
	_hunter_fire = new Sound ("sons/hunter_fire.wav");
	_hunter_hit = new Sound ("sons/hunter_hit.wav");
	if (_wall_hit == 0)
		_wall_hit = new Sound ("sons/hit_wall.wav");
}

void Chasseur::toucher(){
	_life = max(_life-25,0);
	message ("Your life : %d",_life);
	if (_life==0) {
		message ("Loss");
	}
}
void Chasseur::regen(int pv){
	_life = min(pv+_life,100);
	message ("Your life : %d",_life);
}

/*
*	Fait bouger la boule de feu (ceci est une exemple, � vous de traiter les collisions sp�cifiques...)
*/
bool Chasseur::process_fireball (float dx, float dy)
{
	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb -> get_x ()) / Environnement::scale;
	float	y = (_y - _fb -> get_y ()) / Environnement::scale;
	float	dist2 = x*x + y*y;
	// on bouge que dans le vide!
	if (EMPTY == _l -> data ((int)((_fb -> get_x () + dx) / Environnement::scale),
							 (int)((_fb -> get_y () + dy) / Environnement::scale)))
	{
		message ("Woooshh ..... %d", (int) dist2);
		// il y a la place.
		return true;
	}
	// collision...
	// calculer la distance maximum en ligne droite.
	float	dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
	// faire exploser la boule de feu avec un bruit fonction de la distance.
	_wall_hit -> play (1. - dist2/dmax2);
	message ("Booom...");
	return false;
}

/*
 *	Tire sur un ennemi.
 */

void Chasseur::fire (int angle_vertical)
{
	message ("Woooshh...");
	_hunter_fire -> play ();
	_fb -> init (/* position initiale de la boule */ _x, _y, 10.,
				 /* angles de vis�e */ angle_vertical, _angle);
}

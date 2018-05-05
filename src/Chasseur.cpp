#include "Chasseur.h"
#include "Environnement.h"

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
	no_move = 0;
	max_life = _life;
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
		partie_terminee(false);
	}
}
void Chasseur::regen(int pv){
	_life = min(pv+_life,100);
	message ("Your life : %d",_life);
}

void Chasseur::update (void){
	std::cout << "la" << '\n';
	no_move++;
	if(no_move == 100){
		std::cout << "ici" << '\n';
		regen(100);
		no_move=0;
	}
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
		// il y a la place.
		return true;
	}
	// collision...
	// calculer la distance maximum en ligne droite.
	float	dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
	// faire exploser la boule de feu avec un bruit fonction de la distance.
	_wall_hit -> play (1. - dist2/dmax2);
	return false;
}

/*
 *	Tire sur un ennemi.
 */

void Chasseur::fire (int angle_vertical)
{
	_hunter_fire -> play ();
	float coeffPrecision = 5.f;
	float precision = (1.f - _life / max_life) * coeffPrecision;

	float angleH;
	float angleV;
	float randomHorizontal = (float) std::rand() / RAND_MAX;
	float randomVertical = (float) std::rand() / RAND_MAX;
	if ((float) std::rand() / RAND_MAX > 0.5) {
		angleH = _angle + precision * randomHorizontal;
		angleV = angle_vertical + precision * randomVertical;
	} else {
		angleH = _angle - precision * randomHorizontal;
		angleV = angle_vertical - precision * randomVertical;
	}

	_fb->init (_x, _y, 10., angleV, angleH);
}

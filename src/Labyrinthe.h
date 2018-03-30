#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include <vector>
#include <map>
#include <string>
#include "Environnement.h"
#include "Mover.h"


class Labyrinthe : public Environnement {
private:
	int lab_w;
	int lab_h;
	std::vector<Wall> wall;
	std::vector<Box> box;
	std::vector<Mover*> guards;
	std::map<char, std::string> pic;
	std::vector<std::vector<char> > _data;
	std::vector<Wall> affiche;

public:
	Labyrinthe (char*);
	int width () { return lab_w;}	// retourne la largeur du labyrinthe.
	int height () { return lab_h;}	// retourne la longueur du labyrinthe.
	void setdata(int i,int j, char c) {this->_data[i][j] = c;}
	char data (int i, int j)
	{
		return _data[i][j];
		// int isFull = _data [i][j];
		// if (isFull == FULL)
		// 	return isFull;
		// for (auto m : guards) {
		// 	int x = m->_x / scale;
		// 	int y = m->_y / scale;
		// 	if (x == i && y == j) {
		// 		return FULL;
		// 	}
		// }
		// return EMPTY;
	}	// retourne la case (i, j).
};

#endif

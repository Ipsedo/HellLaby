#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;


Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.

Environnement* Environnement::init (char* filename)
{
	return new Labyrinthe (filename);
}


Labyrinthe::Labyrinthe (char* filename)
{


 	int w = 0;
	int h = 0;
	bool vu;
	int tmpi = 0;
	int y = 0;
	float chasx = 0.;
	float chasy = 0.;

	_nwall = 0;
	_nboxes = 0;
	_nguards = 1;

	static vector<Wall> wall;
	static vector<Box> box;
	static vector<Mover*> guards;
	guards.push_back(new Chasseur (this));



	vector<bool> tmpB;
	vector<int> tmpy;
	tmpB.push_back(false);
	tmpy.push_back(0);

	ifstream file(filename);
	string linebuffer;


	_data = new char*[1000];
	for (int i = 0; i < 1000; ++i){
		_data[i] = new char[1000];
		for (int j = 0; j < 1000; ++j)
			_data [i][j] = EMPTY;
	}

	bool start = false;

	while (file && getline(file, linebuffer)){

		if(!start){
			if(linebuffer[linebuffer.find_first_not_of(' ')] != '+'){
				continue;
			}else{
				start = true;
			}
		}
		// vector<char> tmpvec;
		h = max(h,(int)linebuffer.length());
		w++;
		const char *cstr = linebuffer.c_str();
		vu = false;
		for (int i = 0; i < (int)strlen(cstr); i++) {
			if(cstr[i] == '#')break;

			if(i+2>(int)(sizeof(tmpB)/sizeof(tmpB[0]))){
					tmpB.push_back(false);
					tmpy.push_back(0);
			}

			if (vu && cstr[i] == '+') {
				// cout << tmpi << " " << y << " " << i << " " << y << '\n';
				Wall tmp = {y,tmpi,y,i,0};
				wall.push_back(tmp);
				_nwall++;
			}

			if (tmpB[i] && cstr[i] == '+') {
				// cout << i << " " << tmpy[i] << " " << i << " " << y << '\n';
				Wall tmp = {tmpy[i],i,y,i,0};
				wall.push_back(tmp);
				_nwall++;
			}


			switch(cstr[i]) {

				case ' ' : {
					vu = false;
					tmpB[i] = false;
					break;
				}

				case '-' : {
					_data[y][i] = 1;
					break;
				}

				case '|' : {
					_data[y][i] = 1;
					break;
				}


    		case '+' : {
					vu = true;
					tmpB[i] = true;
					tmpi = i;
					tmpy[i] = y;
					_data[y][i] = 1;
					break;
				}

			 case 'x' : {
			 	 Box tmpbox = {y,i,0};
				 box.push_back(tmpbox);
				 _nboxes++;
				 _data[y][i] = 1;
				 break;
			}

				case 'T' : {
					_treasor._x = y;
					_treasor._y = i;
					_data[y][i] = 1;
					break;
				}

				case 'C' : {
					chasx = y*scale+(scale/2);
					chasy = i*scale+(scale/2);
					break;
				}


				case 'G' : {
					Mover* tmpmove = new Gardien (this, "Marvin");
					tmpmove -> _x = y*scale+(scale/2);
					tmpmove -> _y = i*scale+(scale/2);
					guards.push_back(tmpmove);
					_nguards++;
					_data[y][i] = 1;
					break;
				}

				default:
				 				 break;

			}

		}
		y++;
	}

	lab_h = h;
	lab_w = w;
	_walls = wall.data();
	_npicts = 0;
	_picts = new Wall [0];
	_boxes = box.data();
	_guards = guards.data();
	_guards[0] -> _x = chasx;
	_guards[0] -> _y = chasy;

	static Wall affiche [] = {
		{ 4, 0, 6, 0, 0 },		// premi�re affiche.
		{ 8, 0, 10, 0, 0 },		// autre affiche.
	};

	_npicts = 2;
	_picts = affiche;

	// char	tmp [128];
	// sprintf (tmp, "%s/%s", texture_dir, "voiture.jpg");
	// _picts [1]._ntex = wall_texture (tmp);

}

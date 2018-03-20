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
	float chasx = 0.;
	float chasy = 0.;
	_nwall = 0;
	static vector<Wall> wall;

	vector<bool> tmpB;
	vector<int> tmpy;
	tmpB.push_back(false);
	tmpy.push_back(0);

	ifstream file(filename);
	string linebuffer;

	bool vu;
	int tmpi = 0;
	int y = 0;


	// static vector<char*> data;

	_data = new char*[1000];
	for (int i = 0; i < 1000; ++i){
		_data[i] = new char[1000];
		for (int j = 0; j < 1000; ++j)
			_data [i][j] = EMPTY;
	}

	while (file && getline(file, linebuffer)){
		// vector<char> tmpvec;
		h = max(h,(int)linebuffer.length());
		w++;
		const char *cstr = linebuffer.c_str();
		vu = false;
		for (int i = 0; i < (int)strlen(cstr); i++) {

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
				case '-' : _data[y][i] = 1;
									 // tmpvec.push_back(1);
									 break;

				case '|' : _data[y][i] = 1;
									 // tmpvec.push_back(1);
									 break;

    		case '+' : vu = true;
									 tmpB[i] = true;
									 tmpi = i;
									 tmpy[i] = y;
									 _data[y][i] = 1;
									 // tmpvec.push_back(1);
									 break;

  			case ' ' : vu = false;
									 tmpB[i] = false;
									 // tmpvec.push_back(EMPTY);
									 break;

				case 'T' : _treasor._x = y;
									 _treasor._y = i;
									 _data[y][i] = 1;
									 // tmpvec.push_back(1);
									 break;

				case 'C' : chasx = (i*10.)+5.;
									 chasy = (y*10.)+5.;
									 // tmpvec.push_back(EMPTY);
									 break;

			}

		}
		// data.push_back(tmpvec.data());
		y++;
	}


	lab_h = h;
	lab_w = w;

	// _data = new char*[lab_w];
	// for (int i = 0; i < lab_w; ++i){
	// 	_data[i] = new char[lab_h];
	// 	for (int j = 0; j < lab_h; ++j) {
	// 		if (i == 0 || i == lab_w-1 || j == 0 || j == lab_h-1)
	// 			_data [i][j] = 1;
	// 		else
	// 			_data [i][j] = EMPTY;
	// 	}
	// }

	// _data = data.data();
	_walls = wall.data();
	_npicts = 0;
	_picts = new Wall [0];
	_nboxes = 0;
	_boxes = new Box [0];
	//_data [_treasor._x][_treasor._y] = 1;
	_nguards = 0;
	_guards = new Mover* [_nguards];
	_guards [0] = new Chasseur (this);
	_guards [0] -> _x = chasx;
	_guards [0] -> _y = chasy;

}

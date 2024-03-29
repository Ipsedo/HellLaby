#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#include "Dijkstra.h"
#include <stdio.h>
#include <string.h>
#include <ctime>
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

	std::srand(std::time(nullptr));

	bool vu;
	int tmpi = 0;
	int y = 0;
	float chasx = 0.;
	float chasy = 0.;
	lab_h = 0;
	lab_w = 0;


	_nwall = 0;
	_nboxes = 0;
	_nguards = 1;
	_npicts = 0;

	guards.push_back(new Chasseur (this));


	vector<bool> tmpB;
	vector<int> tmpy;
	tmpB.push_back(false);
	tmpy.push_back(0);

	ifstream file(filename);
	string linebuffer;


	bool start = false;

	while (file && getline(file, linebuffer)){
		if(linebuffer[linebuffer.find_first_not_of(' ')] == '#' ||
		 	 linebuffer.length() == 0) continue;
		if(!start){
			if(linebuffer[linebuffer.find_first_not_of(' ')] != '+'){

				stringstream ss(linebuffer);
				string item;
				getline(ss, item, ' ');
				char key = item[0];
				getline(ss, item, ' ');
				pic[key] = item;
				continue;
			}else{
				start = true;
			}
		}
		vector<char> tmpvec;
		lab_h = max(lab_h,(int)linebuffer.length());
		lab_w++;
		const char *cstr = linebuffer.c_str();
		vu = false;
		for (int i = 0; i < (int)strlen(cstr); i++) {
			if(cstr[i] == '#')break;

			if(i > tmpB.size()){
					tmpB.push_back(false);
					tmpy.push_back(0);
			}

			if (vu && cstr[i] == '+') {
				Wall tmp = {y,tmpi,y,i,0};
				wall.push_back(tmp);
				_nwall++;
			}

			if (tmpB[i] && cstr[i] == '+') {
				Wall tmp = {tmpy[i],i,y,i,0};
				wall.push_back(tmp);
				_nwall++;
			}


			switch(cstr[i]) {

				case ' ' : {
					vu = false;
					tmpB[i] = false;
					tmpvec.push_back(EMPTY);
					break;
				}

				case '-' : {
					vu = true;
					tmpvec.push_back(FULL);
					break;
				}

				case '|' : {
					vu = false;
					tmpvec.push_back(FULL);
					break;
				}


    		case '+' : {
					vu = true;
					tmpB[i] = true;
					tmpi = i;
					tmpy[i] = y;
					tmpvec.push_back(FULL);
					break;
				}

			 case 'x' : {
				 vu = false;
			 	 Box tmpbox = {y,i,0};
				 box.push_back(tmpbox);
				 _nboxes++;
				 tmpvec.push_back(FULL);
				 break;
			}

				case 'T' : {
					vu = false;
					_treasor._x = y;
					_treasor._y = i;
					tmpvec.push_back(FULL);
					break;
				}

				case 'C' : {
					vu = false;
					chasx = y*scale+(scale/2);
					chasy = i*scale+(scale/2);
					tmpvec.push_back(EMPTY);
					break;
				}


				case 'G' : {
					vu = false;
					Mover* tmpmove = new Gardien (this, "Marvin");
					tmpmove -> _x = y*scale+(scale/2);
					tmpmove -> _y = i*scale+(scale/2);
					guards.push_back(tmpmove);
					_nguards++;
					tmpvec.push_back(EMPTY);
					break;
				}

				default:
					Wall tmpwall;
					if (vu) {
						tmpwall = {y,i,y,i+1,0};
					}else{
						tmpwall = {y,i,y+1,i,0};
					}

					char	tmp [128];
					sprintf (tmp, "%s/%s", texture_dir, pic[cstr[i]].c_str());
					tmpwall._ntex = wall_texture(tmp);
					affiche.push_back(tmpwall);
					_npicts++;
					tmpvec.push_back(FULL);
				 	break;

			}

		}
		_data.push_back(tmpvec);
		y++;
	}


	_walls = wall.data();
	_boxes = box.data();
	_guards = guards.data();
	_guards[0] -> _x = chasx;
	_guards[0] -> _y = chasy;
	_picts = affiche.data();




}

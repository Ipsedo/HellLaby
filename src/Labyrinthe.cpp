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
	bool vupic;
	int tmpi = 0;
	int y = 0;
	float chasx = 0.;
	float chasy = 0.;


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
		h = max(h,(int)linebuffer.length());
		w++;
		const char *cstr = linebuffer.c_str();
		vu = false;
		vupic = false;
		for (int i = 0; i < (int)strlen(cstr); i++) {
			if(cstr[i] == '#')break;

			if(i+2>(int)(sizeof(tmpB)/sizeof(tmpB[0]))){
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
					vupic = false;
					tmpB[i] = false;
					tmpvec.push_back(EMPTY);
					break;
				}

				case '-' : {
					vupic = true;
					tmpvec.push_back(1);
					break;
				}

				case '|' : {
					vupic = false;
					tmpvec.push_back(1);
					break;
				}


    		case '+' : {
					vupic = true;
					vu = true;
					tmpB[i] = true;
					tmpi = i;
					tmpy[i] = y;
					tmpvec.push_back(1);
					break;
				}

			 case 'x' : {
				 vupic = false;
			 	 Box tmpbox = {y,i,0};
				 box.push_back(tmpbox);
				 _nboxes++;
				 tmpvec.push_back(1);
				 break;
			}

				case 'T' : {
					vupic = false;
					_treasor._x = y;
					_treasor._y = i;
					tmpvec.push_back(1);
					break;
				}

				case 'C' : {
					vupic = false;
					chasx = y*scale+(scale/2);
					chasy = i*scale+(scale/2);
					tmpvec.push_back(EMPTY);
					break;
				}


				case 'G' : {
					vupic = false;
					Mover* tmpmove = new Gardien (this, "Marvin");
					tmpmove -> _x = y*scale+(scale/2);
					tmpmove -> _y = i*scale+(scale/2);
					guards.push_back(tmpmove);
					_nguards++;
					tmpvec.push_back(1);
					break;
				}

				default:
					Wall tmpwall;
					if (vupic) {
						tmpwall = {y,i,y,i+2,0};
					}else{
						tmpwall = {y,i,y+2,i,0};
					}
					vupic = true;
					char	tmp [128];
					sprintf (tmp, "%s/%s", texture_dir, pic[cstr[i]].c_str());
					tmpwall._ntex = wall_texture(tmp);
					affiche.push_back(tmpwall);
					_npicts++;
					tmpvec.push_back(1);
				 	break;

			}

		}
		_data.push_back(tmpvec);
		y++;
	}


	lab_h = h;
	lab_w = w;
	_walls = wall.data();
	_boxes = box.data();
	_guards = guards.data();
	_guards[0] -> _x = chasx;
	_guards[0] -> _y = chasy;
	_picts = affiche.data();


}

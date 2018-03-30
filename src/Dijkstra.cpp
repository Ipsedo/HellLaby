#include "Dijkstra.h"
#include "Mover.h"
#include "Labyrinthe.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <limits>

using namespace std;


int distDij(Environnement* l, Mover* m){
  auto x = l->_treasor._x;
  auto y = l->_treasor._y;

  set<pair<int,int> > djaVu;
  djaVu.insert(make_pair(x,y));
  vector<pair<int,int> > neig;
  neig.push_back(make_pair(x,y));

  vector<std::vector<int> > dist(
    l->width(),vector<int>(l->height(), numeric_limits<int>::max()));

  dist[x][y] = 0;


  while (true) {

    if (neig.size() == 0)  {
     return numeric_limits<int>::max();
    }

    x = neig[0].first;
    y = neig[0].second;

    neig.erase(neig.begin());


    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        auto p = make_pair(x+j,y+i);
        if ( (int) m->_x / l->scale == x+j && (int) m->_y /l->scale == y+i )  {
          return dist[x+j][y+i];
        }
        if ( l->data(x+j,y+i) == EMPTY && !(i==0 && j==0) && djaVu.find(p) == djaVu.end()) {
          dist[x+j][y+i] = dist[x][y]+1;
          neig.push_back(p);
          djaVu.insert(p);

        }
      }
    }
  }

  return 0;

}

pair<int,int> dijkstra(Environnement* l, Mover* m) {

  auto x = l->_treasor._x;
  auto y = l->_treasor._y;

  set<pair<int,int> > djaVu;
  djaVu.insert(make_pair(l->_treasor._x,l->_treasor._y));
  vector<pair<int,int> > neig;
  neig.push_back(make_pair(l->_treasor._x,l->_treasor._y));

  while (true) {

    if (neig.size() == 0)  {
     return make_pair(0,0);
    }

    x = neig[0].first;
    y = neig[0].second;

    neig.erase(neig.begin());


    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        auto p = make_pair(x+j,y+i);
        if ( (int) m->_x / l->scale == x+j && (int) m->_y /l->scale == y+i )  {
          return make_pair(x - (int)(m->_x / l->scale) ,y - (int)(m->_y / l->scale));
        }
        if ( l->data(x+j,y+i) == EMPTY && !(i==0 && j==0) && djaVu.find(p) == djaVu.end()) {
          neig.push_back(p);
          djaVu.insert(p);

        }
      }
    }
  }
}

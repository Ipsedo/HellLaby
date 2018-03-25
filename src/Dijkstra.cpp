#include "Dijkstra.h"
#include "Mover.h"
#include "Labyrinthe.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using namespace std;

void Neighbors(vector<pair<int,int> > *v,set<pair<int,int> > *djaVu, Environnement* l, int x, int y) {
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      auto p = make_pair(x+j,y+i);
      if ( l->data(x+j,y+i) == EMPTY && !(i==0 && j==0) && djaVu -> find(p) == djaVu -> end()) {
        v -> push_back(p);
        djaVu -> insert(p);

      }
    }
  }
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
     // std::cout << "size 0" << '\n';
     return make_pair(0,0);
    }

    x = neig[0].first;
    y = neig[0].second;

    neig.erase(neig.begin());


    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        auto p = make_pair(x+j,y+i);
        if ( l->data(x+j,y+i) == EMPTY && !(i==0 && j==0) && djaVu.find(p) == djaVu.end()) {
          // std::cout << x << " - " << (int)(m->_x / l->scale) << " , " << y << " - " << (int)(m->_y / l->scale) <<  '\n';
          // std::cout << "if " << (int) m->_x / l->scale << " == " << x+j << " && " << (int) m->_y / l->scale << " == " << y+i << '\n';
          if ( (int) m->_x / l->scale == x+j && (int) m->_y /l->scale == y+i )  {
            return make_pair(x - (int)(m->_x / l->scale) ,y - (int)(m->_y / l->scale));
          }
          neig.push_back(p);
          djaVu.insert(p);

        }
      }
    }
  }


  // Neighbors(&neig, &djaVu, l, x, y);
  // while (true) {
  // // for (size_t i = 0; i < 100; i++) {
  //
  //   if (find(neig.begin(),neig.end(),make_pair((int) m -> _x / l->scale,(int) m -> _y /l->scale)) == neig.end()) {
  //     x = neig[0].first;
  //     y = neig[0].second;
  //     neig.erase(neig.begin());
  //     //if(neig.size() == 0) break;
  //     Neighbors(&neig, &djaVu, l, x, y);
  //   }else{
  //     break;
  //   }
  // }
  // pair<int,int> res = make_pair(neig[0].first - (int) m -> _x / l->scale, neig[0].second - (int) m -> _y /l->scale);
  // return res;

}

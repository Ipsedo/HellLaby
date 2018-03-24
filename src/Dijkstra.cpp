#include "Dijkstra.h"
#include "Mover.h"
#include "Labyrinthe.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using namespace std;

void Neighbors(vector<pair<int,int> > *v,set<pair<int,int> > *djaVu, Labyrinthe* l, int x, int y) {
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

pair<int,int> dijkstra(Labyrinthe* l, Mover* m) {



  auto x = l->_treasor._x;
  auto y = l->_treasor._y;

  set<pair<int,int> > djaVu;
  djaVu.insert(make_pair(l->_treasor._x,l->_treasor._y));
  vector<pair<int,int> > neig;
  Neighbors(&neig, &djaVu, l, x, y);
  while (true) {
    if (find(neig.begin(),neig.end(),make_pair((int) m -> _x / l->scale,(int) m -> _y /l->scale)) == neig.end()) {
      x = neig[0].first;
      y = neig[0].second;
      neig.erase(neig.begin());
      Neighbors(&neig, &djaVu, l, x, y);
    }else{
      break;
    }
  }

  // DIR res = U;
  pair<int,int> res = make_pair(neig[0].first - (int) m -> _x / l->scale, neig[0].second - (int) m -> _y /l->scale);

  std::cout << res.first << " " << res.second << '\n';

  return res;

}

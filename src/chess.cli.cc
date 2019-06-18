#include "chess.lib.hh"
#include "accounting.lib.hh"

int main(){
  Game *game1 = new Game;
  accounting();
  game1->play();
  delete game1;
  accounting();
  return 0;
}

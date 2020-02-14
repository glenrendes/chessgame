/* Position and Move class implementations*/

#include "chess.lib.hh"

//---------------------------------------------------
//---------------------------------------------------
// Position class member definitions
void Position::print(){
  os << "(" << x << ", " << y << ")";
}
bool Position::is_on_board(){
  return (0 <= x) && (x < ROWS) && (0 <= y) && (y < COLUMNS);
}

//---------------------------------------------------
//---------------------------------------------------
// Move class member definitions
void Move::print(){
  os << "Move: from "; from.print();
  os << " to "; to.print(); os << "\n";
}
bool Move::is_legal(Board &gb){
  std::vector<Position> legal_offsets = gb.board[from.x][from.y]->possible_moves(from, gb);
  Position dp = to - from;
  std::vector<Position>::iterator it = std::find(legal_offsets.begin(), legal_offsets.end(), dp);
  return it != legal_offsets.end();
}

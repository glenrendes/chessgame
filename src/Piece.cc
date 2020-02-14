/* Piece classes implementations*/

#include "chess.lib.hh"

//---------------------------------------------------
//---------------------------------------------------
// Piece class member definitions

Pawn::Pawn(color c) : Piece(c){ m_type = pawn; }
void Pawn::print(){
  if( m_color == white ) os << 'P';
  else os << 'p';
}
std::vector<Position> Pawn::possible_moves(Position from, Board &gb){
  std::vector<Position> legal_offsets = {};
  switch(m_color){
  case white:
    if( gb.is_open(Position((from.x-1), from.y)) ){
      legal_offsets.push_back(Position(-1, 0));
      if( gb.is_open(Position((from.x-2), from.y)) && from.x == 6 ) legal_offsets.push_back(Position(-2, 0));
    }
    if( gb.is_opponent(Position(from.x-1, from.y-1)) ) legal_offsets.push_back(Position(-1, -1));
    if( gb.is_opponent(Position(from.x-1, from.y+1)) ) legal_offsets.push_back(Position(-1, 1));
    break;

  case black:
    if( gb.is_open(Position(from.x+1, from.y)) ){
      legal_offsets.push_back(Position(1, 0));
      if( gb.is_open(Position(from.x+2, from.y)) && from.x == 1 ) legal_offsets.push_back(Position(2, 0));
    }
    if( gb.is_opponent(Position(from.x+1, from.y-1)) ) legal_offsets.push_back(Position(1, -1));
    if( gb.is_opponent(Position(from.x+1, from.y+1)) ) legal_offsets.push_back(Position(1, 1));
    break;
  }
  return legal_offsets;
}

Rook::Rook(color c) : Piece(c){ m_type = rook; }
void Rook::print(){
  if( m_color == white ) os << 'R';
  else os << 'r';
}
std::vector<Position> Rook::possible_moves(Position from, Board &gb){
  std::vector<Position> legal_offsets = {};
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x, from.y+n)) || gb.is_opponent(Position(from.x, from.y+n)))){
        legal_offsets.push_back(Position(0, n));
        if( !gb.is_open(Position(from.x, from.y+n)) ) n = 9;
        n++;
      }}
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x, from.y-n)) || gb.is_opponent(Position(from.x, from.y-n)))){
        legal_offsets.push_back(Position(0, -n));
        if( !gb.is_open(Position(from.x, from.y-n)) ) n = 9;
        n++;
      }}
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x+n, from.y)) || gb.is_opponent(Position(from.x+n, from.y)))){
        legal_offsets.push_back(Position(n, 0));
        if( !gb.is_open(Position(from.x+n, from.y)) ) n = 9;
        n++;
      }}
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x-n, from.y)) || gb.is_opponent(Position(from.x-n, from.y)))){
        legal_offsets.push_back(Position(-n, 0));
        if( !gb.is_open(Position(from.x-n, from.y)) ) n = 9;
        n++;
      }}
  return legal_offsets;
}

Knight::Knight(color c) : Piece(c){ m_type = knight; }
void Knight::print(){
  if( m_color == white ) os << 'N';
  else os << 'n';
}
std::vector<Position> Knight::possible_moves(Position from, Board &gb){
  std::vector<Position> legal_offsets = {Position(2, 1), Position(2, -1), Position(-2, 1), Position(-2, -1), Position(1, 2), Position(1, -2), Position(-1, 2), Position(-1, -2)};
  return legal_offsets;
}

Bishop::Bishop(color c) : Piece(c){ m_type = bishop; }
void Bishop::print(){
  if( m_color == white ) os << 'B';
  else os << 'b';
}
std::vector<Position> Bishop::possible_moves(Position from, Board &gb){
  std::vector<Position> legal_offsets = {};
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x+n, from.y+n)) || gb.is_opponent(Position(from.x+n, from.y+n)))){
        legal_offsets.push_back(Position(n, n));
        if( !gb.is_open(Position(from.x+n, from.y+n)) ) n = 9;
        n++;
      }}
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x+n, from.y-n)) || gb.is_opponent(Position(from.x+n, from.y-n)))){
        legal_offsets.push_back(Position(n, -n));
        if( !gb.is_open(Position(from.x+n, from.y-n)) ) n = 9;
        n++;
      }}
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x-n, from.y+n)) || gb.is_opponent(Position(from.x-n, from.y+n)))){
        legal_offsets.push_back(Position(-n, n));
        if( !gb.is_open(Position(from.x-n, from.y+n)) ) n = 9;
        n++;
      }}
    {int n=1;
      while(n < 9 && (gb.is_open(Position(from.x-n, from.y-n)) || gb.is_opponent(Position(from.x-n, from.y-n)))){
        legal_offsets.push_back(Position(-n, -n));
        if( !gb.is_open(Position(from.x-n, from.y-n)) ) n = 9;
        n++;
      }}
  return legal_offsets;
}


Queen::Queen(color c) : Piece(c){ m_type = queen; }
void Queen::print(){
  if( m_color == white ) os << 'Q';
  else os << 'q';
}
std::vector<Position> Queen::possible_moves(Position from, Board &gb){
  Rook r_powers(m_color); Bishop b_powers(m_color);
  std::vector<Position> legal_offsets = r_powers.possible_moves(from, gb);
  std::vector<Position> diagonal_offsets = b_powers.possible_moves(from, gb);
  legal_offsets.insert(legal_offsets.end(), diagonal_offsets.begin(), diagonal_offsets.end());
  return legal_offsets;
}

King::King(color c) : Piece(c){ m_type = king; }
void King::print(){
  if( m_color == white ) os << 'K';
  else os << 'k';
}
std::vector<Position> King::possible_moves(Position from, Board &gb){
  std::vector<Position> legal_offsets = {Position(0, 0), Position(0, 1), Position(0, -1), Position(1, 0), Position(1, 1), Position(1, -1), Position(-1, 0), Position(-1, 1), Position(-1, -1),};
  return legal_offsets;
}


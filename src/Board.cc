/* Graveyard and Board class implementations*/

#include "chess.lib.hh"


//---------------------------------------------------
//---------------------------------------------------
// Graveyard class member definitions
void Graveyard::print(){
  if( graveyard.empty() ) os << "The graveyard is empty. Get to killin.\n";
  else{
    os << "The following pieces have been eliminated: \n";
    for( Piece *pc : graveyard ){
      pc->print();
    }
    os << std::endl;
  }
}
bool Graveyard::is_king_dead(){
  bool king_dead = false;
  std::list<Piece *>::iterator it;
  for( it = graveyard.begin(); it != graveyard.end(); it++ ){
    if (!king_dead) king_dead = (*it)->is_king();
    else break;
  }
  return king_dead;
}
void Graveyard::bury(Piece *pc){
  graveyard.push_back(pc);
}
void Graveyard::clear(){
  std::list<Piece *>::iterator it;
  for( it = graveyard.begin(); it != graveyard.end(); it++ ){
    delete (*it);
  }
}

//---------------------------------------------------
//---------------------------------------------------
// Board class member definitions
void Board::print(){
  os << "\n\n        0     1     2     3     4     5     6     7\n";
  os << "      _______________________________________________\n";
  for( int i = 0; i < ROWS; i++ ){
    os << "     |     |     |     |     |     |     |     |     |";
    os << "\n  " << i << "  |";
    for( int j = 0; j < COLUMNS; j++ ){
      os << "  ";
      if( board[i][j] == nullptr ) os << " ";
      else board[i][j]->print();
      os << "  |";
    }
    os << " " << i << std::endl;
    os << "     |_____|_____|_____|_____|_____|_____|_____|_____|\n";
  }
  os << "\n        0     1     2     3     4     5     6     7\n\n";
}
void Board::change_turn(){
  turn = turn == white? black : white;
}
void Board::kill(Position &to){
  Piece *opponent = board[to.x][to.y];
  gy.bury(opponent);
  opponent->print();
  os << " was killed. RIP.\n";
}
void Board::make_move(Move *move){
  move->print();//remove
  if( board[move->to.x][move->to.y] != nullptr ) kill(move->to);
  board[move->to.x][move->to.y] = board[move->from.x][move->from.y];
  board[move->from.x][move->from.y] = nullptr;
}
void Board::clear(){
  for( int i = 0; i < ROWS; i++ ){
    for( int j = 0; j < COLUMNS; j++ ){
      if( board[i][j] != nullptr ){
        delete board[i][j]; board[i][j] = nullptr;
      }
    }
  }
  turn = white;
}
bool Board::is_open(Position pos){
  return pos.is_on_board() && (board[pos.x][pos.y] == nullptr);
}
bool Board::is_mine(Position pos){
  return pos.is_on_board() && !is_open(pos) && (turn == board[pos.x][pos.y]->m_color);
}
bool Board::is_opponent(Position pos){
  return pos.is_on_board() && !is_open(pos) && !is_mine(pos);
}


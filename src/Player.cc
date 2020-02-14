/* Player classes implementations*/

#include "chess.lib.hh"

//---------------------------------------------------
//---------------------------------------------------
// Player class member definitions
std::string Human::get_input(bool quantifier){
  std::string input;
  if( quantifier == FROM ) os << "Please choose piece by position: ";
  else os << "Please choose where to move piece: ";
  is >> input;
  os << input;//remove
  return input;
}
bool Human::is_pos(std::string input){
  if( isdigit(input[0]) && isdigit(input[1]) && input.length() == 2 && input[0] >= 0 ) return true;
  else return false;
}
bool Human::is_valid(Position pos, Board &gb, bool quantifier){
  if( Player::is_valid(pos, gb, quantifier) ) return true;
  if( pos.x == -1 ) return false;
  else if( !pos.is_on_board() )
    os << "Invalid move. Position not on board.\n";
  else if( gb.is_open(pos) && quantifier )
    os << "Invalid move. No piece selected.\n";
  else if( gb.is_opponent(pos) && quantifier)
    os << "Invalid move. Not your piece.\n";
  else if( gb.is_mine(pos) && !quantifier )
    os << "Invalid move. Can't kill your own piece.\n";
  return false;
}
bool CPU::propose_move(Move *move, Board &gb){
  //get possible (and eventually legal) position
  Position from, to;
  for( int i = 0; (i < ROWS && !is_valid(from, gb, FROM)); i++ ){
    for( int j = 0; (j < COLUMNS && !is_valid(from, gb, FROM)); j++ ){
      from.x = i; from.y = j;
    }
  }
  for( int i = 0; (i < ROWS && !is_valid(to, gb, TO)); i++ ){
    for( int j = 0; (j < COLUMNS && !is_valid(to, gb, TO)); j++ ){
      to.x = i; to.y = j;
    }
  }
  move->from = from; move->to = to;
  if( gb.is_opponent(to) ) move->is_kill = true;
  moves.push_back(move);
  return true;
}

bool Human::propose_move(Move *move, Board &gb){
  //check whose turn
  if( m_color == white ) os << "Player 1's turn (White)\n";
  else os << "Player 2's turn (Black)\n";
  
  //get move from user while checking validity
  //get from position and check validity
  std::string input = get_input(FROM);
  if( !is_pos(input) ) return false;
  Position from(input);
  if( !is_valid(from, gb, FROM) ) return propose_move(move, gb); //repeat prompt
  Piece *pc = gb.board[from.x][from.y];
  //get to position and check validity
  std::string input1 = get_input(TO);
  if( !is_pos(input1) ) return false;
  Position to(input1);
  if( !is_valid(to, gb, TO) ) return propose_move(move, gb); //repeat prompt
  move->from = from; move->to = to;
  //check that move is legal according to the rules of chess
  if( move->is_legal(gb) ){
    if( gb.is_opponent(to) ) move->is_kill = true;
    moves.push_back(move);
    return true;
  }
  else {
    os << "This piece cannot move like that.\n";
    return propose_move(move,gb);
  }
}

#include "chess.lib.hh"

#define FROM true
#define TO false


//---------------------------------------------------
//---------------------------------------------------
//define streams
std::ostream &os = std::cout;
std::istream &is = std::cin;


//---------------------------------------------------
//---------------------------------------------------
// Position class member definitions
void Position::print(){
  os << "(" << x << ", " << y << ")";
}


//---------------------------------------------------
//---------------------------------------------------
// Move class member definitions
void Move::print(){
  if( !(from && to) ){
    os << "No move selected.";
    return;
  }
  os << "Move: from "; from.print();
  os << " to "; to.print(); os << "\n";
}
bool Move::is_legal(Move &move, Board &gb){
  //to-do:  "operator square brackets" - overload [][] to return a Piece *
  //to-do: create operator overload for Position - Position
  std::vector<Position> legal_offsets = (*gb.board[from.x][from.y])->possible_moves(move.from, gb);
  int dx = (move.to.x - move.from.x); int dy = (move.to.y - move.from.y);
  Position dp(dx, dy);
  std::vector<Position>::iterator it = std::find(legal_offsets.begin(), legal_offsets.end(), dp);
  return it != legal_offsets.end();
}


//---------------------------------------------------
//---------------------------------------------------
// Piece class member definitions
Piece::Piece(color c, type t) : m_color(c), m_type(t) {}

Pawn::Pawn(color c, type t) : Piece(c, t){}
void Pawn::print(){
  if( m_color == white ) os << 'P';
  else os << 'p';
}
std::vector<Position> Pawn::possible_moves(Position from, Board &gb){
  std::vector legal_offsets = {};
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

Rook::Rook(color c, type t) : Piece(c, t){}
void Rook::print(){
  if( m_color == white ) os << 'R';
  else os << 'r';
}
std::vector<Position> Rook::possible_moves(Position from, Board &gb){
  std::vector legal_offsets = {};
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

Knight::Knight(color c, type t) : Piece(c, t){}
void Knight::print(){
  if( m_color == white ) os << 'N';
  else os << 'n';
}
std::vector<Position> Knight::possible_moves(Position from, Board &gb){
  std::vector legal_offsets = {Position(2, 1), Position(2, -1), Position(-2, 1), Position(-2, -1), Position(1, 2), Position(1, -2), Position(-1, 2), Position(-1, -2)};
  return legal_offsets;
}

Bishop::Bishop(color c, type t) : Piece(c, t){}
void Bishop::print(){
  if( m_color == white ) os << 'B';
  else os << 'b';
}
std::vector<Position> Bishop::possible_moves(Position from, Board &gb){
  std::vector legal_offsets = {};
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


Queen::Queen(color c, type t) : Piece(c, t){}
void Queen::print(){
  if( m_color == white ) os << 'Q';
  else os << 'q';
}
std::vector<Position> King::possible_moves(Position from, Board &gb){
  std::vector legal_offsets = Rook::possible_moves(from, gb);
  std::vector diagonal_offsets = Bishop::possiblemoves(from, gb);
  legal_offsets.insert(legal_offsets.end(), diagonal_offsets.begin(), diagonal_offsets.end());
  return legal_offsets;
}

King::King(color c, type t) : Piece(c, t){}
void King::print(){
  if( m_color == white ) os << 'K';
  else os << 'k';
}
std::vector<Position> King::possible_moves(Position from, Board &gb){
  std::vector legal_offsets = {Position(0, 0), Position(0, 1), Position(0, -1), Position(1, 0), Position(1, 1), Position(1, -1), Position(-1, 0), Position(-1, 1), Position(-1, -1),};
  return legal_offsets;
}



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
void Board::kill(Position &to){
  Piece *opponent = board[to.x][to.y];
  gy.bury(opponent);
  opponent->print();
  os << " was killed. RIP.\n";
}
void Board::make_move(Move *move){
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
}

//---------------------------------------------------
//---------------------------------------------------
// Player class member definitions
std::string Human::get_input(bool quantifier){
  std::string input;
  if( quantifier == FROM ) os << "Please choose piece by position: ";
  else os << "Please choose where to move piece: ";
  is >> input;
  return input;
}
bool Human::is_pos(std::string input){
  if( isdigit(input[0]) && isdigit(input[1]) && input.length() == 2 && input[0] >= 0 ) return true;
  else return false;
}
Position Human::get_pos(std::string input, bool quantifier){
  //to-do: create constructor for Position that takes a string and returns a Position
  Position from, to;
  if( quantifier == FROM ){
    from.x = input[0] - '0';
    from.y = input[1] - '0';
    return from;
  } 
  if( quantifier == TO ){
    to.x = input[0] - '0';
    to.y = input[1] - '0';
    return to;
  }
  return from;
}
void Game::change_turn(){
  turn = turn == white? black : white;
}
bool Position::is_on_board(){
  return (0 <= x) && (x < ROWS) && (0 <= y) && (y < COLUMNS);
}
bool Board::is_open(Position pos){
  return pos.is_on_board() && (board[pos.x][pos.y] == nullptr);
}
bool Board::is_mine(Position pos){
  return pos.is_on_board() && !is_open() && (turn == board[pos.x][pos.y]->m_color);
}
bool Board::is_opponent(Position pos){
  return pos.is_on_board && !is_open() && !is_mine(pos);
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
 bool Human::propose_move(Move *move, Board &gb, Position repeat){
  //check whose turn
  if( m_color == white ) os << "Player 1's turn (White)\n";
  else os << "Player 2's turn (Black)\n";
  
  //get move from user while checking validity
  //get from position and check validity
  if(repeat) Position from = repeat;
  else {
    std::string input = get_input(FROM);
    if( !is_pos(input) ) return false;
    Position from = get_pos(input, FROM);
  }
  if( !is_valid(from, gb, FROM) ) return propose_move(move, gb, NULL); //repeat prompt
  Piece *pc = gb.board[from.x][from.y];
  //get to position and check validity
  std::string input1 = get_input(TO);
  if( !is_pos(input1) ) return false;
  Position to = get_pos(input1, TO);
  if( !is_valid(to, gb, TO) ) return propose_move(move, gb, from); //repeat prompt but skip from
  move->from = from; move->to = to;
  //check that move is legal according to the rules of chess
  if( move->is_legal(*move, gb) ){
    if( gb.is_opponent(to) ) move->is_kill = true;
    moves.push_back(move);
    return true;
  }
  else {
    os << "This piece cannot move like that.\n";
    return propose_move(move,gb);
  }
}


void Game::menu(){
  os << "Current bug focus: rook, bishop, queen not able to kill adjacent pieces\n"
    << "Help menu:\nWhite pieces are capital. Black pieces are lowercase.\n"
     << "Enter positions as number pairs. Ex: 34 = row 3, column 4\n\n"

     << "(c)ontinue  - continue playing this game\n"
     << "(g)raveyard - see eliminated pieces\n"
     << "(m)ode      - change game mode\n"
     << "(u)undo     - undo one move\n"
     << "(s)ave      - store progress in local file\n"
     << "(l)oad      - load and resume a saved game\n"
     << "(r)estart   - restart new game\n"
     << "(h)elp      - see help menu\n"
     << "(q)uit      - quit\n";
}
void Game::undo(){
  if( (p1->moves).empty() && (p2->moves).empty() ) { os << "No moves to undo.\n"; return; }
  Player *p = turn == 'W'? p2 : p1;
  Move *last = (p->moves).back();
  gb.board[(last->from).x][(last->from).y] = gb.board[(last->to).x][(last->to).y];
  if( last->is_kill ){
    gb.board[(last->to).x][(last->to).y] = (gb.gy.graveyard).back();
    (gb.gy.graveyard).pop_back();
  }
  else gb.board[(last->to).x][(last->to).y] = nullptr;
  (p->moves).pop_back();
  change_turn();
  gb.print();
}
void::Game::save_file(){
  //commands to put game in stringstream
  std::stringstream ss;
  std::list<Move *>::iterator it1 = p1->moves.begin(), it2 = p2->moves.begin();
  while( it1 != p1->moves.end() || it2 != p2->moves.end() ){
    if( it1 != p1->moves.end() ){
      ss << (*it1)->from.x << " " << (*it1)->from.y << " " << (*it1)->to.x << " " << (*it1)->to.y << "\n";
    it1++;
    }
    if( it2 != p2->moves.end() ){
      ss << (*it2)->from.x << " " << (*it2)->from.y << " " << (*it2)->to.x << " " << (*it2)->to.y << "\n";
    it2++;
    }
  }
  std::string coordinates = ss.str();
  //write commands to file
  std::ofstream file;
  file.open(saver);
  file << coordinates;
  file.close();
  os << "Game saved as: " << saver << ".\n";
}
void Game::save(){
  std::string answer;
  if( saver.length() > 0 ){//overwrite previous save
    os << "This game is currently saved as " << saver << ".\n"
       << "Overwrite previous save? (y or n) (y rewrites saved file, n writes new file) ";
    is >> answer;
    if( answer == "y" || answer == "Y" || answer == "yes" ) save_file();
    else if( answer == "n" || answer == "N" || answer == "no") answer = "n";
    else{
      os << "This is not a valid entry.\n\n";
      help();
    }
  }
  if( saver.length() == 0 || answer == "n"){//prompt for new file
    os << "Enter name of game file to be saved. It must be more than one character. : ";
    is >> answer;
    if ( answer.length() < 2 && help() ) return;
    else saver = answer + ".chess";
    save_file();
  }
}
bool Game::file_exists(std::string &filename){
  struct stat file;
  return( stat (filename.c_str(), &file) == 0 );
}
void Game::load_file(std::string &filename){
  //access file
  std::ifstream file;
  file.open(filename);
  gb.init();
  //resume game with saved info
  restart();
  int f1, f2, t1, t2;
  while( file >> f1 >> f2 >> t1 >>t2 ){
    Position from(f1, f2), to(t1, t2);
    Move *move = new Move(from, to);
    Player *p = turn == 'W'? p1:p2;
    if( p->gb.is_opponent(to) ){ gb.kill(to); move->is_kill = true; }
    p->moves.push_back(move);
    gb.board[to.x][to.y] = gb.board[from.x][from.y];
    gb.board[from.x][from.y] = nullptr;
    change_turn();
  }
  file.close();
  saver = filename;
  os << "Resuming game: " << saver << ".\n\n";
  gb.print();
}
void Game::load(){
  std::string answer;
  if( saver.length() > 0 ){
    os << "Save this game before opening another? (y or n) ";
    is >> answer;
    if( answer == "y" ) save();
    else if( answer == "n" ) {}
    else help();
  }
  os << "Enter name of game to be loaded (do not include .chess) : ";
  is >> answer;
  if ( answer.length() < 2 ) help();
  else{
    answer = answer + ".chess";
    if( file_exists(answer) ) load_file(answer);
    else{ os << "This game does not exist.\n"; load(); }
  }
}
bool Game::mode(){
  bool mode = 0;
  std::string input;
  os << "1-Player or 2-Player mode? (1 or 2) : ";
  is >> input;
  if( (input.length() == 1) && std::isdigit(input[0]) ) mode = input[0] - '1';
  else{ invalid_input(); return false; }
  if( !mode ){ delete p2; p2 = new CPU(black); gb.print(); }
  else{ delete p2; p2 = new Human(black); gb.print(); }
  return true;
}

void Game::restart(){
  gb.clear();
  gb.gy.clear();
  gb.init();
  turn = 'W';
  gb.print();
}
void Game::invalid_input(){
  os << "This is not a valid entry.\n\n";
}
bool Game::is_sure(std::string &action){
  std::string input;
  os << "Are you sure you want to " << action << "? (y or n) : ";
  is >> input;
  if( input == "y" ){
    os << "Save before? (y or n) : ";
    is >> input;
    if( input == "y" ) save();
    else if( input == "n" );
    else{ invalid_input(); return help(); }
    return true;
  }
  else if( input == "n" ) return false;
  else{ invalid_input(); return help(); }
}
bool Game::help(){
  std::string input;
  menu();
  os << "What would you like to do? ";
  is >> input;
  if( input == "c" || input == "C" || input == "continue" ); 
  else if( input == "g" || input == "G" || input == "graveyard" ) gb.gy.print();
  else if( input == "h" || input == "H" || input == "help" ) menu();
  else if( input == "u" || input == "U" || input == "undo" ) undo();
  else if( input == "m" || input == "M" || input == "mode" ) mode();
  else if( input == "s" || input == "S" || input == "save" ) save();
  else if( input == "l" || input == "L" || input == "load" ) load();
  else if( input == "r" || input == "R" || input == "restart" ){
    std::string action = "restart";
    if( is_sure(action) ) restart();
  }
  else if( input == "q" || input == "Q" || input == "quit" ){
    std::string action = "quit";
    return is_sure(action);
  }
  else{ invalid_input(); return help(); }
  return false;
}

void Game::goodbye(){
  if( turn == black && gb.gy.is_king_dead() )
    os << "The game is over!   White wins.   Well Played!\n ";
  else if( gb.gy.is_king_dead() )
    os << "The game is over!   Black wins.   Well Played!\n ";
  os << "\nGoodbye!\n";
}
void Game::play(){
  menu();
  gb.print();
  while( !mode() );
  while( !game_over ){
    Position from, to;
    Move *move = new Move(from, to);
    bool proposed = turn == white? p1->propose_move(move, gb, NULL):p2->propose_move(move, gb, NULL);
    if( proposed ){ gb.make_move(move); gb.print(); change_turn(); game_over = gb.gy.is_king_dead(); }
    else{ delete move;  game_over = help(); }
  }
  goodbye();
}



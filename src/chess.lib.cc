#include "chess.lib.hh"

#define FROM true
#define TO false

// Piece class functions
Piece::Piece(char c) : color(c) {}
Pawn::Pawn(char c) : Piece(c){}
void Pawn::print(){
  if( color == 'W' ) os << 'P';
  else os << 'p';
}
Rook::Rook(char c) : Piece(c){}
void Rook::print(){
  if( color == 'W' ) os << 'R';
  else os << 'r';
}
Knight::Knight(char c) : Piece(c){}
void Knight::print(){
  if( color == 'W' ) os << 'N';
  else os << 'n';
}
Bishop::Bishop(char c) : Piece(c){}
void Bishop::print(){
  if( color == 'W' ) os << 'B';
  else os << 'b';
}
Queen::Queen(char c) : Piece(c){}
void Queen::print(){
  if( color == 'W' ) os << 'Q';
  else os << 'q';
}
King::King(char c) : Piece(c){}
void King::print(){
  if( color == 'W' ) os << 'K';
  else os << 'k';
}

//---------------------------------------------------
//---------------------------------------------------
// Graveyard class functions
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
    king_dead = (*it)->is_king();
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
// Board class functions
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
// Player class functions
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
  turn = turn == 'W'? 'B':'W';
}
bool Player::is_on_board(Position &pos){
  return (0 <= pos.x) && (pos.x < ROWS) && (0 <= pos.y) && (pos.y < COLUMNS);
}
bool Player::is_mine(Position &pos, Board &gb){
  if( !is_on_board(pos) ) return false;
  if( gb.board[pos.x][pos.y] == nullptr ) return false;
  if( color == gb.board[pos.x][pos.y]->color ) return true;
  else return false;
}
bool Player::is_opponent(Position &pos, Board &gb){
  if( !is_on_board(pos) ) return false;
  if( gb.board[pos.x][pos.y] == nullptr ) return false;
  else return !is_mine(pos, gb);
}
bool Player::is_open(Position &pos, Board &gb){
  return is_on_board(pos) && !is_mine(pos, gb) && !is_opponent(pos, gb);
}
bool Human::is_valid(Position &pos, Board &gb, bool quantifier){
  if( Player::is_valid(pos, gb, quantifier) ) return true;
  if( pos.x == -1 ) return false;
  else if( !is_on_board(pos) )
    os << "Invalid move. Position not on board.\n";
  else if( is_open(pos, gb) && quantifier )
    os << "Invalid move. No piece selected.\n";
  else if( is_opponent(pos, gb) && quantifier)
    os << "Invalid move. Not your piece.\n";
  else if( is_mine(pos, gb) && !quantifier )
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
  if( is_opponent(to, gb) ) move->is_kill = true;
  moves.push_back(move);
  return true;
}
bool Human::propose_move(Move *move, Board &gb){
  //check whose turn
  if( color == 'W' ) os << "Player 1's turn (White)\n";
  else os << "Player 2's turn (Black)\n";
  
  //get move from user while checking validity
      //get from position
  std::string input = get_input(FROM);
  if( is_pos(input) ){ 
    Position from = get_pos(input, FROM);
    if( is_valid(from, gb, FROM) ){
      Piece *pc = gb.board[from.x][from.y];
      //get to position
      std::string input1 = get_input(TO);
      if( is_pos(input1) ){
        Position to = get_pos(input1, TO);
        if( is_valid(to, gb, TO) ){
          move->from = from; move->to = to;
          if( is_opponent(to, gb) ) move->is_kill = true;
          moves.push_back(move);
          return true;
        }
        else return propose_move(move, gb);
      }
      return false;
    }
  else return propose_move(move, gb);
  }
  else return false;
}


void Game::menu(){
  os << "Help menu:\nWhite pieces are capital. Black pieces are lowercase.\n"
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
    if( p->is_opponent(to, gb) ){ gb.kill(to); move->is_kill = true; }
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
  if( !mode ){ delete p2; p2 = new CPU('B'); gb.print(); }
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
  if( turn == 'B' && gb.gy.is_king_dead() )
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
    bool proposed = turn == 'W'? p1->propose_move(move, gb):p2->propose_move(move, gb);
    if( proposed ){ gb.make_move(move); gb.print(); change_turn(); game_over = gb.gy.is_king_dead(); }
    else{ delete move;  game_over = help(); }
  }
  goodbye();
}

//need to move to cli
int main(){
  Game *game1 = new Game;
  accounting();
  game1->play();
  delete game1;
  accounting();
  return 0;
}

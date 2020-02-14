/* Game class implementations*/

#include "chess.lib.hh"


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
  Player *p = gb.turn == white? p2 : p1;
  Move *last = (p->moves).back();
  gb.board[(last->from).x][(last->from).y] = gb.board[last->to.x][last->to.y];
  if( last->is_kill ){
    gb.board[(last->to).x][(last->to).y] = (gb.gy.graveyard).back();
    (gb.gy.graveyard).pop_back();
  }
  else gb.board[(last->to).x][(last->to).y] = nullptr;
  (p->moves).pop_back();
  gb.change_turn();
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
    Player *p = gb.turn == white? p1:p2;
    if( gb.is_opponent(to) ){ gb.kill(to); move->is_kill = true; }
    p->moves.push_back(move);
    gb.board[to.x][to.y] = gb.board[from.x][from.y];
    gb.board[from.x][from.y] = nullptr;
    gb.change_turn();
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
  gb.turn = white;
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
  if( gb.turn == black && gb.gy.is_king_dead() )
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
    bool proposed = gb.turn == white? p1->propose_move(move, gb):p2->propose_move(move, gb);
    if( proposed ){ gb.make_move(move); gb.print(); gb.change_turn(); game_over = gb.gy.is_king_dead(); }
    else{ delete move;  game_over = help(); }
  }
  goodbye();
}

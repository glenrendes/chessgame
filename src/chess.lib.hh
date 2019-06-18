#ifndef CHESS_LIB_HH
#define CHESS_LIB_HH

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>
#include <sstream>
#include <filesystem>
#include <sys/stat.h>
#include <fstream>
#include <cctype>

#define ROWS 8
#define COLUMNS 8

std::ostream &os = std::cout;
std::istream &is = std::cin;

int count = 0;
void *operator new(size_t size){
  //os << "Allocated memory.\n"; //also had constructors and destructors printing class identifiers at some point
  count++;
  void *pt = malloc(size);
  return pt;
}
void operator delete(void* pt){
  //os << "Deallocated memory.\n";
  count--;
  free(pt);
}
void accounting(){
  os << "Outstanding memory allocations: " << count << ".\n";
}

//---------------------------------------------------
//---------------------------------------------------
//a Position is a coordinate on the board
class Position{
public:
  int x;
  int y;
  Position(){ x = -1; y = -1; }
  Position(int ux, int uy){ init(ux, uy); }
  Position(const Position &pos){ init(pos.x, pos.y); }
  Position operator=(const Position &pos){ init(pos.x, pos.y); return *this; }
  bool operator==(const Position &pos){ return (x == pos.x) && (y == pos.y);}
  void init(int ux, int uy){ x = ux; y = uy; }
  void init_standard_notation(){}
  ~Position(){}
  void print(){
    os << "(" << x << ", " << y << ")";
  }
  //void print_standard_notation(){} standard notation functions translate user input in standard to raw coordinates for code and back
};


//---------------------------------------------------
//---------------------------------------------------
// a Move consists of Positions from and to
class Move{
public:
  Position from;
  Position to;
  bool is_kill; //for undo
  Move(){}
  Move(Position ufrom, Position uto){ init(ufrom, uto); }
  Move(const Move &mov){ from = mov.from; to = mov.to; is_kill = mov.is_kill; }
  Move& operator=(const Move &mov){ return *this; }
  bool operator==(const Move &mov){ return (from == mov.from) && (to == mov.to) && (mov.is_kill == is_kill);}
  void init(Position ufrom, Position uto){ from = ufrom; to = uto; is_kill = false; }
  void init_standard_notation(){}
  ~Move(){}
  void print(){
    os << "Move: from "; from.print();
    os << " to "; to.print(); os << "\n";
  }
};

// Piece classes
class Piece{
public:
  char color; //'W' or 'B'
  Piece(char c);
  Piece(const Piece &pc){ color = pc.color; }
  Piece& operator=(const Piece &pc){ return *this; }
  bool operator==(const Piece &pc){ return &pc == this;}
  ~Piece(){}
  virtual void print(){}
  virtual char type(){ return '0';};
  virtual void possible_moves(Position from, std::list<Position> &to){}
  virtual bool is_king(){ return false; }
};
class Pawn : public Piece{
public:
  Pawn(char c);
  ~Pawn(){}
  void print();
  char type(){ return color == 'W'? 'P':'p';};
};
class Rook : public Piece{
public:
  Rook(char c);
  ~Rook(){}
  void print();
  char type(){ return 'R';};
};
class Knight : public Piece{
public:
  Knight(char c);
  ~Knight(){}
  void print();
  char type(){ return 'N';};
};
class Bishop : public Piece{
public:
  Bishop(char c);
  ~Bishop(){}
  void print();
  char type(){ return 'B';};
};
class Queen : public Piece{
public:
  Queen(char c);
  ~Queen(){}
  void print();
  char type(){ return 'Q';};
};
class King : public Piece{
public:
  King(char c);
  ~King(){}
  void print();
  char type(){ return 'K';};
  bool is_king(){ return true; }
};

class Graveyard{
public:
  std::list<Piece *> graveyard;
  Graveyard(){}
  Graveyard(const Graveyard &gy){
    clear();
    for( Piece *pc : gy.graveyard ){
      graveyard.push_back(pc);
    }
  }
  Graveyard& operator=(const Graveyard &gy){ return *this; }
  bool operator==(const Graveyard &gy){ return &gy == this;}
  ~Graveyard(){ clear(); }
  void print();
  void bury(Piece *pc);
  bool is_king_dead();
  void clear();
};


//---------------------------------------------------
//---------------------------------------------------
// for display and positioning
class Board{
public:
  Piece *board[ROWS][COLUMNS];
  Graveyard gy;
  Board(){ init(); }
  Board(const Board &bd){
    for( int i = 0; i < ROWS; i++ ){
      for( int j = 0; j < COLUMNS; j++ ){
        board[i][j] = bd.board[i][j];
      }
    }
  }
  Board& operator=(const Board &bd){ return *this; }
  bool operator==(const Board &bd){ return &bd == this;}
  void init(){
    // clear board
    for( int i = 0; i < ROWS; i++ ){
      for( int j = 0; j < COLUMNS; j++ ){
        board[i][j] = nullptr;
      }
    }
    // black army allocation
    for( int j = 0; j < COLUMNS; j++){
      board[1][j] = new Pawn('B');
    }
    board[0][0] = new Rook('B');
    board[0][1] = new Knight('B');
    board[0][2] = new Bishop('B');
    board[0][3] = new King('B');
    board[0][4] = new Queen('B');
    board[0][5] = new Bishop('B');
    board[0][6] = new Knight('B');
    board[0][7] = new Rook('B');
   
    // white army allocation
    for( int j = 0; j < COLUMNS; j++){
      board[6][j] = new Pawn('W');
    }
    board[7][0] = new Rook('W');
    board[7][1] = new Knight('W');
    board[7][2] = new Bishop('W');
    board[7][3] = new King('W');
    board[7][4] = new Queen('W');
    board[7][5] = new Bishop('W');
    board[7][6] = new Knight('W');
    board[7][7] = new Rook('W');
  }
  ~Board(){
    clear();
  }
  void print();
  void kill(Position &to);
  void make_move(Move *move);
  void clear();
};


//---------------------------------------------------
//---------------------------------------------------
// a Player can be human or computer
class Player{
public:
  char color; //'W' or 'B'
  std::list<Move *> moves;
  Player(){ color = 'W';}
  Player(char ucolor){ init(ucolor); }
  Player(const Player &plyr){ init(plyr.color); for(Move *move : plyr.moves) moves.push_back(move); }
  Player& operator=(const Player &plyr){ return *this; }
  bool operator==(const Player &plyr){ return &plyr == this; }
  void init(char ucolor){ color = ucolor; }
  virtual ~Player(){
    std::list<Move *>::iterator it;
    for( it = moves.begin(); it != moves.end(); it++ ){
      delete (*it);
    }
  }
  virtual bool is_human(){ return false; }
   //validates moves
  bool is_on_board(Position pos);
  bool is_mine(Position to, Board &gb);
  bool is_opponent(Position to, Board &gb);
  bool is_open(Position to, Board &gb);
  std::vector<Position> possible_moves(Position from, Board &gb);
  bool is_legal(Move &move, Board &gb);
  virtual bool is_valid(Position pos, Board &gb, bool quantifier){
    return is_mine(pos, gb) && quantifier || is_opponent(pos, gb) && !quantifier || is_open(pos, gb) && !quantifier;
  }
  //move proposal done by children
  virtual bool propose_move(Move *move, Board &gb){ return false; }
};
class Human : public Player{
public:
  Human(){}
  Human(char ucolor){ Player::init(ucolor); }
  ~Human(){}
  bool is_human(){ return true; }
  bool is_pos(std::string input);
  Position get_pos(std::string input, bool quantifier);
  std::string get_input(bool quantifier);
  bool is_valid(Position pos, Board &gb, bool quantifier);
  bool propose_move(Move *move, Board &gb);
};
class CPU : public Player{
public:
  CPU(){}
  CPU(char ucolor){ Player::init(ucolor); }
  ~CPU(){}
  bool is_human(){ return false; }
  bool propose_move(Move *move, Board &gb);
};


//---------------------------------------------------
//---------------------------------------------------
//controls game play, user interfacing, and file management
class Game{
public:
  friend class Player;
  Player *p1;
  Player *p2;
  Board gb;
  char turn; // p1 or p2 gives 'W' or 'B'
  bool game_over;
  std::string saver;
  Game(){ init(); }
  Game(const Game &gm){ p1 = gm.p1; p2 = gm.p2; gb = gm.gb; turn = gm.turn; game_over = gm.game_over; saver = gm.saver; }
  Game& operator=(const Game &gm){ return *this; }
  bool operator==(const Game &gm){ return this == &gm; }
  void init(){ turn = 'W'; game_over = false; p1 = new Human('W'); p2 = new Human('B'); }
  ~Game(){
    delete p1;
    delete p2;
  }
  // Game control functions
  void menu();
  void undo();
  void save_file();
  void save();
  bool file_exists(std::string &filename);
  void load_file(std::string &filename);
  void load();
  void restart();
  void invalid_input();
  bool is_sure(std::string &action);
  bool help();
  void goodbye();
  // Game play functions
  bool mode();
  void change_turn();
  void play(); 
};


#endif

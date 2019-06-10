#ifndef CHESS_LIB_HH
#define CHESS_LIB_HH

#include <iostream>
#include <string>
#include <map>
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


//---------------------------------------------------
//---------------------------------------------------
//a Position is a coordinate on the board
class Position{
public:
  int x;
  int y;
  Position(){ x = -1; y = -1; }
  Position(int ux, int uy){ init(ux, uy); }
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
  ~Piece(){}
  virtual void print(){}
  virtual void possible_moves(Position from, std::list<Position> &to){}
  virtual bool is_king(){ return false; }
  virtual bool is_legal(Move &move){ return true; }
};
class Pawn : public Piece{
public:
  Pawn(char c);
  ~Pawn(){}
  void print();
  bool is_legal(Move move){ return true; }
};
class Rook : public Piece{
public:
  Rook(char c);
  ~Rook(){}
  void print();
  bool is_legal(Move move){ return true; }
};
class Knight : public Piece{
public:
  Knight(char c);
  ~Knight(){}
  void print();
  bool is_legal(Move move){ return true; }
};
class Bishop : public Piece{
public:
  Bishop(char c);
  ~Bishop(){}
  void print();
  bool is_legal(Move move){ return true; }
};
class Queen : public Piece{
public:
  Queen(char c);
  ~Queen(){}
  void print();
  bool is_legal(Move move){ return true; }
};
class King : public Piece{
public:
  King(char c);
  ~King(){}
  void print();
  bool is_king(){ return true; }
  bool is_legal(Move move){ return true; }
};

class Graveyard{
public:
  std::list<Piece *> graveyard;
  Graveyard(){}
  ~Graveyard(){}
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
  void init(char ucolor){ color = ucolor; }
  virtual ~Player(){
    std::list<Move *>::iterator it;
    for( it = moves.begin(); it != moves.end(); it++ ){
      delete (*it);
    }
  }
  virtual bool is_human(){ return false; }
  // gameplay functions
  bool is_pos(std::string input);
  Position get_pos(std::string input, bool quantifier);
  //validates moves
  bool is_on_board(Position &pos);
  bool is_mine(Position &to, Board &gb);
  bool is_opponent(Position &to, Board &gb);
  bool is_open(Position &to, Board &gb);
  virtual bool is_valid(Position &pos, Board &gb, bool quantifier){
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
  std::string get_input(bool quantifier);
  bool is_valid(Position &pos, Board &gb, bool quantifier);
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
  void init(){ turn = 'W'; game_over = false; gb.init(); p1 = new Human('W'); p2 = new Human('B'); }
  ~Game(){
    delete p1, p2;
    gb.clear();
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

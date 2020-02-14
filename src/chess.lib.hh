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
//#include <math>

#define ROWS 8
#define COLUMNS 8

#define FROM true
#define TO false

enum color { null, white, black };

class Board;

//---------------------------------------------------
//---------------------------------------------------
//a Position is a coordinate on the board
class Position{
public:
  int x;
  int y;
  Position(){ x = -1; y = -1; }
  Position(int input_x, int input_y){ init(input_x, input_y); }
  Position(const Position &pos){ init(pos.x, pos.y); }
  Position(const std::string &input){ init((input[0] - '0'), (input[1] - '0')); }
  Position &operator=(const Position &pos){ init(pos.x, pos.y); return *this; }
  bool operator==(const Position &pos){ return (x == pos.x) && (y == pos.y);}
  Position &operator-(const Position &from){ x -= from.x; y -= from.y; return *this; }
  void init(int input_x, int input_y){ x = input_x; y = input_y; }
  void init_standard_notation(){}
  ~Position(){}
  void print();
  bool is_on_board();
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
  void print();
  bool is_legal(Board &gb);
};

// Piece classes
class Piece{
public:
  enum type { null, pawn, rook, knight, bishop, queen, king };
public:
  color m_color;
  type m_type;
  Piece(color c){ m_color = c; } //intialize Piece with given color, children handle type
  Piece(const Piece &pc){ m_color = pc.m_color; m_type = pc.m_type; }
  Piece& operator=(const Piece &other){ return *this; }
  bool operator==(const Piece &other){ return other.m_color == m_color && other.m_type == m_type; }
  ~Piece(){}
  virtual void print(){}
  virtual std::vector<Position> possible_moves(Position from, Board &gb){ std::vector<Position> pm; return pm; }
  virtual bool is_king(){ return false; }
};
class Pawn : public Piece{
public:
  Pawn(color c);
  ~Pawn(){}
  void print();
  std::vector<Position> possible_moves(Position from, Board &gb);
};
class Rook : public Piece{
public:
  Rook(color c);
  ~Rook(){}
  void print();
  std::vector<Position> possible_moves(Position from, Board &gb);
};
class Knight : public Piece{
public:
  Knight(color c);
  ~Knight(){}
  void print();
  std::vector<Position> possible_moves(Position from, Board &gb);
};
class Bishop : public Piece{
public:
  Bishop(color c);
  ~Bishop(){}
  void print();
  std::vector<Position> possible_moves(Position from, Board &gb);
};
class Queen : public Piece {
public:
  Queen(color c);
  ~Queen(){}
  void print();
  std::vector<Position> possible_moves(Position from, Board &gb);
};
class King : public Piece{
public:
  King(color c);
  ~King(){}
  void print();
  std::vector<Position> possible_moves(Position from, Board &gb);
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
  color turn; // p1 = white, p2 = black
  Board(){ init(); }
  Board(const Board &bd){
    for( int i = 0; i < ROWS; i++ ){
      for( int j = 0; j < COLUMNS; j++ ){
        board[i][j] = bd.board[i][j];
      }
    }
    gy = bd.gy;
    turn = bd.turn;
  }
  Board& operator=(const Board &bd){ return *this; }
  bool operator==(const Board &bd){ return &bd == this;}
  //Piece *operator[](const Position &pos){return board[pos.x][pos.y];}
  void init(){
    // clear board
    for( int i = 0; i < ROWS; i++ ){
      for( int j = 0; j < COLUMNS; j++ ){
        board[i][j] = nullptr;
      }
    }
    // black army allocation
    for( int j = 0; j < COLUMNS; j++){
      board[1][j] = new Pawn(black);
    }
    board[0][0] = new Rook(black);
    board[0][1] = new Knight(black);
    board[0][2] = new Bishop(black);
    board[0][3] = new King(black);
    board[0][4] = new Queen(black);
    board[0][5] = new Bishop(black);
    board[0][6] = new Knight(black);
    board[0][7] = new Rook(black);
   
    // white army allocation
    for( int j = 0; j < COLUMNS; j++){
      board[6][j] = new Pawn(white);
    }
    board[7][0] = new Rook(white);
    board[7][1] = new Knight(white);
    board[7][2] = new Bishop(white);
    board[7][3] = new King(white);
    board[7][4] = new Queen(white);
    board[7][5] = new Bishop(white);
    board[7][6] = new Knight(white);
    board[7][7] = new Rook(white);
    // starts on white's turn
    turn = white;
  }
  ~Board(){
    clear();
  }
  void print();
  void change_turn();
  void kill(Position &to);
  void make_move(Move *move);
  void clear();
  //validation methods
  bool is_open(Position pos);
  bool is_mine(Position pos);
  bool is_opponent(Position pos);
};


//---------------------------------------------------
//---------------------------------------------------
// a Player can be human or computer
class Player{
public:
  color m_color; //black or white
  std::list<Move *> moves;
  Player(){ m_color = white;}
  Player(color c){ init(c); }
  Player(const Player &plyr){ init(plyr.m_color); for(Move *move : plyr.moves) moves.push_back(move); }
  Player& operator=(const Player &plyr){ return *this; }
  bool operator==(const Player &plyr){ return &plyr == this; }
  void init(color c){ m_color = c; }
  virtual ~Player(){
    std::list<Move *>::iterator it;
    for( it = moves.begin(); it != moves.end(); it++ ){
      delete (*it);
    }
  }
   //validates moves
  virtual bool is_valid(Position pos, Board &gb, bool quantifier){
    return gb.is_mine(pos) && quantifier || gb.is_opponent(pos) && !quantifier || gb.is_open(pos) && !quantifier;
  }
  //move proposal done by children
  virtual bool propose_move(Move *move, Board &gb){ return false; }
};
class Human : public Player{
public:
  Human(){}
  Human(color c){ Player::init(c); }
  ~Human(){}
  bool is_pos(std::string input);
  std::string get_input(bool quantifier);
  bool is_valid(Position pos, Board &gb, bool quantifier);
  bool propose_move(Move *move, Board &gb);
};
class CPU : public Player{
public:
  CPU(){}
  CPU(color c){ Player::init(c); }
  ~CPU(){}
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
  bool game_over;
  std::string saver;
  Game(){ init(); }
  Game(const Game &gm){ p1 = gm.p1; p2 = gm.p2; gb = gm.gb; game_over = gm.game_over; saver = gm.saver; }
  Game& operator=(const Game &gm){ return *this; }
  bool operator==(const Game &gm){ return this == &gm; }
  void init(){ game_over = false; p1 = new Human(white); p2 = new Human(black); }
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
  void play(); 
};


#endif

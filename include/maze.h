#ifndef _MAZE_H_
#define _MAZE_H_

    #include <vector>
    #include <string>
    #include <stdexcept>
    #include <fstream>
    #include <iostream>
    #include <ostream>
    #include <stack>

    class Maze
    {
        public: 

            enum turn_t { NONE, UP, RIGHT, DOWN, LEFT };
            enum cell_t { BEGIN, WALL, END, WAY };

            struct Direction
            {
                enum turn_t turn;
                int line;
                int column;

                Direction( turn_t t_ = NONE )
                    : turn( t_ )
                {
                    line = 0;
                    column = 0;
                    if( t_ == UP ) line -= 1;
                    else if( t_ == DOWN ) line += 1;
                    else if( t_ == LEFT ) column -= 1;
                    else if( t_ == RIGHT ) column += 1;
                }
            };
            
            struct Position
            {
                size_t line;
                size_t column;
                
                Position( size_t l_ = 0, size_t c_ = 0 )
                    : line( l_ )
                    , column ( c_ )
                { /* empty */ }

                friend Position operator+( const Position &position_, const Direction &direction_ )
                {
                    auto temp = position_;
                    return Position( temp.line + direction_.line,
                                    temp.column += direction_.column );
                }
            };

            struct Cell
            {
                enum cell_t type;
                enum turn_t last_turn;
                bool marked;
                char value;

                Cell( char v_ = ' ' )
                    : value( v_ )
                {
                    type = decide_cell_type( v_ );
                    last_turn = turn_t::NONE;
                    marked = false;
                }

                cell_t decide_cell_type( char char_ )
                {
                    switch( char_ )
                    {
                        case ' ':
                            return cell_t::WAY;
                        case '#':
                            return cell_t::WALL;
                        case '*':
                            return cell_t::BEGIN;
                        case '$':
                            return cell_t::END;
                        default:
                            return cell_t::WAY;
                    }
                }

                friend bool operator ==( Cell &lhs_, cell_t rhs_ )
                {
                    return lhs_.type == rhs_;
                }

                friend std::ostream & operator <<( std::ostream &os_, Cell &cell_ )
                {
                    if( cell_.marked == true )
                    {
                        os_ << "\u2591\u2591";
                        return os_;
                    }
                    
                    switch( cell_.type )
                    {
                        case cell_t::BEGIN:
                            os_ << "\u2690 ";
                            break;
                        case cell_t::END:
                            os_ << "\u2691 ";
                            break;
                        case cell_t::WAY:
                            os_ << "  ";
                            break;
                        case cell_t::WALL:
                            os_ << "\u2588\u2588";
                            break;
                    }
                    return os_;
                }
            };

            //==> Alias
            using board_t = std::vector< std::vector< Cell > >;
            using path_t = std::stack< Position >;

        private:
            board_t m_board;        // <! A matriz containing NxM cells.
            Position m_curr_pos;    // <! The beginning position of maze.
            path_t m_way;           // <! A stack with the path of positions.

        public:
            //===> CONSTRUCTOR
            Maze() = default;

            //===> AUXILIARIES
            bool is_outside( const Position &position_ );
            bool is_blocked( const Position &position_, const Direction &direction_ );
            bool is_marked( const Position &position_, const Direction &direction_ );
            void next_turn( turn_t &last_turn_ );

            //===> MODIFIERS
            void mark_cell( const Position &position_ );
            void unmark_cell( const Position &position_ );
            void get_board( std::string filename_ );

            //===> FUNCTIONALS
            Position get_start_position( void );
            Position advance_one( void );
            size_t get_steps( void );

            //===> OPERATORS
            friend std::ostream & operator<<( std::ostream &os_, Maze &board_ );
    };

#endif
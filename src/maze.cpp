#include "maze.h"

/*
 * @brief According to the maze, returns the position of the begin.
 */
Maze::Position Maze::get_start_position( void )
{
    for( auto i(0ul); i != m_board.size(); ++i )
    {
        for( auto j(0ul); j != m_board[i].size(); ++j )
        {
            if( m_board[i][j] == Maze::cell_t::BEGIN )
            {
                m_curr_pos = Position( i, j );
                m_way.push( m_curr_pos );
                return Position( i, j );
            }
        }
    }
    return m_curr_pos;
}

/*
 * @brief Analyzes if the end was achieved.
 */
bool Maze::is_outside( const Position &position_ )
{
    return m_board[position_.line][position_.column] == Maze::cell_t::END;
}

/*
 * @brief Analyzes whether the assumed position to be played is blocked.
 */
bool Maze::is_blocked( const Position &position_, const Direction &direction_ )
{
    auto next_step = position_ + direction_;
    return m_board[next_step.line][next_step.column] == Maze::cell_t::WALL 
            or direction_.turn == Maze::turn_t::NONE;
}

/*
 * @brief Sets the current cell as marked.
 */
void Maze::mark_cell( const Position &position_ )
{
    m_board[position_.line][position_.column].marked = true;
}

/*
 * @brief Sets the current cell as not marked.
 */
void Maze::unmark_cell( const Position &position_ )
{
    m_board[position_.line][position_.column].marked = false;
}

/*
 * @brief Analyzes whether the assumed position to be played is marked.
 */
bool Maze::is_marked( const Position &position_, const Direction &direction_ )
{
    auto next_step = position_ + direction_;
    return m_board[next_step.line][next_step.column].marked == true;
}

/*
 * @brief According to the inputted file path, fills the matrix with the values (chars).
 */
void Maze::get_board( std::string filename_ )
{
    std::ifstream file( filename_ );

    if( not file.is_open() )
    {
        throw std::invalid_argument( "get_board(): Error reading the file!\n" );
    }
    else
    {
        size_t height, width;
        std::string line;

        file >> height >> width;
        std::getline( file, line );

        for( auto i(0ul); i != height; ++i )
        {
            std::vector< Maze::Cell > row;
            std::getline( file, line );
            for( auto j(0ul); j != width; ++j )
            {
                auto temp = Cell( line[j] );
                row.push_back( temp );
            }
            m_board.push_back( row );
        }
    }
}

/*
 * @brief Operator << overload. It prints the matrix containing the maze.
 */
std::ostream & operator <<( std::ostream &os_, Maze &maze_ )
{
    for( auto i(0ul); i != maze_.m_board.size(); ++i )
    {
        for( auto j(0ul); j != maze_.m_board[i].size(); ++j )
        {
            if( maze_.m_curr_pos.line == i and maze_.m_curr_pos.column == j )
                os_ << "\u26D1 ";
            else
                os_ << maze_.m_board[i][j];
        }
        os_ << "\n";
    }
    return os_;
}

/*
 * @brief Decides where is the next step is to take in the maze.
 */
Maze::Position Maze::advance_one( void )
{
    Maze::turn_t last_turn_temp = m_board[m_curr_pos.line][m_curr_pos.column].last_turn;
    auto temp_can_advance(false);

    while( last_turn_temp != Maze::turn_t::LEFT )
    {
        next_turn ( last_turn_temp );
        if( ( is_blocked( m_curr_pos, last_turn_temp ) ) or ( is_marked( m_curr_pos, last_turn_temp ) ) )
        {
            temp_can_advance = false;
        }
        else
        {
            temp_can_advance = true;
            break;
        }
    }

    if( temp_can_advance == false ){   
        if( m_board[m_curr_pos.line][m_curr_pos.column].value == '*' )
        {
            std::cout << ">>> Maze without an end!\n";
            exit(0);
        }
        else
        {
            unmark_cell( m_curr_pos );
            m_curr_pos = m_way.top();
            m_way.pop();
            return m_curr_pos;
        }
    }
    m_way.push( m_curr_pos );
    mark_cell( m_curr_pos );
    m_board[m_curr_pos.line][m_curr_pos.column].last_turn = last_turn_temp;
    m_curr_pos = m_curr_pos + last_turn_temp;
    
    return m_curr_pos;
}

/*
 * @brief The turns follow a way. And this function decides it.
 */
void Maze::next_turn( Maze::turn_t &last_turn_ )
{
    if( last_turn_ == Maze::turn_t::NONE ) last_turn_ = Maze::turn_t::UP;
    else if( last_turn_ == Maze::turn_t::UP ) last_turn_ = Maze::turn_t::RIGHT;
    else if( last_turn_ == Maze::turn_t::RIGHT ) last_turn_ = Maze::turn_t::DOWN;
    else if( last_turn_ == Maze::turn_t::DOWN ) last_turn_ = Maze::turn_t::LEFT;
    else last_turn_ = Maze::turn_t::LEFT;
}

/*
 * @brief Returns the length of path represented by the stack.
 */
size_t Maze::get_steps( void )
{
    return m_way.size();
}
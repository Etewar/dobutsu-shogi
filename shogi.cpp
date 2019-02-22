#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <algorithm>

class Piece
{
  public:
    enum TYPE
    {
        KING,
        ROOK,
        BISHOP,
        PAWN,
        EMPTY
    };

    TYPE piece_type;
    std::string name;
    bool upgraded = false;
    char location;                     // ..-1 - on hand | 0..11 - on board | 12.. - neither on hand nor on board
    std::array<bool, 9> allowed_moves; // {  "top left", "top", "top right",
                                       //    "left", "hand", "right",
                                       //    "bottom left", "bottom", "bottom right" }

    Piece()
    {
        piece_type = Piece::TYPE::EMPTY;
        name = "empty";
        location = 12;
        allowed_moves = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    }

    Piece(Piece::TYPE piece_type, int8_t location)
    {
        piece_type = piece_type;
        location = location;
        switch (piece_type)
        {
        case Piece::TYPE::KING:
            name = "king";
            allowed_moves = {1, 1, 1, 1, 0, 1, 1, 1, 1};
            break;
        case Piece::TYPE::ROOK:
            name = "rook";
            allowed_moves = {0, 1, 0, 1, 0, 1, 0, 1, 0};
            break;
        case Piece::TYPE::BISHOP:
            name = "bishop";
            allowed_moves = {1, 0, 1, 0, 0, 0, 1, 0, 1};
            break;
        case Piece::TYPE::PAWN:
            name = "pawn";
            allowed_moves = {0, 1, 0, 0, 0, 0, 0, 0, 0};
            break;
        default:
            allowed_moves = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            break;
        }
    }

    void upgrade()
    {
        upgraded = true;
        switch (piece_type)
        {
        case Piece::TYPE::PAWN:
            name = "upawn";
            allowed_moves = {1, 1, 1, 1, 0, 1, 0, 1, 0};
            break;
        }
    }
};

class Player
{
  public:
    std::string name;
    std::vector<Piece *> pieces;
};

class Game
{
  public:
    bool starting_player; // players[0] or players[1]
    Player players[2];
    unsigned short turn;
    std::array<Piece *, 12> board;
    /* Initial setup:
     * rook king bishop
     * empty pawn empty
     * empty pawn empty
     * bishop king rook
     */
    std::array<Piece, 9> pieces_in_game = {
        Piece(Piece::TYPE::KING, 1),
        Piece(Piece::TYPE::KING, 10),
        Piece(Piece::TYPE::ROOK, 0),
        Piece(Piece::TYPE::ROOK, 11),
        Piece(Piece::TYPE::BISHOP, 2),
        Piece(Piece::TYPE::BISHOP, 9),
        Piece(Piece::TYPE::PAWN, 4),
        Piece(Piece::TYPE::PAWN, 7),
        Piece(),
    };

    bool active_player()
    {
        return (turn + starting_player + 1) % 2;
    }

    int move(unsigned short initial_location, unsigned short final_location)
    {
        bool active = active_player();
        bool inactive = (active + 1) % 2;
        std::vector<Piece *> active_player_pieces = players[active].pieces;
        std::vector<Piece *> inactive_player_pieces = players[inactive].pieces;

        // if locations are correct
        if (!(0 <= initial_location < 12 && 0 <= final_location < 12))
            return 1; //errno1 - initial or final location incorrect

        // if there is no piece in initial location
        if (board[initial_location] == &pieces_in_game[8])
            return 2; // errno2 - no piece in initial location

        // if piece is owned by active player
        if (std::find(active_player_pieces.begin(), active_player_pieces.end(), board[initial_location]) == active_player_pieces.end())
            return 3; // errno3 - active player doesn't own moved piece

        // if there is piece owned by active player in final location
        if (std::find(active_player_pieces.begin(), active_player_pieces.end(), board[final_location]) != active_player_pieces.end())
            return 4; // errno4 - piece owned by active player in final location

        // if piece can move that way
        if (board[initial_location]->allowed_moves[4 + (active - 1) * (final_location - initial_location)])
        {
            // if there is no piece in final location
            if (board[final_location] == &pieces_in_game[8])
            {
                board[final_location] = board[initial_location];
                board[initial_location] = &pieces_in_game[8];
            }
            // if there is inactive player piece in final location
            else if (std::find(inactive_player_pieces.begin(), inactive_player_pieces.end(), board[final_location]) != inactive_player_pieces.end())
            {
                active_player_pieces.push_back(inactive_player_pieces[std::find(inactive_player_pieces.begin(), inactive_player_pieces.end(), board[final_location]) - inactive_player_pieces.begin()]);
                inactive_player_pieces.erase(std::find(inactive_player_pieces.begin(), inactive_player_pieces.end(), board[final_location]));
                board[final_location] = board[initial_location];
                board[initial_location] = &pieces_in_game[8];
            }
        }
    }

    void place()
    {
    }

    Game()
    {
    }

    Game(Player player1, Player player2, bool starting)
    {
        players[0] = player1;
        players[1] = player2;
        starting_player = starting;
        turn = 1;

        players[0].pieces = {
            &pieces_in_game[0],
            &pieces_in_game[2],
            &pieces_in_game[4],
            &pieces_in_game[6],
        };
        players[1].pieces = {
            &pieces_in_game[1],
            &pieces_in_game[3],
            &pieces_in_game[5],
            &pieces_in_game[7],
        };

        board = {
            &pieces_in_game[2],
            &pieces_in_game[0],
            &pieces_in_game[4],
            &pieces_in_game[8],
            &pieces_in_game[6],
            &pieces_in_game[8],
            &pieces_in_game[8],
            &pieces_in_game[7],
            &pieces_in_game[8],
            &pieces_in_game[5],
            &pieces_in_game[1],
            &pieces_in_game[3],
        };
    }
};

int main()
{
    Player pl1, pl2;
    pl1.name = "Player 1";
    pl2.name = "Player 2";
    Game game = Game(pl1, pl2, 0);
    game.turn = 1;
    std::cout << game.board[4]->name << std::endl;
    std::cout << game.board[7]->name << std::endl;
    std::cout << game.players[0].pieces.size() << std::endl;
    game.move(4, 7);
    std::cout << game.board[4]->name << std::endl;
    std::cout << game.board[7]->name << std::endl;
    std::cout << game.players[0].pieces.size() << std::endl;
}
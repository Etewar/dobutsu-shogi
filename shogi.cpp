#include <iostream>
#include <array>
#include <map>

class Player;

class Piece
{
  public:
    enum TYPE
    {
        KING,
        ROOK,
        BISHOP,
        PAWN
    };

    TYPE piece_type;
    bool upgraded = false;
    std::array<bool, 9> allowed_moves; // {  "top left", "top", "top right",
                                       //    "left", "hand", "right",
                                       //    "bottom left", "bottom", "bottom right" }

    Piece(Piece::TYPE piece_type)
    {
        piece_type = piece_type;
        switch (piece_type)
        {
        case Piece::TYPE::KING:
            allowed_moves = {1, 1, 1, 1, 0, 1, 1, 1, 1};
            break;
        case Piece::TYPE::ROOK:
            allowed_moves = {0, 1, 0, 1, 0, 1, 0, 1, 0};
            break;
        case Piece::TYPE::BISHOP:
            allowed_moves = {1, 0, 1, 0, 0, 0, 1, 0, 1};
            break;
        case Piece::TYPE::PAWN:
            allowed_moves = {0, 1, 0, 0, 0, 0, 0, 0, 0};
            break;
        default:
            allowed_moves = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        }
    }

    static void upgrade(Piece piece)
    {
        piece.upgraded = true;
        switch (piece.piece_type)
        {
        case Piece::TYPE::PAWN:
            piece.allowed_moves = {1, 1, 1, 1, 0, 1, 0, 1, 0};
            break;
        }
    }

    static void kill(Piece piece, Player killer)
    {
    }
};

class Player
{
  public:
    std::string name;
    std::map<Piece::TYPE, unsigned short> hand;

    void get_piece(Piece::TYPE piece_type)
    {
        hand[piece_type] += 1;
    }
};

int main()
{
    Player pl;
    pl.get_piece(Piece::TYPE::KING);
    std::cout << pl.hand[Piece::TYPE::KING] << std::endl;
}
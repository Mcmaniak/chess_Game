#ifndef STRUCTURES_H
#define STRUCTURES_H
#pragma once
#include <string>

enum class PieceType { Empty, Pawn, Rook, Knight, Bishop, Queen, King };
#include "utils.h"
enum class PieceColor { None, Black, White };

struct ChessPiece {
    PieceType type;
    std::string symbol;
    PieceColor color;

    // Default constructor
    ChessPiece() : type(PieceType::Empty), symbol(""), color(PieceColor::None) {}

    // Constructor with arguments
    ChessPiece(PieceType t, const std::string& s, PieceColor c) : type(t), symbol(s), color(c) {}
};

// Define a structure to represent a square on the chessboard
struct Square {
    int row;
    int col;
};

#endif // STRUCTURES_H

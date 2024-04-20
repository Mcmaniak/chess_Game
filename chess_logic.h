#ifndef CHESS_LOGIC_H
#define CHESS_LOGIC_H
#include <vector>
#include "structures.h"
#include "utils.h"
extern std::vector<std::vector<ChessPiece>> board;

// Function prototypes for piece movement validation
bool isValidPawnMove(const Square &sourceSquare, const Square &targetSquare, bool isWhiteTurn, bool enPassantPossible, Square enPassantSquare);
bool isValidKnightMove(Square currentSquare, Square targetSquare);
bool isValidBishopMove(Square currentSquare, Square targetSquare);
bool isValidRookMove(Square currentSquare, Square targetSquare);
bool isValidQueenMove(Square currentSquare, Square targetSquare);
bool isValidKingMove(Square currentSquare, Square targetSquare);

// Function prototypes for game logic
bool isCheckmate(Square sourceSquare, Square targetSquare, bool isWhiteTurn);
bool isCheck(const std::vector<std::vector<ChessPiece>>& board, PieceColor kingColor, const Square& kingPosition);
bool isValidMove(const std::vector<std::vector<ChessPiece>>& board, Square sourceSquare, Square targetSquare, bool isWhiteTurn, bool enPassantPossible, Square enPassantSquare, PieceColor playerColor);
void makeMove(std::vector<std::vector<ChessPiece>>& board, Square sourceSquare, Square targetSquare);
bool isSquareEmpty(const std::vector<std::vector<ChessPiece>>& board, Square square);
bool isSquareOccupiedByOpponent(const std::vector<std::vector<ChessPiece>>& board, Square square, bool isWhiteTurn);
bool isSquareOccupiedByPlayer(const std::vector<std::vector<ChessPiece>>& board, Square square, PieceColor pieceColor);
Square getEnPassantSquare(const std::vector<std::vector<ChessPiece>>& board, Square targetSquare, bool isWhiteTurn); 
bool isPieceBlocking(const std::vector<std::vector<ChessPiece>>& board, const Square& sourceSquare, const Square& targetSquare);
bool isSquareValid(Square square);
bool enPassantPossible(const std::vector<std::vector<ChessPiece>>& board, Square sourceSquare, Square targetSquare, bool isWhiteTurn);
void getUserMove(Square &sourceSquare, Square &targetSquare, PieceColor playerColor);

#endif

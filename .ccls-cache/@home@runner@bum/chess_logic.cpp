#include "chess_logic.h"
#include <cmath>
#include <iostream>
#include <cctype>
#include "utils.h"
#include "structures.h"

const int BOARD_SIZE = 8;

void getUserMove(Square &sourceSquare, Square &targetSquare, PieceColor pieceColor) {
    std::string input;
    std::cout << "Enter your move (e.g., e2e4): ";
    std::cin >> input;

    std::cout << "Input move: " << input << std::endl;

    // Convert algebraic notation to Square coordinates
    if (input.length() != 4) {
        std::cout << "Invalid move format. Please enter in the format 'e2e4'." << std::endl;
        getUserMove(sourceSquare, targetSquare, pieceColor); // Recursive call to get valid input
        return;
    }

    // Convert column characters to integers
    int sourceCol = input[0] - 'a';
    int targetCol = input[2] - 'a';

    // Convert row characters to integers
    int sourceRow = 8 - (input[1] - '0');
    int targetRow = 8 - (input[3] - '0');

    // Check if the input coordinates are valid
    if (sourceCol < 0 || sourceCol > 7 || targetCol < 0 || targetCol > 7 ||
        sourceRow < 0 || sourceRow > 7 || targetRow < 0 || targetRow > 7) {
        std::cout << "Invalid coordinates. Please enter valid coordinates." << std::endl;
        getUserMove(sourceSquare, targetSquare, pieceColor); // Recursive call to get valid input
        return;
    }

    // Update sourceSquare and targetSquare
    sourceSquare = {sourceRow, sourceCol};
    targetSquare = {targetRow, targetCol};
}

bool isValidPawnMove(const Square &sourceSquare, const Square &targetSquare, bool isWhiteTurn, bool enPassantPossible, Square enPassantSquare) {
  // Debug statements
  //std::cout << "Source square: " << sourceSquare.row << ", " << sourceSquare.col << std::endl;
  //std::cout << "Target square: " << targetSquare.row << ", " << targetSquare.col << std::endl;

  // Calculate row and column differences
  int rowDiff = targetSquare.row - sourceSquare.row;
  int colDiff = abs(targetSquare.col - sourceSquare.col);

  // Pawn moves
  if (colDiff == 0) {
    if (isWhiteTurn) {
      // Normal move or initial two-square move for white pawn
      if (rowDiff == -1 || (rowDiff == -2 && sourceSquare.row == 6)) {
        // Check if the path is clear for the two-square move
        if (rowDiff == -2) {
          int intermediateRow = sourceSquare.row - 1;
          if (!isSquareEmpty(board, {intermediateRow, sourceSquare.col})) {
            return false;
          }
        }
        return true;
      }
    } else {
      // Normal move or initial two-square move for black pawn
      if (rowDiff == 1 || (rowDiff == 2 && sourceSquare.row == 1)) {
        // Check if the path is clear for the two-square move
        if (rowDiff == 2) {
          int intermediateRow = sourceSquare.row + 1;
          if (!isSquareEmpty(board, {intermediateRow, sourceSquare.col})) {
            return false;
          }
        }
        return true;
      }
    }
  }

  // Check for diagonal capture (up to one square difference in column)
  if (colDiff == 1) {
    // Check if the pawn captures an opponent's piece diagonally
    if ((isWhiteTurn && rowDiff == -1) || (!isWhiteTurn && rowDiff == 1)) {
      // Check if the target square is occupied by an opponent's piece
      if (!isSquareEmpty(board, targetSquare) && isSquareOccupiedByOpponent(board, targetSquare, isWhiteTurn)) {
        return true;
      }
      // Check for en passant capture
      if (enPassantPossible && targetSquare.row == enPassantSquare.row && targetSquare.col == enPassantSquare.col) {
        return true;
      }
    }
  }

  return false; // Default case: Invalid move
}

// Function to check if a move is valid for a knight
bool isValidKnightMove(Square currentSquare, Square targetSquare) {
    int rowDiff = abs(targetSquare.row - currentSquare.row);
    int colDiff = abs(targetSquare.col - currentSquare.col);

    // Knight move (L-shape)
    if ((rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1)) {
        return true;
    }

    return false;
}

// Function to check if a move is valid for a bishop
bool isValidBishopMove(Square currentSquare, Square targetSquare) {
    int rowDiff = abs(targetSquare.row - currentSquare.row);
    int colDiff = abs(targetSquare.col - currentSquare.col);

    // Bishop move (diagonal)
    if (rowDiff == colDiff) {
        return true;
    }

    return false;
}

// Function to check if a move is valid for a rook
bool isValidRookMove(Square currentSquare, Square targetSquare) {
    int rowDiff = abs(targetSquare.row - currentSquare.row);
    int colDiff = abs(targetSquare.col - currentSquare.col);

    // Rook move (horizontal or vertical)
    if (rowDiff == 0 || colDiff == 0) {
        return true;
    }

    return false;
}

// Function to check if a move is valid for a queen
bool isValidQueenMove(Square currentSquare, Square targetSquare) {
    return isValidBishopMove(currentSquare, targetSquare) || isValidRookMove(currentSquare, targetSquare);
}

// Function to check if a move is valid for a king
bool isValidKingMove(Square currentSquare, Square targetSquare) {
    int rowDiff = abs(targetSquare.row - currentSquare.row);
    int colDiff = abs(targetSquare.col - currentSquare.col);

    // King move (one square in any direction)
    if (rowDiff <= 1 && colDiff <= 1) {
        return true;
    }

    return false;
}
bool isCheckmate(Square sourceSquare, Square targetSquare, bool isWhiteTurn) {
    // Placeholder logic: Assume no checkmate for now
    return false;
}

bool isCheck(const std::vector<std::vector<ChessPiece>>& board, PieceColor kingColor, const Square& kingPosition) {
    // Iterate through the board to find opponent pieces
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            const ChessPiece& piece = board[row][col];
            if (piece.color != kingColor && piece.type != PieceType::Empty) { // Check if the piece belongs to the opponent
                Square sourceSquare = {row, col};
                // Check if the opponent piece can legally move to the king's position
                if (isValidMove(board, sourceSquare, kingPosition, false, false, {}, piece.color)) {
                    // Check if there are any pieces between the opponent piece and the king
                    if (!isPieceBlocking(board, sourceSquare, kingPosition)) {
                        std::cout << "King is in check by piece at: " << row << ", " << col << std::endl;
                        return true; // King is in check
                    }
                }
            }
        }
    }
    return false; // King is not in check
}


bool isPieceBlocking(const std::vector<std::vector<ChessPiece>>& board, const Square& sourceSquare, const Square& targetSquare) {
    int rowDiff = targetSquare.row - sourceSquare.row;
    int colDiff = targetSquare.col - sourceSquare.col;

    // Horizontal or vertical movement
    if (rowDiff == 0 || colDiff == 0) {
        // Check if any piece blocks the path
        int stepRow = (rowDiff == 0) ? 0 : ((rowDiff > 0) ? 1 : -1);
        int stepCol = (colDiff == 0) ? 0 : ((colDiff > 0) ? 1 : -1);
        for (int row = sourceSquare.row + stepRow, col = sourceSquare.col + stepCol;
             row != targetSquare.row || col != targetSquare.col;
             row += stepRow, col += stepCol) {
            if (board[row][col].type != PieceType::Empty) {
                return true; // There is a piece blocking the path
            }
        }
    }

    // Diagonal movement
    if (abs(rowDiff) == abs(colDiff)) {
        // Check if any piece blocks the path
        int stepRow = (rowDiff > 0) ? 1 : -1;
        int stepCol = (colDiff > 0) ? 1 : -1;
        for (int row = sourceSquare.row + stepRow, col = sourceSquare.col + stepCol;
             row != targetSquare.row || col != targetSquare.col;
             row += stepRow, col += stepCol) {
            if (board[row][col].type != PieceType::Empty) {
                return true; // There is a piece blocking the path
            }
        }
    }

    return false; // No piece is blocking the path
}



bool isValidMove(const std::vector<std::vector<ChessPiece>>& board, Square sourceSquare, Square targetSquare, bool isWhiteTurn, bool enPassantPossible, Square enPassantSquare, PieceColor playerColor) {
    //std::cout << "Checking if source square is occupied by player's piece..." << std::endl;
    // Check if the source square is occupied by the player's piece

    if (!isSquareOccupiedByPlayer(board, sourceSquare, playerColor)) {
        //std::cout << "Source square is not occupied by player's piece." << std::endl;
        return false;
    }

    // Check if target square is within board bounds
    if (!isSquareValid(targetSquare)) {
        return false;
    }

    // Check for diagonal capture (up to one square difference in column)
    int colDiff = abs(targetSquare.col - sourceSquare.col);
    if (colDiff <= 1) {
        // Check if capturing an opponent's piece diagonally based on turn
        if ((!isWhiteTurn && targetSquare.row - sourceSquare.row == -1 && !isSquareEmpty(board, targetSquare)) ||
            (isWhiteTurn && targetSquare.row - sourceSquare.row == 1 && !isSquareEmpty(board, targetSquare))) {
            if (isSquareOccupiedByOpponent(board, targetSquare, isWhiteTurn)) {
                return true; // Valid capture move
            }
        }
    }

    // Invalid capture from empty square (e.g., trying to capture a non-existent piece)
    if (isSquareEmpty(board, sourceSquare)) {
        return false;
    }

    //std::cout << "Checking if square is occupied by opponent..." << std::endl;
    // Check if the move is valid for the specific piece type
    switch (board[sourceSquare.row][sourceSquare.col].type) {
        case PieceType::Pawn:
            return
              isValidPawnMove(sourceSquare,targetSquare,isWhiteTurn,enPassantPossible, enPassantSquare);
        case PieceType::Knight:
            return isValidKnightMove(sourceSquare, targetSquare);
        case PieceType::Bishop:
            return isValidBishopMove(sourceSquare, targetSquare);
        case PieceType::Rook:
            return isValidRookMove(sourceSquare, targetSquare);
        case PieceType::Queen:
            return isValidQueenMove(sourceSquare, targetSquare);
        case PieceType::King:
            return isValidKingMove(sourceSquare, targetSquare);
        default:
            return false; // Invalid piece type
    }
}


void makeMove(std::vector<std::vector<ChessPiece>>& board, Square sourceSquare, Square targetSquare) {
    // Store the piece from the source square
    ChessPiece movedPiece = board[sourceSquare.row][sourceSquare.col];

    // Clear the source square
    board[sourceSquare.row][sourceSquare.col] = ChessPiece();

    // Move the piece to the target square
    board[targetSquare.row][targetSquare.col] = movedPiece;
}



bool isSquareOccupiedByOpponent(const std::vector<std::vector<ChessPiece>>& board, Square square, bool isWhiteTurn) {
    // Check if the square is empty
    if (isSquareEmpty(board, square)) {
        return false;
    }

    // Get the color of the piece on the square
    PieceColor squareColor = board[square.row][square.col].color;
    PieceColor playerColor = isWhiteTurn ? PieceColor::White : PieceColor::Black;

    // Check if the piece color matches the opponent's color
    if (squareColor != playerColor) {
        return true;
    }

    // If the square is occupied by the player's piece, return false
    return false;
}

bool isSquareOccupiedByPlayer(const std::vector<std::vector<ChessPiece>>& board, Square square, PieceColor playerColor) {
    //std::cout << "Function called for square: " << square.row << ", " << square.col << std::endl;

    // Check if the square is within the board bounds
    if (!isSquareValid(square)) {
        return false;
    }

    // Check if the square is occupied
    if (board[square.row][square.col].type != PieceType::Empty) {
        // Output piece information
        //std::cout << "Piece on square: " << pieceTypeToString(board[square.row][square.col].type) << std::endl;
        //std::cout << "Color of piece: " << (board[square.row][square.col].color == PieceColor::White ? "White" : "Black") << std::endl;
        //std::cout << "Player color: " << (playerColor == PieceColor::White ? "White" : "Black") << std::endl;

        // Check if the occupied piece belongs to the player
        if (board[square.row][square.col].color == playerColor) {
            //std::cout << "Square is occupied by player's piece." << std::endl;
            return true;
        } else {
            //std::cout << "Piece color on square does not match player color." << std::endl;
            return false;
        }
    }

    //std::cout << "Square is empty." << std::endl;
    return false; // Square is not occupied
}

bool isSquareEmpty(const std::vector<std::vector<ChessPiece>>& board, Square square) {
    // Check if the square is within the board bounds
    if (square.row < 0 || square.row >= board.size() || square.col < 0 || square.col >= board[0].size()) {
        return false; // Square is out of bounds, not empty
    }

    // Check if the piece on the square is empty (i.e., PieceType::Empty)
    return board[square.row][square.col].type == PieceType::Empty;
}

bool isSquareValid(Square square) {
  //std::cout << "Checking validity of square: " << square.row << ", " << square.col << std::endl;
  bool valid = square.row >= 0 && square.row < 8 && square.col >= 0 && square.col < 8;
  //std::cout << "Is square valid? " << (valid ? "Yes" : "No") << std::endl;
  return valid;
} 

bool enPassantPossible(const std::vector<std::vector<ChessPiece>>& board, Square sourceSquare, Square targetSquare, bool isWhiteTurn) {
    // Check if the moved piece is a pawn
    if (board[sourceSquare.row][sourceSquare.col].type == PieceType::Pawn) {
        // Check if the pawn moved two squares forward
        int rowDiff = abs(targetSquare.row - sourceSquare.row);
        if (rowDiff == 2) {
            // Check if the pawn is on the correct rank for en passant capture
            int rankForEnPassant = isWhiteTurn ? 3 : 4; // Rank where the opponent's pawn must be to allow en passant capture
            if (targetSquare.row == rankForEnPassant) {
                // Check if there is an opponent's pawn next to the target square
                int opponentPawnRow = isWhiteTurn ? 4 : 3; // Row where the opponent's pawn should be
                if (board[opponentPawnRow][targetSquare.col].type == PieceType::Pawn &&
                    board[opponentPawnRow][targetSquare.col].color != board[sourceSquare.row][sourceSquare.col].color) {
                    return true;
                }
            }
        }
    }
    return false;
}

Square getEnPassantSquare(const std::vector<std::vector<ChessPiece>>& board, Square targetSquare, bool isWhiteTurn) {
    // Initialize the en passant square to indicate en passant is not possible
    Square enPassantSquare = {-1, -1};

    // Check if the target square is on the correct rank for en passant capture
    int rankForEnPassant = isWhiteTurn ? 3 : 4; // Rank where the opponent's pawn must be to allow en passant capture
    if (targetSquare.row == rankForEnPassant) {
        // Determine the file of the potential en passant square
        int enPassantCol = targetSquare.col - 1; // File to the left of the target square for white pawns, or to the right for black pawns

        // Check if the potential en passant square is within the board bounds
        if (enPassantCol >= 0 && enPassantCol < 8) {
            // En passant capture is possible at the potential square
            enPassantSquare = {rankForEnPassant, enPassantCol};
        }
    }

    return enPassantSquare;
}

std::string pieceTypeToString(PieceType type) {
    switch (type) {
        case PieceType::Pawn:
            return "Pawn";
        case PieceType::Rook:
            return "Rook";
        case PieceType::Knight:
            return "Knight";
        case PieceType::Bishop:
            return "Bishop";
        case PieceType::Queen:
            return "Queen";
        case PieceType::King:
            return "King";
        case PieceType::Empty:
            return "Empty";
        default:
            return "Unknown";
    }
}


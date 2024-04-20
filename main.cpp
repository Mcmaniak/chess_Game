#include <iostream>
#include <vector>
#include <string>
#include "chess_logic.h"
#include "utils.h"
#include "structures.h"

using namespace std;

// Define the chessboard
const int BOARD_SIZE = 8;
std::vector<std::vector<ChessPiece>> board(8, std::vector<ChessPiece>(8));
Square targetSquare; // Initialize this variable with the appropriate value
bool isWhiteTurn; // Initialize this variable with the appropriate value
Square sourceSquare;
// Initialize enPassantPossible variable properly
void initializeBoard() {
    for (int col = 0; col < BOARD_SIZE; col++) {
        board[1][col] = ChessPiece(PieceType::Pawn, "♙", PieceColor::Black);
        board[6][col] = ChessPiece(PieceType::Pawn, "♟", PieceColor::White);
    }

    // Initialize empty squares
    for (int row = 2; row < 6; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = ChessPiece(PieceType::Empty, "", PieceColor::None); // Set color to None for empty squares
        }
    }

    // Place other pieces for one side (white)
    board[0][0] = ChessPiece(PieceType::Rook, "♖", PieceColor::Black);
    board[0][7] = ChessPiece(PieceType::Rook, "♖", PieceColor::Black);
    board[0][1] = ChessPiece(PieceType::Knight, "♘", PieceColor::Black);
    board[0][6] = ChessPiece(PieceType::Knight, "♘", PieceColor::Black);
    board[0][2] = ChessPiece(PieceType::Bishop, "♗", PieceColor::Black);
    board[0][5] = ChessPiece(PieceType::Bishop, "♗", PieceColor::Black);
    board[0][3] = ChessPiece(PieceType::Queen, "♕", PieceColor::Black);
    board[0][4] = ChessPiece(PieceType::King, "♔", PieceColor::Black);

    // Place other pieces for the other side (black)
    board[7][0] = ChessPiece(PieceType::Rook, "♜", PieceColor::White);
    board[7][7] = ChessPiece(PieceType::Rook, "♜", PieceColor::White);
    board[7][1] = ChessPiece(PieceType::Knight, "♞", PieceColor::White);
    board[7][6] = ChessPiece(PieceType::Knight, "♞", PieceColor::White);
    board[7][2] = ChessPiece(PieceType::Bishop, "♝", PieceColor::White);
    board[7][5] = ChessPiece(PieceType::Bishop, "♝", PieceColor::White);
    board[7][3] = ChessPiece(PieceType::Queen, "♛", PieceColor::White);
    board[7][4] = ChessPiece(PieceType::King, "♚", PieceColor::White);
}



// Function to print the chessboard

#include <iomanip> // Include for setw

void printBoard() {
    cout << "   a b c d e f g h" << endl;
    cout << "   ----------------" << endl;
    for (int row = 0; row < BOARD_SIZE; row++) {
        cout << 8 - row << "|";

        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].type == PieceType::Empty) {
                // Set the width of empty squares to 2 characters
                cout << setw(2) << ""; 
            } else {
                // Set the width of non-empty squares to 2 characters
                cout << setw(4) << board[row][col].symbol;
            }
        }
        cout << "  |" << 8 - row << endl;
    }
    cout << "   ----------------" << endl;
    cout << "   a b c d e f g h" << endl;
}




void playGame() {
    initializeBoard(); // Initialize the chessboard with starting pieces
    printBoard(); // Print the initial state of the chessboard

    bool isWhiteTurn = true; // Flag to keep track of whose turn it is (true for white, false for black)
    bool gameOver = false; // Flag to keep track of whether the game is over
    bool enPassantPossib; // Variable to store if en passant is possible
    Square enPassantSquare; // Variable to store the square for en passant capture
    
    while (!gameOver) {
        // Prompt the current player to make a move
        std::cout << (isWhiteTurn ? "White's move: " : "Black's move: ");
        Square sourceSquare, targetSquare;
        getUserMove(sourceSquare, targetSquare, isWhiteTurn ? PieceColor::White : PieceColor::Black);
      
        // Validate the move
        enPassantPossib = enPassantPossible(board, sourceSquare, targetSquare, isWhiteTurn);
        enPassantSquare = getEnPassantSquare(board, targetSquare, isWhiteTurn);
      
        if(!isValidMove(board, sourceSquare, targetSquare, isWhiteTurn, enPassantPossib, enPassantSquare, isWhiteTurn ? PieceColor::White : PieceColor::Black)) {
            std::cout << "Invalid move. Please try again." << std::endl;
            continue; // Continue to the next iteration of the loop
        }

        // Make the move on the chessboard
        makeMove(board, sourceSquare, targetSquare);

        // Check if the opponent's king is in checkmate
        if (isCheckmate(sourceSquare, targetSquare, !isWhiteTurn)) {
            std::cout << (isWhiteTurn ? "White" : "Black") << " wins by checkmate!" << std::endl;
            gameOver = true;
            continue; // End the game loop
        }

        // Check if the opponent's king is in check
        if (isCheck(board, isWhiteTurn ? PieceColor::White : PieceColor::Black, targetSquare)) {
            std::cout << (isWhiteTurn ? "Black" : "White") << " is in check!" << std::endl;
        }

        // Check for other game-ending conditions (stalemate, insufficient material, etc.)
        // Implement additional logic as needed

        printBoard(); // Print the updated state of the chessboard

        // Switch turns to the next player
        isWhiteTurn = !isWhiteTurn;
    }
}



int main() {
    playGame();
    return 0;
}

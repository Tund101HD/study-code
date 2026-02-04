#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // For _O_U16TEXT
#include <io.h>    // For _setmode


enum PieceType{ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN, EMPTY};
enum Color{WHITE, BLACK, EMPTY_COLOR};
struct Piece {
    enum PieceType type;
    int row;
    int col;
    enum Color color;
};
struct ChessBoardState {
    struct Piece pieces[64];
    int turn;
    int turnCount;
    char whiteMoves[99][16];
    char blackMoves[99][16];
};
void set_initial_positions(struct ChessBoardState *board);
const wchar_t* designation_from_type_and_color(enum PieceType type, enum Color color);
void printChessBoard(struct ChessBoardState board_state);
void println(const char* content);

int main() {
    // Force terminal into UTF16 wide mode. This will break standard printf and narrow characters!
    _setmode(_fileno(stdout), _O_U16TEXT);
    struct ChessBoardState board;
    set_initial_positions(&board);
    board.turn = WHITE;
    board.turnCount = 1;
    println("Glad to see you here and play some chess! White begins.");
    println("Turn 1 ; White to move.");
    printChessBoard(board);
    while (1) {

        return 0;
    }
    return 0;
}

/*
 * Add all pieces in their initial position to the board. This will override the current board state
 * and should only be called in the beginning or on reset.
 *
 */
void set_initial_positions(struct ChessBoardState *board) {
    const enum PieceType backRank[] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};

    for (int i = 0; i < 64; i++) {
        const int row = i / 8;
        const int col = i % 8;

        board->pieces[i].row = row;
        board->pieces[i].col = col;

        if (row == 0) {
            board->pieces[i].color = WHITE;
            board->pieces[i].type = backRank[col];
        } else if (row == 1) {
            board->pieces[i].color = WHITE;
            board->pieces[i].type = PAWN;
        } else if (row == 6) {
            board->pieces[i].color = BLACK;
            board->pieces[i].type = PAWN;
        } else if (row == 7) {
            board->pieces[i].color = BLACK;
            board->pieces[i].type = backRank[col];
        } else {
            board->pieces[i].type = EMPTY;
            board->pieces[i].color = EMPTY_COLOR;
        }
    }
}

/*
 * Print the chessboard with the (wide) Unicode characters for the pieces.
 */
void printChessBoard(const struct ChessBoardState board_state) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            const int index = i * 8 + j;
            const struct Piece piece = board_state.pieces[index];
            wprintf(L"%ls ", designation_from_type_and_color(piece.type, piece.color));
        }
        wprintf(L"\n");
    }
}

/*
 *Unicode conversion for chess pieces and board squares. We need to use wchar_t for compatability with Windows, chess pieces are wide chars.
 */
const wchar_t* designation_from_type_and_color(const enum PieceType type, const enum Color color) {
    switch (type) {
        case ROOK:   return color == WHITE ? L"\x2656" : L"\x265C";
        case KNIGHT: return color == WHITE ? L"\x2658" : L"\x265E";
        case BISHOP: return color == WHITE ? L"\x2657" : L"\x265D";
        case QUEEN:  return color == WHITE ? L"\x2655" : L"\x265B";
        case KING:   return color == WHITE ? L"\x2654" : L"\x265A";
        case PAWN:   return color == WHITE ? L"\x2659" : L"\x265F";
        case EMPTY:  return L"\u3000";
        default:     return L"?";
    }
}

/*
 * Print a string to the console with new line.
 */
void println(const char* content) {
    // %hs (narrow string) inside a wprintf call.
    // wprintf is needed because of the UTF-16 wide support set by _setmode.
    wprintf(L"%hs\n", content);
}

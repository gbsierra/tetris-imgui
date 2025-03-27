#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <chrono>
#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

using namespace std;
using namespace std::chrono;

// Definitions
#define WaitTime 700
#define InputTime 60

#define Rows 10
#define Columns 22
#define BlockSize 30
#define MatrixBlockSize 5

#define BoardPositionX 400 - (310 / 2)
#define BoardPositionY 400 - (670 / 2)

// Enums
enum {
	Free,
	Filled
};

enum InputKey {
	NO_KEY = 0, ESC, N_1, N_2, N_3, N_4, N_5, N_6, N_7, N_8, N_9, N_0, MINUS, EQUAL, BS, Tab, Q, W, E, R, T, Y, U, I, O, P, LBRACKET, RBRACKET, ENTER, CTRL, A, S, D, F, G,
	H, J, K, L, SEMICOLON, SINGLE_QUOTE, TILDE, LSHIFT, BACKSLASH, Z, X, C, V, B, N, M, COMMA, DOT, FRONTSLASH, RSHIFT, PRINT_SCREEN, ALT, SPACE, CAPS, F1, F2,
	F3, F4, F5, F6, F7, F8, F9, F10, NUM, SCROLL, HOME, UP, PAGE_UP, NUM_MINUS, LEFT, CENTER, RIGHT, PLUS, END, DOWN, PAGE_DOWN, INSERT, DEL
};

// Game Class
class Game {
public:
	// Variables
	int PositionX, PositionY;
	int CurrentPiece, CurrentRotation;

	int NextPiece, NextRotation;

	bool IsKeyDown;

	int Score;

	// Core Functions
	void Initialize();
	bool Update();
	InputKey PollInput();
	void Render();

	int GetRandom(int Min, int Max);

	// Tetrominoes
	vector<vector<vector<vector<int>>>> Tetrominoes;
	vector<vector<vector<int>>> InitialPositions;
	vector<ImColor> TetrominoColors;
	unsigned int GameBoard[Rows][Columns];
	unsigned int ColorBoard[Rows][Columns];
	unsigned int NextBoard[5][5];

	// Pieces
	int GetType(int Piece, int Rotation, int X, int Y);
	int GetX(int Piece, int Rotation);
	int GetY(int Piece, int Rotation);

	// Board
	void Place(int X, int Y, int Piece, int Rotation);
	bool IsFree(int X, int Y);
	bool IsMovePossible(int X, int Y, int Piece, int Rotation);
	void ClearPossibleLines();
	void ClearLine(int Y);
	bool IsGameOver();

	// Game Functions
	void CreateNewPiece();
	void DrawPiece(int X, int Y, int Piece, int Rotation);
	void DrawNextPiece();
	void DrawBoard();
};
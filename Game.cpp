#include "Game.h"

#include "KeiFont.cpp"

// Timers
static high_resolution_clock::time_point NowTime;
static high_resolution_clock::time_point LastUpdate;
static high_resolution_clock::time_point LastInput;
static duration<float, std::milli> UpdateDifference;
static duration<float, std::milli> InputDifference;
ImFont* Font;

// Random Number Generator
int Game::GetRandom(int Min, int Max) {
    return rand() % (Max - Min + 1) + Min;
}

// Initialization
void Game::Initialize() {
    // Set Title
    SetConsoleTitleA("Tetris");

    // Generate Random Number Generator Seed
    srand(time(0));

    // Create The Shapes
    cout << "Create Shapes\n";
    Game::Tetrominoes = {
        // Square
        {
            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}}
        },

        // I
        {
            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 1},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {1, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}}
        },

        // L
        {
            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}}
        },

        // L Mirrored
        {
            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0}}
        },

        // N
        {
            {{0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}}
        },

        // N Mirrored
        {
            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 1, 1, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}}
        },

        // T
        {
            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 2, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 2, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}}
        }
    };

    // Set Initial Positions
    cout << "Set Initial Positions\n";
    Game::InitialPositions = {
        /* Square */
        {{-2, -2}, {-2, -2}, {-2, -2}, {-2, -2}},
        /* I */
        {{-2, -1}, {-2, -3}, {-2, -2}, {-2, -3}},
        /* L */
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}},
        /* L Mirrored */
        {{-2, -3}, {-2, -2}, {-2, -3}, {-2, -3}},
        /* N */
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}},
        /* N Mirrored */
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}},
        /* T */
        {{-2, -3}, {-2, -3}, {-2, -3}, {-2, -2}}
    };

	// Set The Colors
	cout << "Adding The Colors\n";
	TetrominoColors.push_back(ImColor(255, 255, 255, 70)); // Board Color, The Rest Are Tetromino Colors
	TetrominoColors.push_back(ImColor(1, 241, 242, 255));
	TetrominoColors.push_back(ImColor(238, 160, 1, 255));
	TetrominoColors.push_back(ImColor(1, 0, 239, 255));
	TetrominoColors.push_back(ImColor(240, 241, 0, 255));
	TetrominoColors.push_back(ImColor(0, 241, 1, 255));
	TetrominoColors.push_back(ImColor(163, 1, 241, 255));
	TetrominoColors.push_back(ImColor(241, 2, 0, 255));

    // Create and Initialize the Game Board
    cout << "Initialize The Game Board\n";
    for (int X = 0; X < Rows; ++X) {
        for (int Y = 0; Y < Columns; ++Y) {
            GameBoard[X][Y] = Free;
        }
    }

    // Create and Initialize The Next Piece Board
	cout << "Initialize The Next Piece Board\n";
    for (int X = 0; X < 5; ++X) {
        for (int Y = 0; Y < 5; ++Y) {
            NextBoard[X][Y] = Free;
        }
    }

    // Initialize Local Variables
    cout << "Initialize Local Variables\n";
    CurrentPiece = GetRandom(0, 6);
    CurrentRotation = GetRandom(0, 3);
    PositionX = (Rows / 2) + GetX(CurrentPiece, CurrentRotation);
    PositionY = GetY(CurrentPiece, CurrentRotation);

	NextPiece = GetRandom(0, 6);
	NextRotation = GetRandom(0, 3);

	// Set Timers
	cout << "Set Timers\n";
	LastUpdate = high_resolution_clock::now();
	LastInput = high_resolution_clock::now();

	// Set Font
	cout << "Set Font\n";
	ImGuiIO& io = ImGui::GetIO();
    ImFontConfig Config = ImFontConfig();
    Config.OversampleH = Config.OversampleV = 0;
    Config.PixelSnapH = true;
    Config.RasterizerDensity = 1.25f;
    Config.SizePixels = 24.0f;
    Font = io.Fonts->AddFontFromMemoryCompressedTTF(Kei_compressed_data, Kei_compressed_size, Config.SizePixels, &Config);

}

// Piece Functions
int Game::GetType(int Piece, int Rotation, int X, int Y) {
    return Tetrominoes[Piece][Rotation][X][Y];
}

int Game::GetX(int Piece, int Rotation) {
    return InitialPositions[Piece][Rotation][0];
}

int Game::GetY(int Piece, int Rotation) {
    return InitialPositions[Piece][Rotation][1];
}

// Board Functions
void Game::Place(int X, int Y, int Piece, int Rotation) {
    for (int PieceX = X, BoardX = 0; PieceX < X + MatrixBlockSize; PieceX++, BoardX++) {
        for (int PieceY = Y, BoardY = 0; PieceY < Y + MatrixBlockSize; PieceY++, BoardY++) {
            if (Game::GetType(Piece, Rotation, BoardX, BoardY) != 0) {
                GameBoard[PieceX][PieceY] = Filled;
				ColorBoard[PieceX][PieceY] = Piece + 1;
            }
        }
    }
}

bool Game::IsGameOver() {
    for (int PieceX = 0; PieceX < Rows; PieceX++)
        if (GameBoard[PieceX][0] == Filled)
            return true;
    return false;
}

void Game::ClearLine(int Y) {
    for (int BoardY = Y; BoardY > 0; BoardY--) {
        for (int BoardX = 0; BoardX < Rows; BoardX++) {
            GameBoard[BoardX][BoardY] = GameBoard[BoardX][BoardY - 1];
            ColorBoard[BoardX][BoardY] = ColorBoard[BoardX][BoardY - 1];
        }
    }
}

void Game::ClearPossibleLines() {
    int TotalLinesCleared = 0;
    for (int BoardY = 0; BoardY < Columns; BoardY++) {
        int BoardX = 0;
        while (BoardX < Columns) {
            if (GameBoard[BoardX][BoardY] != Filled) break;
            BoardX++;
        }

        if (BoardX == Rows) {
            Game::ClearLine(BoardY);
            TotalLinesCleared++;
        }
    }

    Score += 25;
    if (TotalLinesCleared != 0) {
        Score += (1 << TotalLinesCleared) * 100;
    }
}

bool Game::IsFree(int X, int Y) {
    if (GameBoard[X][Y] == Free) return true;
    else return false;
}

bool Game::IsMovePossible(int X, int Y, int Piece, int Rotation) {
    for (int PieceX = X, BoardX = 0; PieceX < X + MatrixBlockSize; PieceX++, BoardX++) {
        for (int PieceY = Y, BoardY = 0; PieceY < Y + MatrixBlockSize; PieceY++, BoardY++) {
            if (PieceX < 0 || PieceX > Rows - 1 || PieceY > Columns - 1) {
                if (GetType(Piece, Rotation, BoardX, BoardY) != 0)
                    return false;
            }

            if (PieceY >= 0) {
                if ((GetType(Piece, Rotation, BoardX, BoardY) != 0) &&
                    (!IsFree(PieceX, PieceY)))
                    return false;
            }
        }
    }
	return true;
}

// Game
void Game::CreateNewPiece() {
    CurrentPiece = NextPiece;
    CurrentRotation = NextRotation;
    PositionX = (Rows / 2) + GetX(CurrentPiece, CurrentRotation);
    PositionY = GetY(CurrentPiece, CurrentRotation);

    NextPiece = GetRandom(0, 6);
    NextRotation = GetRandom(0, 3);

    // Reset Next Piece
    for (int BoardX = 0; BoardX < 5; BoardX++) {
        for (int BoardY = 0; BoardY < 5; BoardY++) {
            NextBoard[BoardX][BoardY] = Free;
        }
    }
}

// Draw Block
void DrawBlock(int X, int Y, ImColor Color, bool Filled, int ExtraX, int ExtraY) {
    ImVec2 TopLeft = ImVec2((float)(X * BlockSize) + ExtraX, (float)(Y * BlockSize) + ExtraY);
    ImVec2 BottomRight = ImVec2((float)((X + 1) * BlockSize) + ExtraX, (float)((Y + 1) * BlockSize) + ExtraY);

    const ImU32 ColorU32 = Color;
    if (Filled)
        ImGui::GetWindowDrawList()->AddRectFilled(TopLeft, BottomRight, ColorU32);
    else
        ImGui::GetWindowDrawList()->AddRect(TopLeft, BottomRight, ColorU32);
}

// Draw Line
void DrawLine(int X, int Y, int X2, int Y2, ImColor Color, float Thickness) {
    ImVec2 StartPosition = ImVec2((float)X, (float)Y);
    ImVec2 EndPosition = ImVec2((float)X2, (float)Y2);
    const ImU32 ColorU32 = Color;
    ImGui::GetWindowDrawList()->AddLine(StartPosition, EndPosition, ColorU32, Thickness);
}

// Draw Piece
void Game::DrawPiece(int X, int Y, int Piece, int Rotation) {
    for (int PieceX = 0; PieceX < MatrixBlockSize; PieceX++)
        for (int PieceY = 0; PieceY < MatrixBlockSize; PieceY++)
            if (Game::GetType(Piece, Rotation, PieceX, PieceY) != 0)
                DrawBlock(X + PieceX, Y + PieceY, TetrominoColors[Piece + 1], true, 250, 60);
}

// Draw Next Piece
void Game::DrawNextPiece() {
    for (int PieceX = 0, BoardX = 0; PieceX < 5; PieceX++, BoardX++) {
        for (int PieceY = 0, BoardY = 0; PieceY < 5; PieceY++, BoardY++) {
            if (Game::GetType(NextPiece, NextRotation, BoardX, BoardY) != 0)
                NextBoard[PieceX][PieceY] = Filled;
        }
    }

    for (int BoardX = 0; BoardX < 5; BoardX++) {
        for (int BoardY = 0; BoardY < 5; BoardY++) {
            if (NextBoard[BoardX][BoardY] != Free)
                DrawBlock(BoardX, BoardY, TetrominoColors[NextPiece + 1], true, 42, 55);
            DrawBlock(BoardX, BoardY, TetrominoColors[0], false, 42, 55);
        }
    }
}

// Draw Board
void Game::DrawBoard() {
    DrawLine(BoardPositionX + 3, 60, BoardPositionX + 3, 720, ImColor(150, 150, 150, 255), 3.0f);
    DrawLine(BoardPositionX + (305), 60, BoardPositionX + (305), 720, ImColor(150, 150, 150, 255), 3.0f);
    DrawLine(BoardPositionX + 2, 720, BoardPositionX + (307), 720, ImColor(150, 150, 150, 255), 3.0f);

    for (int BoardX = 0; BoardX < Rows; BoardX++) {
        for (int BoardY = 0; BoardY < Columns; BoardY++) {
            if (Game::IsFree(BoardX, BoardY))
                DrawBlock(BoardX, BoardY, TetrominoColors[0], false, 250, 60);
            else
                DrawBlock(BoardX, BoardY, TetrominoColors[ColorBoard[BoardX][BoardY]], true, 250, 60);
        }
    }
}

// Poll Input
InputKey Game::PollInput() {
    vector<InputKey> KeyList = { InputKey::LEFT, InputKey::RIGHT, InputKey::DOWN, InputKey::Z, InputKey::X };

    for (InputKey &Key : KeyList) {
        int VirtualKey = MapVirtualKeyA(Key, MAPVK_VSC_TO_VK);
        if (VirtualKey == 0)
            return InputKey::NO_KEY;

        bool KeyDown = GetAsyncKeyState(VirtualKey) & 0x8000;
        if (KeyDown)
            return Key;
    }

    return InputKey::NO_KEY;
}

// Update
bool IsKeyDown = false;
bool Game::Update() {
    NowTime = high_resolution_clock::now();
    UpdateDifference = NowTime - LastUpdate;
    InputDifference = NowTime - LastInput;
    
    InputKey Key = PollInput();
    switch (Key) {
    case InputKey::LEFT:
        if (!IsKeyDown) {
			if (Game::IsMovePossible(PositionX - 1, PositionY, CurrentPiece, CurrentRotation))
				PositionX--;
			IsKeyDown = true;
        }
        break;
    case InputKey::RIGHT:
        if (!IsKeyDown) {
            if (Game::IsMovePossible(PositionX + 1, PositionY, CurrentPiece, CurrentRotation))
                PositionX++;
            IsKeyDown = true;
        }
        break;
    case InputKey::DOWN:
        if (InputDifference.count() > InputTime) {
			if (Game::IsMovePossible(PositionX, PositionY + 1, CurrentPiece, CurrentRotation))
				PositionY++;
			LastInput = NowTime;
        }
        break;
    case InputKey::Z:
        if (!IsKeyDown) {
            if (Game::IsMovePossible(PositionX, PositionY, CurrentPiece, (CurrentRotation + 1) % 4))
                CurrentRotation = (CurrentRotation + 1) % 4;
            IsKeyDown = true;
        }
        break;
    case InputKey::X:
        if (!IsKeyDown) {
            while (IsMovePossible(PositionX, PositionY, CurrentPiece, CurrentRotation)) { PositionY++; }

            Place(PositionX, PositionY - 1, CurrentPiece, CurrentRotation);

            ClearPossibleLines();

            if (IsGameOver()) return false;

            CreateNewPiece();
            IsKeyDown = true;
        }
        break;
    default:
        IsKeyDown = false;
        break;
    }

    if (UpdateDifference.count() > WaitTime) {
        if (Game::IsMovePossible(PositionX, PositionY + 1, CurrentPiece, CurrentRotation))
            PositionY++;
        else {
            Game::Place(PositionX, PositionY, CurrentPiece, CurrentRotation);
            Game::ClearPossibleLines();
            if (Game::IsGameOver()) return false;
            Game::CreateNewPiece();
        }
        LastUpdate = NowTime;
    }

    return true;
}

// Render
void Game::Render() {
    // Begin Game Frame
    ImGui::SetNextWindowSize(ImVec2(330, 700));
    ImGui::SetNextWindowPos(ImVec2(BoardPositionX - 10, BoardPositionY - 35));
    if (ImGui::Begin("Tetris", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoFocusOnAppearing)) {

        // Draw Game Board
        DrawBoard();
        DrawPiece(PositionX, PositionY, CurrentPiece, CurrentRotation);
    }

    // End Game Frame
    ImGui::End();

    // Begin Next Piece Board
    ImGui::SetNextWindowSize(ImVec2(163, 182));
	ImGui::SetNextWindowPos(ImVec2(35, 30));
    if (ImGui::Begin("Next Piece", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoFocusOnAppearing)) {

        DrawNextPiece();

    } ImGui::End();

    // Score Board
    ImGui::SetNextWindowSize(ImVec2(163, 110));
    ImGui::SetNextWindowPos(ImVec2(35, 225));
    if (ImGui::Begin("Score Board", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoFocusOnAppearing)) {

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Text("   Score:");
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
		ImGui::PushFont(Font);
        ImGui::Dummy(ImVec2(5.0f, 1.0f));
        ImGui::SameLine();
		ImGui::TextColored(ImColor(20, 40, 255, 255), "%d", Score);
		ImGui::PopFont();

    } ImGui::End();
}
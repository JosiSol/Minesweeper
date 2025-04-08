#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <utility>
#include <random>

#define ROWS 10
#define COLS 10

const int SCREENWIDTH = 400;
const int SCREENHEIGHT = 400;

const int CELLWIDTH = SCREENWIDTH / COLS;
const int CELLHEIGHT = SCREENHEIGHT / ROWS;

Texture2D flagSprite;
int minesLeft;
typedef enum GameState{
    PLAYING,
    GAMEOVER,
    WIN
} GameState;
GameState gameState = PLAYING;

float startTime;
float elapsedTime;
typedef struct Cell{
    int i;
    int j;
    bool containsMine;
    bool isRevealed;
    bool isFlagged;
    int neighborMines;
} Cell;

Cell grid[ROWS][COLS];

void Grid();
void CellDraw(Cell cell);
void CellReveal(int i, int j);
bool InBound(int i, int j);
int NeighborMines(int i, int j);
void CellFlag(int i, int j);
void ResetGame();
void CheckWin();

int main()
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Minesweeper");
    SetTargetFPS(60);

    flagSprite = LoadTexture("Assets/flag.png");

    ResetGame();

    while (!WindowShouldClose()){
        // Event Handling
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            int i = mousePos.x / CELLWIDTH;
            int j = mousePos.y / CELLHEIGHT;

            if(gameState == PLAYING && InBound(i, j)){
                CellReveal(i, j);
                CheckWin();
            }
        } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
            Vector2 mousePos = GetMousePosition();
            int i = mousePos.x / CELLWIDTH;
            int j = mousePos.y / CELLHEIGHT;

            if(gameState == PLAYING && InBound(i, j)){
                CellFlag(i, j);
            }
        }
        if (gameState == GAMEOVER || gameState == WIN){
            if (IsKeyPressed(KEY_ENTER)){
                ResetGame();
            }
        }
        
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        for (int i = 0; i < COLS; i++){
            for (int j = 0; j < ROWS; j++){
                CellDraw(grid[i][j]);
            }
        }

        if (gameState == GAMEOVER) {
            DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, Fade(RED, 0.5f));
            DrawText("GAME OVER", SCREENWIDTH / 2 - MeasureText("GAME OVER", 20) / 2, SCREENHEIGHT / 2 - 30, 20, Fade(WHITE, 0.8f));
            DrawText("PRESS 'ENTER' TO RESTART", SCREENWIDTH / 2 - MeasureText("PRESS 'ENTER' TO RESTART", 20) / 2, SCREENHEIGHT / 2 + 10, 20, Fade(WHITE, 0.8f));
            DrawText(TextFormat("Time: %.2f seconds", elapsedTime), 10, 10, 20, WHITE);
        }

        if (gameState == WIN) {
            DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, Fade(GREEN, 0.5f));
            DrawText("YOU WIN", SCREENWIDTH / 2 - MeasureText("YOU WIN", 20) / 2, SCREENHEIGHT / 2 - 30, 20, Fade(WHITE, 0.8f));
            DrawText("PRESS 'ENTER' TO RESTART", SCREENWIDTH / 2 - MeasureText("PRESS 'ENTER' TO RESTART", 20) / 2, SCREENHEIGHT / 2 + 10, 20, Fade(WHITE, 0.8f));
            DrawText(TextFormat("Time: %.2f seconds", elapsedTime), 10, 10, 20, WHITE);
        }

        EndDrawing();
    }    
    CloseWindow();

    return 0;
}

void Grid(){
    // Initialize grid
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            grid[i][j].i = i;
            grid[i][j].j = j;
            grid[i][j].containsMine = false;
            grid[i][j].isRevealed = false;
            grid[i][j].isFlagged = false;
            grid[i][j].neighborMines = -1;

        /* if (i == 0){
            grid[i][j].containsMine = true; // Testing, Set First Column to Mine ✅
            } */
        }
    }
    minesLeft = (ROWS * COLS) / 10; // 10% of the grid
    int mines = minesLeft;

    std::vector<std::pair<int, int>> minePositions;
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            minePositions.push_back({i, j});
        }
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(minePositions.begin(), minePositions.end(), gen);
    
    for (int p = 0; p < mines; p++){
        int i = minePositions[p].first;
        int j = minePositions[p].second;
        grid[i][j].containsMine = true;
    }

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            if (!grid[i][j].containsMine){
                grid[i][j].neighborMines = NeighborMines(i, j);
            } 
        }
    }
}

void ResetGame(){
    Grid();
    gameState = PLAYING;
    startTime = GetTime();
}

void CellDraw(Cell cell){
    if (cell.isRevealed){
        if (cell.containsMine){
            DrawRectangle(cell.i * CELLWIDTH, cell.j * CELLHEIGHT, CELLWIDTH, CELLHEIGHT, RED); 
        } else {
            DrawRectangle(cell.i * CELLWIDTH, cell.j * CELLHEIGHT, CELLWIDTH, CELLHEIGHT, LIGHTGRAY); 
            if (cell.neighborMines){
            DrawText(TextFormat("%d", cell.neighborMines), cell.i * CELLWIDTH + CELLWIDTH / 4, cell.j * CELLHEIGHT + CELLHEIGHT / 4, 20, DARKGRAY);
            }
        }
    } else if (cell.isFlagged){
        Rectangle src = {0, 0, static_cast<float>(flagSprite.width), static_cast<float>(flagSprite.height)};
        Rectangle dest = {
            static_cast<float>(cell.i * CELLWIDTH), static_cast<float>(cell.j * CELLHEIGHT), static_cast<float>(CELLWIDTH), static_cast<float>(CELLHEIGHT)
        };
        Vector2 origin = {0, 0};
        DrawTexturePro(flagSprite, src, dest, origin, 0.0f, LIGHTGRAY);
    }
    DrawRectangleLines(cell.i * CELLWIDTH, cell.j * CELLHEIGHT, CELLWIDTH, CELLHEIGHT, BLACK); 
}

bool InBound(int i, int j) {
    return i >= 0 && i < ROWS && j >= 0 && j < COLS;
}

void CellReveal(int i, int j){
    if (!InBound(i, j)){
        return;
    }
    if (grid[i][j].isFlagged || grid[i][j].isRevealed){
        return;
    }

    grid[i][j].isRevealed = true;
    if (grid[i][j].containsMine){
        // Game Over Logic
        gameState = GAMEOVER;
        elapsedTime = GetTime() - startTime;
    } else{
        // Flood Fill Logic
        if (grid[i][j].neighborMines == 0){
            for (int x = -1; x <= 1; x++){
                for (int y = -1; y <= 1; y++){
                    if (x == 0 && y == 0){
                        continue;
                    }
                    if (InBound(i + x, j + y)){
                        if (!grid[i + x][j + y].isRevealed && !grid[i + x][j + y].containsMine){
                            CellReveal(i + x, j + y);
                        }   
                    }
                }
            }
        }
    }
}

int NeighborMines(int i, int j){
    int count = 0;
    for (int x = -1; x <= 1; x++){
        for (int y = -1; y <= 1; y++){
            if (x == 0 && y == 0){
                continue;
            }
            if (InBound(i + x, j + y)){
                if (grid[i + x][j + y].containsMine){
                    count++;
                }
            }
        }
    }
    return count;
}

void CellFlag(int i, int j){
    if (grid[i][j].isRevealed){
        return;
    }

    if (InBound(i, j)){
        grid[i][j].isFlagged = !grid[i][j].isFlagged;
    }
}

void CheckWin(){
    int revealedCells = 0;
    int correctlyFlaggedMines = 0; // If the Flagged cell contains a mine

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j].isRevealed && !grid[i][j].containsMine) {
                revealedCells++;
            }
            if (grid[i][j].isFlagged && grid[i][j].containsMine) {
                correctlyFlaggedMines++;
            }
        }
    }

    if (revealedCells == (ROWS * COLS) - minesLeft && correctlyFlaggedMines == minesLeft) {
        gameState = WIN;
        elapsedTime = GetTime() - startTime;
    }
}
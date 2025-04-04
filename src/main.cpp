#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <utility>
#include <random>

// #include <iostream>

// ROWS AND COLS OF THE GRID
#define ROWS 10
#define COLS 10

const int SCREENWIDTH = 400;
const int SCREENHEIGHT = 400;

const int CELLWIDTH = SCREENWIDTH / COLS;
const int CELLHEIGHT = SCREENHEIGHT / ROWS;

Texture2D flagSprite;

typedef struct Cell{
    int i;
    int j;
    bool containsMine;
    bool isRevealed;
    bool isFlagged;
    int neighborMines;
} Cell;

Cell grid[ROWS][COLS];

void CellDraw(Cell cell);
void CellReveal(int i, int j);
bool InBound(int i, int j);
int NeighborMines(int i, int j);
void CellFlag(int i, int j);

int main()
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Minesweeper");
    SetTargetFPS(60);

    flagSprite = LoadTexture("Assets/flag.png");

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
    int mines = (ROWS * COLS) / 10; // 10% of the grid

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

    while (!WindowShouldClose()){
        // Event Handling
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            int i = mousePos.x / CELLWIDTH;
            int j = mousePos.y / CELLHEIGHT;

            if(InBound(i, j)){
                CellReveal(i, j);
            }
        } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
            Vector2 mousePos = GetMousePosition();
            int i = mousePos.x / CELLWIDTH;
            int j = mousePos.y / CELLHEIGHT;

            if(InBound(i, j)){
                CellFlag(i, j);
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

        EndDrawing();
    }    
    CloseWindow();

    return 0;
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
    // std::cout << "Cell (" << i << ", " << j << ") revealed." << std::endl;
    if (grid[i][j].containsMine){
        // Game Over Logic
    } else{
        // Play Sound and Continue Logic

        if (grid[i][j].neighborMines == 0){
            // Flood Fill Logic
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
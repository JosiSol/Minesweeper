#include <raylib.h>
#include <raymath.h>

#include <iostream>

#define ROWS 10
#define COLS 10

const int SCREENWIDTH = 400;
const int SCREENHEIGHT = 400;

const int CELLWIDTH = SCREENWIDTH / COLS;
const int CELLHEIGHT = SCREENHEIGHT / ROWS;

typedef struct Cell{
    int i;
    int j;
    bool containsMine;
    bool isRevealed;
} Cell;

Cell grid[ROWS][COLS];

void CellDraw(Cell cell);
void CellReveal(int i, int j);
bool InBound(int i, int j);

int main()
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Minesweeper");
    SetTargetFPS(60);

    // Initialize grid
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            grid[i][j].i = i;
            grid[i][j].j = j;
            grid[i][j].containsMine = false;
            grid[i][j].isRevealed = false;
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
        }
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
    grid[i][j].isRevealed = true;
    std::cout << "Cell (" << i << ", " << j << ") revealed." << std::endl;
    if (grid[i][j].containsMine){
        // Game Over
    } else{
        // Play Sound and Continue Logic
    }
}
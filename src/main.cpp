#include <raylib.h>

int main()
{
    InitWindow(800, 800, "Hello World");
    SetTargetFPS(60);
    while (WindowShouldClose() == false){
        BeginDrawing();
        DrawText("Hello, World!", 190, 200, 20, RED);
        EndDrawing();
    }
    ClearBackground(WHITE);
    CloseWindow();
    return 0;
}
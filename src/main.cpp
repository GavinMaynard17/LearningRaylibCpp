#include "../include/raylib.h"

int main(void)
{
    InitWindow(800, 450, "Learning Raylib!");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("First Commit! yur", 300, 200, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
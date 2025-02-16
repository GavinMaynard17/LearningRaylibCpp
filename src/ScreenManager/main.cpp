#include <raylib.h>

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen; // list of the screens

int main() 
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic screen manager");
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = LOGO;

    int framesCounter = 0;

    

    while(!WindowShouldClose())
    {
        //update
        switch (currentScreen)
        {
            case LOGO:
            {
                framesCounter++;

                // using frames to count time rather than chronos
                if (framesCounter > 120) 
                {
                    currentScreen = TITLE;
                }
                
            } break;
            case TITLE:
            {
                if (IsKeyPressed(KEY_ESCAPE)) {
                    framesCounter = 0;
                    currentScreen = LOGO;
                }
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_DOUBLETAP))
                {
                    currentScreen = GAMEPLAY;
                }

            } break;
            case GAMEPLAY:
            {
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentScreen = LOGO;
                }
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_DOUBLETAP))
                {
                    currentScreen = ENDING;
                }

            } break;
            case ENDING:
            {
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentScreen = LOGO;
                }
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_DOUBLETAP))
                {
                    currentScreen = TITLE;
                }

            } break;
            default: break;
        }
        //check

        //draw
        BeginDrawing();
            ClearBackground(BLACK);
            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

                } break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
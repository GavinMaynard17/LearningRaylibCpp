#include <iostream>
#include <raylib.h>  

const int HEADING_FONT_SIZE = 80;
const int SUBHEADING_FONT_SIZE = 30;
const Color PRIMARY_COLOR = BLACK;

int playerScore = 0;
int oppScore = 0;
int direction_choices[2] = {-1,1};
int framesCounter = 0;
int TARGET_FPS = 60;
int win_score = 5;

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, PAUSED, ENDING } GameScreen;

class Paddle
{
protected:
    void CheckCollision()
    {
        if(y+height >= GetScreenHeight())
        {
            y = GetScreenHeight()-height;
        }
        if(y<=0)
        {
            y = 0;
        }
    }
public:
    float width, height;
    float x, y;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(
            Rectangle{x, y, width, height},
            15, 1, WHITE
        );
    }
    void Update()
    {
        if(IsKeyDown(KEY_W))
        {
            y -= speed;
        }
        if(IsKeyDown(KEY_S))
        {
            y += speed;
        }

        CheckCollision();
        
    }
};

class Opponent : public Paddle
{
public: 
    void Update(int ball_y)
    {
        if(ball_y < y + (height/2)) y-=speed; // go up
        if(ball_y > y + (height/2)) y+=speed; // go down

        CheckCollision();
    }
};

class Ball
{
protected:
    void Reset(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        direction_x = 0;
        direction_y = 0;
        round_starting = true;
        framesCounter = 0;
    }
public:
    int radius;
    float x, y;
    int speed_x, speed_y;
    int direction_x, direction_y;
    bool round_starting;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update()
    {
        x += speed_x*direction_x;
        y += speed_y*direction_y;  
        
        if(x+radius >= GetScreenWidth())
        {
            oppScore++;
            Reset();
        }
        if(x-radius <= 0)
        {
            playerScore++;
            Reset();
        }
        if(y+radius >= GetScreenHeight())
        {
            direction_y = -1;
        }
        if( y-radius <= 0){
            direction_y = 1;
        }
        
    }

    void StartRound()
    {
        round_starting = false;
        direction_x = direction_choices[GetRandomValue(0,1)];
        direction_y = direction_choices[GetRandomValue(0,1)];
    }

    void CheckPaddleCollision(Paddle player, Opponent opp)
    {
        if (CheckCollisionCircleRec(Vector2{x, y}, radius, Rectangle{player.x, player.y, player.width, player.height/2}))
        {
            direction_x = -1;
            direction_y = -1;
        }
        if (CheckCollisionCircleRec(Vector2{x, y}, radius, Rectangle{player.x, player.y+(player.height/2), player.width, player.height/2}))
        {
            direction_x = -1;
            direction_y = 1;
        }
        if (CheckCollisionCircleRec(Vector2{x, y}, radius, Rectangle{opp.x, opp.y, opp.width, opp.height/2}))
        {
            direction_x = 1;
            direction_y = -1;
        }
        if (CheckCollisionCircleRec(Vector2{x, y}, radius, Rectangle{opp.x, opp.y+(opp.height/2), opp.width, opp.height/2}))
        {
            direction_x = 1;
            direction_y = 1;
        }
    }
};

Ball ball;
Paddle player;
Opponent opp;
int main()
{
    int screen_width = 1280;
    int screen_height = 800;
    SetTargetFPS(TARGET_FPS);
    InitWindow(screen_width, screen_height, "GMayPong");
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = LOGO;

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;
    ball.round_starting = true;

    player.x = screen_width-30;
    player.y = screen_height/2-50;
    player.width = 20;
    player.height = 100;
    player.speed = 6;

    opp.x = 10;
    opp.y = screen_height/2-50;
    opp.width = 20;
    opp.height = 100;
    opp.speed = 6;

    while (!WindowShouldClose()){
        
        framesCounter++;
        //Update States
        switch (currentScreen)
        {
            case LOGO:
            {
                if(framesCounter>120) currentScreen = TITLE;
            } break;
            case TITLE:
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_DOUBLETAP))
                {
                    framesCounter = 0;
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                ball.Update();
                player.Update();
                opp.Update(ball.y);

                //Check collisions
                ball.CheckPaddleCollision(player, opp);
                if(IsKeyPressed(KEY_ESCAPE)) currentScreen = PAUSED;
                if (oppScore == win_score || playerScore == win_score)
                    currentScreen = ENDING;
            } break;
            case PAUSED:
            {
                if(IsKeyPressed(KEY_ESCAPE)) 
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case ENDING:
            {
                if(IsKeyPressed(KEY_ENTER))
                {
                    oppScore = 0;
                    playerScore = 0;
                    framesCounter = 0;
                    player.y = screen_height/2-50;
                    opp.y = screen_height/2-50;
                    ball.round_starting = true;
                    currentScreen = GAMEPLAY;
                }
            } break;
            default: break;
        }
        
        

        //Draw Results
        BeginDrawing();
        switch (currentScreen)
        {
            case LOGO:
            {
                const char *logo_abv = "G.G.G.";
                int logo_width = MeasureText(logo_abv, HEADING_FONT_SIZE); 

                const char *logo_text = "GMay GOAT Games";
                int logo_text_width = MeasureText(logo_text, SUBHEADING_FONT_SIZE);

                DrawText(logo_abv, screen_width/2-(logo_width/2), screen_height/2-HEADING_FONT_SIZE, HEADING_FONT_SIZE, LIGHTGRAY);
                DrawText(logo_text, screen_width/2-(logo_text_width/2), screen_height/2+SUBHEADING_FONT_SIZE, SUBHEADING_FONT_SIZE, GRAY);
            } break;
            case TITLE:
            {
                const char *game_title = "GMay Pong";
                const int title_width = MeasureText(game_title, HEADING_FONT_SIZE);

                DrawRectangle(0, 0, screen_width, screen_height, PRIMARY_COLOR);
                DrawText("GMAY PONG", 40, 40, HEADING_FONT_SIZE, WHITE);
                DrawText("PRESS ENTER OR DOUBLE CLICK TO START PLAYING", 40, 220, SUBHEADING_FONT_SIZE, LIGHTGRAY);
            } break;
            case GAMEPLAY:
            {
                ClearBackground(PRIMARY_COLOR);
                DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
                ball.Draw();
                opp.Draw();
                player.Draw();
                DrawText(TextFormat("%i", oppScore),screen_width/4-20, 20, 80, WHITE);
                DrawText(TextFormat("%i", playerScore),3*screen_width/4-20, 20, 80, WHITE);
                if (ball.round_starting)
                {
                    if (framesCounter < 180)
                    {
                        const char *sec_remaining = TextFormat("%i", 3-framesCounter/60);
                        const int sec_remaining_width = MeasureText(sec_remaining, SUBHEADING_FONT_SIZE);

                        DrawRectangle(screen_width/2-3, 48, 6, SUBHEADING_FONT_SIZE+4, PRIMARY_COLOR);
                        DrawText(sec_remaining, screen_width/2-sec_remaining_width/2, 50, SUBHEADING_FONT_SIZE, LIGHTGRAY);
                    } else if (framesCounter<300)
                    {
                        const char *start_text = "START!";
                        const int start_text_width = MeasureText(start_text, SUBHEADING_FONT_SIZE);

                        DrawRectangle(screen_width/2-3, 48, 6, SUBHEADING_FONT_SIZE+4, PRIMARY_COLOR);
                        DrawText(start_text, screen_width/2-start_text_width/2, 50, SUBHEADING_FONT_SIZE, LIGHTGRAY);
                    } else 
                    {
                        ball.StartRound();
                    }
                }
            } break;
            case PAUSED:
            {
                // TODO: Draw PAUSED screen here!
                DrawRectangle(0, 0, screen_width, screen_height, PRIMARY_COLOR);
                DrawText("PAUSE SCREEN", 40,40, HEADING_FONT_SIZE, WHITE);
                DrawText("PRESS ESCAPE to RETURN to GAMEPLAY SCREEN", 40, 220, SUBHEADING_FONT_SIZE, LIGHTGRAY);
            } break;
            case ENDING:
            {
                // TODO: Draw ENDING screen here!
                DrawRectangle(0, 0, screen_width, screen_height, PRIMARY_COLOR);
                const char *game_over = "Game over!";
                const int game_over_width = MeasureText(game_over, HEADING_FONT_SIZE);

                const char *final_score = TextFormat("%i - %i", oppScore, playerScore);
                const int score_width = MeasureText(final_score, SUBHEADING_FONT_SIZE);

                const char *footer_text = "PRESS ENTER TO START A NEW GAME";
                const int footer_width = MeasureText(footer_text, SUBHEADING_FONT_SIZE);

                DrawText(game_over, screen_width/2-game_over_width/2, screen_height/2-HEADING_FONT_SIZE, HEADING_FONT_SIZE, WHITE);
                DrawText(final_score, screen_width/2-score_width/2, screen_height/2+SUBHEADING_FONT_SIZE, SUBHEADING_FONT_SIZE, WHITE);
                DrawText(footer_text, screen_width/2-footer_width/2, screen_height/2+2*SUBHEADING_FONT_SIZE, SUBHEADING_FONT_SIZE, LIGHTGRAY);
            } break;
            default: break;
        }
            
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
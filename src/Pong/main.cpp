#include <iostream>
#include <chrono>
#include <raylib.h>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::milliseconds;    

int playerScore = 0;
int oppScore = 0;
int direction_choices[2] = {-1,1};
auto start_time = high_resolution_clock::now();

class Ball
{
protected:
    void Reset(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        direction_x = 0;
        direction_y = 0;
        round_starting = true;
        start_time = high_resolution_clock::now();
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
};

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

Ball ball;
Paddle player;
Opponent opp;
int main(){
    int screen_width = 1280;
    int screen_height = 800;
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "GMayPong");

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
        //Update States
        ball.Update();
        player.Update();
        opp.Update(ball.y);

        //Check collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height/2}))
        {
            ball.direction_x = -1;
            ball.direction_y = -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y+(player.height/2), player.width, player.height/2}))
        {
            ball.direction_x = -1;
            ball.direction_y = 1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{opp.x, opp.y, opp.width, opp.height/2}))
        {
            ball.direction_x = 1;
            ball.direction_y = -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{opp.x, opp.y+(opp.height/2), opp.width, opp.height/2}))
        {
            ball.direction_x = 1;
            ball.direction_y = 1;
        }


        //Draw results
        BeginDrawing();
            ClearBackground(BLACK);
            DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
            ball.Draw();
            opp.Draw();
            player.Draw();
            DrawText(TextFormat("%i", oppScore),screen_width/4-20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore),3*screen_width/4-20, 20, 80, WHITE);

            if (ball.round_starting)
            {
                auto currentTime = high_resolution_clock::now();
                auto time_passed_ms = duration_cast<milliseconds>(currentTime-start_time);
                int time_passed = time_passed_ms.count()/1000;
                if (time_passed < 3)
                {
                    DrawText(TextFormat("%i", 3-time_passed), screen_width/2, 50, 50, LIGHTGRAY);
                } else if (time_passed<5)
                {
                    DrawText("START!", screen_width/2-25, 50, 50, LIGHTGRAY);
                } else 
                {
                    ball.StartRound();
                }
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
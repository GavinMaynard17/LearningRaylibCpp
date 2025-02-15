#include <iostream>
#include <raylib.h>

class Ball
{
public:
    int radius;
    int x, y;
    int speed_x, speed_y;

    void Draw(){
        

        DrawCircle(x, y, radius, WHITE);
    }

    void Update(){
        x += speed_x;
        y += speed_y;

        if(x+radius >= GetScreenWidth() || x-radius <= 0){
            speed_x *= -1;
        }
        if(y+radius >= GetScreenHeight() || y-radius <= 0){
            speed_y *= -1;
        }
        
    }
};

class Paddle
{
public:
    int width, height;
    int x, y;
    int speed;

    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
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

        
        if(y+height >= GetScreenHeight()){
            y = GetScreenHeight()-height;
        }
        if(y<=0){
            y = 0;
        }
    }
};

class Opponent : public Paddle
{
public: 
    void Update(int ball_y)
    {
        if(ball_y < y) y-=speed;
        if(ball_y > y) y+=speed;
    }
};

int main(){
    int screen_width = 1280;
    int screen_height = 800;
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "GMayPong");
    Ball ball;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    Paddle player;
    player.x = screen_width-30;
    player.y = screen_height/2-50;
    player.width = 20;
    player.height = 100;
    player.speed = 6;

    Opponent opp;
    opp.x = 10;
    opp.y = screen_height/2-50;
    opp.width = 20;
    opp.height = 100;
    opp.speed = 6;

    while (!WindowShouldClose()){
        ball.Update();
        player.Update();
        opp.Update(ball.y);
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
            ball.Draw();
            opp.Draw();
            player.Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
#include <raylib.h>
#include <chrono>
#include <string>

std::string getFormattedTime(int timeSeconds){
    using std::to_string;
    using std::string;

    if(timeSeconds<60) return to_string(timeSeconds);
    else if(timeSeconds/60<60) {
        string min = to_string(timeSeconds/60);
        int remainingSeconds = timeSeconds%60;
        string remainingSecondsStr = to_string(remainingSeconds);
        if (remainingSeconds<10) remainingSecondsStr = "0" + remainingSecondsStr;
        return min + ":" + remainingSecondsStr;
    } else {
        int min = timeSeconds/60%60;
        string minStr = to_string(min);
        string hours = to_string(timeSeconds/(60*60));
        int remainingSeconds = timeSeconds%60;
        string remainingSecondsStr = to_string(remainingSeconds);

        
        if (min<10) minStr = "0" + minStr;
        if (remainingSeconds<10) remainingSecondsStr = "0" + remainingSecondsStr;

        return hours + ":" + minStr + ":" + remainingSecondsStr;
    }

}

int main(void)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::string;
    using std::to_string;

    const auto initTime = high_resolution_clock::now();

    Color colors[7] = {WHITE, RED, BLUE, PINK, YELLOW, DARKGREEN, GRAY};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Learning Raylib!");

    while (!WindowShouldClose())
    {
        const int windowWidth = GetScreenWidth();
        const int windowHeight = GetScreenHeight();

        const auto currentTime = high_resolution_clock::now();
        const auto timePassedMs = duration_cast<milliseconds>(currentTime-initTime);
        const int timePassed = timePassedMs.count()/1000;

        string message = "Time passed since start: " + getFormattedTime(timePassed);
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText(
                message.c_str(),
                5,
                5,
                20,
                WHITE
            );
            DrawCircle(windowWidth/2, windowHeight/2, 15, colors[timePassed%(sizeof(colors)/sizeof(*colors))]);
            // DrawLine(windowWidth/2, windowHeight, windowWidth/2, 0, RED);
            // DrawLine(0, windowHeight/2, windowWidth, windowHeight/2, RED);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}


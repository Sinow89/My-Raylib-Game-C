#include "raylib.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find 
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

int main(void) {
    // Initialize the window
    InitWindow(800, 600, "Raylib - Hello World");
    SetTargetFPS(60);
        float player_xpos = 400;
        float player_ypos = 300;
        float ball_xpos = 200;
        float ball_ypos = 200;
        float speed = 5;
    
    while (!WindowShouldClose()) {  
 
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(ball_xpos, ball_ypos, 10, BLUE);
        DrawRectangle(player_xpos, player_ypos, 75, 25, WHITE);

        Vector2 center = {player_xpos , player_ypos};
        Rectangle rec = {ball_xpos , ball_ypos  , 10, 10};


        while (!CheckCollisionCircleRec(center, 5.0f, rec))
        {
            ball_ypos = ball_ypos + 2;
            ball_xpos = ball_xpos + 2;
            break;
        }
        

        if(CheckCollisionCircleRec(center, 2.0f, rec))
        {
            DrawText("Hit!", 10, 10, 20, LIGHTGRAY);
        }

        if(IsKeyDown(KEY_W) && player_ypos > 0)
        {
            player_ypos = player_ypos-speed;
        }

        if(IsKeyDown(KEY_A) && player_xpos > 0)
        {  
            player_xpos = player_xpos-speed;
        }

        if(IsKeyDown(KEY_S) && player_ypos < 575)
        {
            player_ypos = player_ypos+speed;        
        }

        if(IsKeyDown(KEY_D) && player_xpos < 725)
        {
            player_xpos = player_xpos+speed;
        }
        
        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}
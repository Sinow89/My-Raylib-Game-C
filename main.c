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
        float player_speed = 5;
        float ball_xpos = 200;
        float ball_ypos = 200;
        float ball_speed = 2;
    
    while (!WindowShouldClose()) {  
 
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(ball_xpos, ball_ypos, 10, BLUE);
        DrawRectangle(player_xpos, player_ypos, 75, 25, WHITE);

        //This Vector2 is the center of the ball.
        //The Rectangle defines the Retangle and sets the position and size.
        Vector2 center = {ball_xpos , ball_ypos};
        Rectangle rec = {player_xpos , player_ypos  , 75, 25};

        while (1)
        {
            ball_ypos = ball_ypos + ball_speed;
            ball_xpos = ball_xpos + ball_speed;

            if(CheckCollisionCircleRec(center, 10.0f, rec))
            {
            ball_ypos = ball_ypos - ball_speed;
            ball_xpos = ball_xpos - ball_speed;
            }

            break;
        }
    
        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/

        if(IsKeyDown(KEY_W) && player_ypos > 0)
        {
            player_ypos = player_ypos-player_speed;
        }

        if(IsKeyDown(KEY_A) && player_xpos > 0)
        {  
            player_xpos = player_xpos-player_speed;
        }

        if(IsKeyDown(KEY_S) && player_ypos < 575)
        {
            player_ypos = player_ypos+player_speed;        
        }

        if(IsKeyDown(KEY_D) && player_xpos < 725)
        {
            player_xpos = player_xpos+player_speed;
        }
        
        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}
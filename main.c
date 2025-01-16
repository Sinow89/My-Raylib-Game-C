#include "raylib.h"

/* Use this command in powershell to make it compile. gcc cant find 
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

int main(void) {
    // Initialize the window
    InitWindow(800, 600, "Raylib - Hello World");
    SetTargetFPS(60);
        float xpos = 400;
        float ypos = 300;
        float speed = 5;
    
    while (!WindowShouldClose()) {  
 
        
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(200, 200, 10, BLUE);
        DrawRectangle(xpos, ypos, 75, 25, WHITE);

        Vector2 center = {xpos , ypos};
        Rectangle rec = {200 , 200  , 10, 10};

        
        if(CheckCollisionCircleRec(center, 2.0f, rec))
        {
            DrawText("Hit!", 10, 10, 20, LIGHTGRAY);
        }

        
        if(IsKeyDown(KEY_W))
        {
            ypos = ypos-speed;
        }

        if(IsKeyDown(KEY_A))
        {  
            xpos = xpos-speed;
        }

        if(IsKeyDown(KEY_S))
        {
            ypos = ypos+speed;        
        }

        if(IsKeyDown(KEY_D))
        {
            xpos = xpos+speed;
        }
        
        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}
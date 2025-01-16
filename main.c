#include "raylib.h"

/* Use this command in powershell to make it compile. gcc cant find 
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

int main(void) {
    // Initialize the window
    InitWindow(800, 600, "Raylib - Hello World");
    SetTargetFPS(60);
        int xpos = 400;
        int ypos = 300;
    

    while (!WindowShouldClose()) {  
 
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 10, 10, 20, LIGHTGRAY);
        DrawRectangle(xpos, ypos, 75, 25, GREEN);

        if(IsKeyDown(KEY_D))
        {
            xpos= xpos+10;
        }
        
        if(IsKeyDown(KEY_W))
        {
            ypos= ypos+10;
        }

        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}
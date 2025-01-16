#include "raylib.h"

/* Use this command in powershell to make it compile. gcc cant find 
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

int main(void) {
    // Initialize the window
    InitWindow(800, 600, "Raylib - Hello World");


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 10, 10, 20, LIGHTGRAY);
        EndDrawing();
        GetFrameTime();
        GetTime();
        GetFPS();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}
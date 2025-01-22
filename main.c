#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

//------------------------TO-DO---------------------------------------------//
//1. Fix player boundries connect to screen height and width.
//2. Fix collision on rec.

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float speed;
    float width;
    float height;
} player_t;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed;
} ball_t;

Vector2 AddVector2(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

int main(void) {
    // Initialize the window.
    const int screen_width = 800;
    const int screen_height = 600;
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "RaylibGame");

    // Initialize the player and ball.
    player_t player = {400, 300, 5, 5, 1000, 75, 25};
    ball_t ball = {200, 200, 5, 5, 10, 100};

    //Set the Velocity outside the while loop.

    while (!WindowShouldClose()) {

        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        //The Rectangle defines the Retangle and sets the position and size.
        Rectangle rec = {player.position.x, player.position.y , player.width, player.height};
        DrawRectangle(player.position.x, player.position.y, player.width, player.height, WHITE);

        //Draw the ball and set the speed of ball.
        DrawCircleV(ball.position, ball.radius, BLUE);
        // ball.position.x = ball.position.x+ball.speed*delta_time;
        // ball.position.y = ball.position.y+ball.speed*delta_time;
        // ball.velocity = Vector2Normalize(ball.velocity);
        // ball.velocity = Vector2Scale(ball.velocity, ball.speed);
        // ball.velocity = Vector2Scale(ball.velocity, delta_time);
        ball.position = Vector2Add(ball.position, ball.velocity);

        // Making the ball "bounce" when hiting the walls.
        if (ball.position.x - ball.radius < 0 || ball.position.x + ball.radius > screen_width) {
            ball.velocity.x = -ball.velocity.x; // Reverse x direction
        }
        if (ball.position.y - ball.radius < 0 || ball.position.y + ball.radius > screen_height) {
            ball.velocity.y = -ball.velocity.y; // Reverse y direction
        }
        if (CheckCollisionCircleRec(ball.position, 10.0f, rec)){
            ball.velocity.y = -ball.velocity.y;
        }
        // if (CheckCollisionCircleRec(ball.position, 10.0f, rec)){
        //     ball.velocity.x = -ball.velocity.x;
        // }

        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/
        
        
        // player.velocity = Vector2Normalize(player.velocity);                // 1. make length = 1
        // player.velocity = Vector2Scale(player.velocity, player.speed);      // 2. make length = speed
        // player.velocity = Vector2Scale(player.velocity, delta_time);        // 3. make length proportional to one frame
        // player.position = Vector2Add(player.position, player.velocity);     // 4. apply velocity vector to position vector
    


        if(IsKeyDown(KEY_W) && player.position.y >= 0)
        {
            player.position.y = player.position.y - player.speed*delta_time;
        }

        if(IsKeyDown(KEY_A) && player.position.x >= 0)
        {
            player.position.x = player.position.x - player.speed*delta_time;
        }

        if(IsKeyDown(KEY_S) && player.position.y < (screen_height-32))
        {
            player.position.y = player.position.y + player.speed*delta_time;
        }

        if(IsKeyDown(KEY_D) && player.position.x < (screen_width-80))
        {
            player.position.x = player.position.x + player.speed*delta_time;
        }

        /*-------------------------------------------------------*/
        /*--------------------End-of-Controls--------------------*/
        /*-------------------------------------------------------*/

        //----------------------DEBUGING--------------------------/

        char debugText[256];

        // Format the debug information into a string
        sprintf(debugText, "Velocity: (%.2f, %.2f)", ball.velocity.x, ball.velocity.y);
        DrawText(debugText, 10, 10, 20, DARKGRAY);

        sprintf(debugText, "Position: (%.2f, %.2f)", ball.position.x, ball.position.y);
        DrawText(debugText, 10, 40, 20, DARKGRAY);

        sprintf(debugText, "Speed: %.2f, Delta Time: %.5f", ball.speed, delta_time);
        DrawText(debugText, 10, 70, 20, DARKGRAY);

        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context.
    CloseWindow();
    return 0;
}

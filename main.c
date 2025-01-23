#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

//------------------------TO-DO---------------------------------------------//
//1. Fix collision on blocks.

#define ROWS 5 // Number of lines
#define COLS 10 // Number of columns

typedef struct{
    Vector2 position;
    Vector2 size;
    int lives;
    bool active;
} block_t;

block_t blocks[ROWS][COLS];

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float speed;
    Vector2 size;
} player_t;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed;
} ball_t;

// Vector2 AddVector2(Vector2 v1, Vector2 v2) {
//     return (Vector2){v1.x + v2.x, v1.y + v2.y};
// }

int main(void) {
    // Initialize the window.
    const int screen_width = 800;
    const int screen_height = 600;
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "RaylibGame");

    // Initialize the player and ball.
    player_t player = {350, 500, 5, 5, 1000, 75, 25};
    ball_t ball = {200, 200, -5, -5, 10, 10};
    block_t block = {100, 100, 75,25, 1, true};
    // Rectangle rec_block = {block.position.x, block.position.y, block.size.x, block.size.y};
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            blocks[i][j].position.x = j * block.size.x; // Set x position
            blocks[i][j].position.y = i * block.size.y; // Set y position
            blocks[i][j].active = true; // Set block as active
            blocks[i][j].lives = 1; // Set initial lives
        }
    }

    while (!WindowShouldClose()) {

        float delta_time = GetFrameTime();
        Rectangle rec = { player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y};
        
        /*-------------------------------------------------------*/
        /*----------------------Game-logic-----------------------*/
        /*-------------------------------------------------------*/

        // ball.velocity = Vector2Scale(Vector2Normalize(ball.velocity), ball.speed);
        // ball.position = Vector2Add(ball.position, Vector2Scale(ball.velocity, delta_time));

        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;

        //Making the ball "bounce" when hiting the walls.
        if (ball.position.x - ball.radius < 0 || ball.position.x + ball.radius > screen_width) {
            ball.velocity.x = -ball.velocity.x; // Reverse x direction
        }
        if (ball.position.y - ball.radius < 0 || ball.position.y + ball.radius > screen_height) {
            ball.velocity.y = -ball.velocity.y; // Reverse y direction
        }

        //Collision logic ball with player
        if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
        {
            if (ball.velocity.y > 0)
            {
                ball.velocity.y = -ball.velocity.y;
                ball.velocity.x = (ball.position.x - player.position.x)/(player.size.x/2)*5;
            }
        }

        //Collision logic ball with blocks
        // if (CheckCollisionCircleRec(ball.position, ball.radius, rec_block))
        // {
        //     ball.velocity.y = -ball.velocity.y;
        //     block.lives--;
        //         if(block.lives == 0){
        //             block.active = false;
        //         }
        // }

        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/
        

        // if(IsKeyDown(KEY_W) && player.position.y >= 0)
        // {
        //     player.position.y = player.position.y - player.speed*delta_time;
        // }

        if(IsKeyDown(KEY_A) && player.position.x > 46)
        {
            player.position.x = player.position.x - player.speed*delta_time;
        }

        // if(IsKeyDown(KEY_S) && player.position.y < (screen_height-32))
        // {
        //     player.position.y = player.position.y + player.speed*delta_time;
        // }

        if(IsKeyDown(KEY_D) && player.position.x < screen_width-45)
        {
            player.position.x = player.position.x + player.speed*delta_time;
        }

        /*-------------------------------------------------------*/
        /*--------------------Debugging--------------------------*/
        /*-------------------------------------------------------*/

        // char debugText[256];

        // // Format the debug information into a string
        // sprintf(debugText, "Velocity: (%.2f, %.2f)", ball.velocity.x, ball.velocity.y);
        // DrawText(debugText, 10, 10, 20, DARKGRAY);

        // sprintf(debugText, "Position: (%.2f, %.2f)", ball.position.x, ball.position.y);
        // DrawText(debugText, 10, 40, 20, DARKGRAY);

        // sprintf(debugText, "Speed: %.2f, Delta Time: %.5f", ball.speed, delta_time);
        // DrawText(debugText, 10, 70, 20, DARKGRAY);

        /*-------------------------------------------------------*/
        /*----------------------Drawing--------------------------*/
        /*-------------------------------------------------------*/

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < ROWS; i++){
            for (int j = 0; j < COLS; j++){
                DrawRectangle(blocks[i][j].position.x+30, blocks[i][j].position.y+30, 70, 20, GRAY);
            }
        }

        // if (block.active == true)
        // {
        //     DrawRectangle(block.position.x, block.position.y, block.size.x, block.size.y, GRAY);
        // } 

        //The Rectangle defines the Retangle and sets the position and size.
         DrawRectangle(player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y, WHITE);
        // DrawRectangle(player.position.x, player.position.y, player.width, player.height, WHITE);

        //Draw the ball and set the speed of ball.
        DrawCircleV(ball.position, ball.radius, BLUE);

        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context.
    CloseWindow();
    return 0;
}

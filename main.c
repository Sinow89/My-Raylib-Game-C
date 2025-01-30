#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

//------------------------TO-DO---------------------------------------------//
//1. Fix collision on blocks.
//2. Refactor and create methods for the game states instead.

#define ROWS 5 // Number of lines
#define COLS 10 // Number of columns
#define RED_ROWS 2 // Number of lines
#define RED_COLS 5 // Number of columns

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
    int lives;
} player_t;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed;
} ball_t;

Vector2 add_vector2(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

int read_file(){

    FILE* file;
    int highscore;
    file = fopen("./highscore.txt", "r");
    fscanf(file, "%d", &highscore);
    fclose(file);
    return highscore;

}

int main(void) {
    // Initialize the window.
    const int screen_width = 800;
    const int screen_height = 600;
    bool debug_menu = false; 
    bool pause = true;
    bool lost_life = false;
    int active_blocks = 50;
    int score = 0;
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "RaylibGame");

    // Initialize the player and ball.
    player_t player = {350, 500, 5, 5, 1000, 75, 25, 3};
    ball_t ball = {200, 200, -5, -5, 10, 8};
    block_t block = {100, 100, 75,25, 1, true};
    
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

        if (pause == false)
        {
            ball.position.x += ball.velocity.x;
            ball.position.y += ball.velocity.y; 
            ball.velocity = Vector2Scale(Vector2Normalize(ball.velocity), ball.speed);
            ball.position = add_vector2(ball.position, Vector2Scale(ball.velocity, delta_time));
        }

        //Lose a life mechanics
        if (ball.position.x > 800 || ball.position.y > 575)
        {
            --player.lives;
            ball.position.x = 200;
            ball.position.y = 200;
            player.position.x = 350;
            player.position.y = 500;
            lost_life= true;
        }

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
            if (ball.velocity.y > 0){
                ball.velocity.y = -ball.velocity.y;
                ball.velocity.x = (ball.position.x - player.position.x)/(player.size.x/2)*5;
            }
        }

        //Ball collision logic with blocks and red powerup block
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (blocks[i][j].active) {
                    if (i == RED_ROWS && j == RED_COLS){
                    
                    Rectangle red_block = {blocks[RED_ROWS][RED_COLS].position.x+30, blocks[RED_ROWS][RED_COLS].position.y+30, block.size.x, block.size.y};
                            if (CheckCollisionCircleRec(ball.position, ball.radius, red_block)) {
                                ball.velocity.y = -ball.velocity.y;
                                blocks[RED_ROWS][RED_COLS].lives--;
                            
                            if (blocks[RED_ROWS][RED_COLS].lives == 0) {
                                blocks[RED_ROWS][RED_COLS].active = false;
                                score = score + 100;
                                --active_blocks;
                                ++player.lives;
                            }
                         }
                    }

                    else{
                    Rectangle rec_block = {blocks[i][j].position.x+30, blocks[i][j].position.y+30, block.size.x, block.size.y};
                    
                        if (CheckCollisionCircleRec(ball.position, ball.radius, rec_block)) {
                            ball.velocity.y = -ball.velocity.y;
                            blocks[i][j].lives--;
                            
                            if (blocks[i][j].lives == 0) {
                                blocks[i][j].active = false;
                                score = score+100;
                                --active_blocks;
                            }
                        }
                    }
                }
            }
        }

        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/
        
        if (IsKeyPressed(KEY_SPACE))
        {
            pause = !pause;
        }    

        // if(IsKeyDown(KEY_W) && player.position.y >= 0)
        // {
        //     player.position.y = player.position.y - player.speed*delta_time;
        // }

        if(IsKeyDown(KEY_A) && player.position.x > 46 && pause == false)
        {
            player.position.x = player.position.x - player.speed*delta_time;
        }

        // if(IsKeyDown(KEY_S) && player.position.y < (screen_height-32))
        // {
        //     player.position.y = player.position.y + player.speed*delta_time;
        // }

        if(IsKeyDown(KEY_D) && player.position.x < screen_width-45 && pause == false)
        {
            player.position.x = player.position.x + player.speed*delta_time;
        }

        /*-------------------------------------------------------*/
        /*--------------------Debugging--------------------------*/
        /*-------------------------------------------------------*/

        if (IsKeyPressed(KEY_U)) {
            debug_menu = !debug_menu;  // Toggle the menu visibility
        }

        if (debug_menu) {
                    char debugText[256];

        // Format the debug information into a string
        sprintf(debugText, "Velocity: (%.2f, %.2f)", ball.velocity.x, ball.velocity.y);
        DrawText(debugText, 10, 10, 20, DARKGRAY);

        sprintf(debugText, "Position: (%.2f, %.2f)", ball.position.x, ball.position.y);
        DrawText(debugText, 10, 40, 20, DARKGRAY);

        sprintf(debugText, "Speed: %.2f, Delta Time: %.5f", ball.speed, delta_time);
        DrawText(debugText, 10, 70, 20, DARKGRAY);

        // Add block lives to debug output
        sprintf(debugText, "Block Lives: %d", blocks[4][5].lives);
        DrawText(debugText, 10, 400, 20, DARKGRAY);

        DrawFPS(700, 500);
        } 

        /*-------------------------------------------------------*/
        /*----------------------Drawing--------------------------*/
        /*-------------------------------------------------------*/

        BeginDrawing();
        ClearBackground(BLACK);

        //Start/Pause text
        if(pause == true){
            DrawText("Press Space to begin and pause the game", 200, 300, 20, WHITE);
        }


        if (player.lives == 0)
        {
            while (true){ 
                BeginDrawing();
                ClearBackground(BLACK); // Optional: Clear the screen for better visibility

                DrawText("Game Over!", 200, 300, 20, WHITE);
                DrawText("Press SPACE to Restart", 200, 350, 20, WHITE);

                if (IsKeyPressed(KEY_SPACE)) {
                    // Reset the game or exit the game over state
                    player.lives = 3; // Example: Reset lives
                    score = 0;
                    ball.position.x = 200;
                    ball.position.y = 200;
                    player.position.x = 350;
                    player.position.y = 500;
                    break; // Exit the loop
                    
                }
                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLS; j++) {
                        blocks[i][j].position.x = j * block.size.x; // Set x position
                        blocks[i][j].position.y = i * block.size.y; // Set y position
                        blocks[i][j].active = true; // Set block as active
                        blocks[i][j].lives = 1; // Set initial lives
                    }
                }

                EndDrawing();
            }
        }

        if (active_blocks == 0)
        {
            while (true){ 
                BeginDrawing();
                ClearBackground(BLACK); // Optional: Clear the screen for better visibility

                DrawText("You Won! Congratulations!", 200, 300, 20, WHITE);
                DrawText("Press SPACE to Restart", 200, 350, 20, WHITE);

                if (IsKeyPressed(KEY_SPACE)) {
                    // Reset the game or exit the game over state
                    player.lives = 3; // Example: Reset lives
                    score = 0;
                    ball.position.x = 200;
                    ball.position.y = 200;
                    player.position.x = 350;
                    player.position.y = 500;
                    active_blocks = 50;
                    break; // Exit the loop
                    
                }
                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLS; j++) {
                        blocks[i][j].position.x = j * block.size.x; // Set x position
                        blocks[i][j].position.y = i * block.size.y; // Set y position
                        blocks[i][j].active = true; // Set block as active
                        blocks[i][j].lives = 1; // Set initial lives
                    }
                }

                EndDrawing();
            }
        }

        while (lost_life == true){
                BeginDrawing();
                ClearBackground(BLACK); // Optional: Clear the screen for better visibility

                DrawText("You lost a life", 200, 300, 20, WHITE);
                DrawText("Press SPACE to continue", 200, 350, 20, WHITE);

                if (IsKeyPressed(KEY_SPACE)) {
                    ball.position.x = 200;
                    ball.position.y = 200;
                    player.position.x = 350;
                    player.position.y = 500;
                    lost_life = false;
                    break;
                }
                EndDrawing();
        }

        //Score points
        char score_point[9];
        sprintf(score_point, "Score: %d",score);
        DrawText(score_point, 10, 450, 20, DARKGRAY);

        //Lives points
        char lives_point[9];
        sprintf(lives_point, "Lives: %d",player.lives);
        DrawText(lives_point, 10, 550, 20, DARKGRAY);

        //Drawing of active Rectangles
        for (int i = 0; i < ROWS; i++){
            for (int j = 0; j < COLS; j++){
                if (blocks[i][j].active){
                    if (i == RED_ROWS && j == RED_COLS){
                        DrawRectangle(blocks[RED_ROWS][RED_COLS].position.x+30, blocks[RED_ROWS][RED_COLS].position.y+30, 70, 20, RED);
                    }
                    else
                    DrawRectangle(blocks[i][j].position.x+30, blocks[i][j].position.y+30, 70, 20, GRAY);
                }
            }
        }

        //The Rectangle defines the Retangle and sets the position and size.
         DrawRectangle(player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y, WHITE);
        // DrawRectangle(player.position.x, player.position.y, player.width, player.height, WHITE);

        //Draw the ball and set the speed of ball.
        DrawCircleV(ball.position, ball.radius, BLUE);

        EndDrawing();
    }

    // Close window and OpenGL context.
    CloseWindow();
    return 0;
}
